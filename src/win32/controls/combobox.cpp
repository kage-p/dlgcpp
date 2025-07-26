#include "combobox_p.h"
#include "utility/string.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ComboBoxImpl::ComboBoxImpl(
    ComboBox& comboBox,
    const Position& p) :
    ControlImpl(comboBox, std::string(), p),
    _comboBox(comboBox)
{
    this->border(BorderStyle::Sunken);
}

ComboBoxImpl::~ComboBoxImpl()
{
}

std::string ComboBoxImpl::className() const
{
    return "COMBOBOX";
}

void ComboBoxImpl::notify(DialogMessage& msg)
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
            TextChangedEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == CBN_DROPDOWN)
        {
            ListOpenEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == CBN_CLOSEUP)
        {
            ListCloseEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == CBN_DBLCLK)
        {
            DoubleClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == CBN_SETFOCUS)
        {
            FocusEvent().invoke(control(), true);
        }
        else if (HIWORD(msg.wParam) == CBN_KILLFOCUS)
        {
            FocusEvent().invoke(control(), false);
        }
    }

    ControlImpl::notify(msg);
}

void ComboBoxImpl::rebuild()
{
    ControlImpl::rebuild();
    updateItems();
    updateSelection();
}

unsigned int ComboBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles |= CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT | WS_VSCROLL;

    if (_dropDown)
    {
        // combo has a dropdown list
        if (_editable)
            styles |= CBS_DROPDOWN | CBS_AUTOHSCROLL;
        else
            styles |= CBS_DROPDOWNLIST;
    }
    else
    {
        styles |= CBS_SIMPLE | CBS_AUTOHSCROLL;
    }

    if (_sorted)
        styles |= CBS_SORT;

    return styles;
}

bool ComboBoxImpl::isHandleEqual(void* otherHandle) const
{
    if (ControlImpl::isHandleEqual(otherHandle))
        return true;

    auto hwnd = reinterpret_cast<HWND>(handle());
    if (hwnd == 0)
        return false;

    // match the handle with the list box portion
    auto cbInfo = COMBOBOXINFO();
    cbInfo.cbSize = sizeof(cbInfo);
    if (GetComboBoxInfo(hwnd, &cbInfo) &&
        cbInfo.hwndList == otherHandle)
    {
        return true;
    }
    return false;
}

int ComboBoxImpl::currentIndex() const
{
    return _currentIndex;
}

void ComboBoxImpl::currentIndex(int value)
{
    if (_currentIndex == value)
        return;

    _currentIndex = value;

    // TODO: override text()
    if (_currentIndex < _items.size())
        text(_items.at(_currentIndex));
    else
        text(std::string());

    updateSelection();
}

void ComboBoxImpl::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    auto index = (int)SendMessage(hwnd, CB_GETCURSEL, 0, 0);

    if (index != _currentIndex)
    {
        currentIndex(index);
        SelChangedEvent().invoke(control());
    }
}

void ComboBoxImpl::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, CB_SETCURSEL, (WPARAM)_currentIndex, 0);
}

bool ComboBoxImpl::dropDown() const
{
    return _dropDown;
}

void ComboBoxImpl::dropDown(bool value)
{
    if (_dropDown == value)
        return;

    _dropDown = value;
    rebuild();
}

bool ComboBoxImpl::editable() const
{
    return _editable;
}

void ComboBoxImpl::editable(bool value)
{
    if (_editable == value)
        return;

    _editable = value;
    rebuild();
}

bool ComboBoxImpl::sorted() const
{
    return _sorted;
}

void ComboBoxImpl::sorted(bool value)
{
    if (_sorted == value)
        return;

    _sorted = value;
    rebuild();
}

const std::vector<std::string>& ComboBoxImpl::items() const
{
    return _items;
}

void ComboBoxImpl::items(const std::vector<std::string>& items)
{
    _items = items;
    updateItems();
    updateSelection();
}

void ComboBoxImpl::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
    readSelection(); // remove the selection

    for (const auto& item : _items)
    {
        SendMessageW(hwnd, CB_ADDSTRING, 0, (LPARAM)toWide(item).c_str());
    }
}

void ComboBoxImpl::readInput()
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

IEvent<ISharedControl>& ComboBoxImpl::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl>& ComboBoxImpl::SelCancelEvent()
{
    return _selCancelEvent;
}

IEvent<ISharedControl>& ComboBoxImpl::ListCloseEvent()
{
    return _listCloseEvent;
}

IEvent<ISharedControl>& ComboBoxImpl::ListOpenEvent()
{
    return _listOpenEvent;
}

IEvent<ISharedControl>& ComboBoxImpl::TextChangedEvent()
{
    return _textChangedEvent;
}
