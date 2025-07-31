#include "controls/listview_p.h"
#include "dlgcpp/controls/listview.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListView::ListView(const Position& p)
    : ListView(std::make_shared<ListViewImpl>(this, p))
{
    Control::border(BorderStyle::Sunken);
}

ListView::ListView(std::shared_ptr<ListViewImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

ListView::~ListView()
{
}

int ListView::selectedIndex() const
{
    if (_impl->multiselect())
        return -1;
    return _impl->selectedIndex();
}

void ListView::selectedIndex(int value)
{
    if (_impl->multiselect())
        return;
    if (_impl->selectedIndex() == value)
        return;

    _impl->selectedIndex(value);
}

const std::vector<int>& ListView::selectedIndexes() const
{
    static const std::vector<int> empty;
    if (!_impl->multiselect())
        return empty;
    return _impl->selectedIndexes();
}

void ListView::selectedIndexes(const std::vector<int>& indexes)
{
    if (!_impl->multiselect())
        return;
    if (_impl->selectedIndexes() == indexes)
        return;

    _impl->selectedIndexes(indexes);
}

bool ListView::checkboxes() const
{
    return _impl->checkboxes();
}

void ListView::checkboxes(bool value)
{
    if (_impl->checkboxes() == value)
        return;
    _impl->checkboxes(value);
}

bool ListView::columnHeader() const
{
    return _impl->columnHeader();
}

void ListView::columnHeader(bool value)
{
    if (_impl->columnHeader() == value)
        return;
    _impl->columnHeader(value);
}

bool ListView::sortColumns() const
{
    return _impl->sortColumns();
}

void ListView::sortColumns(bool value)
{
    if (_impl->sortColumns() == value)
        return;
    _impl->sortColumns(value);
}

bool ListView::editing() const
{
    return _impl->editing();
}

bool ListView::beginEditing(size_t row, int role)
{
    return _impl->beginEditing(row, role);
}

void ListView::confirmEditing()
{
    _impl->confirmEditing();
}

void ListView::cancelEditing()
{
    _impl->cancelEditing();
}

bool ListView::gridlines() const
{
    return _impl->gridlines();
}

void ListView::gridlines(bool value)
{
    if (_impl->gridlines() == value)
        return;
    _impl->gridlines(value);
}

bool ListView::multiselect() const
{
    return _impl->multiselect();
}

void ListView::multiselect(bool value)
{
    if (_impl->multiselect() == value)
        return;
    _impl->multiselect(value);
}

ListViewDisplay ListView::display() const
{
    return _impl->display();
}

void ListView::display(ListViewDisplay value)
{
    if (_impl->display() == value)
        return;

    _impl->display(value);
}

int ListView::roleData(int) const
{
    // placeholder
    return 0;
}

ListViewColumn ListView::columnData(int) const
{
    // placeholder
    return ListViewColumn();
}

size_t ListView::columnCount() const
{
    // placeholder
    return 0;
}

std::string ListView::rowData(size_t, int) const
{
    // placeholder
    return std::string();
}

size_t ListView::rowCount() const
{
    // placeholder
    return 0;
}

bool ListView::beginEdit(
    size_t row,
    int role)
{
    // placeholder
    return false;
}

bool ListView::endEdit(
    size_t row,
    int role,
    const std::string& text)
{
    // placeholder
    return false;
}

bool ListView::checked(size_t row) const
{
    // placeholder
    return false;
}

void ListView::checked(size_t row, bool checked)
{
    // placeholder
}

IEvent<ISharedControl>& ListView::ColumnsChangedEvent()
{
    return _impl->ColumnsChangedEvent();
}

IEvent<ISharedControl>& ListView::RowsChangedEvent()
{
    return _impl->RowsChangedEvent();
}

IEvent<ISharedControl>& ListView::SelChangedEvent()
{
    return _impl->SelChangedEvent();
}

IEvent<ISharedControl, int>& ListView::ColumnClickEvent()
{
    return _impl->ColumnClickEvent();
}

IEvent<ISharedControl, size_t, int>& ListView::ItemClickEvent()
{
    return _impl->ItemClickEvent();
}

IEvent<ISharedControl, size_t, int>& ListView::ItemDoubleClickEvent()
{
    return _impl->ItemDoubleClickEvent();
}
