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

        class IListView
        {
        public:
            virtual int selectedIndex() const = 0;
            virtual void selectedIndex(int value) = 0;

            virtual const std::vector<int>& selectedIndexes() const = 0;
            virtual void selectedIndexes(const std::vector<int>& indexes) = 0;

            virtual bool checkboxes() const = 0;
            virtual void checkboxes(bool value) = 0;

            virtual bool gridlines() const = 0;
            virtual void gridlines(bool value) = 0;

            virtual bool multiselect() const = 0;
            virtual void multiselect(bool value) = 0;

            virtual ListViewDisplay display() const = 0;
            virtual void display(ListViewDisplay value) = 0;

            // list data handling
            virtual std::vector<ISharedListViewColumn> columns() const = 0;
            virtual std::string data(size_t row, int role) const = 0;
            virtual size_t count() const = 0;

            // checkbox handling
            virtual bool checked(size_t row) const = 0;
            virtual void checked(size_t row, bool checked) = 0;

            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl>& ItemsChangedEvent() = 0;
            virtual IEvent<ISharedControl>& ColumnsChangedEvent() = 0;
        };

        typedef std::shared_ptr<IListView> ISharedListView;

        class ListView : public dlgcpp::Control,
            public IListView
        {
        public:
            explicit ListView(const Position& p = Position());
            ~ListView() override;

            // IListView impl.
            int selectedIndex() const override;
            void selectedIndex(int value) override;

            const std::vector<int>& selectedIndexes() const override;
            void selectedIndexes(const std::vector<int>& indexes) override;

            bool checkboxes() const override;
            void checkboxes(bool value) override;

            bool gridlines() const override;
            void gridlines(bool value) override;

            bool multiselect() const override;
            void multiselect(bool value) override;

            ListViewDisplay display() const override;
            void display(ListViewDisplay value) override;

            // placeholders
            std::vector<ISharedListViewColumn> columns() const override;
            std::string data(size_t row, int role) const override;
            bool checked(size_t row) const override;
            void checked(size_t row, bool checked);
            size_t count() const override;

            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& ItemsChangedEvent() override;
            IEvent<ISharedControl>& ColumnsChangedEvent() override;

        private:
            struct listview_priv* _priv;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
            void rebuild() override;

            void readSelection(int row, bool selected);
            void updateSelection();
            void updateColumns();
            void updateItems();
            void updateListStyles();

            void onColumnsChanged();
            void onItemsChanged();
            void onSelectionChanged();
        };
    }
}
