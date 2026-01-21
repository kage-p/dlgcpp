#pragma once

#include "dlgcpp/dlgcpp.h"
#include <map>

void controls_listview_demo(dlgcpp::ISharedDialog parent);

class DemoListView : public dlgcpp::ListView
{
public:
    DemoListView();

    enum RoleId
    {
        Column1 = 999,
        Column2 = 998,
        Column3 = 997
    };

    typedef struct DemoListViewItem
    {
        std::string col1;
        std::string col2;
        std::string col3;
        bool checked = false;
    } DemoListViewItem;

    void setItems(const std::vector<DemoListViewItem>& items);

    // ListView impl.
    int roleData(int column) const override;
    dlgcpp::ListViewColumn columnData(int role) const override;
    size_t columnCount() const override;
    std::string rowData(size_t row, int role) const override;
    size_t rowCount() const override;
    bool beginEdit(size_t row, int role) override;
    bool endEdit(size_t row, int role, const std::string& text) override;
    bool checked(size_t row) const override;
    void checked(size_t row, bool checked) override;

private:
    std::map<int, int> _roleMap;
    std::map<int, dlgcpp::ListViewColumn> _columnMap;
    std::vector<DemoListViewItem> _items;
};
