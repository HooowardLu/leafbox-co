#ifndef __SCHED_H__
#define __SCHED_H__

#include <vector>
#include <cstddef>
#include "coro/co.h"
#include <string>

namespace Sched {

class Scheduler {
public:
    static Co* Current;
    static Scheduler& getInstance();
    static void yieldCo();
    static void returnCo();
    static void wakeupCo(Co *c);
    static void schedInit();
    inline static std::vector<Co*> co_list_;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    
    void addCo(Co* co);
    std::vector<Co*>& getCoList();
    void removeCo(Co* co);

    Co* getCo(const std::string &name);
    size_t getCoCount() const;

    void printAllCo();

private:
    Scheduler() = default;
    ~Scheduler() = default;
};


}

#endif