#pragma once
#include "../control.h"
#include "listview_col.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        enum class ListViewDisplay
        {
            Details = 0,
            LargeIcon,
            List,
            SmallIcon
        };

        struct ListViewItem
        {
            size_t row = 0;
            int role = 0;
        };

        class IListView
        {
        public:
            virtual int selectedIndex() const = 0;
            virtual void selectedIndex(int value) = 0;

            virtual const std::vector<int>& selectedIndexes() const = 0;
            virtual void selectedIndexes(const std::vector<int>& indexes) = 0;

            virtual bool checkboxes() const = 0;
            virtual void checkboxes(bool value) = 0;

            virtual bool editing() const = 0;
            virtual bool beginEditing(size_t row, int role) = 0;
            virtual void confirmEditing() = 0;
            virtual void cancelEditing() = 0;

            virtual bool gridlines() const = 0;
            virtual void gridlines(bool value) = 0;

            virtual bool multiselect() const = 0;
            virtual void multiselect(bool value) = 0;

            virtual ListViewDisplay display() const = 0;
            virtual void display(ListViewDisplay value) = 0;

            // list data handling
            virtual int roleData(int column) const = 0;
            virtual ListViewColumn columnData(int role) const = 0;
            virtual size_t columnCount() const = 0;
            virtual std::string rowData(size_t row, int role) const = 0;
            virtual size_t rowCount() const = 0;
            virtual bool beginEdit(size_t row, int role, std::string& text) = 0;
            virtual void endEdit(size_t row, int role, const std::string& text, bool confirmed) = 0;

            // checkbox handling
            virtual bool checked(size_t row) const = 0;
            virtual void checked(size_t row, bool checked) = 0;

            virtual IEvent<ISharedControl>& ColumnsChangedEvent() = 0;
            virtual IEvent<ISharedControl>& RowsChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl, int>& ColumnClickEvent() = 0;
            virtual IEvent<ISharedControl, size_t, int>& ItemClickEvent() = 0;
            virtual IEvent<ISharedControl, size_t, int>& ItemDoubleClickEvent() = 0;
        };

        typedef std::shared_ptr<IListView> ISharedListView;

        class ListView : public dlgcpp::Control,
            public IListView
        {
        public:
            explicit ListView(const Position& p = Position());
            ~ListView() override;

            void notify(struct ctl_message&) override;
            void colors(Color fgColor, Color bgColor) override;

            // IListView impl.
            int selectedIndex() const override;
            void selectedIndex(int value) override;

            const std::vector<int>& selectedIndexes() const override;
            void selectedIndexes(const std::vector<int>& indexes) override;

            bool checkboxes() const override;
            void checkboxes(bool value) override;

            bool editing() const override;
            bool beginEditing(size_t row, int role) override;
            void confirmEditing() override;
            void cancelEditing() override;

            bool gridlines() const override;
            void gridlines(bool value) override;

            bool multiselect() const override;
            void multiselect(bool value) override;

            ListViewDisplay display() const override;
            void display(ListViewDisplay value) override;

            // placeholders
            int roleData(int column) const override;
            ListViewColumn columnData(int role) const override;
            size_t columnCount() const override;
            std::string rowData(size_t row, int role) const override;
            size_t rowCount() const override;
            bool beginEdit(size_t row, int role, std::string& text) override;
            void endEdit(size_t row, int role, const std::string& text, bool confirmed) override;
            bool checked(size_t row) const override;
            void checked(size_t row, bool checked);

            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& ColumnsChangedEvent() override;
            IEvent<ISharedControl>& RowsChangedEvent() override;
            IEvent<ISharedControl, int>& ColumnClickEvent() override;
            IEvent<ISharedControl, size_t, int>& ItemClickEvent() override;
            IEvent<ISharedControl, size_t, int>& ItemDoubleClickEvent() override;

        private:
            struct listview_priv* _priv;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
            void rebuild() override;

            void readSelection(int row, bool selected);
            void updateSelection();
            void updateColumns();
            void updateRows();
            void updateDisplayStyles();
            void updateListStyles();

            void onColumnsChanged();
            void onRowsChanged();
            void onSelectionChanged();
        };
    }
}
