#include <stdio.h>
#include "coro/co.h"

struct coroutine *main_co, *new_co;

void foo()
{
    printf("Here is the new coroutine\n");
    co_ctx_swap(&new_co->ctx, &main_co->ctx);
    printf("new coroutine resumed\n");
    co_ctx_swap(&new_co->ctx, &main_co->ctx);
}

int main()
{
    main_co = co_new(NULL, 0);
    new_co = co_new(foo, 1024 * 1024);
    co_ctx_make(new_co);

    co_ctx_swap(&main_co->ctx, &new_co->ctx);
    printf("main coroutine here\n");
    co_ctx_swap(&main_co->ctx, &new_co->ctx);
    printf("main coroutine finish\n");
    return 0;
}