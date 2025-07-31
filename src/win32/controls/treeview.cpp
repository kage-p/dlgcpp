#include "control_p.h"
#include "treeview_p.h"
#include "utility/string_encoder.h"
#include <strsafe.h>

#include <Windowsx.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

TreeViewImpl::TreeViewImpl(
    ITreeView* treeView,
    const Position& p) :
    ControlImpl(std::string(), p),
    _treeView(treeView)
{
    ControlImpl::border(BorderStyle::Sunken);

    _rootNodeChangedEvent += [this](ISharedControl) { onRootNodeChanged(); };
    _expandNodePrivateEvent += [this](std::shared_ptr<TreeViewNode> node) { expand(node, true); };
}

TreeViewImpl::~TreeViewImpl()
{
}

std::string TreeViewImpl::className() const
{
    return WC_TREEVIEWA;
}

unsigned int TreeViewImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |=
        TVS_HASBUTTONS |
        TVS_HASLINES |
        TVS_LINESATROOT |
        TVS_FULLROWSELECT |
        TVS_EDITLABELS;

    if (_checkboxes)
        styles |= TVS_CHECKBOXES;

    return styles;
}

void TreeViewImpl::notify(ControlMessage& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the treeview colors
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

void TreeViewImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);

        if (nmhdr.code == TVN_GETDISPINFO)
        {
            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);

            auto node = nodeFromItem(nmdi.item.hItem);
            if (node == nullptr)
                return;

            // set the item text
            if (nmdi.item.mask & TVIF_TEXT)
            {
                auto wide = StringEncoder::toWide(node->text());
                StringCchCopy(nmdi.item.pszText, nmdi.item.cchTextMax, wide.data());
            }
        }
        else if (nmhdr.code == TVN_ITEMEXPANDING)
        {
            auto& nmtv = *((LPNMTREEVIEW)msg.lParam);
            auto hitem = nmtv.itemNew.hItem;

            // Prevent duplicate insertions
            if (TreeView_GetChild(nmhdr.hwndFrom, hitem) != nullptr)
            {
                // Already populated
                return;
            }

            auto node = nodeFromItem(hitem);
            if (node != nullptr)
                updateChildNodes(node);
        }
        else if (nmhdr.code == TVN_SELCHANGED)
        {
            auto& nmtv = *((LPNMTREEVIEW)msg.lParam);
            if (!_multiselect)
            {
                // single selection changed
                _selectedNodes.clear();
                if (nmtv.itemNew.hItem != NULL)
                {
                    auto node = nodeFromItem(nmtv.itemNew.hItem);
                    if (node != nullptr)
                        _selectedNodes.push_back(node);
                }
                DLGCPP_CMSG("TVN_SELCHANGED: " << _selectedNodes.size() << " item(s) selected.");
                _selChangedEvent.invoke(control());
            }
        }
        else if (nmhdr.code == TVN_ITEMCHANGED)
        {
            if (_inhibitSelectionMessages.get())
                return;

            auto& nmic = *((NMTVITEMCHANGE*)msg.lParam);
            if (nmic.uChanged & TVIF_STATE)
            {
                // update node check state
                auto hitem = nmic.hItem;
                bool checked = TreeView_GetCheckState(nmhdr.hwndFrom, hitem) == 1;
                auto node = nodeFromItem(hitem);
                if (node != nullptr)
                    node->checked(checked);

                if (_multiselect)
                {
                    bool wasSelected = nmic.uStateOld & TVIS_SELECTED;
                    bool isSelected = nmic.uStateNew & TVIS_SELECTED;

                    // item selection state changes
                    if (!wasSelected && isSelected)
                    {
                        auto node = nodeFromItem(nmic.hItem);
                        if (node == nullptr)
                            return;
                        _selectedNodes.push_back(node);
                    }
                    else if (wasSelected && !isSelected)
                    {
                        auto node = nodeFromItem(nmic.hItem);
                        if (node == nullptr)
                            return;

                        _selectedNodes.erase(
                            std::remove(
                                _selectedNodes.begin(),
                                _selectedNodes.end(),
                                node),
                            _selectedNodes.end());
                    }
                    else
                        return;

                    _selChangedEvent.invoke(control());
                    DLGCPP_CMSG("TVN_ITEMCHANGED: " << _selectedNodes.size() << " item(s) selected.");
                }
            }
        }
        else if (nmhdr.code == TVN_DELETEITEM)
        {
            auto& nmtv = *((LPNMTREEVIEW)msg.lParam);

            // remove from our map
            auto hitem = nmtv.itemOld.hItem;
            _nodeMap.erase(hitem);

            DLGCPP_CMSG("TVN_DELETEITEM: nodeMap.size = " << _nodeMap.size());
        }
        else if (nmhdr.code == TVN_BEGINLABELEDIT)
        {
            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);

            auto hitem = nmdi.item.hItem;
            auto node = nodeFromItem(hitem);
            if (node == nullptr ||
                !_treeView->beginEdit(node))
            {
                // prevent edit of this item
                msg.dlgResult = TRUE;
                msg.msgResult = TRUE;
                return;
            }
            _editingLabel = true;
            msg.dlgResult = TRUE;
            msg.msgResult = FALSE;
        }
        else if (nmhdr.code == TVN_ENDLABELEDIT)
        {
            _editingLabel = false;

            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);
            if (nmdi.item.pszText != NULL)
            {
                // save the edited text
                auto hitem = nmdi.item.hItem;
                auto node = nodeFromItem(hitem);
                if (node != nullptr)
                {
                    if (!_treeView->endEdit(node, StringEncoder::toBytes(nmdi.item.pszText)))
                    {
                        msg.dlgResult = TRUE;
                        msg.msgResult = FALSE;
                        return;
                    }

                    // request item text again
                    TVITEM tvi = {};
                    tvi.mask = TVIF_TEXT;
                    tvi.hItem = hitem;
                    tvi.pszText = LPSTR_TEXTCALLBACK;
                    TreeView_SetItem(nmhdr.hwndFrom, &tvi);
                    msg.dlgResult = TRUE;
                    msg.msgResult = TRUE;
                }
            }
        }
        else if (nmhdr.code == NM_CLICK)
        {
            auto msgCursorPos = GetMessagePos();
            auto ptCursor = POINT{ GET_X_LPARAM(msgCursorPos), GET_Y_LPARAM(msgCursorPos) };
            ScreenToClient(nmhdr.hwndFrom, &ptCursor);

            auto hit = TVHITTESTINFO();
            hit.pt = ptCursor;
            TreeView_HitTest(nmhdr.hwndFrom, &hit);

            if (hit.hItem != NULL && (hit.flags & TVHT_ONITEM))
            {
                auto node = nodeFromItem(hit.hItem);
                if (node != nullptr)
                {
                    // fire item click event
                    _itemClickEvent.invoke(
                        control(),
                        node
                    );
                }
            }
        }
        else if (nmhdr.code == NM_DBLCLK)
        {
            // double click on item
            auto msgCursorPos = GetMessagePos();
            auto ptCursor = POINT{ GET_X_LPARAM(msgCursorPos), GET_Y_LPARAM(msgCursorPos) };
            ScreenToClient(nmhdr.hwndFrom, &ptCursor);

            auto hit = TVHITTESTINFO();
            hit.pt = ptCursor;
            TreeView_HitTest(nmhdr.hwndFrom, &hit);

            if (hit.hItem != NULL && (hit.flags & TVHT_ONITEM))
            {
                auto node = nodeFromItem(hit.hItem);
                if (node != nullptr)
                {
                    // fire item double click event
                    _itemDblClickEvent.invoke(
                        control(),
                        node);
                }
            }
        }
    }

    ControlImpl::notify(msg);
}

