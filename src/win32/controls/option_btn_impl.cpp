#include "option_btn_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

OptionButtonImpl::OptionButtonImpl() :
    ControlImpl()
{
}

OptionButtonImpl::~OptionButtonImpl()
{
}

void OptionButtonImpl::owner(IOptionButton* option)
{
    ControlImpl::owner(option);

    _option = option;
    _option->checked().event() += [&](auto) { onCheckedChanged(); };
    _option->horizontalAlignment().event() += [&](auto) { rebuild(); };
    _option->verticalAlignment().event() += [&](auto) { rebuild(); };
    _option->text().event() += [&](auto) { setText(_option->text()); };
}

void OptionButtonImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    setText(_option->text());
    SendMessage(hwnd(), BM_SETCHECK, (WPARAM)_option->checked(), 0);
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

    switch (_option->horizontalAlignment())
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

    switch (_option->verticalAlignment())
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
            _option->ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            _option->DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            _option->focus() = true;
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            _option->focus() = false;
        }
    }
}

void OptionButtonImpl::onCheckedChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), BM_SETCHECK, (WPARAM)_option->checked(), 0);
}
