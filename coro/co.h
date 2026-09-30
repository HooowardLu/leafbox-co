#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

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

enum CoStatus {
    CO_READY,
    CO_RUNNING,
    CO_SUSPEND,
    CO_DEAD
};

typedef void (*CoroutineFunction)();

class Co {
public:
    struct co_context ctx_;
    std::string& getName();
    CoStatus& getStatus();
    Co(std::string name, CoroutineFunction co_fn);
    ~Co();
private:
    std::string name_;
    char *stack_;
    int stack_size_;
    CoroutineFunction co_fn_;
    CoStatus status_;
    void makeContext(void);
};


// void co_ctx_swap(struct co_context *curr, struct co_context *next);
struct coroutine *co_new(CoroutineFunction start, size_t stack_size);
void co_free(struct coroutine *co);
void co_ctx_make(struct coroutine *co);
extern "C" {
    void co_ctx_swap(struct co_context *curr, struct co_context *next);
}
