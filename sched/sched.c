#include <stdio.h>
#include "coro/co.h"
#include "sched/sched.h"

struct coroutine* g_co_list[1024];
int g_co_nr = 0;
struct coroutine* current;

void sched_add(struct coroutine* co)
{
    g_co_list[g_co_nr++] = co;    
}
