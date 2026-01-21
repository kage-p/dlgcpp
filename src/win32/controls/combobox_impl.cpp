#include "combobox_impl.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ComboBoxImpl::ComboBoxImpl() :
    ControlImpl()
{
}

ComboBoxImpl::~ComboBoxImpl()
{
}

void ComboBoxImpl::owner(IComboBox* comboBox)
{
    ControlImpl::owner(comboBox);

    _comboBox = comboBox;
    _comboBox->dropDown().event() += [&](auto) { rebuild(); };
    _comboBox->editable().event() += [&](auto) { onEditableChanged(); };
    _comboBox->sorted().event() += [&](auto) { rebuild(); };
    _comboBox->items().event() += [&](auto) { onItemsChanged(); };
    _comboBox->text().event() += [&](auto) { onTextChanged(); };
    _comboBox->selectedIndex().event() += [&](auto) { updateSelection(); };
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
        }
        else if (HIWORD(msg.wParam) == CBN_DROPDOWN)
        {
            _comboBox->ListOpenEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == CBN_CLOSEUP)
        {
            _comboBox->ListCloseEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == CBN_DBLCLK)
        {
            _comboBox->DoubleClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == CBN_SETFOCUS)
        {
            _comboBox->focus() = true;
        }
        else if (HIWORD(msg.wParam) == CBN_KILLFOCUS)
        {
            _comboBox->focus() = false;
        }
    }
}

void ComboBoxImpl::build()
{
    ControlImpl::build();
    updateItems();
    updateSelection();
}

unsigned int ComboBoxImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles |= CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT | WS_VSCROLL;

    if (_comboBox->dropDown())
    {
        // combo has a dropdown list
        if (_comboBox->editable())
            styles |= CBS_DROPDOWN | CBS_AUTOHSCROLL;
        else
            styles |= CBS_DROPDOWNLIST;
    }
    else
    {
        styles |= CBS_SIMPLE | CBS_AUTOHSCROLL;
    }

    if (_comboBox->sorted())
    {
        styles |= CBS_SORT;
    }

    return styles;
}

bool ComboBoxImpl::isHandleEqual(void* otherHandle) const
{
    if (ControlImpl::isHandleEqual(otherHandle))
        return true;

    if (hwnd() == 0)
        return false;

    // match the handle with the list box portion
    auto cbInfo = COMBOBOXINFO();
    cbInfo.cbSize = sizeof(cbInfo);
    if (GetComboBoxInfo(hwnd(), &cbInfo) &&
        cbInfo.hwndList == otherHandle)
    {
        return true;
    }
    return false;
}

void ComboBoxImpl::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (hwnd() == 0)
        return;

    auto index = (int)SendMessage(hwnd(), CB_GETCURSEL, 0, 0);

    if (index != _comboBox->selectedIndex())
    {
        _comboBox->selectedIndex() = index;
        _comboBox->SelChangedEvent().invoke();
    }
}

void ComboBoxImpl::updateSelection()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), CB_SETCURSEL, (WPARAM)_comboBox->selectedIndex(), 0);
}

void ComboBoxImpl::onEditableChanged()
{
    if (!_comboBox->editable())
    {
        _comboBox->text().clear();
    }

    rebuild();
}

void ComboBoxImpl::onItemsChanged()
{
    updateItems();
    updateSelection();
}

void ComboBoxImpl::onTextChanged()
{
    setText(_comboBox->text());
}

void ComboBoxImpl::updateItems()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), CB_RESETCONTENT, 0, 0);
    readSelection(); // remove the selection

    auto items = _comboBox->items().value();
    for (const auto& item : items)
    {
        SendMessageW(hwnd(), CB_ADDSTRING, 0, (LPARAM)StringEncoder::toWide(item).c_str());
    }
}

void ComboBoxImpl::readInput()
{
    if (!_comboBox->editable())
        return;

    // if the control input has been changed by the user, save the content
    if (hwnd() == 0)
        return;

    _comboBox->text() = getText(hwnd());
}
