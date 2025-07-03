#include "../dlgmsg.h"
#include "../utility.h"
#include "listbox_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBox::ListBox(const Position& p) :
    Control(std::string(), p),
    _props(new listbox_props())
{
    this->border(BorderStyle::Sunken);
}

ListBox::~ListBox()
{
    delete _props;
}

IEvent<ISharedControl>& ListBox::SelChangedEvent()
{
    return _props->selChangedEvent;
}

IEvent<ISharedControl>& ListBox::SelCancelEvent()
{
    return _props->selCancelEvent;
}

std::string ListBox::className() const
{
    return "LISTBOX";
}

void ListBox::notify(dlg_message& msg)
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
            SelCancelEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == LBN_DBLCLK)
        {
            DoubleClickEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == LBN_SETFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), true);
        }
        else if (HIWORD(msg.wParam) == LBN_KILLFOCUS)
        {
            FocusEvent().invoke(shared_from_this(), false);
        }
    }
    Control::notify(msg);
}

void ListBox::rebuild()
{
    Control::rebuild();
    updateItems();
    updateSelection();
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
    if (_props->multiselect)
        return -1;
    return _props->currentIndex;
}

void ListBox::currentIndex(int value)
{
    if (_props->multiselect)
        return;
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

    SendMessage(hwnd, LB_SETCURSEL, (WPARAM)_props->currentIndex, 0);
}

const std::vector<int>& ListBox::currentIndexes() const
{
    static const std::vector<int> empty;
    if (!_props->multiselect)
        return empty;
    return _props->currentIndexes;
}

void ListBox::currentIndexes(const std::vector<int>& indexes)
{
    if (!_props->multiselect)
        return;
    if (_props->currentIndexes == indexes)
        return;

    _props->currentIndexes.clear();
    for (int index : indexes)
    {
        // skip invalid
        if (_props->items.empty() ||
            index < 0 ||
            index >(int)_props->items.size())
            continue;

        // skip duplicates
        if (std::find(_props->currentIndexes.begin(),
            _props->currentIndexes.end(),
            index) != _props->currentIndexes.end())
            continue;

        _props->currentIndexes.push_back(index);
    }

    updateSelection();
}

void ListBox::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_props->multiselect)
    {
        _props->currentIndexes.clear();
        _props->currentIndexes.resize(_props->items.size());
        auto count = (int)SendMessage(hwnd,
            LB_GETSELITEMS,
            (WPARAM)_props->currentIndexes.size(),
            (LPARAM)&_props->currentIndexes[0]);
        if (count < 1)
            _props->currentIndexes.clear();
        else
            _props->currentIndexes.resize(count);
    }
    else
    {
        auto index = (int)SendMessage(hwnd, LB_GETCURSEL, 0, 0);

        if (index != _props->currentIndex)
        {
            currentIndex(index);
            SelChangedEvent().invoke(shared_from_this());
        }
    }
}

void ListBox::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    if (_props->multiselect)
    {
        for (int index = 0; index < (int)_props->items.size(); index++)
        {
            bool selected = std::find(_props->currentIndexes.begin(),
                _props->currentIndexes.end(),
                index) != _props->currentIndexes.end();
            SendMessage(hwnd, LB_SETSEL, selected, index);
        }
    }
    else
    {
        SendMessage(hwnd, LB_SETCURSEL, (WPARAM)_props->currentIndex, 0);
    }
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
    updateSelection();
}

void ListBox::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
    readSelection(); // remove the selection

    for (const auto& item : _props->items)
    {
        SendMessageW(hwnd, LB_ADDSTRING, 0, (LPARAM)toWide(item).c_str());
    }
}
