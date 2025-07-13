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

        inline void operator-=(EventBinding ref) override
        {
            _listeners.erase(ref);
        }

        inline size_t count() override
        {
            return _listeners.size();
        }

        inline void clear() override
        {
            _listeners.clear();
        }

        inline void invoke(Args ... args) override
        {
            for (const auto& [id, fn] : _listeners)
            {
                fn(args...);
            }
        };

        inline void invoke() override
        {
            for (const auto& [id, fn] : _listeners)
            {
                std::apply([&fn](Args... args) {
                    fn(args...);
                    }, _defaultArgs);
            }
        };

        void setDefaultArgs(Args... args)
        {
            _defaultArgs = std::make_tuple(std::forward<Args>(args)...);
        }

    private:
        std::unordered_map<EventBinding, std::function<void(Args...)>> _listeners;
        std::tuple<Args...> _defaultArgs;
        EventBinding _nextBindingRef = 1;
    };
}
