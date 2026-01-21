#include "controls/progress_impl.h"
#include "dlgcpp/controls/progress.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Progress::Progress(const Position& p)
    : Progress(std::make_shared<ProgressImpl>(), p)
{
    border() = BorderStyle::Thin;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
    _vertical.reset(false, nullptr, ownerFn, "vertical");
    _range.reset(std::make_pair(0, 100), nullptr, ownerFn, "range");

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

Progress::Progress(
    std::shared_ptr<ProgressImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Progress::~Progress()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& Progress::vertical()
{
    return _vertical;
}

void Progress::vertical(bool value)
{
    _vertical = value;
}

IProperty<int, ISharedControl>& Progress::value()
{
    return _value;
}

void Progress::value(int value)
{
    _value = value;
}

IProperty<std::pair<int, int>, ISharedControl>& Progress::range()
{
    return _range;
}

void Progress::range(int from, int to)
{
    _range = std::make_pair(from, to);
}

IEvent<ISharedControl>& Progress::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& Progress::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& Progress::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& Progress::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}