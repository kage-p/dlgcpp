#include "controls/listview_impl.h"
#include "dlgcpp/controls/listview.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListView::ListView(const Position& p)
    : ListView(std::make_shared<ListViewImpl>(), p)
{
    border() = BorderStyle::Sunken;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    // properties
    _display.reset(ListViewDisplay::Details, nullptr, ownerFn, "display");
    _columnHeader.reset(true, nullptr, ownerFn, "columnHeader");
    _sortColumns.reset(true, nullptr, ownerFn, "sortColumns");
    _checkboxes.reset(false, nullptr, ownerFn, "checkboxes");
    _gridlines.reset(false, nullptr, ownerFn, "gridlines");
    _editing.reset(false, [](auto) { return false; }, ownerFn, "editing");
    _multiselect.reset(false, nullptr, ownerFn, "multiselect");

    _selectedIndex.reset(
        -1,
        [&](int index)
        {
            if (index == -1)
                return true;

            if (_multiselect)
                return false;

            return (index > -1 && index < rowCount());
        },
        ownerFn, "selectedIndex");

    _selectedIndexes.reset(
        std::vector<int>(),
        [&](const std::vector<int>& indexes)
        {
            if (indexes.empty())
                return true;

            if (!_multiselect)
                return false;

            std::vector<int> cleanIndexes;
            for (int index : indexes)
            {
                // disallow invalid
                if (index < 0 ||
                    index >= (int)rowCount())
                {
                    return false;
                }

                // disallow duplicates
                if (std::find(cleanIndexes.begin(),
                    cleanIndexes.end(),
                    index) != cleanIndexes.end())
                {
                    return false;
                }

                cleanIndexes.push_back(index);
            }

            return true;
        },
        ownerFn, "selectedIndexes");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _dblRightClickEvent.reset(ownerFn, "DoubleRightClickEvent");
    _rightClickEvent.reset(ownerFn, "RightClickEvent");
    _columnsChangedEvent.reset(ownerFn, "ColumnsChangedEvent");
    _rowsChangedEvent.reset(ownerFn, "RowsChangedEvent");
    _selChangedEvent.reset(ownerFn, "SelChangedEvent");
    _columnClickEvent.reset(ownerFn, "ColumnClickEvent");
    _itemClickEvent.reset(ownerFn, "ItemClickEvent");
    _itemDblClickEvent.reset(ownerFn, "ItemDoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

ListView::ListView(
    std::shared_ptr<ListViewImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

ListView::~ListView()
{
    _impl.reset();
}

IProperty<ListViewDisplay, ISharedControl>& ListView::display()
{
    return _display;
}

void ListView::display(ListViewDisplay value)
{
    _display = value;
}

IProperty<bool, ISharedControl>& ListView::columnHeader()
{
    return _columnHeader;
}

void ListView::columnHeader(bool value)
{
    _columnHeader = value;
}

IProperty<bool, ISharedControl>& ListView::sortColumns()
{
    return _sortColumns;
}

void ListView::sortColumns(bool value)
{
    _sortColumns = value;
}

IProperty<bool, ISharedControl>& ListView::checkboxes()
{
    return _checkboxes;
}

void ListView::checkboxes(bool value)
{
    _checkboxes = value;
}

IProperty<bool, ISharedControl>& ListView::gridlines()
{
    return _gridlines;
}

void ListView::gridlines(bool value)
{
    _gridlines = value;
}

IProperty<bool, ISharedControl>& ListView::editing()
{
    return _editing;
}

IProperty<bool, ISharedControl>& ListView::multiselect()
{
    return _multiselect;
}

void ListView::multiselect(bool value)
{
    _multiselect = value;
}

IProperty<int, ISharedControl>& ListView::selectedIndex()
{
    return _selectedIndex;
}

void ListView::selectedIndex(int value)
{
    _selectedIndex = value;
}

IProperty<std::vector<int>, ISharedControl>& ListView::selectedIndexes()
{
    return _selectedIndexes;
}

void ListView::selectedIndexes(const std::vector<int>& indexes)
{
    _selectedIndexes = indexes;
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

IEvent<ISharedControl>& ListView::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& ListView::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& ListView::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& ListView::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}

IEvent<ISharedControl>& ListView::ColumnsChangedEvent()
{
    return _columnsChangedEvent;
}

IEvent<ISharedControl>& ListView::RowsChangedEvent()
{
    return _rowsChangedEvent;
}

IEvent<ISharedControl>& ListView::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl, int>& ListView::ColumnClickEvent()
{
    return _columnClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListView::ItemClickEvent()
{
    return _itemClickEvent;
}

IEvent<ISharedControl, size_t, int>& ListView::ItemDoubleClickEvent()
{
    return _itemDblClickEvent;
}
