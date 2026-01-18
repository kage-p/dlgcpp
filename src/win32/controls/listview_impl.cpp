#include "listview_impl.h"
#include "utility/convert.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;


ListViewImpl::ListViewImpl() :
    ControlImpl()
{
    _selChangedPrivateEvent.reset([]() {return 1; }, "SelChangedPrivateEvent");
}

ListViewImpl::~ListViewImpl()
{
}

void ListViewImpl::owner(IListView* listView)
{
    ControlImpl::owner(listView);
    _listView = listView;

    _listView->display().event() += [&](auto) { onDisplayChanged(); };
    _listView->columnHeader().event() += [&](auto) { rebuild(); };
    _listView->sortColumns().event() += [&](auto) { rebuild(); };
    _listView->checkboxes().event() += [&](auto) { updateExtStyles(); };
    _listView->gridlines().event() += [&](auto) { updateExtStyles(); };
    _listView->multiselect().event() += [&](auto) { onMultiselectChanged(); };
    _listView->selectedIndex().event() += [&](auto) { updateSelection(); };
    _listView->selectedIndexes().event() += [&](auto) { updateSelection(); };
    _listView->colors().event() += [&](auto) { updateDisplayStyles(); };

    _listView->ColumnsChangedEvent() += [&](ISharedControl) { onColumnsChanged(); };
    _listView->RowsChangedEvent() += [&](ISharedControl) { onRowsChanged(); };

    _selChangedPrivateEvent += [&](auto) { onSelectionChanged(); };
}

std::string ListViewImpl::className() const
{
    return WC_LISTVIEWA;
}

unsigned int ListViewImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= LVS_OWNERDATA;

    if (!_listView->multiselect())
        styles |= LVS_SINGLESEL;
    if (!_listView->columnHeader())
        styles |= LVS_NOCOLUMNHEADER;
    if (!_listView->sortColumns())
        styles |= LVS_NOSORTHEADER;

    switch (_listView->display())
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

bool ListViewImpl::wantInternalEvents() const
{
    // we need internal messages for editing only; this requires a subclass.
    return true;
}

void ListViewImpl::notify(ControlMessage& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the listview colors
        auto hdc = (HDC)msg.wParam;

        auto clrPair = _listView->colors().value();

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
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        switch (nmhdr.code)
        {
        case LVN_GETDISPINFO:
        {
            auto& nmdi = *((NMLVDISPINFO*)msg.lParam);

            if (nmdi.item.mask & LVIF_TEXT)
            {
                // check the role
                const auto colCount = _listView->columnCount();
                if (colCount > 0 || nmdi.item.iSubItem < (int)colCount)
                {
                    // extract text
                    int role = _listView->roleData(nmdi.item.iSubItem);
                    auto text = _listView->rowData(nmdi.item.iItem, role);

                    _displayBuffer = StringEncoder::toWide(text);
                    nmdi.item.pszText = _displayBuffer.data();
                }
            }

            if (nmdi.item.mask & LVIF_STATE)
            {
                bool isChecked = _listView->checked(nmdi.item.iItem);
                nmdi.item.stateMask = LVIS_STATEIMAGEMASK;
                nmdi.item.state = INDEXTOSTATEIMAGEMASK(isChecked ? 2 : 1);
            }
            break;
        }
        case LVN_ITEMCHANGED:
        {
            auto& nmlv = *((NMLISTVIEW*)msg.lParam);

            if (nmlv.uChanged & LVIF_STATE)
            {
                // item selection state changed
                if (_inhibitSelectionMessages.get())
                    return;

                // fire internal event
                _selChangedPrivateEvent.invoke();
            }
            break;
        }
        case NM_CLICK:
        {
            // left click on control
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            auto hit = LVHITTESTINFO();
            hit.pt = nmItemActivate.ptAction;
            ListView_SubItemHitTest(nmItemActivate.hdr.hwndFrom, &hit);

            if (_listView->checkboxes() &&
                (hit.flags & LVHT_ONITEMSTATEICON) &&
                (hit.iItem >= 0))
            {
                // checkbox clicked
                _listView->checked(nmItemActivate.iItem, !_listView->checked(nmItemActivate.iItem));
                ListView_RedrawItems(nmItemActivate.hdr.hwndFrom, hit.iItem, hit.iItem);
            }
            else if (hit.iItem >= 0)
            {
                // fire item click event
                _listView->ItemClickEvent().invoke(
                    nmItemActivate.iItem,
                    _listView->roleData(nmItemActivate.iSubItem));
            }
            break;
        }
        case NM_DBLCLK:
        {
            // left-double click on control
            auto& nmItemActivate = *(LPNMITEMACTIVATE)msg.lParam;

            beginEditing(
                nmItemActivate.iItem,
                _listView->roleData(nmItemActivate.iSubItem));

            if (nmItemActivate.iItem >= 0)
            {
                // fire item double click event
                _listView->ItemDoubleClickEvent().invoke(
                    nmItemActivate.iItem,
                    _listView->roleData(nmItemActivate.iSubItem));
            }
            break;
        }
        case LVN_COLUMNCLICK:
        {
            // column header clicked
            auto& nmlv = *((NMLISTVIEW*)msg.lParam);

            _listView->ColumnClickEvent().invoke(
                _listView->roleData(nmlv.iSubItem));

            break;
        }
        case NM_SETFOCUS:
        {
            if (_listView->editing())
                confirmEditing();
            break;
        }
        }
    }
    }

    // default notify handler
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        switch (nmhdr.code)
        {
        case NM_CLICK:
            _listView->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _listView->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _listView->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _listView->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _listView->focus() = true;
            break;

        case NM_KILLFOCUS:
            _listView->focus() = false;
            break;
        }
    }
    }
}

void ListViewImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    updateColumns();
    updateRows();
    updateDisplayStyles();
    updateExtStyles();
    updateSelection();
}

void ListViewImpl::destruct()
{
    // remove editor
    cancelEditing();
    ControlImpl::destruct();
}

void ListViewImpl::updateSelection()
{
    if (hwnd() == 0)
        return;

    auto hwnd = this->hwnd();

    // deselect all
    auto lvi = LVITEM();
    lvi.stateMask = LVIS_SELECTED;
    SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&lvi);

    if (_listView->multiselect())
    {
        const auto& selectedIndexes = _listView->selectedIndexes().value();
        for (int index : selectedIndexes)
        {
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&lvi);
        }
    }
    else
    {
        auto selectedIndex = _listView->selectedIndex().value();
        if (selectedIndex > -1)
        {
            auto lvi = LVITEM();
            lvi.state = LVIS_SELECTED;
            lvi.stateMask = LVIS_SELECTED;
            SendMessage(hwnd, LVM_SETITEMSTATE, (WPARAM)selectedIndex, (LPARAM)&lvi);
        }
    }
}

bool ListViewImpl::beginEditing(size_t row, int role)
{
    if (hwnd() == 0)
        return false;
    auto hwnd = this->hwnd();

    if (_listView->editing())
    {
        if (_editState.row == row &&
            _editState.role == role)
            // already editing the same
            return true;

        confirmEditing();
    }

    // request to edit the item
    if (!_listView->beginEdit(row, role))
        return false;

    std::string pretext = _listView->rowData(row, role);

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
        StringEncoder::toWide(pretext).data(),
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
    _editState.listView = _listView;
    _editState.row = row;
    _editState.column = column;
    _editState.role = role;
    _listView->editing() = true;

    return true;
}

void ListViewImpl::confirmEditing()
{
    if (!_listView->editing())
        return;

    auto text = getText(_editState.hwndEditor);

    bool changed = _listView->endEdit(
        _editState.row,
        _editState.role,
        text);

    if (changed && hwnd() != 0)
    {
        ListView_RedrawItems(
            hwnd(),
            (int)_editState.row,
            (int)_editState.row);
    }

    cancelEditing();
}

