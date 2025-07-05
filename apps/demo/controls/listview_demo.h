#pragma once

#include "dlgcpp/dlgcpp.h"
#include <map>

void controls_listview_demo(std::shared_ptr<dlgcpp::IDialog> parent);

class ListViewDemoImpl : public dlgcpp::controls::ListView
{
public:
    ListViewDemoImpl();

    enum RoleId
    {
        Column1 = 999,
        Column2 = 998,
        Column3 = 997
    };

    typedef struct ListViewDemoItem
    {
        std::string col1;
        std::string col2;
        std::string col3;
        bool checked = false;
    } ListViewDemoItem;

    // ListView impl.
    int roleData(int column) const override;
    dlgcpp::controls::ListViewColumn columnData(int role) const override;
    size_t columnCount() const override;
    std::string rowData(size_t row, int role) const override;
    size_t rowCount() const override;
    bool beginEdit(size_t row, int role, std::string& text) override;
    void endEdit(size_t row, int role, const std::string& text, bool confirmed) override;
    bool checked(size_t row) const override;
    void checked(size_t row, bool checked) override;

    void setItems(const std::vector<ListViewDemoItem>& items);

private:


    std::map<int, int> _roleMap;
    std::map<int, dlgcpp::controls::ListViewColumn> _columnMap;
    std::vector<ListViewDemoItem> _items;
};
