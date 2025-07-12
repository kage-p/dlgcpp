#include "control_p.h"
#include "treeview_p.h"
#include "utility/message.h"
#include "utility/string.h"
#include "utility/units.h"
#include <strsafe.h>

#include <Windowsx.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

HTREEITEM itemFromNode(
    treeview_priv& priv,
    std::shared_ptr<TreeViewNode> node);

std::shared_ptr<TreeViewNode> nodeFromItem(
    treeview_priv& priv,
    HTREEITEM hitem);

TreeView::TreeView(const Position& p) :
    Control(std::string(), p),
    _priv(new treeview_priv())
{
    Control::border(BorderStyle::Sunken);

    _priv->rootNodeChangedEvent += [this](ISharedControl) { onRootNodeChanged(); };
}

TreeView::~TreeView()
{
    delete _priv;
}

std::string TreeView::className() const
{
    return WC_TREEVIEWA;
}

unsigned int TreeView::styles() const
{
    auto styles = Control::styles();

    styles |=
        TVS_HASBUTTONS |
        TVS_HASLINES |
        TVS_LINESATROOT |
        TVS_FULLROWSELECT |
        TVS_EDITLABELS;

    if (_priv->checkboxes)
        styles |= TVS_CHECKBOXES;

    return styles;
}

void TreeView::notify(ctl_message& msg)
{
    if (msg.wMsg == WM_CTLCOLOREDIT)
    {
        // use the treeview colors
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

void TreeView::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);

        if (nmhdr.code == TVN_GETDISPINFO)
        {
            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);

            auto node = nodeFromItem(*_priv, nmdi.item.hItem);
            if (node == nullptr)
                return;

            // set the item text
            if (nmdi.item.mask & TVIF_TEXT)
            {
                auto wide = toWide(node->text());
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

            auto node = nodeFromItem(*_priv, hitem);
            if (node != nullptr)
                updateChildNodes(node);
        }
        else if (nmhdr.code == TVN_SELCHANGED)
        {
            auto& nmtv = *((LPNMTREEVIEW)msg.lParam);
            if (!_priv->multiselect)
            {
                // single selection changed
                _priv->selectedNodes.clear();
                if (nmtv.itemNew.hItem != NULL)
                {
                    auto node = nodeFromItem(*_priv, nmtv.itemNew.hItem);
                    if (node != nullptr)
                        _priv->selectedNodes.push_back(node);
                }
                DLGCPP_CMSG("TVN_SELCHANGED: " << _priv->selectedNodes.size() << " item(s) selected.");
                _priv->selChangedEvent.invoke(shared_from_this());
            }
        }
        else if (nmhdr.code == TVN_ITEMCHANGED)
        {
            if (_priv->inhibitSelectionMessages.get())
                return;

            auto& nmic = *((NMTVITEMCHANGE*)msg.lParam);
            if (nmic.uChanged & TVIF_STATE)
            {
                // update node check state
                auto hitem = nmic.hItem;
                bool checked = TreeView_GetCheckState(nmhdr.hwndFrom, hitem) == 1;
                auto node = nodeFromItem(*_priv, hitem);
                if (node != nullptr)
                    node->checked(checked);

                if (_priv->multiselect)
                {
                    bool wasSelected = nmic.uStateOld & TVIS_SELECTED;
                    bool isSelected = nmic.uStateNew & TVIS_SELECTED;

                    // item selection state changes
                    if (!wasSelected && isSelected)
                    {
                        auto node = nodeFromItem(*_priv, nmic.hItem);
                        if (node == nullptr)
                            return;
                        _priv->selectedNodes.push_back(node);
                    }
                    else if (wasSelected && !isSelected)
                    {
                        auto node = nodeFromItem(*_priv, nmic.hItem);
                        if (node == nullptr)
                            return;

                        _priv->selectedNodes.erase(
                            std::remove(
                                _priv->selectedNodes.begin(),
                                _priv->selectedNodes.end(),
                                node),
                            _priv->selectedNodes.end());
                    }
                    else
                        return;

                    _priv->selChangedEvent.invoke(shared_from_this());
                    DLGCPP_CMSG("TVN_ITEMCHANGED: " << _priv->selectedNodes.size() << " item(s) selected.");
                }
            }
        }
        else if (nmhdr.code == TVN_DELETEITEM)
        {
            auto& nmtv = *((LPNMTREEVIEW)msg.lParam);

            // remove from our map
            auto hitem = nmtv.itemOld.hItem;
            _priv->nodeMap.erase(hitem);

            DLGCPP_CMSG("TVN_DELETEITEM: nodeMap.size = " << _priv->nodeMap.size());
        }
        else if (nmhdr.code == TVN_BEGINLABELEDIT)
        {
            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);

            auto hitem = nmdi.item.hItem;
            auto node = nodeFromItem(*_priv, hitem);
            if (node == nullptr ||
                !beginEdit(node))
            {
                // prevent edit of this item
                msg.dlgResult = TRUE;
                msg.msgResult = TRUE;
                return;
            }
            _priv->editingLabel = true;
            msg.dlgResult = TRUE;
            msg.msgResult = FALSE;
        }
        else if (nmhdr.code == TVN_ENDLABELEDIT)
        {
            _priv->editingLabel = false;

            auto& nmdi = *((NMTVDISPINFO*)msg.lParam);
            if (nmdi.item.pszText != NULL)
            {
                // save the edited text
                auto hitem = nmdi.item.hItem;
                auto node = nodeFromItem(*_priv, hitem);
                if (node != nullptr)
                {
                    if (!endEdit(node, toBytes(nmdi.item.pszText)))
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
                auto node = nodeFromItem(*_priv, hit.hItem);
                if (node != nullptr)
                {
                    // fire item click event
                    _priv->itemClickEvent.invoke(
                        shared_from_this(),
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
                auto node = nodeFromItem(*_priv, hit.hItem);
                if (node != nullptr)
                {
                    // fire item double click event
                    _priv->itemDblClickEvent.invoke(
                        shared_from_this(),
                        node);
                }
            }
        }
    }

    Control::notify(msg);
}

void TreeView::rebuild()
{
    // remove editor
    cancelEditing();

    Control::rebuild();

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
    updateExtStyles();
    updateRootNode();
    updateSelection();
}

void TreeView::colors(Color fgColor, Color bgColor)
{
    Control::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
}

std::shared_ptr<TreeViewNode> TreeView::selectedNode() const
{
    if (_priv->multiselect || _priv->selectedNodes.empty())
        return nullptr;
    return _priv->selectedNodes.at(0);
}

void TreeView::selectedNode(std::shared_ptr<TreeViewNode> node)
{
    if (_priv->multiselect)
        return;

    if (!_priv->selectedNodes.empty() &&
        _priv->selectedNodes.front() == node)
        // already selected
        return;

    if (itemFromNode(*_priv, node) == nullptr)
        // cannot select if the node is not in the tree.
        return;

    _priv->selectedNodes.clear();
    _priv->selectedNodes.push_back(node);
    updateSelection();
}

const std::vector<std::shared_ptr<TreeViewNode>>& TreeView::selectedNodes() const
{
    static const std::vector<std::shared_ptr<TreeViewNode>> empty;
    if (!_priv->multiselect)
        return empty;
    return _priv->selectedNodes;
}

void TreeView::selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes)
{
    if (!_priv->multiselect)
        return;
    if (_priv->selectedNodes == nodes)
        return;

    _priv->selectedNodes.clear();
    for (auto& node : nodes)
    {
        // skip invalid
        if (itemFromNode(*_priv, node) == nullptr)
            // cannot select if the node is not in the tree.
            continue;

        // skip duplicates
        if (std::find(
            _priv->selectedNodes.begin(),
            _priv->selectedNodes.end(),
            node) != _priv->selectedNodes.end())
            continue;

        _priv->selectedNodes.push_back(node);
    }

    updateSelection();
}

