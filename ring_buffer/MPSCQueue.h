#pragma once
#include <atomic>
#include <cstdlib>
#include <cassert>

namespace ring_buffer
{
    template<typename T>
    class MPSCQueue
    {
    public:
        MPSCQueue(size_t size_) : _size(size_), _head(0), _tail(0), _stub(0)
        {
            _data = static_cast<T*>(std::aligned_alloc(sizeof(T), sizeof(T) * size_));
        }

        template<typename... Args>
        bool push(Args&&... args_)
        {
            int h;
            int t = _tail.load(std::memory_order_acquire);
            int newTail;
            do
            {
                h = _head.load(std::memory_order_acquire);
                newTail = (t + 1) % _size;
                if (h == newTail) {
                    return false;
                }
            } while (!_tail.compare_exchange_strong(t, newTail, std::memory_order_acq_rel, std::memory_order_acquire));
            T& slot = _data[t];
            new (&slot) T(std::forward<Args>(args_)...);
            auto s = t;
            do
            {
                s = t;
            } while (!_stub.compare_exchange_strong(s, newTail, std::memory_order_release, std::memory_order_relaxed));
            return true;
        }

        bool pop(T& obj_)
        {
            auto h = _head.load(std::memory_order_acquire);
            auto s = _stub.load(std::memory_order_acquire);
            if (h == s) {
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
        size_t _size;
        T* _data;
        alignas(64) std::atomic<int> _head;
        alignas(64) std::atomic<int> _tail;
        alignas(64) std::atomic<int> _stub;
    };
} // namespace ring_buffer
