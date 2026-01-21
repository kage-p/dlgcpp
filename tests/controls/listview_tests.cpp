#include "dlgcpp/controls/listview.h"
#include "listview_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

struct ListViewTestItem
{
    std::string first;
    std::string second;
    bool checked = false;
};;

class ListViewInTest : public ListView
{
public:
    enum Roles : int
    {
        First = 1,
        Second = 2
    };

    explicit ListViewInTest(
        const Position& p = Position()) : ListView(p)
    {
    }
    ~ListViewInTest() override = default;

    void setItems(const std::vector<ListViewTestItem>& items)
    {
        _items = items;
        RowsChangedEvent().invoke();
    }

    // override default placeholders
    int roleData(int column) const override
    {
        if (column == 0)
            return First;
        else if (column == 1)
            return Second;
        else
            return 0;
    }

    ListViewColumn columnData(int role) const override
    {
        return ListViewColumn();
    }

    size_t columnCount() const override
    {
        return 0;
    }

    std::string rowData(size_t row, int role) const override
    {
        if (row >= _items.size())
            return {};

        if (role == First)
            return _items[row].first;
        else if (role == Second)
            return _items[row].second;
        else
            return {};
    }

    size_t rowCount() const override
    {
        return _items.size();
    }

    bool beginEdit(size_t row, int role) override
    {
        if (row >= _items.size())
            return false;
        return true;
    }

    bool endEdit(size_t row, int role, const std::string& text) override
    {
        return true;
    }

    bool checked(size_t row) const override
    {
        if (row >= _items.size())
            return false;
        return _items.at(row).checked;
    }

    void checked(size_t row, bool checked) override
    {
        if (row >= _items.size())
            return;

        _items.at(row).checked = checked;
    }

private:
    std::vector<ListViewTestItem> _items;
};

TEST_F(ListViewTests, constructor_default)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ListViewTests, constructor_with_params)
{
    auto target = std::make_shared<ListView>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ListViewTests, display)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->display(), ListViewDisplay::Details);

    target->display(ListViewDisplay::LargeIcon);
    EXPECT_EQ(target->display(), ListViewDisplay::LargeIcon);

    target->display(ListViewDisplay::SmallIcon);
    EXPECT_EQ(target->display(), ListViewDisplay::SmallIcon);

    target->display(ListViewDisplay::List);
    EXPECT_EQ(target->display(), ListViewDisplay::List);

    target->display(ListViewDisplay::Details);
    EXPECT_EQ(target->display(), ListViewDisplay::Details);
}

TEST_F(ListViewTests, checkboxes)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->checkboxes(), false);

    target->checkboxes(true);
    EXPECT_EQ(target->checkboxes(), true);

    target->checkboxes(false);
    EXPECT_EQ(target->checkboxes(), false);
}

TEST_F(ListViewTests, editing)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->editing(), false);
}

TEST_F(ListViewTests, beginEditing)
{
    auto target = std::make_shared<ListView>();

    // no-op
    EXPECT_FALSE(target->beginEditing(0, 0));

    EXPECT_EQ(target->editing(), false);
}

TEST_F(ListViewTests, confirmEditing)
{
    auto target = std::make_shared<ListView>();

    // no-op
    target->confirmEditing();

    EXPECT_EQ(target->editing(), false);
}

TEST_F(ListViewTests, cancelEditing)
{
    auto target = std::make_shared<ListView>();

    // no-op
    target->cancelEditing();

    EXPECT_EQ(target->editing(), false);
}

// Disabled because it requires an active handle to function properly
TEST_F(ListViewTests, DISABLED_beginEditing_with_items)
{
    auto target = std::make_shared<ListViewInTest>();

    // test with items
    target->setItems(
        {
            {"Item 1", "Subitem 1", false},
            {"Item 2", "Subitem 2", true},
            {"Item 3", "Subitem 3", false}
        });

    EXPECT_TRUE(target->beginEditing(0, 0));

    EXPECT_EQ(target->editing(), true);
}

// Disabled because it requires an active handle to function properly
TEST_F(ListViewTests, DISABLED_confirmEditing_with_items)
{
    auto target = std::make_shared<ListViewInTest>();

    // test with items
    target->setItems(
        {
            {"Item 1", "Subitem 1", false},
            {"Item 2", "Subitem 2", true},
            {"Item 3", "Subitem 3", false}
        });

    EXPECT_TRUE(target->beginEditing(0, 0));

    target->confirmEditing();

    EXPECT_EQ(target->editing(), false);
}

// Disabled because it requires an active handle to function properly
TEST_F(ListViewTests, DISABLED_cancelEditing_with_items)
{
    auto target = std::make_shared<ListViewInTest>();

    // test with items
    target->setItems(
        {
            {"Item 1", "Subitem 1", false},
            {"Item 2", "Subitem 2", true},
            {"Item 3", "Subitem 3", false}
        });

    EXPECT_TRUE(target->beginEditing(0, 0));

    target->cancelEditing();

    EXPECT_EQ(target->editing(), false);
}

