#include <iostream>
#include <unistd.h>

void idleRoutineInvoke() {
    std::cout << "===>[idle] sleep1" << std::endl;
    sleep(1);
}