#include <iostream>
#include <unistd.h>

void idleRoutineInvoke() {
    std::cout << "idleRoutine. sleep2" << std::endl;
    sleep(2);
}