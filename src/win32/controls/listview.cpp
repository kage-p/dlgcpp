#include "listview_p.h"
#include "utility/string.h"
#include "utility/units.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;


ListViewImpl::ListViewImpl(ListView& listView, const Position& p) :
    ControlImpl(listView, std::string(), p),
    _listView(listView)
{
    ControlImpl::border(BorderStyle::Sunken);

    // we need internal messages for editing only; this requires a subclass.
    ControlImpl::wantInternalEvents(true);

    _columnsChangedEvent += [this](ISharedControl) { onColumnsChanged(); };
    _rowsChangedEvent += [this](ISharedControl) { onRowsChanged(); };
    _selChangedEventInternal += [this](ISharedControl) { onSelectionChanged(); };
}

ListViewImpl::~ListViewImpl()
{
}

std::string ListViewImpl::className() const
{
    return WC_LISTVIEWA;
}

unsigned int ListViewImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= LVS_OWNERDATA;

    if (!_multiselect)
        styles |= LVS_SINGLESEL;

    switch (_displayType)
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

void ListViewImpl::notify(ControlMessage& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the listview colors
        auto hdc = (HDC)msg.wParam;

        auto clrPair = ControlImpl::colors();

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

            msg.result = (LRESULT)backgroundBrush();
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

    ControlImpl::notify(msg);
}

void ListViewImpl::notify(DialogMessage& msg)
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
                const auto colCount = _listView.columnCount();
                if (colCount == 0 || nmdi.item.iSubItem >= (int)colCount)
                    return;

                // extract text
                int role = _listView.roleData(nmdi.item.iSubItem);
                auto text = _listView.rowData(nmdi.item.iItem, role);

                _displayBuffer = toWide(text);
                nmdi.item.pszText = _displayBuffer.data();
            }

            if (nmdi.item.mask & LVIF_STATE)
            {
                bool isChecked = _listView.checked(nmdi.item.iItem);
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
                _selChangedEventInternal.invoke(control());
            }
        }
        else if (nmhdr.code == NM_CLICK)
        {
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            auto hit = LVHITTESTINFO();
            hit.pt = nmItemActivate.ptAction;
            ListView_SubItemHitTest(nmhdr.hwndFrom, &hit);

            if (_checkboxes &&
                (hit.flags & LVHT_ONITEMSTATEICON) &&
                (hit.iItem >= 0))
            {
                // checkbox clicked
                _listView.checked(nmItemActivate.iItem, !_listView.checked(nmItemActivate.iItem));
                ListView_RedrawItems(nmhdr.hwndFrom, hit.iItem, hit.iItem);
            }
            else if (hit.iItem >= 0)
            {
                // fire item click event
                _itemClickEvent.invoke(
                    control(),
                    nmItemActivate.iItem,
                    _listView.roleData(nmItemActivate.iSubItem));
            }
        }
        else if (nmhdr.code == NM_DBLCLK)
        {
            // double click on item
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            beginEditing(
                nmItemActivate.iItem,
                _listView.roleData(nmItemActivate.iSubItem));

            if (nmItemActivate.iItem >= 0)
            {
                // fire item double click event
                _itemDblClickEvent.invoke(
                    control(),
                    nmItemActivate.iItem,
                    _listView.roleData(nmItemActivate.iSubItem));
            }
        }
        else if (nmhdr.code == LVN_COLUMNCLICK)
        {
            // column header clicked
            auto& nmlv = *((NMLISTVIEW*)msg.lParam);

            _columnClickEvent.invoke(
                control(),
                _listView.roleData(nmlv.iSubItem));
        }
        else if (nmhdr.code == NM_SETFOCUS)
        {
            if (_editState.editing)
                confirmEditing();
        }
    }

    ControlImpl::notify(msg);
}

void ListViewImpl::rebuild()
{
    // remove editor
    cancelEditing();

    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;

    updateColumns();
    updateRows();
    updateDisplayStyles();
    updateExtStyles();
    updateSelection();
}

void ListViewImpl::colors(Color fgColor, Color bgColor)
{
    ControlImpl::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
}

int ListViewImpl::selectedIndex() const
{
    if (_multiselect)
        return -1;
    return _selectedIndex;
}

