#include "dlgcpp/controls/treeview.h"
#include "treeview_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

class TreeViewInTest : public TreeView
{
public:
    explicit TreeViewInTest(
        const Position& p = Position()) : TreeView(p)
    {
    }
    ~TreeViewInTest() override = default;

    void setRootNode(ISharedTreeViewNode node)
    {
        _rootNode = node;
        RootNodeChangedEvent().invoke();
    }

    void setChildNodes(std::vector<ISharedTreeViewNode> nodes)
    {
        _childNodes = nodes;
    }

    // override default placeholders
    ISharedTreeViewNode rootNode() const override
    {
        return _rootNode;
    }

    std::vector<ISharedTreeViewNode> childNodes(ISharedTreeViewNode parent) const
    {
        return std::vector<ISharedTreeViewNode>();
    }

    bool beginEdit(ISharedTreeViewNode node) override
    {
        return false;
    }

    bool endEdit(ISharedTreeViewNode node, const std::string& text) override
    {
        return false;
    }
private:
    ISharedTreeViewNode _rootNode;
    std::vector<ISharedTreeViewNode> _childNodes;
};

TEST_F(TreeViewTests, constructor_default)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(TreeViewTests, constructor_with_params)
{
    auto target = std::make_shared<TreeView>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(TreeViewTests, checkboxes)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_EQ(target->checkboxes(), false);

    target->checkboxes(true);
    EXPECT_EQ(target->checkboxes(), true);

    target->checkboxes(false);
    EXPECT_EQ(target->checkboxes(), false);
}

TEST_F(TreeViewTests, editing)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_EQ(target->editing(), false);
}

TEST_F(TreeViewTests, beginEditing)
{
    auto target = std::make_shared<TreeView>();

    // no-op
    target->beginEditing(nullptr);

    EXPECT_EQ(target->editing(), false);
}

TEST_F(TreeViewTests, confirmEditing)
{
    auto target = std::make_shared<TreeView>();

    // no-op
    target->confirmEditing();
}

TEST_F(TreeViewTests, cancelEditing)
{
    auto target = std::make_shared<TreeView>();

    // no-op
    target->cancelEditing();
}

TEST_F(TreeViewTests, multiselect)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_EQ(target->multiselect(), false);

    target->multiselect(true);
    EXPECT_EQ(target->multiselect(), true);

    target->multiselect(false);
    EXPECT_EQ(target->multiselect(), false);
}

TEST_F(TreeViewTests, selectedNode)
{
    auto target = std::make_shared<TreeViewInTest>();

    EXPECT_EQ(target->selectedNode(), nullptr);

    auto root = std::make_shared<TreeViewNode>("root");
    target->setRootNode(root);

    // expect failure as control is not active, and node does not exist in treeview
    target->selectedNode(root);
    EXPECT_EQ(target->selectedNode().empty(), true);

    target->selectedNode(nullptr);
    EXPECT_EQ(target->selectedNode().empty(), true);
}

TEST_F(TreeViewTests, selectedNodes)
{
    auto target = std::make_shared<TreeViewInTest>();

    target->multiselect(true);

    EXPECT_EQ(target->selectedNode(), nullptr);

    auto root = std::make_shared<TreeViewNode>("root");
    target->setRootNode(root);

    // expect failure as control is not active, and node does not exist in treeview
    auto selectedNodes = std::vector<ISharedTreeViewNode>{ root };
    target->selectedNodes(selectedNodes);
    EXPECT_EQ(target->selectedNodes().empty(), true);

    selectedNodes.clear();
    target->selectedNodes(selectedNodes);
    EXPECT_EQ(target->selectedNodes().empty(), true);
}

// test placeholder defaults

TEST_F(TreeViewTests, rootNode)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_EQ(target->rootNode(), nullptr);
}

TEST_F(TreeViewTests, childNodes)
{
    auto target = std::make_shared<TreeView>();

    EXPECT_TRUE(target->childNodes(nullptr).empty());
}

TEST_F(TreeViewTests, beginEdit)
{
    auto target = std::make_shared<TreeView>();

    // no op
    ASSERT_FALSE(target->beginEdit(nullptr));
}

TEST_F(TreeViewTests, endEdit)
{
    auto target = std::make_shared<TreeView>();

    // no op
    ASSERT_FALSE(target->endEdit(nullptr, {}));
}

TEST_F(TreeViewTests, expand)
{
    auto target = std::make_shared<TreeView>();

    // no op
    target->expand(nullptr);
}

TEST_F(TreeViewTests, collapse)
{
    auto target = std::make_shared<TreeView>();

    // no op
    target->collapse(nullptr);
    target->collapse(nullptr, true);
}

TEST_F(TreeViewTests, events)
{
    auto target = std::make_shared<TreeView>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();
    auto& rootNodeChangedEvent = target->RootNodeChangedEvent();
    auto& selChangedEvent = target->SelChangedEvent();
    auto& itemClickEvent = target->ItemClickEvent();
    auto& itemDoubleClickEvent = target->ItemDoubleClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