TEST_F(ListViewTests, gridlines)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->gridlines(), false);

    target->gridlines(true);
    EXPECT_EQ(target->gridlines(), true);

    target->gridlines(false);
    EXPECT_EQ(target->gridlines(), false);
}

TEST_F(ListViewTests, multiselect)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->multiselect(), false);

    target->multiselect(true);
    EXPECT_EQ(target->multiselect(), true);

    target->multiselect(false);
    EXPECT_EQ(target->multiselect(), false);
}

TEST_F(ListViewTests, selectedIndex)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->selectedIndex(), -1);

    target->selectedIndex(2);
    EXPECT_EQ(target->selectedIndex(), -1);
}

TEST_F(ListViewTests, selectedIndex_with_items)
{
    auto target = std::make_shared<ListViewInTest>();

    // test with items
    target->setItems(
        {
            {"Item 1", "Subitem 1", false},
            {"Item 2", "Subitem 2", true},
            {"Item 3", "Subitem 3", false}
        });

    target->selectedIndex(0);
    EXPECT_EQ(target->selectedIndex(), 0);
    target->selectedIndex(1);
    EXPECT_EQ(target->selectedIndex(), 1);
    target->selectedIndex(2);
    EXPECT_EQ(target->selectedIndex(), 2);
    target->selectedIndex(3);
    EXPECT_EQ(target->selectedIndex(), 2);

    target->selectedIndex(-1);
    EXPECT_EQ(target->selectedIndex(), -1);
}

TEST_F(ListViewTests, selectedIndexes)
{
    auto target = std::make_shared<ListView>();

    target->multiselect(true);

    std::vector<int> indexes;

    indexes = { 2 };
    target->selectedIndexes() = indexes;

    indexes.clear();
    EXPECT_EQ(target->selectedIndexes(), indexes);
}

// Disabled because it requires an active handle to function properly
TEST_F(ListViewTests, DISABLED_selectedIndexes_with_items)
{
    auto target = std::make_shared<ListViewInTest>();

    target->multiselect(true);

    // test with items
    target->setItems(
        {
            {"Item 1", "Subitem 1", false},
            {"Item 2", "Subitem 2", true},
            {"Item 3", "Subitem 3", false}
        });

    std::vector<int> indexes;

    indexes = { 2 };
    target->selectedIndexes() = indexes;
    EXPECT_EQ(target->selectedIndexes(), indexes);

    indexes = { 0, 1, 2 };
    target->selectedIndexes() = indexes;
    EXPECT_EQ(target->selectedIndexes(), indexes);

    indexes.clear();
    target->selectedIndexes() = indexes;
    EXPECT_EQ(target->selectedIndexes(), indexes);
}

TEST_F(ListViewTests, columnHeader)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->columnHeader(), true);

    target->columnHeader(false);
    EXPECT_EQ(target->columnHeader(), false);

    target->columnHeader(true);
    EXPECT_EQ(target->columnHeader(), true);
}

TEST_F(ListViewTests, sortColumns)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->sortColumns(), true);

    target->sortColumns(false);
    EXPECT_EQ(target->sortColumns(), false);

    target->sortColumns(true);
    EXPECT_EQ(target->sortColumns(), true);
}

// test placeholder defaults

TEST_F(ListViewTests, roleData)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->roleData(0), 0);
}

TEST_F(ListViewTests, columnData)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->columnData(0), ListViewColumn());
}

TEST_F(ListViewTests, columnCount)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->columnCount(), 0);
}

TEST_F(ListViewTests, rowCount)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->rowCount(), 0);
}

TEST_F(ListViewTests, beginEdit)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->beginEdit(0, 0), false);
}

TEST_F(ListViewTests, endEdit)
{
    auto target = std::make_shared<ListView>();

    EXPECT_EQ(target->endEdit(0, 0, "text"), false);
}

TEST_F(ListViewTests, checked)
{
    auto target = std::make_shared<ListView>();

    target->checked(0, true);
    EXPECT_EQ(target->checked(0), false);
}

TEST_F(ListViewTests, checked_with_items)
{
    auto target = std::make_shared<ListViewInTest>();


    target->setItems({
        {"Item 1", "Subitem 1", false},
        {"Item 2", "Subitem 2", true},
        {"Item 3", "Subitem 3", false}
        });

    target->checked(2, true);
    EXPECT_EQ(target->checked(2), true);

    target->checked(1, true);
    EXPECT_EQ(target->checked(1), true);

    target->checked(0, true);
    EXPECT_EQ(target->checked(0), true);
}

TEST_F(ListViewTests, events)
{
    auto target = std::make_shared<ListView>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();

    auto& selChangedEvent = target->SelChangedEvent();
    auto& columnsChangedEvent = target->ColumnsChangedEvent();
    auto& rowsChangedEvent = target->RowsChangedEvent();
    auto& columnClickEvent = target->ColumnClickEvent();
    auto& itemClickEvent = target->ItemClickEvent();
    auto& itemDoubleClickEvent = target->ItemDoubleClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
