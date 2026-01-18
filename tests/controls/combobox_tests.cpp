#include "combobox_tests.h"
#include "dlgcpp/controls/combobox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ComboBoxTests, constructor_default)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ComboBoxTests, constructor_with_params)
{
    auto target = std::make_shared<ComboBox>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ComboBoxTests, selectedIndex)
{
    auto target = std::make_shared<ComboBox>();

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

TEST_F(ComboBoxTests, dropDown)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->dropDown(), true);

    target->dropDown(false);
    EXPECT_EQ(target->dropDown(), false);

    target->dropDown(true);
    EXPECT_EQ(target->dropDown(), true);
}

TEST_F(ComboBoxTests, editable)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->editable(), false);

    target->editable(true);
    EXPECT_EQ(target->editable(), true);

    target->editable(false);
    EXPECT_EQ(target->editable(), false);
}

TEST_F(ComboBoxTests, sorted)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->sorted(), true);

    target->sorted(false);
    EXPECT_EQ(target->sorted(), false);

    target->sorted(true);
    EXPECT_EQ(target->sorted(), true);
}

TEST_F(ComboBoxTests, items)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->items(), std::vector<std::string>());

    std::vector<std::string> items = { "Item1", "Item2", "Item3" };
    target->items(items);
    EXPECT_EQ(target->items(), items);

    target->items({});
    EXPECT_EQ(target->items(), std::vector<std::string>());
}

TEST_F(ComboBoxTests, text)
{
    auto target = std::make_shared<ComboBox>();

    EXPECT_EQ(target->text(), std::string());

    // must be editable
    target->text("abc");
    EXPECT_EQ(target->text(), std::string());

    // editable
    target->editable(true);
    target->text("text");
    EXPECT_EQ(target->text(), std::string("text"));

    // not editable
    target->editable(false);
    EXPECT_EQ(target->text(), std::string());
}

TEST_F(ComboBoxTests, events)
{
    auto target = std::make_shared<ComboBox>();

    auto& selChangedEvent = target->SelChangedEvent();
    auto& selCancelEvent = target->SelCancelEvent();
    auto& listOpenEvent = target->ListOpenEvent();
    auto& listCloseEvent = target->ListCloseEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
