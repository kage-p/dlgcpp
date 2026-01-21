#include "dlgcpp/controls/treeview_node.h"
#include "treeview_node_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(TreeViewNodeTests, constructor_default)
{
    auto target = std::make_shared<TreeViewNode>();

    EXPECT_EQ(target->id(), 0);
    EXPECT_EQ(target->tag(), std::string());
}

TEST_F(TreeViewNodeTests, constructor_with_id_param)
{
    auto target = std::make_shared<TreeViewNode>(123, "text", true);

    EXPECT_EQ(target->id(), 123);
    EXPECT_EQ(target->tag(), std::string());
    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->checked(), true);
}

TEST_F(TreeViewNodeTests, constructor_with_tag_param)
{
    auto target = std::make_shared<TreeViewNode>("tag", "text", true);

    EXPECT_EQ(target->id(), 0);
    EXPECT_EQ(target->tag(), std::string("tag"));
    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->checked(), true);
}

TEST_F(TreeViewNodeTests, text)
{
    auto target = std::make_shared<TreeViewNode>();

    EXPECT_EQ(target->text(), std::string());

    target->text() = "text";
    EXPECT_EQ(target->text(), std::string("text"));

    target->text() = "";
    EXPECT_EQ(target->text(), std::string());
}

TEST_F(TreeViewNodeTests, checked)
{
    auto target = std::make_shared<TreeViewNode>();

    EXPECT_EQ(target->checked(), false);

    target->checked() = true;
    EXPECT_EQ(target->checked(), true);

    target->checked() = false;
    EXPECT_EQ(target->checked(), false);
}
