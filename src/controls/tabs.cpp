#include "controls/tabs_impl.h"
#include "dlgcpp/controls/tabs.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Tabs::Tabs(const Position& p)
    : Tabs(std::make_shared<TabsImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _area.reset(Position(), nullptr, ownerFn, "area");

    _items.reset(
        std::vector<ISharedTabItem>(),
        [&]
        (const std::vector<ISharedTabItem>& items)
        {
            // reset index if exceeds item count
            if (_selectedIndex >= items.size())
                _selectedIndex = -1;

            return true;
        },
        ownerFn,
        "items");

    _selectedIndex.reset(
        -1,
        [&](int index)
        {
            if (index == -1)
                return true;
            return (index > -1 && index < _items.value().size());
        },
        ownerFn,
        "selectedIndex");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _dblRightClickEvent.reset(ownerFn, "DoubleRightClickEvent");
    _rightClickEvent.reset(ownerFn, "RightClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Tabs::Tabs(
    std::shared_ptr<TabsImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Tabs::~Tabs()
{
    _impl.reset();
}

IProperty<Position, ISharedControl>& Tabs::area()
{
    return _area;
}

IProperty<int, ISharedControl>& Tabs::selectedIndex()
{
    return _selectedIndex;
}

void Tabs::selectedIndex(int value)
{
    _selectedIndex = value;
}

IProperty<std::vector<ISharedTabItem>, ISharedControl>& Tabs::items()
{
    return _items;
}

void Tabs::items(const std::vector<ISharedTabItem>& items)
{
    _items = items;
}

IEvent<ISharedControl>& Tabs::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& Tabs::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& Tabs::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& Tabs::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}