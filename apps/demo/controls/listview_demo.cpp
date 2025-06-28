#include "listview_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_listview_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("ListView control demo");
    dlg->resize({200,125});
    dlg->center();

    auto listview = std::make_shared<ListViewDemoImpl>();
    listview->p(Position{10, 10, 180, 105});
    listview->colors(Color::Black, Color::Orange);
    dlg->add(listview);

    std::vector<ListViewDemoImpl::ListViewDemoItem> items;

    listview->setItems(items);

    dlg->exec();
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

void ListViewDemoImpl::setItems(const std::vector<ListViewDemoItem>& items)
{
    _items = items;
    ItemsChangedEvent().invoke(shared_from_this());
}
