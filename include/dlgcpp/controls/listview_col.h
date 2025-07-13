#pragma once

#include <memory>
#include <string>

namespace dlgcpp
{
    namespace controls
    {
        class IListViewColumn;
        typedef std::shared_ptr<IListViewColumn> ISharedListViewColumn;

        class IListViewColumn
        {
        public:
            virtual const std::string& text() const = 0;
            virtual void text(const std::string& value) = 0;

            virtual int width() const = 0;
            virtual void width(int) = 0;
        };

        class ListViewColumn : public IListViewColumn
        {
        public:
            ListViewColumn();
            ListViewColumn(const std::string& text, int width = 100);
            ListViewColumn(const ListViewColumn& other);
            ListViewColumn& operator=(const ListViewColumn& other);
            virtual ~ListViewColumn();

            const std::string& text() const override;
            void text(const std::string& value) override;

            int width() const override;
            void width(int value) override;

        private:
            struct listview_col_priv* _priv;
        };
    }
}
