#include "controls/label_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Label::Label(const std::string& text, const Position& p)
    : Label(std::make_shared<LabelImpl>(*this, text, p))
{
}

Label::Label(std::shared_ptr<LabelImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Label::~Label()
{
}

bool Label::autoSize() const
{
    return _impl->autoSize();
}

void Label::autoSize(bool value)
{
    if (_impl->autoSize() == value)
        return;
    _impl->autoSize(value);
}

bool Label::clickable() const
{
    return _impl->clickable();
}

void Label::clickable(bool value)
{
    if (_impl->clickable() == value)
        return;
    _impl->clickable(value);
}

bool Label::elipsis() const
{
    return _impl->elipsis();
}

void Label::elipsis(bool value)
{
    if (_impl->elipsis() == value)
        return;
    _impl->elipsis(value);
}

HorizontalAlign Label::horizontalAlignment() const
{
    return _impl->horizontalAlignment();
}

void Label::horizontalAlignment(HorizontalAlign value)
{
    if (_impl->horizontalAlignment() == value)
        return;
    _impl->horizontalAlignment(value);
}

VerticalAlign Label::verticalAlignment() const
{
    return _impl->verticalAlignment();
}

void Label::verticalAlignment(VerticalAlign value)
{
    if (_impl->verticalAlignment() == value)
        return;

    _impl->verticalAlignment(value);
}
