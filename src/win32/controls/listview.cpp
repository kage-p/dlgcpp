#include "../dlgmsg.h"
#include "../utility.h"
#include "listview_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListView::ListView(const Position& p) :
    Control(std::string(), p),
    _priv(new listview_priv())
{
    this->border(BorderStyle::Sunken);
    _priv->columnsChangedEvent += (std::function<void()>)std::bind(&ListView::onColumnsChanged, this);
    _priv->itemsChangedEvent += (std::function<void()>)std::bind(&ListView::onItemsChanged, this);
    _priv->selChangedEventInternal += (std::function<void()>)std::bind(&ListView::onSelectionChanged, this);
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
        auto& nmhdr = *((NMHDR*)msg.lParam);
        if (nmhdr.code == LVN_GETDISPINFO)
        {
            auto& nmdi = *((NMLVDISPINFO*)msg.lParam);

            if (nmdi.item.mask & LVIF_TEXT)
            {
                // check the role
                const auto& cols = _priv->columns;
                if (cols.empty() || nmdi.item.iSubItem >= (int)cols.size())
                    return;

                // extract text
                int role = cols.at(nmdi.item.iSubItem)->role();
                auto text = data(nmdi.item.iItem, role);

                _priv->_displayBuffer = toWide(text);
                nmdi.item.pszText = _priv->_displayBuffer.data();
            }

            if (nmdi.item.mask & LVIF_STATE)
            {
                bool isChecked = checked(nmdi.item.iItem);
                nmdi.item.stateMask = LVIS_STATEIMAGEMASK;
                nmdi.item.state = INDEXTOSTATEIMAGEMASK(isChecked ? 2 : 1);
            }
        }
        else if (nmhdr.code == LVN_ITEMCHANGED)
        {
            auto& nmlv = *((NMLISTVIEW*)msg.lParam);

            if (nmlv.uChanged & LVIF_STATE)
            {
                // item selection state changed
                // fire internal event
                _priv->selChangedEventInternal.invoke(shared_from_this());
            }
        }
        else if (nmhdr.code == NM_CLICK)
        {
            auto& nmclick = *(LPNMITEMACTIVATE)msg.lParam;

            auto hit = LVHITTESTINFO();
            hit.pt = nmclick.ptAction;

            ListView_SubItemHitTest(nmhdr.hwndFrom, &hit);
            if ((hit.flags & LVHT_ONITEMSTATEICON) && (hit.iItem >= 0))
            {
                checked(nmclick.iItem, !checked(nmclick.iItem));

                // Force redraw
                ListView_RedrawItems(nmhdr.hwndFrom, hit.iItem, hit.iItem);
            }
        }
    }

    Control::notify(msg);
}

void ListView::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;

    _priv->columns = columns();
    updateColumns();
    updateItems();
    updateListStyles();
    updateSelection();
}

int ListView::selectedIndex() const
{
    if (_priv->multiselect)
        return -1;
    return _priv->selectedIndex;
}

void ListView::selectedIndex(int value)
{
    if (_priv->multiselect)
        return;
    if (_priv->selectedIndex == value)
        return;

    _priv->selectedIndex = value;
    updateSelection();
}

const std::vector<int>& ListView::selectedIndexes() const
{
    static const std::vector<int> empty;
    if (!_priv->multiselect)
        return empty;
    return _priv->selectedIndexes;
}

void ListView::selectedIndexes(const std::vector<int>& indexes)
{
    if (!_priv->multiselect)
        return;
    if (_priv->selectedIndexes == indexes)
        return;

    _priv->selectedIndexes.clear();
    for (int index : indexes)
    {
        // skip invalid
        if (index < 0 ||
            index >(int)count())
            continue;

        // skip duplicates
        if (std::find(_priv->selectedIndexes.begin(),
            _priv->selectedIndexes.end(),
            index) != _priv->selectedIndexes.end())
            continue;

        _priv->selectedIndexes.push_back(index);
    }

    updateSelection();
}

