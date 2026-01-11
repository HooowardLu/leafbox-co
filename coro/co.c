#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "coro/co.h"


struct coroutine *co_new(start_coroutine start, size_t stack_size) {
    struct coroutine *co = malloc(sizeof(struct coroutine));
    memset(&co->ctx, 0, sizeof(co->ctx));
    if (stack_size) {
        co->stack = malloc(stack_size);
    } else {
        co->stack = NULL;
    }
    co->stack_size = stack_size;
    co->start = start;

    return co;
}

void co_free(struct coroutine *co) {
    free(co->stack);
    free(co);
}

void co_ctx_make(struct coroutine *co) {
    char *sp = co->stack + co->stack_size - sizeof(void*);
    sp = (char*)((intptr_t)sp & -16LL);
    *(void**)sp = (void*)co->start;
    co->ctx.regs[CO_RSP] = sp;
}

