#include "checkbox_p.h"
#include "../dlgmsg.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

CheckBox::CheckBox(const std::string& text, const Position& p) :
    Control(text, p),
    _props(new checkbox_props())
{
}

CheckBox::~CheckBox()
{
    delete _props;
}

void CheckBox::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_props->checked, 0);
}

std::string CheckBox::className() const
{
    return "BUTTON";
}

unsigned int CheckBox::styles() const
{
    auto styles = Control::styles();

    styles |= BS_CHECKBOX | BS_NOTIFY;

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

void CheckBox::notify(dlg_message& msg)
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
}

bool CheckBox::checked() const
{
    return _props->checked;
}

void CheckBox::checked(bool value)
{
    if (_props->checked == value)
        return;
    _props->checked = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_props->checked, 0);
}

HorizontalAlign CheckBox::horizontalAlignment() const
{
    return _props->horzAlign;
}

void CheckBox::horizontalAlignment(HorizontalAlign value)
{
    if (_props->horzAlign == value)
        return;
    _props->horzAlign = value;
    rebuild();
}

VerticalAlign CheckBox::verticalAlignment() const
{
    return _props->vertAlign;
}

void CheckBox::verticalAlignment(VerticalAlign value)
{
    if (_props->vertAlign == value)
        return;
    _props->vertAlign = value;
    rebuild();
}
