#include "controls/slider_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Slider::Slider(const std::string& text, const Position& p)
    : Slider(std::make_shared<SliderImpl>(*this, text, p))
{
    this->border(BorderStyle::Thin);
}

Slider::Slider(std::shared_ptr<SliderImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Slider::~Slider()
{
}

bool Slider::vertical() const
{
    return _impl->vertical();
}

void Slider::vertical(bool value)
{
    if (_impl->vertical() == value)
        return;
    _impl->vertical(value);
}

int Slider::value() const
{
    return _impl->value();
}

void Slider::value(int value)
{
    if (_impl->value() == value)
        return;
    _impl->value(value);
}

std::pair<int, int> Slider::range() const
{
    return _impl->range();
}

void Slider::range(int from, int to)
{
    if (_impl->range() == std::pair<int, int>(from, to))
        return;
    _impl->range(from, to);
}

Color Slider::barColor() const
{
    return _impl->barColor();
}

void Slider::barColor(Color value)
{
    if (_impl->barColor() == value)
        return;
    _impl->barColor(value);
}

IEvent<ISharedControl>& Slider::ChangedEvent()
{
    return _impl->ChangedEvent();
}
