#include "listview_demo.h"

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

    auto listview = std::make_shared<ListViewDemoImpl>();
    listview->p(Position{ 10, 25, 430, 315 });
    listview->colors(Color::Black, Color::Orange);
    dlg->add(listview);

    auto menu = createMenu(listview);

    listview->wantMouseActions(true);

    listview->MouseDownEvent() +=
        [menu](ISharedControl control, MouseEvent event)
        {
            // show the menu on right click
            if (event.button == MouseButton::Right)
                menu->popup(control, event.point);
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

    listview->setItems(items);

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
    item->ClickEvent() += [listview](ISharedMenuItem m)
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

    item = std::make_shared<MenuItem>("Multi-select list");
    menu->add(item);
    item->checked(listview->multiselect());
    item->ClickEvent() += [listview](ISharedMenuItem m)
        {
            listview->multiselect(!listview->multiselect());
            m->checked(listview->multiselect());
        };

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


    return menu;
}

std::vector<ISharedListViewColumn> ListViewDemoImpl::columns() const
{
    std::vector<ISharedListViewColumn> r;
    r.push_back(std::make_shared<ListViewColumn>("Column 1", RoleId::Column1, 45));
    r.push_back(std::make_shared<ListViewColumn>("Column 2", RoleId::Column2, 80));
    r.push_back(std::make_shared<ListViewColumn>("Column 3", RoleId::Column3, 45));
    return r;
}

std::string ListViewDemoImpl::data(size_t row, int role) const
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

size_t ListViewDemoImpl::count() const
{
    return _items.size();
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
    ItemsChangedEvent().invoke(shared_from_this());
}