void TreeView::updateSelection()
{
    MessageLocker selectionChangedLock(_priv->inhibitSelectionMessages);

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    for (const auto& [hitem, itemNode] : _priv->nodeMap)
    {
        bool selected = std::find(
            _priv->selectedNodes.begin(),
            _priv->selectedNodes.end(),
            itemNode) != _priv->selectedNodes.end();

        TreeView_SetItemState(hwnd, hitem, selected ? TVIS_SELECTED : 0, TVIS_SELECTED);
    }
}

bool TreeView::checkboxes() const
{
    return _priv->checkboxes;
}

void TreeView::checkboxes(bool value)
{
    if (_priv->checkboxes == value)
        return;
    _priv->checkboxes = value;

    if (handle() == nullptr)
        return;

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

bool TreeView::editing() const
{
    return _priv->editingLabel;
}

bool TreeView::beginEditing(std::shared_ptr<TreeViewNode> node)
{
    if (handle() == nullptr)
        return false;

    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(*_priv, node);
    if (hitem == NULL)
        return false;

    if (TreeView_EditLabel(hwnd, hitem) == NULL)
        return false;

    return true;
}

void TreeView::confirmEditing()
{
    if (!_priv->editingLabel)
        return;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    TreeView_EndEditLabelNow(hwnd, FALSE);
}

void TreeView::cancelEditing()
{
    if (!_priv->editingLabel)
        return;
    _priv->editingLabel = false;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    TreeView_EndEditLabelNow(hwnd, TRUE);
}

bool TreeView::multiselect() const
{
    return _priv->multiselect;
}

void TreeView::multiselect(bool value)
{
    if (_priv->multiselect == value)
        return;
    _priv->multiselect = value;

    if (handle() == nullptr)
        return;

    updateExtStyles();

    // clear selections
    _priv->selectedNodes.clear();
    updateSelection();
}

std::shared_ptr<TreeViewNode> TreeView::rootNode() const
{
    // placeholder
    return nullptr;
}

std::vector<std::shared_ptr<TreeViewNode>> TreeView::childNodes(std::shared_ptr<TreeViewNode> parent) const
{
    // placeholder
    return {};
}

bool TreeView::beginEdit(std::shared_ptr<TreeViewNode> node)
{
    // placeholder
    return false;
}

bool TreeView::endEdit(std::shared_ptr<TreeViewNode> node, const std::string& text)
{
    // placeholder
    return false;
}

void TreeView::updateDisplayStyles()
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

    TreeView_SetBkColor(hwnd, backColor);
    TreeView_SetTextColor(hwnd, textColor);
}

