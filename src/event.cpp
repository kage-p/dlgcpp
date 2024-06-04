#include "event.h"

using namespace dlgcpp;

Event& Event::operator+=(std::function<void(void)> fn)
{
    _listeners.push_back(fn);
    return *this;
}

void Event::invoke()
{
    for (auto& f : _listeners)
        f();
}
