#pragma once

#include "dlgcpp/event.h"
#include <unordered_map>

namespace dlgcpp
{
    template <class... Args>
    class Event : public IEvent<Args...>
    {
    public:
        virtual ~Event() = default;

        inline EventBinding operator+=(std::function<void(Args...args)> fn) override
        {
            EventBinding ref = _nextBindingRef++;
            _listeners[ref] = std::move(fn);
            return ref;
        }

        inline EventBinding operator+=(std::function<void()> fn) override
        {
            EventBinding ref = _nextBindingRef++;
            _basic_listeners[ref] = std::move(fn);
            return ref;
        }

        inline void operator-=(EventBinding ref) override
        {
            _listeners.erase(ref);
            _basic_listeners.erase(ref);
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
            for (const auto& [id, fn] : _listeners)
                fn(args...);
            for (const auto& [id, fn] : _basic_listeners)
                fn();
        };

        inline void invoke() override
        {
            // TODO: need to call parameter listeners
            //for (auto& f : _listeners)
            //    f(_defaultArgs);
            for (const auto& [id, fn] : _basic_listeners)
                fn();
        };

    private:
        std::unordered_map<EventBinding, std::function<void(Args...)>> _listeners;
        std::unordered_map<EventBinding, std::function<void()>> _basic_listeners;
        std::tuple<Args...> _defaultArgs;
        EventBinding _nextBindingRef = 1;
    };
}
