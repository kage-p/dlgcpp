#include "controls/button_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Button::Button(const std::string& text, const Position& p)
    : Button(std::make_shared<ButtonImpl>(*this, text, p))
{
}

Button::Button(std::shared_ptr<ButtonImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Button::~Button()
{
}

HorizontalAlign Button::horizontalAlignment() const
{
    return _impl->horizontalAlignment();
}

void Button::horizontalAlignment(HorizontalAlign value)
{
    if (_impl->horizontalAlignment() == value)
        return;
    _impl->horizontalAlignment(value);
}

VerticalAlign Button::verticalAlignment() const
{
    return _impl->verticalAlignment();
}

void Button::verticalAlignment(VerticalAlign value)
{
    if (_impl->verticalAlignment() == value)
        return;
    _impl->verticalAlignment(value);
}
