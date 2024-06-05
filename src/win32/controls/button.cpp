#include "button_p.h"
#include "../dlgmsg.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Button::Button(std::shared_ptr<IDialog> parent, const std::string& text, const Position& p)
    : Control(parent), _props(new button_props())
{
    this->text(text);
    this->p(p);
}

Button::~Button()
{
    delete _props;
}

std::string Button::className() const
{
    return "BUTTON";
}

unsigned int Button::styles() const
{
    auto styles = Control::styles();

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

    return styles | BS_NOTIFY;
}

void Button::notify(struct dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == BN_CLICKED)
        {
            ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            FocusEvent().invoke(true);
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            FocusEvent().invoke(false);
        }
    }
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
