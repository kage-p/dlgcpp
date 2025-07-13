#include "option_btn_p.h"
#include "utility/message.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

OptionButton::OptionButton(const std::string& text, const Position& p) :
    Control(text, p),
    _props(new optionbtn_props())
{
}

OptionButton::~OptionButton()
{
    delete _props;
}

void OptionButton::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_props->checked, 0);
}

std::string OptionButton::className() const
{
    return "BUTTON";
}

unsigned int OptionButton::styles() const
{
    auto styles = Control::styles();

    // note: auto style is not supported as we don't have a simple
    // method of reading the state from other controls in a group.
    styles |= BS_RADIOBUTTON | BS_NOTIFY;

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

void OptionButton::notify(dlg_message& msg)
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

bool OptionButton::checked() const
{
    return _props->checked;
}

void OptionButton::checked(bool value)
{
    if (_props->checked == value)
        return;
    _props->checked = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, BM_SETCHECK, (WPARAM)_props->checked, 0);
}

HorizontalAlign OptionButton::horizontalAlignment() const
{
    return _props->horzAlign;
}

void OptionButton::horizontalAlignment(HorizontalAlign value)
{
    if (_props->horzAlign == value)
        return;
    _props->horzAlign = value;
    rebuild();
}

VerticalAlign OptionButton::verticalAlignment() const
{
    return _props->vertAlign;
}

void OptionButton::verticalAlignment(VerticalAlign value)
{
    if (_props->vertAlign == value)
        return;
    _props->vertAlign = value;
    rebuild();
}
