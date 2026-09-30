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
    scheduler.Current = scheduler.getCo("idle");
    while(1) {
        idleRoutineInvoke();
        std::cout << "===>[main idle] ReScheduling..." << std::endl;
        Scheduler::yieldCo(); // re-schedule to other coroutines
    }
    std::cout<< "Exit..." << std::endl;
}

void Scheduler::addCo(Co* co) {
    co_list_.push_back(co);
}

void Scheduler::removeCo(Co* co) {
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
            if (co->getStatus() == CO_DEAD) {
                std::cout << "Removing dead coroutine: " << co->getName() << std::endl; 
            }
            return co->getStatus() == CO_DEAD; }),
        co_list.end());

    for (auto* co : scheduler.getCoList()) {
        if (co != Current) {
            Current = co;
            std::cout << "($yield) " << oldCurrent->getName() << " => " << Current->getName() << std::endl;
            printAllCo();
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
        }
    }
    std::cout << "($yield) " << oldCurrent->getName() << " => " << Current->getName() << std::endl;
}

void Scheduler::returnCo() {
    Co *oldCurrent = Current;

    oldCurrent->getStatus() = CO_DEAD;
    std::cout << oldCurrent->getName() << " => " << "DEAD" << std::endl;

    for (auto &co : scheduler.getCoList()) {
        if (co != Current) {
            Current = co;
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
        }
    }
}

void Scheduler::wakeupCo(Co *c) {
    Co *oldCurrent = Current;
    oldCurrent->getStatus() = CO_DEAD;
    Current = c;
    co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
}

void Scheduler::printAllCo() {
    std::cout << "================All coroutines================" << std::endl;
    for (size_t i = 0; i < co_list_.size(); ++i) {
        std::cout << "Co " << i << ": " << co_list_[i]->getName() << std::endl;
    }
    std::cout << "==============================================" << std::endl;
}