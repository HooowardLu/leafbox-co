#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

using namespace Sched;

Co *root, *a, *b;

void foo_c(void) {
    std::cout << "===>[Co c] hello" << std::endl;
    Scheduler::yieldCo();
    std::cout << "===>[Co c] resumed" << std::endl;
    Scheduler::returnCo();
}

void foo_a(void) {
    Co *c = new Co("c", foo_c);

    std::cout << "===>[Co a] hello" << std::endl;
    Scheduler::yieldCo();
    std::cout << "===>[Co a] resumed" << std::endl;
    Scheduler::returnCo();
}

void foo_b(void) {
    std::cout << "===>[Co b] hello1" << std::endl;
    Scheduler::yieldCo();
    std::cout << "===>[Co b] resumed2" << std::endl;
    Scheduler::wakeupCo(a);
    std::cout << "===>[Co b] resumed3" << std::endl;
    Scheduler::yieldCo();
    std::cout << "===>[Co b] resumed4" << std::endl;
    Scheduler::returnCo();
}

int main(int argc, char *argv[]) {
    root = new Co("root", NULL);
    a = new Co("a", foo_a);
    b = new Co("b", foo_b);

    Scheduler::getInstance().printAllCo();
    Scheduler::schedInit();
    return 0;
}