#pragma once
#include <thread>
#include <vector>
#include <future>
#include "ThreadSafeQueue.H"

namespace thread_pool
{
    class SimpleThreadPool
    {
    public:
        SimpleThreadPool(int threadNum_) : _threadNum(threadNum_)
        {
            auto worker = [&]() {
                std::function<void()> task;
                while (true)
                {
                    if (!_queue.wait_and_pop(task))
                    {
                        return;
                    }
                    task();
                }
            };
            for (int i=0; i<_threadNum; i++)
            {
                _pool.emplace_back(std::thread(worker));
            }
        }

        ~SimpleThreadPool()
        {
            stop();
            clear();
        }

        void stop()
        {
            _queue.stop();
        }

        void clear()
        {
            for (auto& t : _pool)
            {
                t.join();
            }
        }

        template<typename Func>
        void commit(Func func_)
        {
            _queue.push(func_);
        }

    private:
        int _threadNum;
        std::vector<std::thread> _pool;
        threadsafe_queue<std::function<void()>> _queue;
    };
} // namespace thread_pool
