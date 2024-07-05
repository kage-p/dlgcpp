#include "combobox_p.h"
#include "../dlgmsg.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ComboBox::ComboBox(const Position& p) :
    Control(std::string(), p),
    _props(new combobox_props())
{
    this->border(BorderStyle::Sunken);
}

ComboBox::~ComboBox()
{
    delete _props;
}

IEvent<ISharedControl>& ComboBox::SelChangedEvent()
{
    return _props->selChangedEvent;
}

IEvent<ISharedControl>& ComboBox::SelCancelEvent()
{
    return _props->selCancelEvent;
}

IEvent<ISharedControl>& ComboBox::ListCloseEvent()
{
    return _props->listCloseEvent;
}

IEvent<ISharedControl>& ComboBox::ListOpenEvent()
{
    return _props->listOpenEvent;
}

IEvent<ISharedControl>& ComboBox::TextChangedEvent()
{
    return _props->textChangedEvent;
}

std::string ComboBox::className() const
{
    return "COMBOBOX";
}

void ComboBox::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == CBN_SELCHANGE)
        {
            readSelection();
        }
        else if (HIWORD(msg.wParam) == CBN_EDITCHANGE)
        {
            readInput();
            TextChangedEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == CBN_DROPDOWN)
        {
            ListOpenEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == CBN_CLOSEUP)
        {
            ListCloseEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == CBN_DBLCLK)
        {
            DoubleClickEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == CBN_SETFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), true);
        }
        else if (HIWORD(msg.wParam) == CBN_KILLFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), false);
        }
    }
}

void ComboBox::rebuild()
{
    Control::rebuild();
    updateItems();
    updateSelection();
}

unsigned int ComboBox::styles() const
{
    auto styles = Control::styles();
    styles |= CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT | WS_VSCROLL;

    if (_props->dropDown)
    {
        // combo has a dropdown list
        if (_props->editable)
            styles |= CBS_DROPDOWN | CBS_AUTOHSCROLL;
        else
            styles |= CBS_DROPDOWNLIST;
    }
    else
    {
        styles |= CBS_SIMPLE | CBS_AUTOHSCROLL;
    }

    if (_props->sorted)
        styles |= CBS_SORT;

    return styles;
}

int ComboBox::currentIndex() const
{    
    return _props->currentIndex;
}

void ComboBox::currentIndex(int value)
{
    if (_props->currentIndex == value)
        return;

    _props->currentIndex = value;

    // TODO: override text()
    if (_props->currentIndex < _props->items.size())
        text(_props->items.at(_props->currentIndex));
    else
        text(std::string());

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, CB_SETCURSEL, (WPARAM)_props->currentIndex, 0);
}

void ComboBox::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    auto index = (int)SendMessage(hwnd, CB_GETCURSEL, 0, 0);

    if (index != _props->currentIndex)
    {
        currentIndex(index);
        SelChangedEvent().invoke(shared_from_this());
    }
}

void ComboBox::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, CB_SETCURSEL, (WPARAM)_props->currentIndex, 0);
}

bool ComboBox::dropDown() const
{
    return _props->dropDown;
}

void ComboBox::dropDown(bool value)
{
    if (_props->dropDown == value)
        return;

    _props->dropDown = value;
    rebuild();
}

bool ComboBox::editable() const
{
    return _props->editable;
}

void ComboBox::editable(bool value)
{
    if (_props->editable == value)
        return;

    _props->editable = value;
    rebuild();
}

bool ComboBox::sorted() const
{
    return _props->sorted;
}

void ComboBox::sorted(bool value)
{
    if (_props->sorted == value)
        return;

    _props->sorted = value;
    rebuild();
}

const std::vector<std::string>& ComboBox::items() const
{
    return _props->items;
}

void ComboBox::items(const std::vector<std::string>& items)
{
    _props->items = items;
    updateItems();
    updateSelection();
}

void ComboBox::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
    readSelection(); // remove the selection

    for (const auto& item : _props->items)
    {
        SendMessageW(hwnd, CB_ADDSTRING, 0, (LPARAM)toWide(item).c_str());
    }
}

void ComboBox::readInput()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto cb = (int)GetWindowTextLengthW(hwnd) + 1;
    std::wstring buf(cb, 0);
    GetWindowTextW(hwnd, &buf[0], cb);
    text(toBytes(buf.c_str()));
}
