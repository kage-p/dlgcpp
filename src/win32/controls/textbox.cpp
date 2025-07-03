#include "../dlgmsg.h"
#include "../utility.h"
#include "textbox_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TextBox::TextBox(const std::string& text,
    const Position& p) :
    Control(text, p),
    _props(new textbox_props())
{
    this->border(BorderStyle::Sunken);
}

TextBox::~TextBox()
{
    delete _props;
}

void TextBox::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)_props->maxChars, 0);
}

std::string TextBox::className() const
{
    return "EDIT";
}

unsigned int TextBox::styles() const
{
    auto styles = Control::styles();
    if (_props->password)
        styles |= ES_PASSWORD;
    if (_props->readOnly)
        styles |= ES_READONLY;

    if (_props->multiline)
    {
        styles |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL;
        if (!_props->wrapText)
            styles |= ES_AUTOHSCROLL | WS_HSCROLL;
    }
    else
        styles |= ES_AUTOHSCROLL;

    switch (_props->horzAlign)
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

IEvent<ISharedControl>& TextBox::ChangedEvent()
{
    return _props->changedEvent;
}

void TextBox::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == EN_CHANGE)
        {
            readInput();
            ChangedEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == EN_SETFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), true);
        }
        else if (HIWORD(msg.wParam) == EN_KILLFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), false);
        }
    }
    Control::notify(msg);
}

void TextBox::readInput()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto cb = (int)GetWindowTextLengthW(hwnd) + 1;
    std::wstring buf(cb, 0);
    GetWindowTextW(hwnd, &buf[0], cb);
    text(toBytes(buf.c_str()));
}

HorizontalAlign TextBox::horizontalAlignment() const
{
    return _props->horzAlign;
}

void TextBox::horizontalAlignment(HorizontalAlign value)
{
    if (_props->horzAlign == value)
        return;
    _props->horzAlign = value;
    rebuild();
}

int TextBox::maxChars() const
{
    return _props->maxChars;
}

void TextBox::maxChars(int value)
{
    if (value < 0)
        value = 0;
    if (_props->maxChars == value)
        return;
    _props->maxChars = value;

    // auto truncate
    if (_props->maxChars > 0 && text().size() > (size_t)_props->maxChars)
        text(text().substr(0, (size_t)_props->maxChars));

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)_props->maxChars, 0);
}

bool TextBox::password() const
{
    return _props->password;
}

void TextBox::password(bool value)
{
    if (_props->password == value)
        return;
    _props->password = value;

    rebuild();
}

bool TextBox::readOnly() const
{
    return _props->readOnly;
}

void TextBox::readOnly(bool value)
{
    if (_props->readOnly == value)
        return;
    _props->readOnly = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, EM_SETREADONLY, (WPARAM)_props->readOnly, 0);
}

bool TextBox::multiline() const
{
    return _props->multiline;
}

void TextBox::multiline(bool value)
{
    if (_props->multiline == value)
        return;
    _props->multiline = value;
    rebuild();
}

bool TextBox::wrapText() const
{
    return _props->wrapText;
}

void TextBox::wrapText(bool value)
{
    if (_props->wrapText == value)
        return;
    _props->wrapText = value;
    rebuild();
}
