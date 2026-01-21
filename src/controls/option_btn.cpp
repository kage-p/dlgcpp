#include "controls/option_btn_impl.h"
#include "dlgcpp/controls/option_btn.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

OptionButton::OptionButton(const std::string& text, const Position& p)
    : OptionButton(std::make_shared<OptionButtonImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _checked.reset(false, nullptr, ownerFn, "checked");
    _autoCheck.reset(true, nullptr, ownerFn, "autoCheck");
    _horizontalAlignment.reset(HorizontalAlign::Left, nullptr, ownerFn, "horizontalAlignment");
    _verticalAlignment.reset(VerticalAlign::Center, nullptr, ownerFn, "verticalAlignment");
    _text.reset(text, nullptr, ownerFn, "text");
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

OptionButton::OptionButton(
    std::shared_ptr<OptionButtonImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

OptionButton::~OptionButton()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& OptionButton::checked()
{
    return _checked;
}

void OptionButton::checked(bool value)
{
    _checked = value;
}

IProperty<bool, ISharedControl>& OptionButton::autoCheck()
{
    return _autoCheck;
}

void OptionButton::autoCheck(bool value)
{
    _autoCheck = value;
}

IProperty<HorizontalAlign, ISharedControl>& OptionButton::horizontalAlignment()
{
    return _horizontalAlignment;
}

void OptionButton::horizontalAlignment(HorizontalAlign value)
{
    _horizontalAlignment = value;
}

IProperty<VerticalAlign, ISharedControl>& OptionButton::verticalAlignment()
{
    return _verticalAlignment;
}

void OptionButton::verticalAlignment(VerticalAlign value)
{
    _verticalAlignment = value;
}

IProperty<std::string, ISharedControl>& OptionButton::text()
{
    return _text;
}

void OptionButton::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& OptionButton::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& OptionButton::DoubleClickEvent()
{
    return _dblClickEvent;
}