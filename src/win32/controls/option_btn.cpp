#include "option_btn_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

OptionButtonImpl::OptionButtonImpl(
    const std::string& text,
    const Position& p) :
    ControlImpl(text, p)
{
}

OptionButtonImpl::~OptionButtonImpl()
{
}

void OptionButtonImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_checked, 0);
}

std::string OptionButtonImpl::className() const
{
    return "BUTTON";
}

unsigned int OptionButtonImpl::styles() const
{
    auto styles = ControlImpl::styles();

    // note: auto style is not supported as we don't have a simple
    // method of reading the state from other controls in a group.
    styles |= BS_RADIOBUTTON | BS_NOTIFY;

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

void OptionButtonImpl::notify(DialogMessage& msg)
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

bool OptionButtonImpl::checked() const
{
    return _checked;
}

void OptionButtonImpl::checked(bool value)
{
    if (_checked == value)
        return;
    _checked = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_checked, 0);
}

HorizontalAlign OptionButtonImpl::horizontalAlignment() const
{
    return _horzAlign;
}

void OptionButtonImpl::horizontalAlignment(HorizontalAlign value)
{
    if (_horzAlign == value)
        return;
    _horzAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

VerticalAlign OptionButtonImpl::verticalAlignment() const
{
    return _vertAlign;
}

void OptionButtonImpl::verticalAlignment(VerticalAlign value)
{
    if (_vertAlign == value)
        return;
    _vertAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}