void ListViewImpl::cancelEditing()
{
    if (!_listView->editing())
        return;

    if (hwnd() == 0)
        return;

    _editState.row = -1;
    _editState.column = -1;
    _editState.role = -1;
    _editState.confirmed = false;

    // destroy last so focus changes after
    DestroyWindow(_editState.hwndEditor);
    _editState.hwndEditor = NULL;

    _listView->editing() = false;
}

void ListViewImpl::onMultiselectChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());

    // clear selections
    _listView->selectedIndex() = -1;
    _listView->selectedIndexes().clear();
    updateSelection();
}

void ListViewImpl::onDisplayChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());
}

void ListViewImpl::updateDisplayStyles()
{
    if (hwnd() == 0)
        return;

    auto clrPair = _listView->colors().value();

    COLORREF backColor =
        clrPair.second != Color::Default
        ? (COLORREF)clrPair.second
        : GetSysColor(COLOR_WINDOW);
    COLORREF textColor =
        clrPair.first != Color::Default
        ? (COLORREF)clrPair.first
        : GetSysColor(COLOR_WINDOWTEXT);

    auto hwnd = this->hwnd();
    ListView_SetBkColor(hwnd, backColor);
    ListView_SetTextBkColor(hwnd, backColor);
    ListView_SetTextColor(hwnd, textColor);
}

void ListViewImpl::updateExtStyles()
{
    if (hwnd() == 0)
        return;

    auto dwStyle = (UINT)SendMessage(hwnd(), LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

    // always full row select in report mode
    dwStyle |= LVS_EX_FULLROWSELECT;

    if (_listView->gridlines())
        dwStyle |= LVS_EX_GRIDLINES;
    else
        dwStyle &= ~LVS_EX_GRIDLINES;

    if (_listView->checkboxes())
        dwStyle |= LVS_EX_CHECKBOXES;
    else
        dwStyle &= ~LVS_EX_CHECKBOXES;

    SendMessage(hwnd(), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

void ListViewImpl::updateColumns()
{
    if (hwnd() == 0)
        return;
    auto hwnd = this->hwnd();

    // remove existing columns
    while (SendMessageW(hwnd, LVM_DELETECOLUMN, 0, 0) != 0);

    auto colCount = (int)_listView->columnCount();
    if (colCount == 0)
        return;

    int index = 0;
    _columnRoles.clear();
    for (auto colIdx = 0; colIdx < colCount; colIdx++)
    {
        auto role = _listView->roleData(colIdx);
        _columnRoles[role] = colIdx;

        auto colData = _listView->columnData(role);

        auto text = StringEncoder::toWide(colData.text());
        auto lvc = LVCOLUMNW();
        lvc.mask = LVCF_TEXT | LVCF_WIDTH;
        lvc.pszText = text.data();

        Size s = Convert(GetParent(hwnd)).toPixels(Size(colData.width(), 0));
        lvc.cx = s.width();

        SendMessageW(hwnd, LVM_INSERTCOLUMNW, (WPARAM)index, (LPARAM)&lvc);
        index++;
    }
}

void ListViewImpl::updateRows()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), LVM_SETITEMCOUNT, (WPARAM)_listView->rowCount(), 0);
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
    MessageLocker selectionChangedLock(_inhibitSelectionMessages);

    // selection state has changed; this handler will read the
    // new selection states and fire a public event if changed
    if (hwnd() == 0)
        return;

    if (_listView->multiselect())
    {
        auto selectedIndexes = std::vector<int>();

        int index = -1;
        for (;;)
        {
            index = (int)SendMessage(hwnd(), LVM_GETNEXTITEM, index, LVNI_SELECTED);
            if (index < 0)
                break;
            selectedIndexes.push_back(index);
        }

        _listView->selectedIndex() = -1;
        if (_listView->selectedIndexes() != selectedIndexes)
        {
            _listView->selectedIndexes() = selectedIndexes;
            _listView->SelChangedEvent().invoke();

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
        int selectedIndex = (int)SendMessage(hwnd(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

        _listView->selectedIndexes().clear();
        if (_listView->selectedIndex() != selectedIndex)
        {
            _listView->selectedIndex() = selectedIndex;
            _listView->SelChangedEvent().invoke();

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
        if (!priv->listView->editing())
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
