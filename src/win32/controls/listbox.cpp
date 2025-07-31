#include "listbox_p.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBoxImpl::ListBoxImpl(
    const Position& p) :
    ControlImpl(std::string(), p)
{
    this->border(BorderStyle::Sunken);
}

ListBoxImpl::~ListBoxImpl()
{
}

std::string ListBoxImpl::className() const
{
    return "LISTBOX";
}

void ListBoxImpl::notify(DialogMessage& msg)
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
            SelCancelEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == LBN_DBLCLK)
        {
            DoubleClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == LBN_SETFOCUS)
        {
            FocusEvent().invoke(control(), true);
        }
        else if (HIWORD(msg.wParam) == LBN_KILLFOCUS)
        {
            FocusEvent().invoke(control(), false);
        }
    }
    ControlImpl::notify(msg);
}

void ListBoxImpl::rebuild()
{
    ControlImpl::rebuild();
    updateItems();
    updateSelection();
}

unsigned int ListBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles |= LBS_NOTIFY | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL;

    if (!_highlight)
        styles |= LBS_NOSEL;

    if (_multiselect)
        styles |= LBS_MULTIPLESEL | LBS_EXTENDEDSEL;

    if (_sorted)
        styles |= LBS_SORT;

    return styles;
}

int ListBoxImpl::currentIndex() const
{
    if (_multiselect)
        return -1;
    return _currentIndex;
}

void ListBoxImpl::currentIndex(int value)
{
    if (_multiselect)
        return;
    if (_currentIndex == value)
        return;

    _currentIndex = value;

    // TODO: override text()
    if (_currentIndex < _items.size())
        text(_items.at(_currentIndex));
    else
        text(std::string());

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LB_SETCURSEL, (WPARAM)_currentIndex, 0);
}

const std::vector<int>& ListBoxImpl::currentIndexes() const
{
    static const std::vector<int> empty;
    if (!_multiselect)
        return empty;
    return _currentIndexes;
}

void ListBoxImpl::currentIndexes(const std::vector<int>& indexes)
{
    if (!_multiselect)
        return;
    if (_currentIndexes == indexes)
        return;

    _currentIndexes.clear();
    for (int index : indexes)
    {
        // skip invalid
        if (_items.empty() ||
            index < 0 ||
            index >(int)_items.size())
            continue;

        // skip duplicates
        if (std::find(_currentIndexes.begin(),
            _currentIndexes.end(),
            index) != _currentIndexes.end())
            continue;

        _currentIndexes.push_back(index);
    }

    updateSelection();
}

void ListBoxImpl::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_multiselect)
    {
        _currentIndexes.clear();
        _currentIndexes.resize(_items.size());
        auto count = (int)SendMessage(hwnd,
            LB_GETSELITEMS,
            (WPARAM)_currentIndexes.size(),
            (LPARAM)&_currentIndexes[0]);
        if (count < 1)
            _currentIndexes.clear();
        else
            _currentIndexes.resize(count);
    }
    else
    {
        auto index = (int)SendMessage(hwnd, LB_GETCURSEL, 0, 0);

        if (index != _currentIndex)
        {
            currentIndex(index);
            SelChangedEvent().invoke(control());
        }
    }
}

void ListBoxImpl::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    if (_multiselect)
    {
        for (int index = 0; index < (int)_items.size(); index++)
        {
            bool selected = std::find(_currentIndexes.begin(),
                _currentIndexes.end(),
                index) != _currentIndexes.end();
            SendMessage(hwnd, LB_SETSEL, selected, index);
        }
    }
    else
    {
        SendMessage(hwnd, LB_SETCURSEL, (WPARAM)_currentIndex, 0);
    }
}

bool ListBoxImpl::highlight() const
{
    return _highlight;
}

void ListBoxImpl::highlight(bool value)
{
    if (_highlight == value)
        return;

    _highlight = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

bool ListBoxImpl::multiselect() const
{
    return _multiselect;
}

void ListBoxImpl::multiselect(bool value)
{
    if (_multiselect == value)
        return;

    _multiselect = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

bool ListBoxImpl::sorted() const
{
    return _sorted;
}

void ListBoxImpl::sorted(bool value)
{
    if (_sorted == value)
        return;

    _sorted = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

const std::vector<std::string>& ListBoxImpl::items() const
{
    return _items;
}

void ListBoxImpl::items(const std::vector<std::string>& items)
{
    _items = items;
    updateItems();
    updateSelection();
}

void ListBoxImpl::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
    readSelection(); // remove the selection

    for (const auto& item : _items)
    {
        SendMessageW(hwnd, LB_ADDSTRING, 0, (LPARAM)StringEncoder::toWide(item).c_str());
    }
}

IEvent<ISharedControl>& ListBoxImpl::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl>& ListBoxImpl::SelCancelEvent()
{
    return _selCancelEvent;
}
