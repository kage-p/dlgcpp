#include "control_impl.h"
#include "treeview_impl.h"
#include "utility/string_encoder.h"
#include <strsafe.h>

#include <Windowsx.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

TreeViewImpl::TreeViewImpl() :
    ControlImpl()
{
    _expandNodePrivateEvent.reset([]() {return 1; }, "ExpandNodePrivateEvent");
}

TreeViewImpl::~TreeViewImpl()
{
}

void TreeViewImpl::owner(ITreeView* treeView)
{
    ControlImpl::owner(treeView);
    _treeView = treeView;

    _treeView->colors().event() += [&](auto) { updateDisplayStyles(); };

    _treeView->checkboxes().event() += [&](auto) { onCheckboxesChanged(); };
    _treeView->multiselect().event() += [&](auto) { onMultiselectChanged(); };
    _treeView->selectedNode().event() += [&](auto) { onSelectedNodeChanged(); };
    _treeView->selectedNodes().event() += [&](auto) { onSelectedNodesChanged(); };
    _treeView->RootNodeChangedEvent() += [this](ISharedControl) { onRootNodeChanged(); };

    _expandNodePrivateEvent += [this](int, ISharedTreeViewNode node) { expand(node, true); };
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

    if (_treeView->checkboxes())
    {
        styles |= TVS_CHECKBOXES;
    }

    return styles;
}

void TreeViewImpl::notify(ControlMessage& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the treeview colors
        auto hdc = (HDC)msg.wParam;

        auto clrPair = _treeView->colors().value();

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
            if (!_treeView->multiselect())
            {
                // single selection changed
                auto selectedNode = ISharedTreeViewNode();
                if (nmtv.itemNew.hItem != NULL)
                {
                    auto node = nodeFromItem(nmtv.itemNew.hItem);
                    if (node != nullptr)
                        selectedNode = node;
                }
                DLGCPP_CMSG("TVN_SELCHANGED: " << (selectedNode != nullptr ? 1 : 0) << " item(s) selected.");

                _treeView->selectedNodes().clear();
                _treeView->selectedNode() = selectedNode;
                _treeView->SelChangedEvent().invoke();
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
                {
                    node->checked() = checked;
                }

                if (_treeView->multiselect())
                {
                    bool wasSelected = nmic.uStateOld & TVIS_SELECTED;
                    bool isSelected = nmic.uStateNew & TVIS_SELECTED;

                    // item selection state changes
                    std::vector<ISharedTreeViewNode> selectedNodes = _treeView->selectedNodes();
                    if (!wasSelected && isSelected)
                    {
                        auto node = nodeFromItem(nmic.hItem);
                        if (node == nullptr)
                            return;
                        selectedNodes.push_back(node);
                    }
                    else if (wasSelected && !isSelected)
                    {
                        auto node = nodeFromItem(nmic.hItem);
                        if (node == nullptr)
                            return;

                        selectedNodes.erase(
                            std::remove(
                                selectedNodes.begin(),
                                selectedNodes.end(),
                                node),
                            selectedNodes.end());
                    }
                    else
                    {
                        // no change
                        return;
                    }

                    _treeView->selectedNode().clear();
                    _treeView->selectedNodes() = selectedNodes;
                    _treeView->SelChangedEvent().invoke();

                    DLGCPP_CMSG("TVN_ITEMCHANGED: " << selectedNodes.size() << " item(s) selected.");
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
            _treeView->editing() = true;
            msg.dlgResult = TRUE;
            msg.msgResult = FALSE;
        }
        else if (nmhdr.code == TVN_ENDLABELEDIT)
        {
            _treeView->editing() = false;

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
                    _treeView->ItemClickEvent().invoke(node);
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
                    _treeView->ItemDoubleClickEvent().invoke(node);
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
            _treeView->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _treeView->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _treeView->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _treeView->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _treeView->focus() = true;
            break;

        case NM_KILLFOCUS:
            _treeView->focus() = false;
            break;
        }
    }
    }
}

void TreeViewImpl::build()
{
    // remove editor
    cancelEditing();

    ControlImpl::build();

    if (hwnd() == 0)
        return;

    updateDisplayStyles();
    updateExtStyles();
    updateRootNode();
    updateSelection();
}

void TreeViewImpl::destruct()
{
    // remove editor
    cancelEditing();

    ControlImpl::destruct();
}

void TreeViewImpl::onCheckboxesChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());
}

void TreeViewImpl::onMultiselectChanged()
{
    if (hwnd() == 0)
        return;

    updateExtStyles();

    // clear selections
    _treeView->selectedNode().clear();
    _treeView->selectedNodes().clear();
    updateSelection();
}

// selection logic must be handled by impl.
void TreeViewImpl::onSelectedNodeChanged()
{
    if (_treeView->multiselect())
    {
        _treeView->selectedNode().clear();
        return;
    }

    auto selectedNode = _treeView->selectedNode().value();

    if (itemFromNode(selectedNode) == nullptr)
    {
        // cannot select if the node is not in the tree.
        _treeView->selectedNode().clear();
        return;
    }

    updateSelection();
}

