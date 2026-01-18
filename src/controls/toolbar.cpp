#include "controls/toolbar_impl.h"
#include "dlgcpp/controls/toolbar.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ToolBar::ToolBar(const Position& p)
    : ToolBar(std::make_shared<ToolBarImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _buttonSize.reset(
        Size(24, 24),
        [&](const Size& value) { return !value.empty(); },
        ownerFn,
        "buttonSize");

    _imageSize.reset(
        Size(20, 20),
        [&](const Size& value) { return !value.empty(); },
        ownerFn,
        "imageSize");

    // TODO: Limit to toolbarIdRange
    _items.reset(std::vector<ISharedToolBarItem>(), nullptr, ownerFn, "items");

    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _dblRightClickEvent.reset(ownerFn, "DoubleRightClickEvent");
    _rightClickEvent.reset(ownerFn, "RightClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

ToolBar::ToolBar(
    std::shared_ptr<ToolBarImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

ToolBar::~ToolBar()
{
    _impl.reset();
}

IProperty<Size, ISharedControl>& ToolBar::buttonSize()
{
    return _buttonSize;
}

void ToolBar::buttonSize(const Size& value)
{
    _buttonSize = value;
}

IProperty<Size, ISharedControl>& ToolBar::imageSize()
{
    return _imageSize;
}

void ToolBar::imageSize(const Size& value)
{
    _imageSize = value;
}

IProperty<std::vector<ISharedToolBarItem>, ISharedControl>& ToolBar::items()
{
    return _items;
}

void ToolBar::items(const std::vector<ISharedToolBarItem>& items)
{
    _items = items;
}

IEvent<ISharedControl>& ToolBar::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& ToolBar::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& ToolBar::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& ToolBar::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}