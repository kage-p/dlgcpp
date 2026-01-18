#include "dlgcpp/controls/toolbar.h"
#include "toolbar_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ToolBarTests, constructor_default)
{
    auto target = std::make_shared<ToolBar>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ToolBarTests, constructor_with_params)
{
    auto target = std::make_shared<ToolBar>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ToolBarTests, buttonSize)
{
    auto target = std::make_shared<ToolBar>();

    EXPECT_EQ(target->buttonSize(), Size(24, 24));

    target->buttonSize(Size(48, 48));
    EXPECT_EQ(target->buttonSize(), Size(48, 48));

    // empty is invalid
    target->buttonSize(Size(0, 0));
    EXPECT_EQ(target->buttonSize(), Size(48, 48));

    target->buttonSize(Size(1, 1));
    EXPECT_EQ(target->buttonSize(), Size(1, 1));
}

TEST_F(ToolBarTests, imageSize)
{
    auto target = std::make_shared<ToolBar>();

    EXPECT_EQ(target->imageSize(), Size(20, 20));

    target->imageSize(Size(48, 48));
    EXPECT_EQ(target->imageSize(), Size(48, 48));

    // empty is invalid
    target->imageSize(Size(0, 0));
    EXPECT_EQ(target->imageSize(), Size(48, 48));

    target->imageSize(Size(1, 1));
    EXPECT_EQ(target->imageSize(), Size(1, 1));
}

TEST_F(ToolBarTests, items)
{
    auto target = std::make_shared<ToolBar>();

    std::vector<ISharedToolBarItem> items;

    EXPECT_EQ(target->items(), items);

    items =
    {
        std::make_shared<ToolBarItem>(ImageSource{}),
        std::make_shared<ToolBarItem>(ImageSource{}),
        std::make_shared<ToolBarItem>(ImageSource{}),
    };
    target->items(items);
    EXPECT_EQ(target->items(), items);

    items = {};
    target->items(items);
    EXPECT_EQ(target->items(), items);
}

TEST_F(ToolBarTests, events)
{
    auto target = std::make_shared<ToolBar>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
