#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

using namespace Sched;

void foo_c(void) {
    std::cout << "===>[Co c] hello" << std::endl;
    Scheduler::Yield();
    std::cout << "===>[Co c] resumed" << std::endl;
    Scheduler::Return();
}

void foo_a(void) {
    Co *c = new Co("c", foo_c);

    std::cout << "===>[Co a] hello" << std::endl;
    Scheduler::Yield();
    std::cout << "===>[Co a] resumed" << std::endl;
    Scheduler::Return();
}

void foo_b(void) {
    std::cout << "===>[Co b] hello1" << std::endl;
    Scheduler::Yield();
    std::cout << "===>[Co b] resumed2" << std::endl;
    Scheduler::Yield();
    std::cout << "===>[Co b] resumed3" << std::endl;
    Scheduler::Yield();
    std::cout << "===>[Co b] resumed4" << std::endl;
    Scheduler::Return();
}

int main(int argc, char *argv[]) {
    Co *root, *a, *b;
    root = new Co("root", NULL);
    a = new Co("a", foo_a);
    b = new Co("b", foo_b);

    Scheduler::getInstance().printAllCo();
    Scheduler::SchedInit();
    return 0;
}