void ListViewImpl::selectedIndex(int value)
{
    if (_multiselect)
        return;
    if (_selectedIndex == value)
        return;

    _selectedIndex = value;
    updateSelection();
}

const std::vector<int>& ListViewImpl::selectedIndexes() const
{
    static const std::vector<int> empty;
    if (!_multiselect)
        return empty;
    return _selectedIndexes;
}

void ListViewImpl::selectedIndexes(const std::vector<int>& indexes)
{
    if (!_multiselect)
        return;
    if (_selectedIndexes == indexes)
        return;

    _selectedIndexes.clear();
    for (int index : indexes)
    {
        // skip invalid
        if (index < 0 ||
            index >(int)_listView.rowCount())
            continue;

        // skip duplicates
        if (std::find(_selectedIndexes.begin(),
            _selectedIndexes.end(),
            index) != _selectedIndexes.end())
            continue;

        _selectedIndexes.push_back(index);
    }

    updateSelection();
}

void ListViewImpl::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    // deselect all
    auto lvi = LVITEM();
    lvi.stateMask = LVIS_SELECTED;
    SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&lvi);

    if (_multiselect)
    {
        for (int index : _selectedIndexes)
        {
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&lvi);
        }
    }
    else
    {
        if (_selectedIndex > -1)
        {
            auto lvi = LVITEM();
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)_selectedIndex, (LPARAM)&lvi);
        }
    }
}

bool ListViewImpl::checkboxes() const
{
    return _checkboxes;
}

void ListViewImpl::checkboxes(bool value)
{
    if (_checkboxes == value)
        return;
    _checkboxes = value;

    if (handle() == nullptr)
        return;

    updateExtStyles();
}

bool ListViewImpl::editing() const
{
    return _editState.editing;
}

bool ListViewImpl::beginEditing(size_t row, int role)
{
    if (handle() == nullptr)
        return false;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_editState.editing)
    {
        if (_editState.row == row &&
            _editState.role == role)
            // already editing the same
            return true;

        confirmEditing();
    }

    // request to edit the item
    if (!_listView.beginEdit(row, role))
        return false;

    std::string pretext = _listView.rowData(row, role);

    // get column from role
    auto column = _columnRoles.at(role);

    RECT rc = { 0 };
    rc.top = column;
    rc.left = LVIR_LABEL;
    if (SendMessage(hwnd, LVM_GETSUBITEMRECT, (WPARAM)row, (LPARAM)&rc) == 0)
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
        hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(_editState.idEditor)),
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
        reinterpret_cast<DWORD_PTR>(&_editState));

    auto font = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
    SendMessage(hwndEditor, WM_SETFONT, (WPARAM)font, 0);

    SetWindowPos(
        hwndEditor, HWND_TOP, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);

    SetFocus(hwndEditor);

    _editState.hwndEditor = hwndEditor;
    _editState.listView = &_listView;
    _editState.row = row;
    _editState.column = column;
    _editState.role = role;
    _editState.editing = true;

    return true;
}

void ListViewImpl::confirmEditing()
{
    if (!_editState.editing)
        return;

    // Get length of text
    int len = GetWindowTextLengthW(_editState.hwndEditor);
    std::wstring buf(len, L'\0');

    if (len > 0)
        GetWindowTextW(_editState.hwndEditor, buf.data(), len + 1);

    auto text = toBytes(buf.data());

    bool changed = _listView.endEdit(
        _editState.row,
        _editState.role,
        text);

    if (changed && handle() != nullptr)
    {
        auto hwnd = reinterpret_cast<HWND>(handle());

        ListView_RedrawItems(
            hwnd,
            (int)_editState.row,
            (int)_editState.row);
    }

    cancelEditing();
}

void ListViewImpl::cancelEditing()
{
    if (!_editState.editing)
        return;

    if (handle() == nullptr)
        return;

    _editState.editing = false;
    _editState.row = -1;
    _editState.column = -1;
    _editState.role = -1;
    _editState.confirmed = false;

    // destroy last so focus changes after
    DestroyWindow(_editState.hwndEditor);
    _editState.hwndEditor = NULL;
}