void ListView::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    // deselect all
    auto lvi = LVITEM();
    lvi.stateMask = LVIS_SELECTED;
    SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&lvi);

    if (_priv->multiselect)
    {
        for (int index : _priv->selectedIndexes)
        {
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&lvi);
        }
    }
    else
    {
        if (_priv->selectedIndex > -1)
        {
            auto lvi = LVITEM();
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)_priv->selectedIndex, (LPARAM)&lvi);
        }
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

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());

    // clear selections
    _priv->selectedIndex = -1;
    _priv->selectedIndexes.clear();
    updateSelection();
}

bool ListView::checkboxes() const
{
    return _priv->checkboxes;
}

void ListView::checkboxes(bool value)
{
    if (_priv->checkboxes == value)
        return;
    _priv->checkboxes = value;

    if (handle() == nullptr)
        return;

    // update list styles
    updateListStyles();
}

bool ListView::gridlines() const
{
    return _priv->gridlines;
}

void ListView::gridlines(bool value)
{
    if (_priv->gridlines == value)
        return;
    _priv->gridlines = value;

    if (handle() == nullptr)
        return;

    // update list styles
    updateListStyles();
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

std::vector<ISharedListViewColumn> ListView::columns() const
{
    // placeholder
    return std::vector<ISharedListViewColumn>();
}

std::string ListView::data(size_t, int) const
{
    // placeholder
    return std::string();
}

bool ListView::checked(size_t row) const
{
    // placeholder
    return false;
}

void ListView::checked(size_t row, bool checked)
{
    // placeholder
}

size_t ListView::count() const
{
    // placeholder
    return 0;
}

void ListView::updateListStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto dwStyle = (UINT)SendMessage(hwnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

    // always full row select in report mode
    dwStyle |= LVS_EX_FULLROWSELECT;

    if (_priv->gridlines)
        dwStyle |= LVS_EX_GRIDLINES;
    else
        dwStyle &= ~LVS_EX_GRIDLINES;

    if (_priv->checkboxes)
        dwStyle |= LVS_EX_CHECKBOXES;
    else
        dwStyle &= ~LVS_EX_CHECKBOXES;

    SendMessage(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

void ListView::updateColumns()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    const auto& cols = _priv->columns;
    int index = 0;
    for (const auto& col : cols)
    {
        auto text = toWide(col->text());
        auto lvc = LVCOLUMNW();
        lvc.mask = LVCF_TEXT | LVCF_WIDTH;
        lvc.pszText = text.data();

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

void ListView::onSelectionChanged()
{
    // selection state has changed; this handler will read the
    // new selection states and fire a public event if changed
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_priv->multiselect)
    {
        auto selectedIndexes = std::vector<int>();

        int index = -1;
        for (;;)
        {
            index = (int)SendMessage(hwnd, LVM_GETNEXTITEM, index, LVNI_SELECTED);
            if (index < 0)
                break;
            selectedIndexes.push_back(index);
        }


        _priv->selectedIndex = -1;
        if (_priv->selectedIndexes != selectedIndexes)
        {
            _priv->selectedIndexes = selectedIndexes;
            _priv->selChangedEvent.invoke(shared_from_this());

            std::string indexes;
            for (int index : selectedIndexes)
            {
                indexes += std::to_string(index) + " ";
            }

            DLGCPP_CMSG("ListView::onSelectionChanged(): selectedIndexes = " << indexes);
        }
    }
    else
    {
        int selectedIndex = (int)SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

        _priv->selectedIndexes.clear();
        if (_priv->selectedIndex != selectedIndex)
        {
            _priv->selectedIndex = selectedIndex;
            _priv->selChangedEvent.invoke(shared_from_this());

            DLGCPP_CMSG("ListView::onSelectionChanged(): selectedIndex = " << selectedIndex);
        }
    }
}
