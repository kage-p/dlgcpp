#include "controls/textbox_p.h"
#include "dlgcpp/controls/textbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TextBox::TextBox(const std::string& text, const Position& p)
    : TextBox(std::make_shared<TextBoxImpl>(text, p))
{
    this->border(BorderStyle::Sunken);
}

TextBox::TextBox(std::shared_ptr<TextBoxImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

TextBox::~TextBox()
{
}

HorizontalAlign TextBox::horizontalAlignment() const
{
    return _impl->horizontalAlignment();
}

void TextBox::horizontalAlignment(HorizontalAlign value)
{
    if (_impl->horizontalAlignment() == value)
        return;
    _impl->horizontalAlignment(value);
}

int TextBox::maxChars() const
{
    return _impl->maxChars();
}

void TextBox::maxChars(int value)
{
    if (value < 0)
        value = 0;
    if (_impl->maxChars() == value)
        return;
    _impl->maxChars(value);
}

bool TextBox::password() const
{
    return _impl->password();
}

void TextBox::password(bool value)
{
    if (_impl->password() == value)
        return;
    _impl->password(value);
}

bool TextBox::readOnly() const
{
    return _impl->readOnly();
}

void TextBox::readOnly(bool value)
{
    if (_impl->readOnly() == value)
        return;
    _impl->readOnly(value);
}

bool TextBox::multiline() const
{
    return _impl->multiline();
}

void TextBox::multiline(bool value)
{
    if (_impl->multiline() == value)
        return;
    _impl->multiline(value);
}

bool TextBox::wrapText() const
{
    return _impl->wrapText();
}

void TextBox::wrapText(bool value)
{
    if (_impl->wrapText() == value)
        return;
    _impl->wrapText(value);
}

IEvent<ISharedControl>& TextBox::ChangedEvent()
{
    return _impl->ChangedEvent();
}
