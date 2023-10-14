#pragma once
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace thread_pool {

template<typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() : _stop(0) {}

    bool wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]() {
            return !data_queue.empty() || _stop.load(std::memory_order_acquire) == 1;
        });
        if (_stop.load(std::memory_order_acquire) == 1)
        {
            return false;
        }
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
        {
            return false;
        }
        
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;    
    }

    bool push(T&& new_value)
    {
        if (_stop.load(std::memory_order_acquire))
        {
            return false;
        }
        std::lock_guard<std::mutex> lk(mut);
        data_queue.emplace(std::move(new_value));
        data_cond.notify_one();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    void stop()
    {
        _stop.store(1, std::memory_order_release);
    }

private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
    alignas(64) std::atomic<int> _stop;
};

}