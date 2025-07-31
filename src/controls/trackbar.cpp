#include "controls/trackbar_p.h"
#include "dlgcpp/controls/trackbar.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TrackBar::TrackBar(const Position& p)
    : TrackBar(std::make_shared<TrackBarImpl>(p))
{
}

TrackBar::TrackBar(std::shared_ptr<TrackBarImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}
TrackBar::~TrackBar()
{
}

bool TrackBar::vertical() const
{
    return _impl->vertical();
}

void TrackBar::vertical(bool value)
{
    if (_impl->vertical() == value)
        return;
    _impl->vertical(value);
}

int TrackBar::value() const
{
    return _impl->value();
}

void TrackBar::value(int value)
{
    if (_impl->value() == value)
        return;
    _impl->value(value);
}

std::pair<int, int> TrackBar::range() const
{
    return _impl->range();
}

void TrackBar::range(int from, int to)
{
    if (_impl->range() == std::pair<int, int>(from, to))
        return;
    _impl->range(from, to);
}

int TrackBar::pageSize() const
{
    return _impl->pageSize();
}

void TrackBar::pageSize(int value)
{
    if (_impl->pageSize() == value)
        return;
    _impl->pageSize(value);
}

TrackBarTickMark TrackBar::tickMarks() const
{
    return _impl->tickMarks();
}

void TrackBar::tickMarks(TrackBarTickMark value)
{
    if (_impl->tickMarks() == value)
        return;
    _impl->tickMarks(value);
}

std::pair<Color, Color> TrackBar::barColors() const
{
    return _impl->barColors();
}

void TrackBar::barColors(Color thumbColor, Color barColor)
{
    _impl->barColors(thumbColor, barColor);
}

IEvent<ISharedControl>& TrackBar::ChangedEvent()
{
    return _impl->ChangedEvent();
}
