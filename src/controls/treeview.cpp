#include "controls/treeview_impl.h"
#include "dlgcpp/controls/treeview.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TreeView::TreeView(const Position& p)
    : TreeView(std::make_shared<TreeViewImpl>(), p)
{
    border() = BorderStyle::Sunken;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
    _checkboxes.reset(false, nullptr, ownerFn, "checkboxes");
    _editing.reset(false, [](auto) {return false; }, ownerFn, "editing");
    _multiselect.reset(false, nullptr, ownerFn, "multiselect");
    _selectedNode.reset(nullptr, nullptr, ownerFn, "selectedNode");
    _selectedNodes.reset(std::vector<ISharedTreeViewNode>(), nullptr, ownerFn, "selectedNodes");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _dblRightClickEvent.reset(ownerFn, "DoubleRightClickEvent");
    _rightClickEvent.reset(ownerFn, "RightClickEvent");
    _rootNodeChangedEvent.reset(ownerFn, "RootNodeChangedEvent");
    _selChangedEvent.reset(ownerFn, "SelChangedEvent");
    _itemClickEvent.reset(ownerFn, "ItemClickEvent");
    _itemDblClickEvent.reset(ownerFn, "ItemDoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

TreeView::TreeView(
    std::shared_ptr<TreeViewImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

TreeView::~TreeView()
{
    _impl.reset();
}

IProperty<ISharedTreeViewNode, ISharedControl>& TreeView::selectedNode()
{
    return _selectedNode;
}

void TreeView::selectedNode(ISharedTreeViewNode node)
{
    _selectedNode = node;
}

IProperty<std::vector<ISharedTreeViewNode>, ISharedControl>& TreeView::selectedNodes()
{
    return _selectedNodes;
}

void TreeView::selectedNodes(const std::vector<ISharedTreeViewNode>& nodes)
{
    _selectedNodes = nodes;
}

IProperty<bool, ISharedControl>& TreeView::checkboxes()
{
    return _checkboxes;
}

void TreeView::checkboxes(bool value)
{
    _checkboxes = value;
}

IProperty<bool, ISharedControl>& TreeView::editing()
{
    return _editing;
}

IProperty<bool, ISharedControl>& TreeView::multiselect()
{
    return _multiselect;
}

void TreeView::multiselect(bool value)
{
    _multiselect = value;
}


bool TreeView::beginEditing(ISharedTreeViewNode node)
{
    return _impl->beginEditing(node);
}

void TreeView::confirmEditing()
{
    _impl->confirmEditing();
}

void TreeView::cancelEditing()
{
    _impl->cancelEditing();
}

ISharedTreeViewNode TreeView::rootNode() const
{
    // placeholder
    return nullptr;
}

std::vector<ISharedTreeViewNode> TreeView::childNodes(ISharedTreeViewNode parent) const
{
    // placeholder
    return {};
}

bool TreeView::beginEdit(ISharedTreeViewNode node)
{
    // placeholder
    return false;
}

bool TreeView::endEdit(ISharedTreeViewNode node, const std::string& text)
{
    // placeholder
    return false;
}

void TreeView::expand(
    ISharedTreeViewNode node,
    bool allChildNodes)
{
    _impl->expand(node, allChildNodes);
}

void TreeView::collapse(
    ISharedTreeViewNode node,
    bool allChildNodes)
{
    _impl->collapse(node, allChildNodes);
}

IEvent<ISharedControl>& TreeView::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& TreeView::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& TreeView::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& TreeView::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}

IEvent<ISharedControl>& TreeView::RootNodeChangedEvent()
{
    return _rootNodeChangedEvent;
}

IEvent<ISharedControl>& TreeView::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl, ISharedTreeViewNode>& TreeView::ItemClickEvent()
{
    return _itemClickEvent;
}

IEvent<ISharedControl, ISharedTreeViewNode>& TreeView::ItemDoubleClickEvent()
{
    return _itemDblClickEvent;
}
