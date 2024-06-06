#include "listbox_p.h"
#include "../dlgmsg.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBox::ListBox(std::shared_ptr<IDialog> parent, const Position& p)
    : Control(parent), _props(new listbox_props())
{
    this->p(p);
    this->border(BorderStyle::Sunken);
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

void ListBox::notify(struct dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == LBN_SELCHANGE)
        {
            readSelection();
        }
        else if (HIWORD(msg.wParam) == LBN_SELCANCEL)
        {
            readSelection();
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

void ListBox::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto index = (int)SendMessage(hwnd, LB_GETCURSEL, 0, 0);

    if (index != _props->currentIndex)
    {
        currentIndex(index);
        SelChangedEvent().invoke();
    }
}

void ListBox::rebuild()
{
    Control::rebuild();
    updateItems();
}

unsigned int ListBox::styles() const
{
    auto styles = Control::styles();
    styles |= LBS_NOTIFY | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL;

    if (!_props->highlight)
        styles |= LBS_NOSEL;

    if (_props->multiselect)
        styles |= LBS_MULTIPLESEL | LBS_EXTENDEDSEL;

    if (_props->sorted)
        styles |= LBS_SORT;

    return styles;
}

int ListBox::currentIndex() const
{
    return _props->currentIndex;
}

void ListBox::currentIndex(int value)
{
    if (_props->currentIndex == value)
        return;

    _props->currentIndex = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LB_SETCURSEL, (WPARAM)_props->currentIndex, 0);
}

bool ListBox::highlight() const
{
    return _props->highlight;
}

void ListBox::highlight(bool value)
{
    if (_props->highlight == value)
        return;

    _props->highlight = value;
    rebuild();
}

bool ListBox::multiselect() const
{
    return _props->multiselect;
}

void ListBox::multiselect(bool value)
{
    if (_props->multiselect == value)
        return;

    _props->multiselect = value;
    rebuild();
}

bool ListBox::sorted() const
{
    return _props->sorted;
}

void ListBox::sorted(bool value)
{
    if (_props->sorted == value)
        return;

    _props->sorted = value;
    rebuild();
}

const std::vector<std::string>& ListBox::items() const
{
    return _props->items;
}

void ListBox::items(const std::vector<std::string>& items)
{
    _props->items = items;
    updateItems();
}

void ListBox::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
    for (const auto& item : _props->items)
    {
        SendMessageW(hwnd, LB_ADDSTRING, 0, (LPARAM)toWide(item).c_str());
    }
    readSelection();
}
