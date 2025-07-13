#pragma once

#include <functional>

namespace dlgcpp
{
    using EventBinding = size_t;

    template <class... Args>
    class IEvent
    {
    public:
        virtual ~IEvent() = default;
        virtual EventBinding operator+=(std::function<void(Args...args)> fn) = 0;
        virtual void operator-=(EventBinding ref) = 0;
        virtual size_t count() = 0;
        virtual void clear() = 0;

        virtual void invoke(Args ... args) = 0;
        virtual void invoke() = 0;
    };
}
