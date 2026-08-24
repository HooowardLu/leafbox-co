#ifndef __SCHED_H__
#define __SCHED_H__

#include <vector>
#include <cstddef>
#include "coro/co.h"

class SchedManager {
public:
    static SchedManager& getInstance();

    SchedManager(const SchedManager&) = delete;
    SchedManager& operator=(const SchedManager&) = delete;

    void addCo(Co* co);
    void removeCo(Co* co);

    Co* getCo(size_t index);
    size_t getCoCount() const;

    void schedule();
    void printAllCo();

private:
    SchedManager() = default;
    ~SchedManager() = default;

    std::vector<Co*> co_list_;
};

#endif