#include <stdio.h>
#include "coro/co.h"
#include "sched/sched.h"

struct coroutine *main_co, *foo_co;

void foo()
{
    while (1) {
        printf("here is the new coroutine\n");
        sched_yield(foo_co);
        printf("foo coroutine resumed\n");
        sched_yield(foo_co);
    }

}

int main()
{
    main_co = co_new(NULL, 0);
    sched_add(main_co);
    foo_co = co_new(foo, 1024 * 1024);
    co_ctx_make(foo_co);
    sched_add(foo_co);

    sched_yield(main_co);
    printf("main coroutine here\n");
    sched_yield(main_co);
    printf("main coroutine finish\n");
    return 0;
}