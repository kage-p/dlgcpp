#include "controls/checkbox_impl.h"
#include "dlgcpp/controls/checkbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

CheckBox::CheckBox(const std::string& text, const Position& p)
    : CheckBox(std::make_shared<CheckBoxImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
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

CheckBox::CheckBox(
    std::shared_ptr<CheckBoxImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

CheckBox::~CheckBox()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& CheckBox::checked()
{
    return _checked;
}

void CheckBox::checked(bool value)
{
    _checked = value;
}

IProperty<bool, ISharedControl>& CheckBox::autoCheck()
{
    return _autoCheck;
}

void CheckBox::autoCheck(bool value)
{
    _autoCheck = value;
}

IProperty<HorizontalAlign, ISharedControl>& CheckBox::horizontalAlignment()
{
    return _horizontalAlignment;
}

void CheckBox::horizontalAlignment(HorizontalAlign value)
{
    _horizontalAlignment = value;
}

IProperty<VerticalAlign, ISharedControl>& CheckBox::verticalAlignment()
{
    return _verticalAlignment;
}

void CheckBox::verticalAlignment(VerticalAlign value)
{
    _verticalAlignment = value;
}

IProperty<std::string, ISharedControl>& CheckBox::text()
{
    return _text;
}

void CheckBox::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& CheckBox::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& CheckBox::DoubleClickEvent()
{
    return _dblClickEvent;
}