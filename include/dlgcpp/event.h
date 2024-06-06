#pragma once

#include <functional>

namespace dlgcpp
{
    template <class... Args>
    class IEvent
    {
    public:
        virtual IEvent& operator+=(std::function<void(Args...args)> fn) = 0;
        virtual void invoke(Args ... args) = 0;
    };
}
