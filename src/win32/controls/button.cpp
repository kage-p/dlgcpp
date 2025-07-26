#include "button_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ButtonImpl::ButtonImpl(
    Button& button,
    const std::string& text,
    const Position& p) :
    ControlImpl(button, text, p),
    _button(button)
{
}

ButtonImpl::~ButtonImpl()
{
}

std::string ButtonImpl::className() const
{
    return "BUTTON";
}

unsigned int ButtonImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= BS_PUSHBUTTON | BS_NOTIFY;

    switch (_horzAlign)
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

    switch (_vertAlign)
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

void ButtonImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == BN_CLICKED)
        {
            ClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            DoubleClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            FocusEvent().invoke(control(), true);
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            FocusEvent().invoke(control(), false);
        }
    }

    ControlImpl::notify(msg);
}

HorizontalAlign ButtonImpl::horizontalAlignment() const
{
    return _horzAlign;
}

void ButtonImpl::horizontalAlignment(HorizontalAlign value)
{
    if (_horzAlign == value)
        return;
    _horzAlign = value;
    rebuild();
}

VerticalAlign ButtonImpl::verticalAlignment() const
{
    return _vertAlign;
}

void ButtonImpl::verticalAlignment(VerticalAlign value)
{
    if (_vertAlign == value)
        return;
    _vertAlign = value;
    rebuild();
}
