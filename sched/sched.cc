#include <algorithm>
#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"
#include <unistd.h>

using namespace Sched;

Co* Sched::Scheduler::Current = nullptr;

Scheduler& Scheduler::getInstance() {
    static Scheduler instance;
    return instance;
}

void Scheduler::SchedInit() {
    std::cout<< "SchedInit" << std::endl;
    while(1) {
        if (Scheduler::getInstance().getCoCount() == 1 && 
            Scheduler::getInstance().getCoList()[0]->getName() == "root") {
            std::cout << "idle..." << std::endl;
            sleep(1);
        }
        Scheduler::Yield(); // re-schedule to other coroutines
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

void Scheduler::Yield() {
    Co *oldCurrent = Current;
    std::vector<Co*>& co_list = Scheduler::getInstance().getCoList();

    //clean up dead coroutines
    co_list.erase(std::remove_if(co_list.begin(), co_list.end(),
        [](Co* co) {
            if (co->getStatus() == CO_DEAD) {
                std::cout << "Removing dead coroutine: " << co->getName() << std::endl; 
            }
            return co->getStatus() == CO_DEAD; }),
        co_list.end());

    for (auto* co : Scheduler::getInstance().getCoList()) {
        if (co != Current) {
            Current = co;
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
        }
    }
}

void Scheduler::Return() {
    Co *oldCurrent = Current;

    oldCurrent->getStatus() = CO_DEAD;

    for (auto &co : Scheduler::getInstance().getCoList()) {
        if (co != Current) {
            Current = co;
            co_ctx_swap(&oldCurrent->ctx_, &Current->ctx_);
        }
    }
}

void Scheduler::printAllCo() {
    std::cout << "================All coroutines================" << std::endl;
    for (size_t i = 0; i < co_list_.size(); ++i) {
        std::cout << "Co " << i << ": " << co_list_[i]->getName() << std::endl;
    }
    std::cout << "==============================================" << std::endl;
}