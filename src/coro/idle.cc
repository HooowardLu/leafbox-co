#include <iostream>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include "coro/co.h"
#include "sched/sched.h"


using namespace Sched;

Co *idle = new Co("idle", NULL);

void idleRoutineInvoke() {
    while(1) {
        spdlog::info("===>[idle] sleep1");
        sleep(1);
        spdlog::info("===>[idle] ReScheduling...");
        scheduler.yieldCo(); // re-schedule to other coroutines

        if (scheduler.getCoCount() == 1 &&
            scheduler.getCoList()[0]->getName() == "idle") {
            spdlog::info("===>[idle] No other coroutines");
            break;
        }
    }
}