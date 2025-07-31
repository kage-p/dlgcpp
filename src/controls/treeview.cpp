#include "controls/treeview_p.h"
#include "dlgcpp/controls/treeview.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TreeView::TreeView(const Position& p)
    : TreeView(std::make_shared<TreeViewImpl>(this, p))
{
}

TreeView::TreeView(std::shared_ptr<TreeViewImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
    Control::border(BorderStyle::Sunken);
}

TreeView::~TreeView()
{
}

std::shared_ptr<TreeViewNode> TreeView::selectedNode() const
{
    return _impl->selectedNode();
}

void TreeView::selectedNode(std::shared_ptr<TreeViewNode> node)
{
    _impl->selectedNode(node);
}

const std::vector<std::shared_ptr<TreeViewNode>>& TreeView::selectedNodes() const
{
    return _impl->selectedNodes();
}

void TreeView::selectedNodes(const std::vector<std::shared_ptr<TreeViewNode>>& nodes)
{
    if (_impl->selectedNodes() == nodes)
        return;

    _impl->selectedNodes(nodes);
}

bool TreeView::checkboxes() const
{
    return _impl->checkboxes();
}

void TreeView::checkboxes(bool value)
{
    if (_impl->checkboxes() == value)
        return;
    _impl->checkboxes(value);
}

bool TreeView::editing() const
{
    return _impl->editing();
}

bool TreeView::beginEditing(std::shared_ptr<TreeViewNode> node)
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

bool TreeView::multiselect() const
{
    return _impl->multiselect();
}

void TreeView::multiselect(bool value)
{
    if (_impl->multiselect() == value)
        return;
    _impl->multiselect(value);
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

void TreeView::expand(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    _impl->expand(node, allChildNodes);
}

void TreeView::collapse(
    std::shared_ptr<TreeViewNode> node,
    bool allChildNodes)
{
    _impl->collapse(node, allChildNodes);
}

IEvent<ISharedControl>& TreeView::RootNodeChangedEvent()
{
    return _impl->RootNodeChangedEvent();
}

IEvent<ISharedControl>& TreeView::SelChangedEvent()
{
    return _impl->SelChangedEvent();
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeView::ItemClickEvent()
{
    return _impl->ItemClickEvent();
}

IEvent<ISharedControl, std::shared_ptr<TreeViewNode>>& TreeView::ItemDoubleClickEvent()
{
    return _impl->ItemDoubleClickEvent();
}
