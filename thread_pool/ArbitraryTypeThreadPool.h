#pragma once
#include "ThreadSafeQueue.H"
#include <vector>
#include <thread>
#include <future>
#include <type_traits>
#include <functional>

namespace thread_pool
{
    class ArbitraryThreadPool
    {
    public:
        ArbitraryThreadPool(int size_) : _threadNo(size_)
        {
            init();
        }

        ~ArbitraryThreadPool()
        {
            stop();
            clear();
        }

        void stop()
        {
            _taskQue.stop();
        }

        void clear()
        {
            for (auto& t : _pool) {
                t.join();
            }
        }

        void init()
        {
            auto worker = [&]() {
                std::function<void()> task;
                while (true)
                {
                    if (!_taskQue.wait_and_pop(task))
                    {
                        return;
                    }
                    task();
                }
            };
            _pool.reserve(_threadNo);
            for (int i=0; i<_threadNo; i++)
            {
                _pool.emplace_back(worker);
            }
        }

        template<typename Func, typename... Args>
        auto commit(Func f_, Args&&... args_) -> std::future<std::invoke_result_t<Func, Args...>>
        {
            using ReturnType = std::invoke_result_t<Func, Args...>;
            // auto func = [=]() -> ReturnType {
            //     return f_(args_...);
            // };
            auto func = std::bind(f_, std::forward<Args>(args_)...);
            auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(func);
            auto wrapper = [t = taskPtr]() {
                (*t)();
            };
            _taskQue.push(wrapper);
            return taskPtr->get_future();
        }
    private:
        int _threadNo;
        std::vector<std::thread> _pool;
        threadsafe_queue<std::function<void()>> _taskQue;
    };
} // namespace thread_pool
