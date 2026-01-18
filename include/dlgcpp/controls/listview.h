#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/listview_col.h"
#include <ostream>
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        enum class ListViewDisplay
        {
            Details = 0,
            List,
            LargeIcon,
            SmallIcon
        };

        inline std::ostream& operator<<(std::ostream& os, ListViewDisplay e)
        {
            switch (e) {
            case ListViewDisplay::Details:   return os << "Details";
            case ListViewDisplay::List:  return os << "List";
            case ListViewDisplay::LargeIcon: return os << "LargeIcon";
            case ListViewDisplay::SmallIcon:  return os << "SmallIcon";
            }
            return os << static_cast<int>(e); // fallback
        }

        struct ListViewItem
        {
            size_t row = 0;
            int role = 0;
        };

        class IListView : public virtual IControl
        {
        public:
            virtual IProperty<ListViewDisplay, ISharedControl>& display() = 0;
            virtual IProperty<bool, ISharedControl>& columnHeader() = 0;
            virtual IProperty<bool, ISharedControl>& sortColumns() = 0;
            virtual IProperty<bool, ISharedControl>& checkboxes() = 0;
            virtual IProperty<bool, ISharedControl>& gridlines() = 0;
            virtual IProperty<bool, ISharedControl>& editing() = 0;
            virtual IProperty<bool, ISharedControl>& multiselect() = 0;
            virtual IProperty<int, ISharedControl>& selectedIndex() = 0;
            virtual IProperty<std::vector<int>, ISharedControl>& selectedIndexes() = 0;

            // editing
            virtual bool beginEditing(size_t row, int role) = 0;
            virtual void confirmEditing() = 0;
            virtual void cancelEditing() = 0;

            // list data handling
            virtual int roleData(int column) const = 0;
            virtual ListViewColumn columnData(int role) const = 0;
            virtual size_t columnCount() const = 0;
            virtual std::string rowData(size_t row, int role) const = 0;
            virtual size_t rowCount() const = 0;
            virtual bool beginEdit(size_t row, int role) = 0;
            virtual bool endEdit(size_t row, int role, const std::string& text) = 0;

            // checkbox handling
            virtual bool checked(size_t row) const = 0;
            virtual void checked(size_t row, bool checked) = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
            virtual IEvent<ISharedControl>& ColumnsChangedEvent() = 0;
            virtual IEvent<ISharedControl>& RowsChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl, int>& ColumnClickEvent() = 0;
            virtual IEvent<ISharedControl, size_t, int>& ItemClickEvent() = 0;
            virtual IEvent<ISharedControl, size_t, int>& ItemDoubleClickEvent() = 0;
        };

        typedef std::shared_ptr<IListView> ISharedListView;

        class ListViewImpl;

        class ListView :
            public Control,
            public IListView
        {
        public:
            explicit ListView(const Position& p = Position());
            ~ListView() override;

            // IListView impl.
            IProperty<ListViewDisplay, ISharedControl>& display() override;
            IProperty<bool, ISharedControl>& columnHeader() override;
            IProperty<bool, ISharedControl>& sortColumns() override;
            IProperty<bool, ISharedControl>& checkboxes() override;
            IProperty<bool, ISharedControl>& gridlines() override;
            IProperty<bool, ISharedControl>& editing() override;
            IProperty<bool, ISharedControl>& multiselect() override;
            IProperty<int, ISharedControl>& selectedIndex() override;
            IProperty<std::vector<int>, ISharedControl>& selectedIndexes() override;

            // editing
            bool beginEditing(size_t row, int role) override;
            void confirmEditing() override;
            void cancelEditing() override;

            // placeholders
            int roleData(int column) const override;
            ListViewColumn columnData(int role) const override;
            size_t columnCount() const override;
            std::string rowData(size_t row, int role) const override;
            size_t rowCount() const override;
            bool beginEdit(size_t row, int role) override;
            bool endEdit(size_t row, int role, const std::string& text) override;
            bool checked(size_t row) const override;
            void checked(size_t row, bool checked) override;

            // events
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;
            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& ColumnsChangedEvent() override;
            IEvent<ISharedControl>& RowsChangedEvent() override;
            IEvent<ISharedControl, int>& ColumnClickEvent() override;
            IEvent<ISharedControl, size_t, int>& ItemClickEvent() override;
            IEvent<ISharedControl, size_t, int>& ItemDoubleClickEvent() override;

            // compatibility setters
            void display(ListViewDisplay value);
            void columnHeader(bool value);
            void sortColumns(bool value);
            void checkboxes(bool value);
            void gridlines(bool value);
            void multiselect(bool value);
            void selectedIndex(int value);
            void selectedIndexes(const std::vector<int>& indexes);

        private:
            ListView(std::shared_ptr<ListViewImpl> impl, const Position& p);

            std::shared_ptr<ListViewImpl> _impl;

            Property<ListViewDisplay, ISharedControl> _display;
            Property<bool, ISharedControl> _columnHeader;
            Property<bool, ISharedControl> _sortColumns;
            Property<bool, ISharedControl> _checkboxes;
            Property<bool, ISharedControl> _gridlines;
            Property<bool, ISharedControl> _editing;
            Property<bool, ISharedControl> _multiselect;
            Property<int, ISharedControl> _selectedIndex;
            Property<std::vector<int>, ISharedControl> _selectedIndexes;

            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
            Event<ISharedControl> _columnsChangedEvent;
            Event<ISharedControl> _rowsChangedEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl, int> _columnClickEvent;
            Event<ISharedControl, size_t, int> _itemClickEvent;
            Event<ISharedControl, size_t, int> _itemDblClickEvent;
        };
    }
}
