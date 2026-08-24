#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "coro/co.h"
#include "sched/sched.h"


Co::Co(std::string name, start_coroutine start_fn) {
    name_ = name;
    stack_size_ = 1024 * 1024;
    stack_ = (char *)malloc(stack_size_);
    start_ = start_fn;
    //if (name_ != "main")
    makeContext();
    SchedManager::getInstance().addCo(this);
}

Co::~Co() {
    free(stack_);
}

std::string& Co::getName() {
    return name_;
}

void Co::makeContext(void) {
    char *sp = stack_ + stack_size_ - sizeof(void*);
    sp = (char*)((intptr_t)sp & -16LL);
    *(void**)sp = (void*)start_;
    ctx_.regs[CO_RSP] = sp;
}