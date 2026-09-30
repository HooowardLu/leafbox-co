#pragma once

#include <vector>
#include <cstddef>
#include "coro/co.h"
#include <string>

namespace Sched {

class Scheduler {
public:
    inline static Co* Current = nullptr;
    inline static std::vector<Co*> co_list_;
    static Scheduler& getInstance();

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    
    void addCo(Co* co);
    std::vector<Co*>& getCoList();
    void deleteCo(Co* co);
    Co* getCo(const std::string &name);
    size_t getCoCount() const;
    void yieldCo();
    void returnCo();
    void wakeupCo(const std::string &name);
    void schedStart();

    void printAllCo();

private:
    Scheduler() = default;
    ~Scheduler() = default;
};

    extern Scheduler& scheduler;

}
