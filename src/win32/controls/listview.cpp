#include "listview_p.h"
#include "../dlgmsg.h"
#include "../utility.h"
#include <strsafe.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListView::ListView(const Position& p) :
    Control(std::string(), p),
    _priv(new listview_priv())
{
    this->border(BorderStyle::Sunken);
    _priv->columnsChangedEvent += (std::function<void()>)std::bind(&ListView::onColumnsChanged, this);
    _priv->itemsChangedEvent += (std::function<void()>)std::bind(&ListView::onItemsChanged, this);
}

ListView::~ListView()
{
    delete _priv;
}

IEvent<ISharedControl>& ListView::SelChangedEvent()
{
    return _priv->selChangedEvent;
}

IEvent<ISharedControl>& ListView::ColumnsChangedEvent()
{
    return _priv->columnsChangedEvent;
}

IEvent<ISharedControl>& ListView::ItemsChangedEvent()
{
    return _priv->itemsChangedEvent;
}

std::string ListView::className() const
{
    return WC_LISTVIEWA;
}

unsigned int ListView::styles() const
{
    auto styles = Control::styles();

    styles |= LVS_OWNERDATA;

    if (!_priv->multiselect)
        styles |= LVS_SINGLESEL;

    switch (_priv->_disp)
    {
    case ListViewDisplay::Details:
        styles |= LVS_REPORT;
        break;
    case ListViewDisplay::LargeIcon:
        styles |= LVS_ICON;
        break;
    case ListViewDisplay::List:
        styles |= LVS_LIST;
        break;
    case ListViewDisplay::SmallIcon:
        styles |= LVS_SMALLICON;
        break;
    }

    return styles;
}

void ListView::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        // TODO: notification to events
        auto nmhdr = *((NMHDR*)msg.lParam);
        if (nmhdr.code == LVN_GETDISPINFO)
        {
            auto nmdi = *((NMLVDISPINFO*)msg.lParam);

            if (!(nmdi.item.mask & LVIF_TEXT))
                // text change only
                return;

            // TODO: LVN_GETDISPINFO should own buffer or it will be restricted in size.

            // extract the role
            const auto cols = _priv->columns;
            if (cols.empty() || nmdi.item.iSubItem >= (int)cols.size())
                return;

            int role = cols.at(nmdi.item.iSubItem)->role();
            auto text = data(nmdi.item.iItem, role);
            StringCchCopyW(nmdi.item.pszText, nmdi.item.cchTextMax, toWide(text).c_str());
        }
        else if (nmhdr.code == LVN_ITEMCHANGED)
        {
            auto nmlv = *((NMLISTVIEW*)msg.lParam);

            if (!(nmlv.uChanged & LVIF_STATE))
                return;
            if (!(nmlv.uOldState & LVIS_SELECTED) && !(nmlv.uNewState & LVIS_SELECTED))
                return;

            // selection changed
            readSelection();
        }
    }
}

void ListView::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto dwStyle = (UINT)SendMessage(hwnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
    SendMessage(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    _priv->columns = columns();
    updateColumns();
    updateItems();
    updateSelection();
}

int ListView::currentIndex() const
{
    if (_priv->multiselect)
        return -1;
    return _priv->currentIndex;
}

void ListView::currentIndex(int value)
{
    if (_priv->multiselect)
        return;
    if (_priv->currentIndex == value)
        return;

    _priv->currentIndex = value;
    updateSelection();
}

const std::vector<int>& ListView::currentIndexes() const
{
    static const std::vector<int> empty;
    if (!_priv->multiselect)
        return empty;
    return _priv->currentIndexes;
}

void ListView::currentIndexes(const std::vector<int>& indexes)
{
    if (!_priv->multiselect)
        return;
    if (_priv->currentIndexes == indexes)
        return;

    _priv->currentIndexes.clear();
    for (int index : indexes)
    {
        // skip invalid
        if (index < 0 ||
            index > (int)count())
            continue;

        // skip duplicates
        if (std::find(_priv->currentIndexes.begin(),
                      _priv->currentIndexes.end(),
                      index) != _priv->currentIndexes.end())
            continue;

        _priv->currentIndexes.push_back(index);
    }

    updateSelection();
}

void ListView::readSelection()
{
    // if the control input has been changed by the user, save the content
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_priv->multiselect)
    {
        _priv->currentIndexes.clear();

        int index = -1;
        for (;;)
        {
            index = (int)SendMessage(hwnd, LVM_GETNEXTITEM, index, LVNI_SELECTED);
            if (index < 0)
                break;
            _priv->currentIndexes.push_back(index);
        }
    }
    else
    {
        int index = (int)SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

        if (index != _priv->currentIndex)
        {
            currentIndex(index);
            SelChangedEvent().invoke(shared_from_this());
        }
    }
}

void ListView::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    if (_priv->multiselect)
    {
        // deselect all
        auto lvi = LVITEM();
        lvi.stateMask = LVIS_SELECTED;
        SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&lvi);

        for (int index : _priv->currentIndexes)
        {
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&lvi);
        }
    }
    else
    {
        auto lvi = LVITEM();
        lvi.state = LVIS_SELECTED;
        lvi.stateMask = LVIS_SELECTED;
        SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)_priv->currentIndex, (LPARAM)&lvi);
    }
}

bool ListView::multiselect() const
{
    return _priv->multiselect;
}

void ListView::multiselect(bool value)
{
    if (_priv->multiselect == value)
        return;
    _priv->multiselect = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

ListViewDisplay ListView::display() const
{
    return _priv->_disp;
}

void ListView::display(ListViewDisplay value)
{
    if (_priv->_disp == value)
        return;

    _priv->_disp = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

// std::vector<ISharedListViewColumn> ListView::columns() const
// {
//     return std::vector<ISharedListViewColumn>();
// }

// std::string ListView::data(int, int) const
// {
//     return std::string();
// }

// size_t ListView::count() const
// {
//     return 0;
// }

void ListView::updateColumns()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto cols = _priv->columns;
    int index = 0;
    for (auto& col : cols)
    {
        auto text = toWide(col->text());
        auto lvc = LVCOLUMNW();
        lvc.mask = LVCF_TEXT | LVCF_WIDTH;
        lvc.pszText = &text[0];

        Size s(col->width(), 0);
        toPixels(GetParent(hwnd), s);
        lvc.cx = s.width();

        SendMessageW(hwnd, LVM_INSERTCOLUMNW, (WPARAM)index, (LPARAM)&lvc);
        index++;
    }
}

void ListView::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LVM_SETITEMCOUNT, (WPARAM)count(), 0);
}

void ListView::onColumnsChanged()
{
    _priv->columns = columns();
    updateColumns();
}

void ListView::onItemsChanged()
{
    updateItems();
}
