#pragma once

#include "dlgcpp/event.h"
#include <vector>

namespace dlgcpp
{
    template <class... Args>
    class Event : public IEvent<Args...>
    {
    public:
        inline Event& operator+=(std::function<void(Args...args)> fn) override
        {
            _listeners.push_back(fn);
            return *this;
        }

        inline Event& operator+=(std::function<void()> fn) override
        {
            _basic_listeners.push_back(fn);
            return *this;
        }

        inline size_t count() override
        {
            return _listeners.size() + _basic_listeners.size();
        }

        inline void clear() override
        {
            _listeners.clear();
            _basic_listeners.clear();
        }

        inline void invoke(Args ... args) override
        {
            for (auto& f : _listeners)
                f(args...);
            for (auto& f : _basic_listeners)
                f();
        };

        inline void invoke() override
        {
            // TODO: need to call parameter listeners
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
