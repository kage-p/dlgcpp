#include "controls/option_btn_p.h"
#include "dlgcpp/controls/option_btn.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

OptionButton::OptionButton(const std::string& text, const Position& p)
    : OptionButton(std::make_shared<OptionButtonImpl>(text, p))
{
}

OptionButton::OptionButton(std::shared_ptr<OptionButtonImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

OptionButton::~OptionButton()
{
}

bool OptionButton::checked() const
{
    return _impl->checked();
}

void OptionButton::checked(bool value)
{
    if (_impl->checked() == value)
        return;
    _impl->checked(value);
}

HorizontalAlign OptionButton::horizontalAlignment() const
{
    return _impl->horizontalAlignment();
}

void OptionButton::horizontalAlignment(HorizontalAlign value)
{
    if (_impl->horizontalAlignment() == value)
        return;
    _impl->horizontalAlignment(value);
}

VerticalAlign OptionButton::verticalAlignment() const
{
    return _impl->verticalAlignment();
}

void OptionButton::verticalAlignment(VerticalAlign value)
{
    if (_impl->verticalAlignment() == value)
        return;
    _impl->verticalAlignment(value);
}
