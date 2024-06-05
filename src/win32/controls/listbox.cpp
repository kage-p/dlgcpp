#include "listbox_p.h"
#include "../dlgmsg.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBox::ListBox(std::shared_ptr<IDialog> parent, const Position& p)
    : Control(parent), _props(new listbox_props())
{
    this->p(p);
}

ListBox::~ListBox()
{
    delete _props;
}

IEvent<>& ListBox::SelChangedEvent()
{
    return _props->selChangedEvent;
}

IEvent<>& ListBox::SelCancelEvent()
{
    return _props->selCancelEvent;
}

std::string ListBox::className() const
{
    return "LISTBOX";
}

unsigned int ListBox::exStyles() const
{
    return Control::exStyles() | WS_EX_CLIENTEDGE;
}

void ListBox::notify(struct dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == LBN_SELCHANGE)
        {
            SelChangedEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == LBN_SELCANCEL)
        {
            SelCancelEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == LBN_DBLCLK)
        {
            DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == LBN_SETFOCUS)
        {
            FocusEvent().invoke(true);
        }
        else if (HIWORD(msg.wParam) == LBN_KILLFOCUS)
        {
            FocusEvent().invoke(false);
        }
    }
}
