#include "../control_p.h"
#include "../dlgmsg.h"
#include "../utility.h"
#include "listview_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;


LRESULT CALLBACK ListViewEdit_SubclassProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData);


ListView::ListView(const Position& p) :
    Control(std::string(), p),
    _priv(new listview_priv())
{
    Control::border(BorderStyle::Sunken);

    // we need internal messages for editing only; this requires a subclass.
    Control::wantInternalEvents(true);

    _priv->columnsChangedEvent += [this](ISharedControl) { onColumnsChanged(); };
    _priv->rowsChangedEvent += [this](ISharedControl) { onRowsChanged(); };
    _priv->selChangedEventInternal += [this](ISharedControl) { onSelectionChanged(); };
}

ListView::~ListView()
{
    delete _priv;
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

void ListView::notify(ctl_message& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the listview colors
        auto ctl_priv = priv();
        auto hdc = (HDC)msg.wParam;

        auto clrPair = Control::colors();

        if (clrPair.second != Color::Default &&
            clrPair.second != Color::None)
        {
            // has background color
            COLORREF textColor =
                clrPair.first != Color::Default
                ? (COLORREF)clrPair.first
                : GetSysColor(COLOR_WINDOWTEXT);

            SetTextColor(hdc, textColor);
            SetBkColor(hdc, (COLORREF)clrPair.second);
            msg.result = (LRESULT)ctl_priv->state.hbrBack;
        }
        else if (clrPair.first != Color::Default)
        {
            // has text color only
            SetTextColor(hdc, (COLORREF)clrPair.first);
            SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
            msg.result = (LRESULT)GetSysColorBrush(COLOR_WINDOW);
        }

        return;
    }

    Control::notify(msg);
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
                const auto colCount = columnCount();
                if (colCount == 0 || nmdi.item.iSubItem >= (int)colCount)
                    return;

                // extract text
                int role = roleData(nmdi.item.iSubItem);
                auto text = rowData(nmdi.item.iItem, role);

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
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            auto hit = LVHITTESTINFO();
            hit.pt = nmItemActivate.ptAction;
            ListView_SubItemHitTest(nmhdr.hwndFrom, &hit);

            if (_priv->checkboxes &&
                (hit.flags & LVHT_ONITEMSTATEICON) &&
                (hit.iItem >= 0))
            {
                // checkbox clicked
                checked(nmItemActivate.iItem, !checked(nmItemActivate.iItem));
                ListView_RedrawItems(nmhdr.hwndFrom, hit.iItem, hit.iItem);
            }
            else if (hit.iItem >= 0)
            {
                // fire item click event
                _priv->itemClickEvent.invoke(
                    shared_from_this(),
                    nmItemActivate.iItem,
                    roleData(nmItemActivate.iSubItem));
            }
        }
        else if (nmhdr.code == NM_DBLCLK)
        {
            // double click on item
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            beginEditing(
                nmItemActivate.iItem,
                roleData(nmItemActivate.iSubItem));

            if (nmItemActivate.iItem >= 0)
            {
                // fire item double click event
                _priv->itemDblClickEvent.invoke(
                    shared_from_this(),
                    nmItemActivate.iItem,
                    roleData(nmItemActivate.iSubItem));
            }
        }
        else if (nmhdr.code == LVN_COLUMNCLICK)
        {
            // column header clicked
            auto& nmlv = *((NMLISTVIEW*)msg.lParam);

            _priv->columnClickEvent.invoke(
                shared_from_this(),
                roleData(nmlv.iSubItem));
        }
        else if (nmhdr.code == NM_SETFOCUS)
        {
            if (_priv->editState.editing)
                confirmEditing();
        }
    }

    Control::notify(msg);
}

void ListView::rebuild()
{
    // remove editor
    cancelEditing();

    Control::rebuild();

    if (handle() == nullptr)
        return;

    updateColumns();
    updateRows();
    updateDisplayStyles();
    updateListStyles();
    updateSelection();
}

void ListView::colors(Color fgColor, Color bgColor)
{
    Control::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
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
            index >(int)rowCount())
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

