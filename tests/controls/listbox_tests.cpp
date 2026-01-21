#include "dlgcpp/controls/listbox.h"
#include "listbox_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ListBoxTests, constructor_default)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ListBoxTests, constructor_with_params)
{
    auto target = std::make_shared<ListBox>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ListBoxTests, selectedIndex)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->selectedIndex(), -1);

    // set index with no items
    target->selectedIndex(0);
    EXPECT_EQ(target->selectedIndex(), -1);

    // set index with items
    std::vector<std::string> items = { "Item1", "Item2", "Item3" };
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

TEST_F(ListBoxTests, selectedIndexes)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->selectedIndexes(), std::vector<int>());

    target->multiselect(true);

    // set indexes with no items
    target->selectedIndexes({ 1 });
    EXPECT_EQ(target->selectedIndexes(), std::vector<int>());

    // set indexes with items
    std::vector<std::string> items = { "Item1", "Item2", "Item3" };
    target->items(items);
    auto selItems = std::vector<int>{ 0,1,2 };
    target->selectedIndexes(selItems);
    EXPECT_EQ(target->selectedIndexes(), selItems);

    // clear indexes
    target->selectedIndex(-1);
    EXPECT_EQ(target->selectedIndex(), -1);

    // index reset after clearing items
    target->selectedIndexes(selItems);
    target->items({});
    EXPECT_EQ(target->selectedIndexes(), std::vector<int>());
}

TEST_F(ListBoxTests, highlight)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->highlight(), true);

    target->highlight(false);
    EXPECT_EQ(target->highlight(), false);

    target->highlight(true);
    EXPECT_EQ(target->highlight(), true);
}

TEST_F(ListBoxTests, multiselect)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->multiselect(), false);

    target->multiselect(true);
    EXPECT_EQ(target->multiselect(), true);

    target->multiselect(false);
    EXPECT_EQ(target->multiselect(), false);
}

TEST_F(ListBoxTests, sorted)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->sorted(), true);

    target->sorted(false);
    EXPECT_EQ(target->sorted(), false);

    target->sorted(true);
    EXPECT_EQ(target->sorted(), true);
}

TEST_F(ListBoxTests, items)
{
    auto target = std::make_shared<ListBox>();

    EXPECT_EQ(target->items(), std::vector<std::string>());

    std::vector<std::string> items = { "Item1", "Item2", "Item3" };
    target->items(items);
    EXPECT_EQ(target->items(), items);

    target->items({});
    EXPECT_EQ(target->items(), std::vector<std::string>());
}

TEST_F(ListBoxTests, events)
{
    auto target = std::make_shared<ListBox>();

    auto& selChangedEvent = target->SelChangedEvent();
    auto& selCancelEvent = target->SelCancelEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
