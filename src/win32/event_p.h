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

        Event& operator+=(std::function<void()> fn) override
        {
            _basic_listeners.push_back(fn);
            return *this;
        }

        void clear() override
        {
            _listeners.clear();
            _basic_listeners.clear();
        }

        void invoke(Args ... args) override
        {
            for (auto& f : _listeners)
                f(args...);
            for (auto& f : _basic_listeners)
                f();
        };

        void invoke() override
        {
            //for (auto& f : _listeners)
            //    f(_defaultArgs);
            for (auto& f : _basic_listeners)
                f();
        };

    private:
        std::vector<std::function<void(Args...args)>> _listeners;
        std::vector<std::function<void()>> _basic_listeners;
        std::tuple<Args...> _defaultArgs;
    };
}
