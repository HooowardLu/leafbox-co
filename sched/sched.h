#ifndef __SCHED_H__
#define __SCHED_H__

extern int g_co_nr;
extern struct coroutine* g_co_list[];

void sched_add(struct coroutine* co);

static inline void sched_yield(struct coroutine* co)
{
    for (int i = 0; i < g_co_nr; i++) {
        if (g_co_list[i] != co)
            co_ctx_swap(&co->ctx, &g_co_list[i]->ctx);
    }
}


#endif