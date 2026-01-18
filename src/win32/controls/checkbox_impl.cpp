#include "checkbox_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

CheckBoxImpl::CheckBoxImpl() :
    ControlImpl()
{
}

CheckBoxImpl::~CheckBoxImpl()
{
}

void CheckBoxImpl::owner(ICheckBox* checkBox)
{
    ControlImpl::owner(checkBox);

    _checkBox = checkBox;
    _checkBox->checked().event() += [&](auto) { onCheckedChanged(); };
    _checkBox->autoCheck().event() += [&](auto) { rebuild(); };
    _checkBox->horizontalAlignment().event() += [&](auto) { rebuild(); };
    _checkBox->verticalAlignment().event() += [&](auto) { rebuild(); };
    _checkBox->text().event() += [&](auto) { setText(_checkBox->text()); };
}

void CheckBoxImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    setText(_checkBox->text());
    SendMessage(hwnd(), BM_SETCHECK, (WPARAM)_checkBox->checked(), 0);
}

std::string CheckBoxImpl::className() const
{
    return "BUTTON";
}

unsigned int CheckBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= (_checkBox->autoCheck() ? BS_AUTOCHECKBOX : BS_CHECKBOX) | BS_NOTIFY;

    switch (_checkBox->horizontalAlignment())
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

    switch (_checkBox->verticalAlignment())
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
            if (_checkBox->autoCheck())
            {
                // handle auto check state
                auto state = SendMessage(hwnd(), BM_GETCHECK, 0, 0);
                _checkBox->checked() = state;
            }

            _checkBox->ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_DBLCLK)
        {
            _checkBox->DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == BN_SETFOCUS)
        {
            _checkBox->focus() = true;
        }
        else if (HIWORD(msg.wParam) == BN_KILLFOCUS)
        {
            _checkBox->focus() = false;
        }
    }
}

void CheckBoxImpl::onCheckedChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), BM_SETCHECK, (WPARAM)_checkBox->checked(), 0);
}
