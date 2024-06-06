#pragma once

#include "dlgcpp/event.h"
#include <vector>

namespace dlgcpp
{
    template <class... Args>
    class Event : public IEvent<Args...>
    {
    public:
        Event& operator+=(std::function<void(Args...args)> fn) override
        {
            _listeners.push_back(fn);
            return *this;
        }

        void invoke(Args ... args) override
        {
            for (auto& f : _listeners)
                f(args...);
        };

    private:
        std::vector<std::function<void(Args...args)>> _listeners;
    };
}
