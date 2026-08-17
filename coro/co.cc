#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include "coro/co.h"

Co::Co(std::string name, start_coroutine start_fn) {
    stack_size_ = 1024 * 1024;
    stack_ = (char *)malloc(stack_size_);
    start_ = start_fn;
    if (name_ != "main")
        makeContext();
}

Co::~Co() {
    free(stack_);
}

void Co::makeContext(void) {
    char *sp = stack_ + stack_size_ - sizeof(void*);
    sp = (char*)((intptr_t)sp & -16LL);
    *(void**)sp = (void*)start_;
    ctx_.regs[CO_RSP] = sp;
}