bool ListView::editing() const
{
    return _priv->editState.editing;
}

bool ListView::beginEditing(size_t row, int role)
{
    if (_priv->editState.editing)
    {
        if (_priv->editState.row == row &&
            _priv->editState.role == role)
            // already editing the same
            return true;

        confirmEditing();
    }

    // request to edit the item
    std::string pretext;
    if (!beginEdit(row, role, pretext))
        return false;

    // get column from role
    auto column = _priv->columnRoles.at(role);

    auto hwndListView = (HWND)handle();

    RECT rc = { 0 };
    rc.top = column;
    rc.left = LVIR_LABEL;
    if (SendMessage(hwndListView, LVM_GETSUBITEMRECT, (WPARAM)row, (LPARAM)&rc) == 0)
    {
        // cannot get item rect
        return false;
    }

    auto hwndEditor = CreateWindowEx(
        0,
        WC_EDIT,
        toWide(pretext).data(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        rc.left + 2,
        rc.top > 0 ? rc.top - 1 : 0,
        (rc.right - rc.left) + 2,
        (rc.bottom - rc.top) + 3,
        hwndListView,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(_priv->editState.idEditor)),
        NULL,
        NULL);

    if (hwndEditor == NULL)
    {
        // cannot create editor
        return false;
    }

    SetWindowSubclass(
        hwndEditor,
        ListViewEdit_SubclassProc,
        1,
        reinterpret_cast<DWORD_PTR>(&_priv->editState));

    auto font = (HFONT)SendMessage(hwndListView, WM_GETFONT, 0, 0);
    SendMessage(hwndEditor, WM_SETFONT, (WPARAM)font, 0);

    SetWindowPos(
        hwndEditor, HWND_TOP, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);

    SetFocus(hwndEditor);

    _priv->editState.hwndEditor = hwndEditor;
    _priv->editState.listView = this;
    _priv->editState.row = row;
    _priv->editState.column = column;
    _priv->editState.role = role;
    _priv->editState.editing = true;
    return true;
}

void ListView::confirmEditing()
{
    if (!_priv->editState.editing)
        return;

    // Get length of text
    int len = GetWindowTextLengthW(_priv->editState.hwndEditor);
    std::wstring buf(len, L'\0');

    if (len > 0)
        GetWindowTextW(_priv->editState.hwndEditor, buf.data(), len + 1);

    auto text = toBytes(buf.data());

    endEdit(
        _priv->editState.row,
        _priv->editState.role,
        text,
        _priv->editState.confirmed);

    cancelEditing();
}

void ListView::cancelEditing()
{
    if (!_priv->editState.editing)
        return;

    if (handle() == nullptr)
        return;

    _priv->editState.editing = false;
    _priv->editState.row = -1;
    _priv->editState.column = -1;
    _priv->editState.role = -1;
    _priv->editState.confirmed = false;

    // destroy last so focus changes after
    DestroyWindow(_priv->editState.hwndEditor);
    _priv->editState.hwndEditor = NULL;
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

int ListView::roleData(int) const
{
    // placeholder
    return 0;
}

ListViewColumn ListView::columnData(int) const
{
    // placeholder
    return ListViewColumn();
}

size_t ListView::columnCount() const
{
    // placeholder
    return 0;
}

std::string ListView::rowData(size_t, int) const
{
    // placeholder
    return std::string();
}

size_t ListView::rowCount() const
{
    // placeholder
    return 0;
}

bool ListView::beginEdit(
    size_t row,
    int role,
    std::string& text)
{
    return false;
}

void ListView::endEdit(
    size_t row,
    int role,
    const std::string& text,
    bool confirmed)
{
    // placeholder
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

void ListView::updateDisplayStyles()
{
    auto hwnd = (HWND)handle();
    auto clrPair = Control::colors();

    COLORREF backColor =
        clrPair.second != Color::Default
        ? (COLORREF)clrPair.second
        : GetSysColor(COLOR_WINDOW);
    COLORREF textColor =
        clrPair.first != Color::Default
        ? (COLORREF)clrPair.first
        : GetSysColor(COLOR_WINDOWTEXT);

    ListView_SetBkColor(hwnd, backColor);
    ListView_SetTextBkColor(hwnd, backColor);
    ListView_SetTextColor(hwnd, textColor);
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

    auto colCount = (int)columnCount();
    int index = 0;

    _priv->columnRoles.clear();
    for (auto colIdx = 0; colIdx < colCount; colIdx++)
    {
        auto role = roleData(colIdx);
        _priv->columnRoles[role] = colIdx;

        auto colData = columnData(role);

        auto text = toWide(colData.text());
        auto lvc = LVCOLUMNW();
        lvc.mask = LVCF_TEXT | LVCF_WIDTH;
        lvc.pszText = text.data();

        Size s(colData.width(), 0);
        toPixels(GetParent(hwnd), s);
        lvc.cx = s.width();

        SendMessageW(hwnd, LVM_INSERTCOLUMNW, (WPARAM)index, (LPARAM)&lvc);
        index++;
    }
}

void ListView::updateRows()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LVM_SETITEMCOUNT, (WPARAM)rowCount(), 0);
}

