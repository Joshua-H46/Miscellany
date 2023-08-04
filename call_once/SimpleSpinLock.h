#pragma once
#include <atomic>

namespace spin_lock
{

class simple_spin_lock {
public:
    void lock() {
        while (f.test_and_set(std::memory_order_acquire)) {}
    }

    void unlock() {
        f.clear(std::memory_order_release);
    }

private:
    std::atomic_flag f = ATOMIC_FLAG_INIT;
};

}