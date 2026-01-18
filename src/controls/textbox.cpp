#include "controls/textbox_impl.h"
#include "dlgcpp/controls/textbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TextBox::TextBox(const std::string& text, const Position& p)
    : TextBox(std::make_shared<TextBoxImpl>(), p)
{
    border() = BorderStyle::Sunken;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _maxChars.reset(0, [](int value) {return value >= 0; }, ownerFn, "maxChars");
    _password.reset(false, nullptr, ownerFn, "password");
    _readOnly.reset(false, nullptr, ownerFn, "readOnly");
    _multiline.reset(false, nullptr, ownerFn, "multiline");
    _wrapText.reset(false, nullptr, ownerFn, "wrapText");
    _horizontalAlignment.reset(HorizontalAlign::Left, nullptr, ownerFn, "horizontalAlignment");
    _text.reset(text, nullptr, ownerFn, "text");

    // events
    _changedEvent.reset(ownerFn, "ChangedEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

TextBox::TextBox(
    std::shared_ptr<TextBoxImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

TextBox::~TextBox()
{
    _impl.reset();
}

IProperty<HorizontalAlign, ISharedControl>& TextBox::horizontalAlignment()
{
    return _horizontalAlignment;
}

void TextBox::horizontalAlignment(HorizontalAlign value)
{
    _horizontalAlignment = value;
}

IProperty<int, ISharedControl>& TextBox::maxChars()
{
    return _maxChars;
}

void TextBox::maxChars(int value)
{
    _maxChars = value;
}

IProperty<bool, ISharedControl>& TextBox::password()
{
    return _password;
}

void TextBox::password(bool value)
{
    _password = value;
}

IProperty<bool, ISharedControl>& TextBox::readOnly()
{
    return _readOnly;
}

void TextBox::readOnly(bool value)
{
    _readOnly = value;
}

IProperty<bool, ISharedControl>& TextBox::multiline()
{
    return _multiline;
}

void TextBox::multiline(bool value)
{
    _multiline = value;
}

IProperty<bool, ISharedControl>& TextBox::wrapText()
{
    return _wrapText;
}

void TextBox::wrapText(bool value)
{
    _wrapText = value;
}

IProperty<std::string, ISharedControl>& TextBox::text()
{
    return _text;
}

void TextBox::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& TextBox::ChangedEvent()
{
    return _changedEvent;
}
