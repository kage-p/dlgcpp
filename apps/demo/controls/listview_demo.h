#pragma once

#include "dlgcpp/dlgcpp.h"

void controls_listview_demo(std::shared_ptr<dlgcpp::IDialog> parent);

class ListViewDemoImpl : public dlgcpp::controls::ListView
{
public:
    typedef struct ListViewDemoItem
    {
        std::string col1;
        std::string col2;
        std::string col3;
    } ListViewDemoItem;

    // ListView impl.
    std::vector<dlgcpp::controls::ISharedListViewColumn> columns() const override;
    std::string data(size_t row, int role) const override;
    size_t count() const override;

    void setItems(const std::vector<ListViewDemoItem>& items);

private:
    enum RoleId
    {
        Column1 = 0,
        Column2,
        Column3
    };

    std::vector<ListViewDemoItem> _items;

};
