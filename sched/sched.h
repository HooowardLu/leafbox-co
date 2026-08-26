#ifndef __SCHED_H__
#define __SCHED_H__

#include <vector>
#include <cstddef>
#include "coro/co.h"

namespace Sched {

class SchedManager {
public:
    static Co* Current;
    static SchedManager& getInstance();
    static void Yield();

    SchedManager(const SchedManager&) = delete;
    SchedManager& operator=(const SchedManager&) = delete;

    void addCo(Co* co);
    void removeCo(Co* co);

    Co* getCo(size_t index);
    size_t getCoCount() const;

    void printAllCo();

private:
    SchedManager() = default;
    ~SchedManager() = default;

    std::vector<Co*> co_list_;
};


}

#endif