#include "listbox_impl.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBoxImpl::ListBoxImpl() :
    ControlImpl()
{
}

ListBoxImpl::~ListBoxImpl()
{
}

void ListBoxImpl::owner(IListBox* listBox)
{
    ControlImpl::owner(listBox);
    _listBox = listBox;

    _listBox->highlight().event() += [&](auto) { rebuild(); };
    _listBox->multiselect().event() += [&](auto) { rebuild(); };
    _listBox->sorted().event() += [&](auto) { rebuild(); };
    _listBox->items().event() += [&](auto) { onItemsChanged(); };
    _listBox->selectedIndex().event() += [&](auto) { updateSelection(); };
    _listBox->selectedIndexes().event() += [&](auto) { updateSelection(); };
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
            _listBox->SelCancelEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == LBN_DBLCLK)
        {
            _listBox->DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == LBN_SETFOCUS)
        {
            _listBox->focus() = false;
        }
        else if (HIWORD(msg.wParam) == LBN_KILLFOCUS)
        {
            _listBox->focus() = true;
        }
    }
}

void ListBoxImpl::build()
{
    ControlImpl::build();
    updateItems();
    updateSelection();
}

unsigned int ListBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles |= LBS_NOTIFY | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL;

    if (!_listBox->highlight())
        styles |= LBS_NOSEL;

    if (_listBox->multiselect())
        styles |= LBS_MULTIPLESEL | LBS_EXTENDEDSEL;

    if (_listBox->sorted())
        styles |= LBS_SORT;

    return styles;
}

void ListBoxImpl::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (hwnd() == 0)
        return;

    if (_listBox->multiselect())
    {
        auto selectedIndexes = std::vector<int>();
        selectedIndexes.resize(_listBox->items()->size());
        auto count = (int)SendMessage(hwnd(),
            LB_GETSELITEMS,
            (WPARAM)selectedIndexes.size(),
            (LPARAM)&selectedIndexes[0]);

        if (count < 1)
            selectedIndexes.clear();
        else
            selectedIndexes.resize(count);

        _listBox->selectedIndexes() = selectedIndexes;
        _listBox->selectedIndex() = -1;
    }
    else
    {
        auto index = (int)SendMessage(hwnd(), LB_GETCURSEL, 0, 0);

        if (index != _listBox->selectedIndex())
        {
            _listBox->selectedIndex() = index;
            _listBox->SelChangedEvent().invoke();
        }

        _listBox->selectedIndexes() = std::vector<int>();
    }
}

void ListBoxImpl::updateSelection()
{
    if (hwnd() == 0)
        return;

    if (_listBox->multiselect())
    {
        auto itemCount = _listBox->items().value().size();
        const auto& selectedIndexes = _listBox->selectedIndexes().value();
        for (int index = 0; index < (int)itemCount; index++)
        {
            bool selected = std::find(
                selectedIndexes.begin(),
                selectedIndexes.end(),
                index) != selectedIndexes.end();

            SendMessage(hwnd(), LB_SETSEL, selected, index);
        }
    }
    else
    {
        SendMessage(hwnd(), LB_SETCURSEL, (WPARAM)_listBox->selectedIndex().value(), 0);
    }
}

void ListBoxImpl::onItemsChanged()
{
    updateItems();
    updateSelection();
}

void ListBoxImpl::updateItems()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), LB_RESETCONTENT, 0, 0);

    const auto& items = _listBox->items().value();
    for (const auto& item : items)
    {
        SendMessageW(hwnd(), LB_ADDSTRING, 0, (LPARAM)StringEncoder::toWide(item).c_str());
    }
}
