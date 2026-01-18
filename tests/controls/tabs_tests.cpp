#include "dlgcpp/controls/tabs.h"
#include "tabs_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(TabsTests, constructor_default)
{
    auto target = std::make_shared<Tabs>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(TabsTests, constructor_with_params)
{
    auto target = std::make_shared<Tabs>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(TabsTests, area)
{
    auto target = std::make_shared<Tabs>();

    EXPECT_EQ(target->area(), Position());
}

TEST_F(TabsTests, selectedIndex)
{
    auto target = std::make_shared<Tabs>();

    EXPECT_EQ(target->selectedIndex(), -1);

    // set index with no items
    target->selectedIndex(0);
    EXPECT_EQ(target->selectedIndex(), -1);

    // set index with items
    std::vector<ISharedTabItem> items =
    {
        std::make_shared<TabItem>("Item1"),
        std::make_shared<TabItem>("Item2"),
        std::make_shared<TabItem>("Item3"),
    };
    target->items(items);
    target->selectedIndex(2);
    EXPECT_EQ(target->selectedIndex(), 2);

    // clear index
    target->selectedIndex(-1);
    EXPECT_EQ(target->selectedIndex(), -1);

    // index reset after clearing items
    target->selectedIndex(1);
    target->items({});
    EXPECT_EQ(target->selectedIndex(), -1);
}

TEST_F(TabsTests, items)
{
    auto target = std::make_shared<Tabs>();

    std::vector<ISharedTabItem> items;

    EXPECT_EQ(target->items(), items);

    items = {
        std::make_shared<TabItem>("Item1"),
        std::make_shared<TabItem>("Item2"),
        std::make_shared<TabItem>("Item3"),
    };
    target->items(items);
    EXPECT_EQ(target->items(), items);

    items.clear();
    target->items(items);
    EXPECT_EQ(target->items(), items);
}

TEST_F(TabsTests, events)
{
    auto target = std::make_shared<Tabs>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
