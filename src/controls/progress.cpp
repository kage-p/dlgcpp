#include "controls/progress_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Progress::Progress(const Position& p)
    : Progress(std::make_shared<ProgressImpl>(*this, p))
{
}

Progress::Progress(std::shared_ptr<ProgressImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Progress::~Progress()
{
}

bool Progress::vertical() const
{
    return _impl->vertical();
}

void Progress::vertical(bool value)
{
    if (_impl->vertical() == value)
        return;
    _impl->vertical(value);
}

int Progress::value() const
{
    return _impl->value();
}

void Progress::value(int value)
{
    if (_impl->value() == value)
        return;
    _impl->value(value);
}

std::pair<int, int> Progress::range() const
{
    return _impl->range();
}

void Progress::range(int from, int to)
{
    if (_impl->range() == std::pair<int, int>(from, to))
        return;
    _impl->range(from, to);
}

IEvent<ISharedControl>& Progress::ChangedEvent()
{
    return _impl->ChangedEvent();
}
