#include "checkbox_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

CheckBoxImpl::CheckBoxImpl(
    const std::string& text,
    const Position& p) :
    ControlImpl(text, p)
{
}

CheckBoxImpl::~CheckBoxImpl()
{
}

void CheckBoxImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_checked, 0);
}

std::string CheckBoxImpl::className() const
{
    return "BUTTON";
}

unsigned int CheckBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= (_autoCheck ? BS_AUTOCHECKBOX : BS_CHECKBOX) | BS_NOTIFY;

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

void CheckBoxImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == BN_CLICKED)
        {
            if (_autoCheck)
            {
                // handle auto check state
                auto hwnd = (HWND)handle();
                auto state = SendMessage(hwnd, BM_GETCHECK, 0, 0);
                checked(state);
            }

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

bool CheckBoxImpl::checked() const
{
    return _checked;
}

void CheckBoxImpl::checked(bool value)
{
    if (_checked == value)
        return;
    _checked = value;

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_checked, 0);
}

bool CheckBoxImpl::autoCheck() const
{
    return _autoCheck;
}

void CheckBoxImpl::autoCheck(bool value)
{
    if (_autoCheck == value)
        return;
    _autoCheck = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

HorizontalAlign CheckBoxImpl::horizontalAlignment() const
{
    return _horzAlign;
}

void CheckBoxImpl::horizontalAlignment(HorizontalAlign value)
{
    if (_horzAlign == value)
        return;
    _horzAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

VerticalAlign CheckBoxImpl::verticalAlignment() const
{
    return _vertAlign;
}

void CheckBoxImpl::verticalAlignment(VerticalAlign value)
{
    if (_vertAlign == value)
        return;
    _vertAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}
