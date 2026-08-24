#include <algorithm>
#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

SchedManager& SchedManager::getInstance() {
    static SchedManager instance;
    return instance;
}

void SchedManager::addCo(Co* co) {
    co_list_.push_back(co);
}

void SchedManager::removeCo(Co* co) {
    auto it = std::find(co_list_.begin(), co_list_.end(), co);
    if (it != co_list_.end()) {
        co_list_.erase(it);
    }
}

Co* SchedManager::getCo(size_t index) {
    if (index < co_list_.size()) {
        return co_list_[index];
    }
    return nullptr;
}

size_t SchedManager::getCoCount() const {
    return co_list_.size();
}

void SchedManager::schedule() {
    for (auto* co : co_list_) {
    }
}

void SchedManager::printAllCo() {
    for (size_t i = 0; i < co_list_.size(); ++i) {
        std::cout << "Co " << i << ": " << co_list_[i]->getName() << std::endl;
    }
}