void TreeViewImpl::rebuild()
{
    // remove editor
    cancelEditing();

    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
    updateExtStyles();
    updateRootNode();
    updateSelection();
}

void TreeViewImpl::colors(Color fgColor, Color bgColor)
{
    ControlImpl::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
}

std::shared_ptr<TreeViewNode> TreeViewImpl::selectedNode() const
{
    if (_multiselect || _selectedNodes.empty())
        return nullptr;
    return _selectedNodes.at(0);
}

void TreeViewImpl::selectedNode(std::shared_ptr<TreeViewNode> node)
{
    if (_multiselect)
        return;

    if (!_selectedNodes.empty() &&
        _selectedNodes.front() == node)
        // already selected
        return;

    if (itemFromNode(node) == nullptr)
        // cannot select if the node is not in the tree.
        return;

    _selectedNodes.clear();
    _selectedNodes.push_back(node);
    updateSelection();
}

const std::vector<std::shared_ptr<TreeViewNode>>& TreeViewImpl::selectedNodes() const
{
    static const std::vector<std::shared_ptr<TreeViewNode>> empty;
    if (!_multiselect)
        return empty;
    return _selectedNodes;
}

void TreeViewImpl::selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes)
{
    if (!_multiselect)
        return;
    if (_selectedNodes == nodes)
        return;

    _selectedNodes.clear();
    for (auto& node : nodes)
    {
        // skip invalid
        if (itemFromNode(node) == nullptr)
            // cannot select if the node is not in the tree.
            continue;

        // skip duplicates
        if (std::find(
            _selectedNodes.begin(),
            _selectedNodes.end(),
            node) != _selectedNodes.end())
            continue;

        _selectedNodes.push_back(node);
    }

    updateSelection();
}