void TreeView::updateExtStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto dwStyle = (UINT)SendMessage(hwnd, TVM_GETEXTENDEDSTYLE, 0, 0);

    if (_priv->multiselect)
        dwStyle |= TVS_EX_MULTISELECT;

    SendMessage(hwnd, TVM_SETEXTENDEDSTYLE, 0, dwStyle);
}

void TreeView::updateRootNode()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto node = rootNode();
    TreeView_DeleteAllItems(hwnd);

    _priv->hRootNode = NULL;
    _priv->nodeMap.clear();

    if (node == nullptr)
        return;

    TVINSERTSTRUCT tvis = {};
    tvis.hParent = TVI_ROOT;
    tvis.hInsertAfter = TVI_FIRST;
    tvis.item.mask = TVIF_TEXT | TVIF_CHILDREN;
    tvis.item.pszText = LPSTR_TEXTCALLBACK;

    auto childCount = childNodes(node).size();
    tvis.item.cChildren = (int)childCount;

    auto hItem = TreeView_InsertItem(hwnd, &tvis);

    if (hItem == NULL)
        DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());

    _priv->hRootNode = hItem;
    _priv->nodeMap[hItem] = node;
    DLGCPP_CMSG("Set root node " << node->id() << "  " << node->text() << "  nodeMap.size = " << _priv->nodeMap.size());
}

void TreeView::updateChildNodes(std::shared_ptr<TreeViewNode> parent)
{
    if (parent == nullptr)
        return;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto children = childNodes(parent);
    if (children.empty())
    {
        return;
    }

    auto hParent = itemFromNode(*_priv, parent);
    if (hParent == NULL)
        return;

    TVINSERTSTRUCT tvis = {};
    tvis.item.mask = TVIF_TEXT | TVIF_CHILDREN;
    tvis.hParent = hParent;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.pszText = LPSTR_TEXTCALLBACK;

    for (auto& node : children)
    {
        auto childCount = childNodes(node).size();
        tvis.item.cChildren = (int)childCount;

        auto hItem = TreeView_InsertItem(hwnd, &tvis);

        if (hItem == NULL)
        {
            DLGCPP_CERR("TreeView_InsertItem " << hItem << "   GetLastError = " << GetLastError());
            break;
        }

        _priv->nodeMap[hItem] = node;
        tvis.hInsertAfter = hItem;

        DLGCPP_CMSG("Added child node " << node->id() << "  " << node->text() << "  nodeMap.size = " << _priv->nodeMap.size());
    }
}

void TreeView::onRootNodeChanged()
{
    updateRootNode();
}

void TreeView::expand(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(*_priv, node);
    if (hitem == NULL)
        return;

    TreeView_Expand(hwnd, hitem, TVE_EXPAND);

    if (allChildNodes)
    {
        // recurse into children
        hitem = TreeView_GetChild(hwnd, hitem);
        while (hitem != NULL)
        {
            TreeView_Expand(hwnd, hitem, TVE_EXPAND);
            hitem = TreeView_GetNextSibling(hwnd, hitem);
        }
    }
}

void TreeView::collapse(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto hitem = itemFromNode(*_priv, node);
    if (hitem == NULL)
        return;

    UINT flags = TVE_COLLAPSE;

    if (allChildNodes)
        // note: items are removed via TVN_DELETEITEM 
        flags |= TVE_COLLAPSERESET;

    TreeView_Expand(hwnd, hitem, flags);
}

IEvent<ISharedControl>& TreeView::RootNodeChangedEvent()
{
    return _priv->rootNodeChangedEvent;
}

IEvent<ISharedControl>& TreeView::SelChangedEvent()
{
    return _priv->selChangedEvent;
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeView::ItemClickEvent()
{
    return _priv->itemClickEvent;
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeView::ItemDoubleClickEvent()
{
    return _priv->itemDblClickEvent;
}

HTREEITEM itemFromNode(
    treeview_priv& priv,
    std::shared_ptr<TreeViewNode> node)
{
    for (const auto& [hitem, itemNode] : priv.nodeMap)
    {
        if (itemNode == node)
        {
            return hitem;
        }
    }
    return NULL;
}

std::shared_ptr<TreeViewNode> nodeFromItem(
    treeview_priv& priv,
    HTREEITEM hitem)
{
    auto it = priv.nodeMap.find(hitem);
    if (it == priv.nodeMap.end())
        return nullptr;
    return it->second;
}