bool ListViewImpl::gridlines() const
{
    return _gridlines;
}

void ListViewImpl::gridlines(bool value)
{
    if (_gridlines == value)
        return;
    _gridlines = value;

    if (handle() == nullptr)
        return;

    updateExtStyles();
}

bool ListViewImpl::multiselect() const
{
    return _multiselect;
}

void ListViewImpl::multiselect(bool value)
{
    if (_multiselect == value)
        return;
    _multiselect = value;

    if (handle() == nullptr)
        return;

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());

    // clear selections
    _selectedIndex = -1;
    _selectedIndexes.clear();
    updateSelection();
}

ListViewDisplay ListViewImpl::display() const
{
    return _displayType;
}

void ListViewImpl::display(ListViewDisplay value)
{
    if (_displayType == value)
        return;

    _displayType = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

void ListViewImpl::updateDisplayStyles()
{
    auto hwnd = (HWND)handle();
    auto clrPair = ControlImpl::colors();

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

void ListViewImpl::updateExtStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto dwStyle = (UINT)SendMessage(hwnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

    // always full row select in report mode
    dwStyle |= LVS_EX_FULLROWSELECT;

    if (_gridlines)
        dwStyle |= LVS_EX_GRIDLINES;
    else
        dwStyle &= ~LVS_EX_GRIDLINES;

    if (_checkboxes)
        dwStyle |= LVS_EX_CHECKBOXES;
    else
        dwStyle &= ~LVS_EX_CHECKBOXES;

    SendMessage(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

void ListViewImpl::updateColumns()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto colCount = (int)_listView.columnCount();
    int index = 0;

    _columnRoles.clear();
    for (auto colIdx = 0; colIdx < colCount; colIdx++)
    {
        auto role = _listView.roleData(colIdx);
        _columnRoles[role] = colIdx;

        auto colData = _listView.columnData(role);

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

void ListViewImpl::updateRows()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, LVM_SETITEMCOUNT, (WPARAM)_listView.rowCount(), 0);
}

void ListViewImpl::onColumnsChanged()
{
    updateColumns();
}

void ListViewImpl::onRowsChanged()
{
    updateRows();
}

void ListViewImpl::onSelectionChanged()
{
    // selection state has changed; this handler will read the
    // new selection states and fire a public event if changed
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_multiselect)
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

        _selectedIndex = -1;
        if (_selectedIndexes != selectedIndexes)
        {
            _selectedIndexes = selectedIndexes;
            _selChangedEvent.invoke(control());

            std::string indexes;
            for (int index : selectedIndexes)
            {
                indexes += std::to_string(index) + " ";
            }

            DLGCPP_CMSG("ListViewImpl::onSelectionChanged(): selectedIndexes = " << indexes);
        }
    }
    else
    {
        int selectedIndex = (int)SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

        _selectedIndexes.clear();
        if (_selectedIndex != selectedIndex)
        {
            _selectedIndex = selectedIndex;
            _selChangedEvent.invoke(control());

            DLGCPP_CMSG("ListViewImpl::onSelectionChanged(): selectedIndex = " << selectedIndex);
        }
    }
}

/// <summary>
/// Subclass procedure for the in-place editor control
/// </summary>
LRESULT CALLBACK ListViewImpl::ListViewEdit_SubclassProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData)
{
    auto* priv = reinterpret_cast<ListViewEditProps*>(dwRefData);
    if (priv == nullptr)
        return DefSubclassProc(hwnd, msg, wParam, lParam);

    auto moveEdit = [](ListViewEditProps* priv, int dx, int dy)
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

IEvent<ISharedControl>& ListViewImpl::ColumnsChangedEvent()
{
    return _columnsChangedEvent;
}

IEvent<ISharedControl>& ListViewImpl::RowsChangedEvent()
{
    return _rowsChangedEvent;
}

IEvent<ISharedControl>& ListViewImpl::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl, int>& ListViewImpl::ColumnClickEvent()
{
    return _columnClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListViewImpl::ItemClickEvent()
{
    return _itemClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListViewImpl::ItemDoubleClickEvent()
{
    return _itemDblClickEvent;
}
