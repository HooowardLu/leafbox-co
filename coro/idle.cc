#include <iostream>
#include <unistd.h>
#include "coro/co.h"
#include "sched/sched.h"


using namespace Sched;

Co *idle = new Co("idle", NULL);

void idleRoutineInvoke() {
    while(1) {
        std::cout << "===>[idle] sleep1" << std::endl;
        sleep(1);
        std::cout << "===>[idle] ReScheduling..." << std::endl;
        scheduler.yieldCo(); // re-schedule to other coroutines
    }
}