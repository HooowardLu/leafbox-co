#include <stdio.h>
#include "coro/co.h"
#include "sched/sched.h"

// Co* g_co_list[1024];
// int g_co_nr = 0;
// Co &current;

// void sched_add(struct coroutine* co)
// {
//     g_co_list[g_co_nr++] = co;    
// }

// void sched_yield(Co &co)
// {
//     Co &next_co;

//     // next_co = pick_next_co();
//     co_ctx_swap(&co->ctx, &next_co->ctx);
//     for (int i = 0; i < g_co_nr; i++) {
//         if (g_co_list[i] != co)
//             co_ctx_swap(&co->ctx, &g_co_list[i]->ctx);
//     }
// }
