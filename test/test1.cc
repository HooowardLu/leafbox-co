#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

using namespace Sched;

Co *ma, *a;

void foo(void) {
    std::cout << "===> hello" << std::endl;
    SchedManager::Yield();
    std::cout << "===> resumed" << std::endl;
    SchedManager::Yield();
}

int main(int argc, char *argv[]) {
    ma = new Co("main", NULL);
    a = new Co("a", foo);

    SchedManager::getInstance().printAllCo();
    SchedManager::Yield();
    printf("main coroutine here\n");
    SchedManager::Yield();
    printf("main coroutine finish\n");

    return 0;
}