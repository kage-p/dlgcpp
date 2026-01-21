#include "controls/slider_impl.h"
#include "dlgcpp/controls/slider.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Slider::Slider(const std::string& text, const Position& p)
    : Slider(std::make_shared<SliderImpl>(), p)
{
    border() = BorderStyle::Thin;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
    _vertical.reset(false, nullptr, ownerFn, "vertical");
    _range.reset(std::make_pair(0, 100), nullptr, ownerFn, "range");
    _barColor.reset(Color::Default, nullptr, ownerFn, "barColor");
    _text.reset(text, nullptr, ownerFn, "text");

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
    _changedEvent.reset(ownerFn, "ChangedEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Slider::Slider(
    std::shared_ptr<SliderImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Slider::~Slider()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& Slider::vertical()
{
    return _vertical;
}

void Slider::vertical(bool value)
{
    _vertical = value;
}

IProperty<int, ISharedControl>& Slider::value()
{
    return _value;
}

void Slider::value(int value)
{
    _value = value;
}

IProperty<std::pair<int, int>, ISharedControl>& Slider::range()
{
    return _range;
}

void Slider::range(int from, int to)
{
    _range = std::make_pair(from, to);
}

IProperty<Color, ISharedControl>& Slider::barColor()
{
    return _barColor;
}

void Slider::barColor(Color value)
{
    _barColor = value;
}

IProperty<std::string, ISharedControl>& Slider::text()
{
    return _text;
}

void Slider::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& Slider::ChangedEvent()
{
    return _changedEvent;
}
