#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

using namespace Sched;

void foo_idle() {
    while (true) {
        std::cout << "===>[foo_idle] hello" << std::endl;
        scheduler.yieldCo();
        std::cout << "===>[foo_idle] resumed" << std::endl;
    }
}

void foo_c(void) {
    std::cout << "===>[Co c] hello" << std::endl;
    scheduler.yieldCo();
    std::cout << "===>[Co c] resumed" << std::endl;
    scheduler.returnCo();
}

void foo_a(void) {
    Co *c = new Co("c", foo_c);

    std::cout << "===>[Co a] hello" << std::endl;
    scheduler.yieldCo();
    std::cout << "===>[Co a] resumed" << std::endl;
    scheduler.returnCo();
}

void foo_b(void) {
    std::cout << "===>[Co b] hello1" << std::endl;
    scheduler.yieldCo();
    std::cout << "===>[Co b] resumed2" << std::endl;
    scheduler.wakeupCo("a");
    std::cout << "===>[Co b] resumed3" << std::endl;
    scheduler.yieldCo();
    std::cout << "===>[Co b] resumed4" << std::endl;
    scheduler.returnCo();
}




int main(int argc, char *argv[]) {
    Co *a = new Co("a", foo_a);
    Co *b = new Co("b", foo_b);

    scheduler.printAllCo();
    scheduler.schedStart();
    return 0;
}