#pragma once
#include <iostream>
#include <memory>

namespace raii_guard
{
    template<typename Unused, typename Func>
    class RaiiGuard
    {
    public:
        explicit RaiiGuard(Func f_)
        {
            ::new (getCallbackBuffer()) Func(std::move(f_));
            _enable = true;
        }
        RaiiGuard(RaiiGuard&& other_)
        {
            ::new (getCallbackBuffer()) Func(std::move(other_.getCallback()));
            _enable = true;
            other_.cancel();
        }
        RaiiGuard(const RaiiGuard&) = delete;
        RaiiGuard& operator=(const RaiiGuard&) = delete;
        RaiiGuard& operator=(RaiiGuard&&) = delete;
        ~RaiiGuard()
        {
            if (_enable)
            {
                invoke();
            }
        }
        void cancel()
        {
            _enable = false;
            getCallback().~Func();
        }
        bool enable() const
        {
            return _enable;
        }

    private:
        void* getCallbackBuffer()
        {
            return static_cast<void*>(_callback);
        }
        Func& getCallback()
        {
            return *static_cast<Func*>(getCallbackBuffer());
        }
        void invoke()
        {
            getCallback()();            // should we call std::move here?
        }
        
        bool _enable{false};
        alignas(Func) char _callback[sizeof(Func)];     // allocate on stack 
    };

    template<typename Func>
    RaiiGuard(Func) -> RaiiGuard<void, Func>;
}