// selection logic must be handled by impl.
void TreeViewImpl::onSelectedNodesChanged()
{
    if (!_treeView->multiselect())
    {
        _treeView->selectedNodes().clear();
        return;
    }

    auto selectedNodes = _treeView->selectedNodes().value();
    auto validatedNodes = std::vector<ISharedTreeViewNode>();
    for (auto& node : selectedNodes)
    {
        // skip invalid
        if (itemFromNode(node) == nullptr)
        {
            // cannot select if the node is not in the tree.
            continue;
        }

        // skip duplicates
        if (std::find(
            validatedNodes.begin(),
            validatedNodes.end(),
            node) != validatedNodes.end())
            continue;

        validatedNodes.push_back(node);
    }

    if (_treeView->selectedNodes().value() != validatedNodes)
    {
        // update the selection; this will trigger the event again
        _treeView->selectedNodes() = validatedNodes;
        return;
    }

    updateSelection();
}

void TreeViewImpl::updateSelection()
{
    MessageLocker selectionChangedLock(_inhibitSelectionMessages);

    if (hwnd() == 0)
        return;

    auto selectedNodes = std::vector<ISharedTreeViewNode>();

    if (!_treeView->multiselect())
    {
        auto& selectedNode = _treeView->selectedNode().value();
        if (selectedNode != nullptr)
        {
            selectedNodes.push_back(selectedNode);
        }
    }
    else
    {
        selectedNodes = _treeView->selectedNodes().value();
    }

    for (const auto& [hitem, itemNode] : _nodeMap)
    {
        bool selected = std::find(
            selectedNodes.begin(),
            selectedNodes.end(),
            itemNode) != selectedNodes.end();

        TreeView_SetItemState(hwnd(), hitem, selected ? TVIS_SELECTED : 0, TVIS_SELECTED);
    }
}

bool TreeViewImpl::beginEditing(ISharedTreeViewNode node)
{
    if (hwnd() == 0)
        return false;

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return false;

    if (TreeView_EditLabel(hwnd(), hitem) == NULL)
        return false;

    return true;
}

void TreeViewImpl::confirmEditing()
{
    if (!_treeView->editing())
        return;

    if (hwnd() == 0)
        return;

    TreeView_EndEditLabelNow(hwnd(), FALSE);
}

void TreeViewImpl::cancelEditing()
{
    if (!_treeView->editing())
        return;
    _treeView->editing() = false;

    if (hwnd() == 0)
        return;

    TreeView_EndEditLabelNow(hwnd(), TRUE);
}

void TreeViewImpl::updateDisplayStyles()
{
    if (hwnd() == 0)
        return;

    auto clrPair = _treeView->colors().value();

    COLORREF backColor =
        clrPair.second != Color::Default
        ? (COLORREF)clrPair.second
        : GetSysColor(COLOR_WINDOW);
    COLORREF textColor =
        clrPair.first != Color::Default
        ? (COLORREF)clrPair.first
        : GetSysColor(COLOR_WINDOWTEXT);

    TreeView_SetBkColor(hwnd(), backColor);
    TreeView_SetTextColor(hwnd(), textColor);
}

void TreeViewImpl::updateExtStyles()
{
    auto dwStyle = (UINT)SendMessage(hwnd(), TVM_GETEXTENDEDSTYLE, 0, 0);

    if (_treeView->multiselect())
    {
        dwStyle |= TVS_EX_MULTISELECT;
    }

    SendMessage(hwnd(), TVM_SETEXTENDEDSTYLE, 0, dwStyle);
}

void TreeViewImpl::updateRootNode()
{
    if (hwnd() == 0)
        return;

    auto node = _treeView->rootNode();
    TreeView_DeleteAllItems(hwnd());

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

    auto hItem = TreeView_InsertItem(hwnd(), &tvis);

    if (hItem == NULL)
        DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());

    _hRootNode = hItem;
    _nodeMap[hItem] = node;
    DLGCPP_CMSG("Set root node " << node->id().value() << "  " << node->text().value() << "  nodeMap.size = " << _nodeMap.size());
}

void TreeViewImpl::updateChildNodes(ISharedTreeViewNode parent)
{
    if (parent == nullptr)
        return;

    if (hwnd() == 0)
        return;

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

        auto hItem = TreeView_InsertItem(hwnd(), &tvis);

        if (hItem == NULL)
        {
            DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());
            break;
        }

        _nodeMap[hItem] = node;
        tvis.hInsertAfter = hItem;

        DLGCPP_CMSG("Added child node " << node->id().value() << "  " << node->text().value() << "  nodeMap.size = " << _nodeMap.size());
    }
}

void TreeViewImpl::onRootNodeChanged()
{
    updateRootNode();
}

void TreeViewImpl::expand(
    ISharedTreeViewNode node,
    bool allChildNodes)
{
    if (hwnd() == 0)
        return;

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return;

    auto hwnd = this->hwnd();
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
    ISharedTreeViewNode node,
    bool allChildNodes)
{
    if (hwnd() == 0)
        return;
    auto hwnd = this->hwnd();

    auto hitem = itemFromNode(node);
    if (hitem == NULL)
        return;

    UINT flags = TVE_COLLAPSE;

    if (allChildNodes)
        // note: items are removed via TVN_DELETEITEM 
        flags |= TVE_COLLAPSERESET;

    TreeView_Expand(hwnd, hitem, flags);
}

HTREEITEM TreeViewImpl::itemFromNode(ISharedTreeViewNode node)
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

ISharedTreeViewNode TreeViewImpl::nodeFromItem(HTREEITEM hitem)
{
    auto it = _nodeMap.find(hitem);
    if (it == _nodeMap.end())
        return nullptr;
    return it->second;
}
