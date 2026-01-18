#include "button_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ButtonImpl::ButtonImpl() :
    ControlImpl()
{
}

ButtonImpl::~ButtonImpl()
{
}

void ButtonImpl::owner(IButton* button)
{
    ControlImpl::owner(button);

    _button = button;
    _button->horizontalAlignment().event() += [&](auto) { rebuild(); };
    _button->verticalAlignment().event() += [&](auto) { rebuild(); };
    _button->text().event() += [&](auto) { setText(_button->text()); };
}

void ButtonImpl::build()
{
    ControlImpl::build();

    setText(_button->text());
}

std::string ButtonImpl::className() const
{
    return "BUTTON";
}

unsigned int ButtonImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= BS_PUSHBUTTON | BS_NOTIFY;

    switch (_button->horizontalAlignment())
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

    switch (_button->verticalAlignment())
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
            _button->ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            _button->DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            _button->focus() = true;
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            _button->focus() = false;
        }
    }
}