void ListView::onColumnsChanged()
{
    updateColumns();
}

void ListView::onRowsChanged()
{
    updateRows();
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

/// <summary>
/// Subclass procedure for the in-place editor control
/// </summary>
LRESULT CALLBACK ListViewEdit_SubclassProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData)
{
    auto* priv = reinterpret_cast<listview_edit_priv*>(dwRefData);
    if (priv == nullptr)
        return DefSubclassProc(hwnd, msg, wParam, lParam);

    auto moveEdit = [](listview_edit_priv* priv, int dx, int dy)
        {
            if (!priv || !priv->listView)
                return;

            int colCount = priv->listView->columnCount();
            int rowCount = priv->listView->rowCount();

            if (colCount < 1 || rowCount < 1)
                return;

            int newCol = priv->column + dx;
            int newRow = priv->row + dy;

            // Wrap column
            if (newCol < 0)
                newCol = colCount - 1;
            else if (newCol >= colCount)
                newCol = 0;

            // Clamp row
            if (newRow < 0)
                newRow = 0;
            else if (newRow >= rowCount)
                newRow = rowCount - 1;

            auto role = priv->listView->roleData(newCol);
            priv->listView->beginEditing(newRow, role);
        };

    switch (msg)
    {
    case WM_KEYDOWN:
    {
        if (!priv->editing)
            break;
        bool shiftDown = (GetAsyncKeyState(VK_SHIFT) & 0x8000);

        switch (wParam)
        {
        case VK_RETURN:

            priv->confirmed = true;
            priv->listView->confirmEditing();
            // no further processing
            return 0;

        case VK_ESCAPE:
            // always cancels editing
            priv->listView->cancelEditing();
            // no further processing
            return 0;

        case VK_TAB:
            // TAB moves to next column, or back to first.
            moveEdit(priv, shiftDown ? -1 : 1, 0);
            // no further processing
            return 0;

        case VK_UP:
            moveEdit(priv, 0, -1);
            // no further processing
            return 0;

        case VK_DOWN:
            moveEdit(priv, 0, 1);
            // no further processing
            return 0;
        }
        break;
    }

    case WM_GETDLGCODE:
        return DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTTAB;

    case WM_NCDESTROY:
        RemoveWindowSubclass(hwnd, ListViewEdit_SubclassProc, uIdSubclass);
        break;
    }

    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

IEvent<ISharedControl>& ListView::ColumnsChangedEvent()
{
    return _priv->columnsChangedEvent;
}

IEvent<ISharedControl>& ListView::RowsChangedEvent()
{
    return _priv->rowsChangedEvent;
}

IEvent<ISharedControl>& ListView::SelChangedEvent()
{
    return _priv->selChangedEvent;
}

IEvent<ISharedControl, int>& ListView::ColumnClickEvent()
{
    return _priv->columnClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListView::ItemClickEvent()
{
    return _priv->itemClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListView::ItemDoubleClickEvent()
{
    return _priv->itemDblClickEvent;
}