void TreeViewImpl::updateSelection()
{
    MessageLocker selectionChangedLock(_inhibitSelectionMessages);

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    for (const auto& [hitem, itemNode] : _nodeMap)
    {
        bool selected = std::find(
            _selectedNodes.begin(),
            _selectedNodes.end(),
            itemNode) != _selectedNodes.end();

        TreeView_SetItemState(hwnd, hitem, selected ? TVIS_SELECTED : 0, TVIS_SELECTED);
    }
}

bool TreeViewImpl::checkboxes() const
{
    return _checkboxes;
}

void TreeViewImpl::checkboxes(bool value)
{
    if (_checkboxes == value)
        return;
    _checkboxes = value;

    if (handle() == nullptr)
        return;

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

bool TreeViewImpl::editing() const
{
    return _editingLabel;
}

bool TreeViewImpl::beginEditing(std::shared_ptr<TreeViewNode> node)
{
    if (handle() == nullptr)
        return false;

    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return false;

    if (TreeView_EditLabel(hwnd, hitem) == NULL)
        return false;

    return true;
}

void TreeViewImpl::confirmEditing()
{
    if (!_editingLabel)
        return;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    TreeView_EndEditLabelNow(hwnd, FALSE);
}

void TreeViewImpl::cancelEditing()
{
    if (!_editingLabel)
        return;
    _editingLabel = false;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    TreeView_EndEditLabelNow(hwnd, TRUE);
}

bool TreeViewImpl::multiselect() const
{
    return _multiselect;
}

void TreeViewImpl::multiselect(bool value)
{
    if (_multiselect == value)
        return;
    _multiselect = value;

    if (handle() == nullptr)
        return;

    updateExtStyles();

    // clear selections
    _selectedNodes.clear();
    updateSelection();
}

void TreeViewImpl::updateDisplayStyles()
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

    TreeView_SetBkColor(hwnd, backColor);
    TreeView_SetTextColor(hwnd, textColor);
}

void TreeViewImpl::updateExtStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto dwStyle = (UINT)SendMessage(hwnd, TVM_GETEXTENDEDSTYLE, 0, 0);

    if (_multiselect)
        dwStyle |= TVS_EX_MULTISELECT;

    SendMessage(hwnd, TVM_SETEXTENDEDSTYLE, 0, dwStyle);
}

