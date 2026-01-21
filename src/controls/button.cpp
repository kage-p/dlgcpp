#include "controls/button_impl.h"
#include "dlgcpp/controls/button.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Button::Button(const std::string& text, const Position& p)
    : Button(std::make_shared<ButtonImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
    _horizontalAlignment.reset(HorizontalAlign::Center, nullptr, ownerFn, "horizontalAlignment");
    _verticalAlignment.reset(VerticalAlign::Center, nullptr, ownerFn, "verticalAlignment");
    _text.reset(text, nullptr, ownerFn, "text");
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Button::Button(
    std::shared_ptr<ButtonImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Button::~Button()
{
    _impl.reset();
}

IProperty<HorizontalAlign, ISharedControl>& Button::horizontalAlignment()
{
    return _horizontalAlignment;
}

void Button::horizontalAlignment(HorizontalAlign value)
{
    _horizontalAlignment = value;
}

IProperty<VerticalAlign, ISharedControl>& Button::verticalAlignment()
{
    return _verticalAlignment;
}

void Button::verticalAlignment(VerticalAlign value)
{
    _verticalAlignment = value;
}

IProperty<std::string, ISharedControl>& Button::text()
{
    return _text;
}

void Button::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& Button::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& Button::DoubleClickEvent()
{
    return _dblClickEvent;
}