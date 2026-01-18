#include "dlgcpp/controls/listview_col.h"
#include "listview_col_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ListViewColumnTests, constructor_default)
{
    auto target = std::make_shared<ListViewColumn>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->width(), 100);
}

TEST_F(ListViewColumnTests, constructor_with_params)
{
    auto target = std::make_shared<ListViewColumn>("text", 123);

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->width(), 123);
}

TEST_F(ListViewColumnTests, constructor_with_copy)
{
    auto target = std::make_shared<ListViewColumn>("text", 123);
    auto copy = *target;

    EXPECT_EQ(*target, copy);
}

TEST_F(ListViewColumnTests, op_assignment)
{
    auto target1 = std::make_shared<ListViewColumn>("text", 123);
    auto target2 = std::make_shared<ListViewColumn>();
    auto target3 = std::make_shared<ListViewColumn>("xx", 321);
    auto target4 = std::make_shared<ListViewColumn>();

    *target2 = *target1;

    *target4 = *target3;

    EXPECT_EQ(*target1, *target2);
    EXPECT_EQ(*target3, *target4);
    EXPECT_NE(*target1, *target3);
}

TEST_F(ListViewColumnTests, text)
{
    auto target = std::make_shared<ListViewColumn>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text() = testText;

    EXPECT_EQ(target->text(), testText);
}

TEST_F(ListViewColumnTests, width)
{
    auto target = std::make_shared<ListViewColumn>();

    EXPECT_EQ(target->width(), 100);

    int testWidth = 123;
    target->width() = testWidth;

    EXPECT_EQ(target->width(), testWidth);
}