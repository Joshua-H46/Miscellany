#pragma once
#include <cstdlib>
#include <atomic>
#include <iostream>

namespace ring_buffer
{
    template<typename T>
    class SPSCQueue
    {
    public:
        SPSCQueue(size_t size_) : _size(size_), _head(0), _tail(0)
        {
            _data = static_cast<T*>(std::aligned_alloc(sizeof(T), sizeof(T) * size_));
        }

        template<typename... Args>
        bool push(Args&&... args_)
        {
            auto h = _head.load(std::memory_order_acquire);
            auto t = _tail.load(std::memory_order_acquire);
            auto newTail = (t + 1) % _size;
            if (h == newTail) {
                return false;
            }
            T& slot = _data[t];
            new (&slot) T(std::forward<Args>(args_)...);
            _tail.store(newTail, std::memory_order_release);
            return true;
        }

        bool pop(T& obj_) {
            auto h = _head.load(std::memory_order_acquire);
            auto t = _tail.load(std::memory_order_acquire);
            if (t == h) {
                return false;
            }
            T& slot = _data[h];
            obj_ = std::move(slot);
            slot.~T();
            auto newHead = (h + 1) % _size;
            _head.store(newHead, std::memory_order_release);
            return true;
        }

    private:
        T* _data;
        size_t _size;
        alignas(64) std::atomic<int> _head;
        alignas(64) std::atomic<int> _tail;
    };
} // namespace ring_buffer
    