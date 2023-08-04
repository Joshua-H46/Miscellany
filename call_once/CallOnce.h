#pragma once
#include <atomic>
#include <functional>

namespace call_once {

    enum OnceStatus
    {
        kOnceInit = 0,
        kOnceRunning = 255,
        kOnceDone = 250
    };

    // calls with the same OnceFlag object will be executed once
    class OnceFlag
    {
    public:
        OnceFlag() : _control{0} {};
        OnceFlag(const OnceFlag&) = delete;
        OnceFlag& operator=(const OnceFlag&) = delete;
        std::atomic_uint& control() { return _control; }
        
    private:
        std::atomic_uint _control;
    };

    template<typename Func, typename ...Args>
    void callOnce(OnceFlag& onceFlag_, Func&& f_, Args&&... args_)
    {
        uint32_t old = kOnceInit;
        if (onceFlag_.control().compare_exchange_strong(old, kOnceRunning, std::memory_order_relaxed))
        {
            std::invoke(f_, std::forward<Args>(args_)...);
            onceFlag_.control().store(kOnceDone, std::memory_order_release);
        }
        else
        {
            while (onceFlag_.control().load(std::memory_order_acquire) != kOnceDone) {}
            // maybe we could sleep here
        }
    }
}