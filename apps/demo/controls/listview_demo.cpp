#include "listview_demo.h"
#include <map>

using namespace dlgcpp;
using namespace dlgcpp::controls;

std::shared_ptr<Menu> createMenu(
    std::shared_ptr<ListViewDemoImpl> listview);

void controls_listview_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("ListView control demo");
    dlg->resize({ 450,350 });
    dlg->center();

    auto label = std::make_shared<Label>("Right click on list for options", Position{ 10, 10, 430, 15 });
    dlg->add(label);

    auto listView = std::make_shared<ListViewDemoImpl>();
    listView->p(Position{ 10, 25, 430, 315 });
    listView->colors(Color::Teal, Color::White);
    dlg->add(listView);

    auto menu = createMenu(listView);

    // explicitly enabled these events so the handlers get fired
    listView->wantKeyboardEvents(true);
    listView->wantMouseEvents(true);

    listView->MouseDownEvent() +=
        [menu](ISharedControl control, MouseEvent event)
        {
            // show the menu on right click
            if (event.button == MouseButton::Right)
                menu->popup(control, event.point);
        };

    listView->KeyDownEvent() +=
        [listView](ISharedControl, KeyboardEvent event)
        {
            if (event.key == Key::Enter)
            {
                // user pressed enter key in the ListView
                if (listView->selectedIndex() > -1)
                {
                    listView->beginEditing(
                        listView->selectedIndex(),
                        ListViewDemoImpl::RoleId::Column1);
                }
            }
        };

    auto items = std::vector<ListViewDemoImpl::ListViewDemoItem>
    {
        { "Item 1", "Value 1.1", "Value 1.2"},
        { "Item 2", "Value 2.1", "Value 2.2"},
        { "Item 3", "Value 3.1", "Value 3.2"},
        { "Item 4", "Value 4.1", "Value 4.2"},
        { "Item 5", "Value 5.1", "Value 5.2"},
        { "Item 6", "Value 6.1", "Value 6.2"},
        { "Item 7", "Value 7.1", "Value 7.2"},
        { "Item 8", "Value 8.1", "Value 8.2"},
        { "Item 9", "Value 9.1", "Value 9.2"},
        { "Item 10", "Value 10.1", "Value 10.2"}
    };

    listView->setItems(items);

    dlg->exec();
}

std::shared_ptr<Menu> createMenu(
    std::shared_ptr<ListViewDemoImpl> listview)
{
    auto menu = std::make_shared<Menu>();
    std::shared_ptr<IMenuItem> item;

    item = std::make_shared<MenuItem>("Switch view");
    menu->add(item);
    item->checked(listview->multiselect());
    item->ClickEvent() += [listview](ISharedMenuItem)
        {
            switch (listview->display())
            {
            case ListViewDisplay::LargeIcon:
                listview->display(ListViewDisplay::SmallIcon);
                break;
            case ListViewDisplay::SmallIcon:
                listview->display(ListViewDisplay::List);
                break;
            case ListViewDisplay::List:
                listview->display(ListViewDisplay::Details);
                break;
            case ListViewDisplay::Details:
                listview->display(ListViewDisplay::LargeIcon);
                break;
            }
        };

    item = std::make_shared<MenuItem>();
    menu->add(item);

    item = std::make_shared<MenuItem>("Show check boxes");
    menu->add(item);
    item->checked(listview->checkboxes());
    item->ClickEvent() += [listview](ISharedMenuItem m)
        {
            listview->checkboxes(!listview->checkboxes());
            m->checked(listview->checkboxes());
        };

    item = std::make_shared<MenuItem>("Show grid lines");
    menu->add(item);
    item->checked(listview->gridlines());
    item->ClickEvent() += [listview](ISharedMenuItem m)
        {
            listview->gridlines(!listview->gridlines());
            m->checked(listview->gridlines());
        };

    item = std::make_shared<MenuItem>("Multi-select list");
    menu->add(item);
    item->checked(listview->multiselect());
    item->ClickEvent() += [listview](ISharedMenuItem m)
        {
            listview->multiselect(!listview->multiselect());
            m->checked(listview->multiselect());
        };

    return menu;
}

ListViewDemoImpl::ListViewDemoImpl() :
    ListView()
{
    _roleMap =
    {
        { 0, RoleId::Column1 },
        { 1, RoleId::Column2 },
        { 2, RoleId::Column3 }
    };
    _columnMap =
    {
        { RoleId::Column1, ListViewColumn("Column 1", 45) },
        { RoleId::Column2, ListViewColumn("Column 2", 80) },
        { RoleId::Column3, ListViewColumn("Column 3", 45) }
    };
}

int ListViewDemoImpl::roleData(int column) const
{
    auto it = _roleMap.find(column);
    if (it != _roleMap.end())
        return it->second;
    // Invalid role
    return -1;
}

ListViewColumn ListViewDemoImpl::columnData(int role) const
{
    auto it = _columnMap.find(role);
    if (it != _columnMap.end())
        return it->second;
    // return an empty column if role not found
    return ListViewColumn();
}

size_t ListViewDemoImpl::columnCount() const
{
    return _columnMap.size();
}

std::string ListViewDemoImpl::rowData(size_t row, int role) const
{
    if (row >= _items.size())
        return std::string();

    const auto& item = _items.at(row);
    switch ((RoleId)role)
    {
    case RoleId::Column1:
        return item.col1;
    case RoleId::Column2:
        return item.col2;
    case RoleId::Column3:
        return item.col3;
    }

    return std::string();
}

size_t ListViewDemoImpl::rowCount() const
{
    return _items.size();
}

bool ListViewDemoImpl::beginEdit(
    size_t row,
    int role,
    std::string& text)
{
    if (row >= _items.size())
        return false;

    auto& item = _items.at(row);

    // return text for edit operation
    switch ((RoleId)role)
    {
    case RoleId::Column1:
        text = item.col1;
        break;
    case RoleId::Column2:
        text = item.col2;
        break;
    case RoleId::Column3:
        text = item.col3;
        break;
    default:
        // not valid
        return false;
    }

    // to prevent editing, return false
    return true;
}

void ListViewDemoImpl::endEdit(
    size_t row,
    int role,
    const std::string& text,
    bool confirmed)
{
    if (row >= _items.size())
        return;

    auto& item = _items.at(row);
    switch ((RoleId)role)
    {
    case RoleId::Column1:
        item.col1 = text;
        break;
    case RoleId::Column2:
        item.col2 = text;
        break;
    case RoleId::Column3:
        item.col3 = text;
        break;
    }
}

bool ListViewDemoImpl::checked(size_t row) const
{
    if (row >= _items.size())
        return false;

    const auto& item = _items.at(row);

    return item.checked;
}

void ListViewDemoImpl::checked(size_t row, bool checked)
{
    if (row >= _items.size())
        return;

    auto& item = _items.at(row);
    item.checked = checked;
}

void ListViewDemoImpl::setItems(const std::vector<ListViewDemoItem>& items)
{
    _items = items;
    RowsChangedEvent().invoke();
}
