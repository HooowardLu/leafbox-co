#ifndef __SCHED_H__
#define __SCHED_H__

extern int g_co_nr;
extern struct coroutine* g_co_list[];

void sched_add(struct coroutine* co);
void sched_yield(struct coroutine* co);

#endif