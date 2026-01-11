#ifndef __CO_H__
#define __CO_H__

#include <stddef.h>

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

struct coroutine {
    struct co_context ctx;
    char *stack;
    size_t stack_size;
    start_coroutine start;
};


void co_ctx_swap(struct co_context *curr, struct co_context *next);
struct coroutine *co_new(start_coroutine start, size_t stack_size);
void co_free(struct coroutine *co);
void co_ctx_make(struct coroutine *co);

#endif // __CO_H__