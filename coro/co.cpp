#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

extern "C" {
// 注意：参数类型要和汇编实现完全匹配（struct co_context*，而非 struct coroutine*）
void co_ctx_swap(struct co_context *curr, struct co_context *next);
}

enum {
    CO_R15 = 0,
    CO_R14,
    CO_R13,
    CO_R12,
    CO_R9,
    CO_R8,
    CO_RBP,
    CO_RDI,
    CO_RSI,
    CO_RDX,
    CO_RCX,
    CO_RBX,
    CO_RSP,
};

struct co_context {
    void *regs[13];
};

typedef void (*start_coroutine)();

class Co {
public:
    Co(std::string name, start_coroutine start_fn) {
        stack_size_ = 1024 * 1024;
        stack_ = (char *)malloc(stack_size_);
        start_ = start_fn;
        if (name_ != "main")
            makeContext();
    }

    ~Co() {
        free(stack_);
    }

    struct co_context *getContext() {
        return &ctx_;
    }

private:
    std::string name_;
    char *stack_;
    int stack_size_;
    start_coroutine start_;
    struct co_context ctx_;
    void makeContext() {
        char *sp = stack_ + stack_size_ - sizeof(void*);
        sp = (char*)((intptr_t)sp & -16LL);
        *(void**)sp = (void*)start_;
        ctx_.regs[CO_RSP] = sp;
    }
};

Co *ma, *a;

void foo(void) {
    std::cout << "===> hello" << std::endl;
    co_ctx_swap(a->getContext(), ma->getContext());
}


int main(int argc, char *argv[]) {
    ma = new Co("main", NULL);
    a = new Co("a", foo); 
    co_ctx_swap(ma->getContext(), a->getContext());
    std::cout << "<=== back in main body" << std::endl;
    return 0;
}