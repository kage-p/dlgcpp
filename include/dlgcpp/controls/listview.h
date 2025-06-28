#pragma once
#include "../control.h"
#include "listview_col.h"

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
            virtual int currentIndex() const = 0;
            virtual void currentIndex(int value) = 0;

            virtual const std::vector<int>& currentIndexes() const = 0;
            virtual void currentIndexes(const std::vector<int>& indexes) = 0;

            virtual bool multiselect() const = 0;
            virtual void multiselect(bool value) = 0;

            virtual ListViewDisplay display() const = 0;
            virtual void display(ListViewDisplay value) = 0;

            virtual std::vector<ISharedListViewColumn> columns() const = 0;
            virtual std::string data(size_t row, int role) const = 0;
            virtual size_t count() const = 0;

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
            int currentIndex() const override;
            void currentIndex(int value) override;

            const std::vector<int>& currentIndexes() const override;
            void currentIndexes(const std::vector<int>& indexes) override;

            bool multiselect() const override;
            void multiselect(bool value) override;

            ListViewDisplay display() const override;
            void display(ListViewDisplay value) override;

            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& ItemsChangedEvent() override;
            IEvent<ISharedControl>& ColumnsChangedEvent() override;

        private:
            struct listview_priv* _priv;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
            void rebuild() override;

            void readSelection();
            void updateSelection();
            void updateColumns();
            void updateItems();

            void onColumnsChanged();
            void onItemsChanged();
        };
    }
}
