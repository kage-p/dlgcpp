#include "textbox_p.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TextBoxImpl::TextBoxImpl(
    const std::string& text,
    const Position& p) :
    ControlImpl(text, p)
{
    this->border(BorderStyle::Sunken);
}

TextBoxImpl::~TextBoxImpl()
{
}

void TextBoxImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)_maxChars, 0);
}

std::string TextBoxImpl::className() const
{
    return "EDIT";
}

unsigned int TextBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    if (_password)
        styles |= ES_PASSWORD;
    if (_readOnly)
        styles |= ES_READONLY;

    if (_multiline)
    {
        styles |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL;
        if (!_wrapText)
            styles |= ES_AUTOHSCROLL | WS_HSCROLL;
    }
    else
        styles |= ES_AUTOHSCROLL;

    switch (_horzAlign)
    {
    case HorizontalAlign::Left:
        styles |= ES_LEFT;
        break;
    case HorizontalAlign::Center:
        styles |= ES_CENTER;
        break;
    case HorizontalAlign::Right:
        styles |= ES_RIGHT;
        break;
    }

    return styles;
}

IEvent<ISharedControl>& TextBoxImpl::ChangedEvent()
{
    return _changedEvent;
}

void TextBoxImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == EN_CHANGE)
        {
            readInput();
            ChangedEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == EN_SETFOCUS)
        {
            FocusEvent().invoke(control(), true);
        }
        else if (HIWORD(msg.wParam) == EN_KILLFOCUS)
        {
            FocusEvent().invoke(control(), false);
        }
    }
    ControlImpl::notify(msg);
}

void TextBoxImpl::readInput()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto cb = (int)GetWindowTextLengthW(hwnd) + 1;
    std::wstring buf(cb, 0);
    GetWindowTextW(hwnd, &buf[0], cb);
    text(StringEncoder::toBytes(buf.c_str()));
}

HorizontalAlign TextBoxImpl::horizontalAlignment() const
{
    return _horzAlign;
}

void TextBoxImpl::horizontalAlignment(HorizontalAlign value)
{
    if (_horzAlign == value)
        return;
    _horzAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

int TextBoxImpl::maxChars() const
{
    return _maxChars;
}

void TextBoxImpl::maxChars(int value)
{
    if (value < 0)
        value = 0;
    if (_maxChars == value)
        return;
    _maxChars = value;

    // auto truncate
    if (_maxChars > 0 && text().size() > (size_t)_maxChars)
        text(text().substr(0, (size_t)_maxChars));

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)_maxChars, 0);
}

bool TextBoxImpl::password() const
{
    return _password;
}

void TextBoxImpl::password(bool value)
{
    if (_password == value)
        return;
    _password = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

bool TextBoxImpl::readOnly() const
{
    return _readOnly;
}

void TextBoxImpl::readOnly(bool value)
{
    if (_readOnly == value)
        return;
    _readOnly = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_SETREADONLY, (WPARAM)_readOnly, 0);
}

bool TextBoxImpl::multiline() const
{
    return _multiline;
}

void TextBoxImpl::multiline(bool value)
{
    if (_multiline == value)
        return;
    _multiline = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

bool TextBoxImpl::wrapText() const
{
    return _wrapText;
}

void TextBoxImpl::wrapText(bool value)
{
    if (_wrapText == value)
        return;
    _wrapText = value;

    if (handle() == nullptr)
        return;
    rebuild();
}