void TreeViewImpl::updateRootNode()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto node = _treeView->rootNode();
    TreeView_DeleteAllItems(hwnd);

    _hRootNode = NULL;
    _nodeMap.clear();

    if (node == nullptr)
        return;

    TVINSERTSTRUCT tvis = {};
    tvis.hParent = TVI_ROOT;
    tvis.hInsertAfter = TVI_FIRST;
    tvis.item.mask = TVIF_TEXT | TVIF_CHILDREN;
    tvis.item.pszText = LPSTR_TEXTCALLBACK;

    auto childCount = _treeView->childNodes(node).size();
    tvis.item.cChildren = (int)childCount;

    auto hItem = TreeView_InsertItem(hwnd, &tvis);

    if (hItem == NULL)
        DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());

    _hRootNode = hItem;
    _nodeMap[hItem] = node;
    DLGCPP_CMSG("Set root node " << node->id() << "  " << node->text() << "  nodeMap.size = " << _nodeMap.size());
}

void TreeViewImpl::updateChildNodes(std::shared_ptr<TreeViewNode> parent)
{
    if (parent == nullptr)
        return;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto children = _treeView->childNodes(parent);
    if (children.empty())
    {
        return;
    }

    auto hParent = itemFromNode(parent);
    if (hParent == NULL)
        return;

    TVINSERTSTRUCT tvis = {};
    tvis.item.mask = TVIF_TEXT | TVIF_CHILDREN;
    tvis.hParent = hParent;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.pszText = LPSTR_TEXTCALLBACK;

    for (auto& node : children)
    {
        auto childCount = _treeView->childNodes(node).size();
        tvis.item.cChildren = (int)childCount;

        auto hItem = TreeView_InsertItem(hwnd, &tvis);

        if (hItem == NULL)
        {
            DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());
            break;
        }

        _nodeMap[hItem] = node;
        tvis.hInsertAfter = hItem;

        DLGCPP_CMSG("Added child node " << node->id() << "  " << node->text() << "  nodeMap.size = " << _nodeMap.size());
    }
}

void TreeViewImpl::onRootNodeChanged()
{
    updateRootNode();
}

void TreeViewImpl::expand(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return;

    TreeView_Expand(hwnd, hitem, TVE_EXPAND);

    if (allChildNodes)
    {
        // recurse into children
        hitem = TreeView_GetChild(hwnd, hitem);
        while (hitem != NULL)
        {
            // expand further with the event
            _expandNodePrivateEvent.invoke(nodeFromItem(hitem));

            hitem = TreeView_GetNextSibling(hwnd, hitem);
        }
    }
}

void TreeViewImpl::collapse(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return;

    UINT flags = TVE_COLLAPSE;

    if (allChildNodes)
        // note: items are removed via TVN_DELETEITEM 
        flags |= TVE_COLLAPSERESET;

    TreeView_Expand(hwnd, hitem, flags);
}

IEvent<ISharedControl>& TreeViewImpl::RootNodeChangedEvent()
{
    return _rootNodeChangedEvent;
}

IEvent<ISharedControl>& TreeViewImpl::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeViewImpl::ItemClickEvent()
{
    return _itemClickEvent;
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeViewImpl::ItemDoubleClickEvent()
{
    return _itemDblClickEvent;
}

HTREEITEM TreeViewImpl::itemFromNode(std::shared_ptr<TreeViewNode> node)
{
    for (const auto& [hitem, itemNode] : _nodeMap)
    {
        if (itemNode == node)
        {
            return hitem;
        }
    }
    return NULL;
}

std::shared_ptr<TreeViewNode> TreeViewImpl::nodeFromItem(HTREEITEM hitem)
{
    auto it = _nodeMap.find(hitem);
    if (it == _nodeMap.end())
        return nullptr;
    return it->second;
}
