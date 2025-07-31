#include "controls/checkbox_p.h"
#include "dlgcpp/controls/checkbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

CheckBox::CheckBox(const std::string& text, const Position& p)
    : CheckBox(std::make_shared<CheckBoxImpl>(text, p))
{
}

CheckBox::CheckBox(std::shared_ptr<CheckBoxImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

CheckBox::~CheckBox()
{
}

bool CheckBox::checked() const
{
    return _impl->checked();
}

void CheckBox::checked(bool value)
{
    if (_impl->checked() == value)
        return;
    _impl->checked(value);
}

bool CheckBox::autoCheck() const
{
    return _impl->autoCheck();
}

void CheckBox::autoCheck(bool value)
{
    if (_impl->autoCheck() == value)
        return;
    _impl->autoCheck(value);
}

HorizontalAlign CheckBox::horizontalAlignment() const
{
    return _impl->horizontalAlignment();
}

void CheckBox::horizontalAlignment(HorizontalAlign value)
{
    if (_impl->horizontalAlignment() == value)
        return;
    _impl->horizontalAlignment(value);
}

VerticalAlign CheckBox::verticalAlignment() const
{
    return _impl->verticalAlignment();
}

void CheckBox::verticalAlignment(VerticalAlign value)
{
    if (_impl->verticalAlignment() == value)
        return;
    _impl->verticalAlignment(value);
}
