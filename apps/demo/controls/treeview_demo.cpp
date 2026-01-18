#include "treeview_demo.h"
#include <vector>

using namespace dlgcpp;

std::shared_ptr<Menu> createMenu(
    std::shared_ptr<DemoTreeView> TreeView);

void controls_treeview_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("TreeView control demo");
    dlg->resize({ 450,350 });
    dlg->center();

    auto label = std::make_shared<Label>("Right click on tree for options", Position{ 10, 10, 430, 15 });
    dlg->add(label);

    auto treeView = std::make_shared<DemoTreeView>();
    treeView->p(Position{ 10, 25, 430, 315 });
    treeView->colors(Color::Gray, Color::White);
    treeView->font({ "Arial", 12, 0 });
    dlg->add(treeView);

    auto menu = createMenu(treeView);

    // explicitly enabled these events so the handlers get fired
    treeView->wantKeyboardEvents(true);
    treeView->wantMouseEvents(true);

    treeView->MouseDownEvent() +=
        [menu](ISharedControl control, MouseEvent event)
        {
            // show the menu on right click
            if (event.button == MouseButton::Right)
                menu->popup(control, event.point);
        };

    treeView->KeyDownEvent() +=
        [treeView](ISharedControl, KeyboardEvent event)
        {
            if (event.key == Key::Enter)
            {
                // user pressed enter key in the TreeView
                if (treeView->selectedNode() != nullptr)
                {
                    treeView->beginEditing(treeView->selectedNode());
                }
            }
            else if (event.key == Key::Escape)
            {
                // user pressed escape key in the TreeView
                treeView->cancelEditing();
            }
        };

    // create sample items using path

    auto items = std::vector<DemoTreeViewItem>
    {
        { "Root", "Root" },
        { "Root/Group1", "Group 1" },
        { "Root/Group2", "Group 2" },
        { "Root/Group3", "Group 3" },
        { "Root/Group1/Item1", "Group 1 - Item 1" },
        { "Root/Group1/Item1/Leaf1", "Leaf 1" },
        { "Root/Group2/Item1", "Group 2 - Item 1" },
        { "Root/Group2/Item1/Leaf1", "Leaf 1" },
        { "Root/Group2/Item1/Leaf2", "Leaf 2" },
        { "Root/Group3/Item1", "Group 3 - Item 1" },
        { "Root/Group3/Item1/Leaf1", "Leaf 1" },
        { "Root/Group3/Item1/Leaf2", "Leaf 2" },
        { "Root/Group3/Item1/Leaf3", "Leaf 3" }
    };
    treeView->setItems(items);

    dlg->exec();
}

std::shared_ptr<Menu> createMenu(
    std::shared_ptr<DemoTreeView> treeView)
{
    auto menu = std::make_shared<Menu>();
    ISharedMenuItem item;

    item = std::make_shared<MenuItem>("Expand all");
    menu->add(item);
    item->ClickEvent() += [treeView](ISharedMenuItem m)
        {
            treeView->expand(treeView->rootNode(), true);
        };

    item = std::make_shared<MenuItem>("Collapse all");
    menu->add(item);
    item->ClickEvent() += [treeView](ISharedMenuItem m)
        {
            treeView->collapse(treeView->rootNode(), true);
        };

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    item = std::make_shared<MenuItem>("Show check boxes");
    menu->add(item);
    item->checked() = treeView->checkboxes();
    item->ClickEvent() += [treeView](ISharedMenuItem m)
        {
            treeView->checkboxes(!treeView->checkboxes());
            m->checked() = treeView->checkboxes();
        };

    item = std::make_shared<MenuItem>("Multi-select items");
    menu->add(item);
    item->checked() = treeView->multiselect();
    item->ClickEvent() += [treeView](ISharedMenuItem m)
        {
            treeView->multiselect(!treeView->multiselect());
            m->checked() = treeView->multiselect();
        };

    return menu;
}

DemoTreeView::DemoTreeView() :
    TreeView()
{
}

ISharedTreeViewNode DemoTreeView::rootNode() const
{
    return _rootNode;
}

std::vector<ISharedTreeViewNode> DemoTreeView::childNodes(ISharedTreeViewNode parent) const
{
    // return nodes for parent item
    auto children = std::vector<ISharedTreeViewNode>();

    auto [first, last] = _nodeMap.equal_range(parent->tag());
    for (auto& it = first; it != last; ++it)
        children.push_back(it->second);

    return children;
}

bool DemoTreeView::beginEdit(ISharedTreeViewNode node)
{
    if (node == rootNode())
        // prevent editing root node
        return false;

    // allow edit
    return true;
}

bool DemoTreeView::endEdit(ISharedTreeViewNode node, const std::string& text)
{
    if (node == rootNode())
        // prevent editing root node
        return false;

    node->text() = text;
    return true;
}

void DemoTreeView::setItems(
    const std::vector<DemoTreeViewItem>& items)
{
    _rootNode = nullptr;

    auto getParentPath =
        [](const std::string& path) -> std::string
        {
            size_t pos = path.rfind('/');
            if (pos == std::string::npos)
                return { }; // No parent
            return path.substr(0, pos);
        };

    // store nodes to preserve state info 
    for (const auto& item : items)
    {
        std::string key = getParentPath(item.path);

        auto node = std::make_shared<TreeViewNode>(
            item.path,
            item.text);

        _nodeMap.insert({ key, node });
    }

    auto it = _nodeMap.find({});
    if (it != _nodeMap.end())
    {
        _rootNode = it->second;
    }

    // tell the treeview we changed the root node
    RootNodeChangedEvent().invoke();
}