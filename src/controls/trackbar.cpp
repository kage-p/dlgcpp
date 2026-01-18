#include "controls/trackbar_impl.h"
#include "dlgcpp/controls/trackbar.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TrackBar::TrackBar(const Position& p)
    : TrackBar(std::make_shared<TrackBarImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _vertical.reset(false, nullptr, ownerFn, "vertical");
    _range.reset(std::make_pair(0, 100), nullptr, ownerFn, "range");
    _pageSize.reset(1, nullptr, ownerFn, "pageSize");
    _tickMarks.reset(TrackBarTickMark::Above, nullptr, ownerFn, "tickMarks");
    _thumbColor.reset(Color::Default, nullptr, ownerFn, "thumbColor");
    _barColor.reset(Color::Default, nullptr, ownerFn, "barColor");

    auto valueValidatorFn = [&](int value)
        {
            auto& range = _range.value();
            if (value < range.first ||
                value > range.second)
                return false;
            return true;
        };
    _value.reset(0, valueValidatorFn, ownerFn, "value");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _dblRightClickEvent.reset(ownerFn, "DoubleRightClickEvent");
    _rightClickEvent.reset(ownerFn, "RightClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

TrackBar::TrackBar(
    std::shared_ptr<TrackBarImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

TrackBar::~TrackBar()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& TrackBar::vertical()
{
    return _vertical;
}

void TrackBar::vertical(bool value)
{
    _vertical = value;
}

IProperty<int, ISharedControl>& TrackBar::value()
{
    return _value;
}

void TrackBar::value(int value)
{
    _value = value;
}

IProperty<std::pair<int, int>, ISharedControl>& TrackBar::range()
{
    return _range;
}

void TrackBar::range(int from, int to)
{
    _range = std::make_pair(from, to);
}

IProperty<int, ISharedControl>& TrackBar::pageSize()
{
    return _pageSize;
}

void TrackBar::pageSize(int value)
{
    _pageSize = value;
}

IProperty<TrackBarTickMark, ISharedControl>& TrackBar::tickMarks()
{
    return _tickMarks;
}

void TrackBar::tickMarks(TrackBarTickMark value)
{
    _tickMarks = value;
}

IProperty<Color, ISharedControl>& TrackBar::barColor()
{
    return _barColor;
}

void TrackBar::barColor(Color value)
{
    _barColor = value;
}

IProperty<Color, ISharedControl>& TrackBar::thumbColor()
{
    return _thumbColor;
}

void TrackBar::thumbColor(Color value)
{
    _thumbColor = value;
}

IEvent<ISharedControl>& TrackBar::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& TrackBar::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& TrackBar::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& TrackBar::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}
