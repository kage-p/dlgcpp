#include "../dlgmsg.h"
#include "button_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Button::Button(const std::string& text, const Position& p) :
    Control(text, p),
    _props(new button_props())
{
}

Button::~Button()
{
    delete _props;
}

void Button::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    //auto hwnd = reinterpret_cast<HWND>(handle());
}

std::string Button::className() const
{
    return "BUTTON";
}

unsigned int Button::styles() const
{
    auto styles = Control::styles();

    styles |= BS_PUSHBUTTON | BS_NOTIFY;

    switch (_props->horzAlign)
    {
    case HorizontalAlign::Left:
        styles |= BS_LEFT;
        break;
    case HorizontalAlign::Center:
        styles |= BS_CENTER;
        break;
    case HorizontalAlign::Right:
        styles |= BS_RIGHT;
        break;
    }

    switch (_props->vertAlign)
    {
    case VerticalAlign::Top:
        styles |= BS_TOP;
        break;
    case VerticalAlign::Center:
        styles |= BS_VCENTER;
        break;
    case VerticalAlign::Bottom:
        styles |= BS_BOTTOM;
        break;
    }

    return styles;
}

void Button::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == BN_CLICKED)
        {
            ClickEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            DoubleClickEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), true);
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), false);
        }
    }

    Control::notify(msg);
}

HorizontalAlign Button::horizontalAlignment() const
{
    return _props->horzAlign;
}

void Button::horizontalAlignment(HorizontalAlign value)
{
    if (_props->horzAlign == value)
        return;
    _props->horzAlign = value;
    rebuild();
}

VerticalAlign Button::verticalAlignment() const
{
    return _props->vertAlign;
}

void Button::verticalAlignment(VerticalAlign value)
{
    if (_props->vertAlign == value)
        return;
    _props->vertAlign = value;
    rebuild();
}
