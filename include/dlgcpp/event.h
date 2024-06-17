#pragma once

#include <functional>

namespace dlgcpp
{
    template <class... Args>
    class IEvent
    {
    public:
        virtual IEvent& operator+=(std::function<void(Args...args)> fn) = 0;
        virtual IEvent& operator+=(std::function<void()> fn) = 0;
        virtual size_t count() = 0;
        virtual void clear() = 0;

        virtual void invoke(Args ... args) = 0;
        virtual void invoke() = 0;
    };
}
