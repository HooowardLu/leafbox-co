#include <iostream>
#include <algorithm>
#include "coro/co.h"
#include "coro/idle.h"
#include "sched/sched.h"


using namespace Sched;

Scheduler& Scheduler::getInstance() {
    static Scheduler SchedulerInstance;
    return SchedulerInstance;
}

namespace Sched {
    Scheduler& scheduler = Scheduler::getInstance();
}

void Scheduler::schedStart() {
    std::cout<< "schedStart" << std::endl;
    scheduler.Current = scheduler.getCo("idle"); // One will be the idle if it is set to the first Current.
    scheduler.Current->getStatus() = Co::CO_RUNNING;
    idleRoutineInvoke();
    std::cout<< "Exit..." << std::endl;
}

void Scheduler::addCo(Co* co) {
    co->getStatus() = Co::CO_READY;
    co_list_.push_back(co);
}

void Scheduler::deleteCo(Co* co) {
    auto it = std::find(co_list_.begin(), co_list_.end(), co);
    if (it != co_list_.end()) {
        co_list_.erase(it);
    }
}

Co* Scheduler::getCo(const std::string &name) {
    for (auto* co : co_list_) {
        if (co->getName() == name) {
            return co;
        }
    }
    return nullptr;
}

size_t Scheduler::getCoCount() const {
    return co_list_.size();
}

std::vector<Co*>& Scheduler::getCoList() {
    return co_list_;
}

void Scheduler::yieldCo() {
    Co *oldCurrent = Current;
    std::vector<Co*>& co_list = scheduler.getCoList();

    /* Need to clean up dead coroutines, 
     * because if the Co is Returned, it will be marked as CO_DEAD, and we need to remove it from the list.
     *  Otherwise, the scheduler will keep switching to the dead coroutine, which will cause a crash.
     */
    co_list.erase(std::remove_if(co_list.begin(), co_list.end(),
        [](Co* co) {
            if (co->getStatus() == Co::CO_DEAD) {
                std::cout << "Removing dead coroutine: " << co->getName() << std::endl; 
            }
            return co->getStatus() == Co::CO_DEAD; }),
        co_list.end());

    for (auto* co : scheduler.getCoList()) {
        if (co != Current) {
            Current = co;
            printAllCo();

            std::cout << "($yield) " << oldCurrent->getName() << " => " << Current->getName() << std::endl;
            Current->getStatus() = Co::CO_RUNNING;
            oldCurrent->getStatus() = Co::CO_PENDING;
            /* in the first yield in main function, idle context will be overlayed by the main function stack */
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_); /* if a Co is swap back, it will continue from here. */
        }
    }
    // std::cout << "($yield) " << oldCurrent->getName() << " => " << Current->getName() << std::endl;
}

void Scheduler::returnCo() {
    Co *oldCurrent = Current;

    oldCurrent->getStatus() = Co::CO_DEAD;
    std::cout << oldCurrent->getName() << " => " << "DEAD" << std::endl;

    for (auto &co : scheduler.getCoList()) {
        if (co != Current) {
            Current = co;
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
        }
    }
}

void Scheduler::wakeupCo(const std::string &name) {
    Co *oldCurrent = Current;
    oldCurrent->getStatus() = Co::CO_DEAD;

    Current = getCo(name);
    co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
}

void Scheduler::printAllCo() {
    std::cout << "================All coroutines================" << std::endl;
    for (size_t i = 0; i < co_list_.size(); ++i) {
        std::cout << co_list_[i]->getName() << "\t " << Co::CoStatusDecode[co_list_[i]->getStatus()] << std::endl;
    }
    std::cout << "==============================================" << std::endl;
}