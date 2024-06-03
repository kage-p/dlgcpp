#pragma once

#include <functional>

namespace dlgcpp
{
    //template <typename T>
    class IEvent
    {
    public:
        //virtual IEvent& operator+=(std::function<T> fn) = 0;
        virtual IEvent& operator+=(std::function<void(void)> fn) = 0;
        virtual void invoke() = 0;
    };

    //template <typename T>
    //class Event : public IEvent<T>
    class Event : public IEvent
    {
    public:

        //Event& operator+=(std::function<T> fn) override
        Event& operator+=(std::function<void(void)> fn) override
        {
            _listeners.push_back(fn);
            return *this; // return new value by reference
        }

        //void invoke(T)
        void invoke() override
        {
            for (auto& f : _listeners)
                f();
        }

    private:
        //std::vector<std::function<T>> _listeners;
        std::vector<std::function<void(void)>> _listeners;
    };
}
