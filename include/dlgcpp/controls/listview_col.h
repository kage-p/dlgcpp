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
            virtual const std::string& text() const  = 0;
            virtual void text(const std::string& value) = 0;

            virtual int role() const  = 0;
            virtual void role(int) = 0;

            virtual int width() const  = 0;
            virtual void width(int) = 0;
        };

        class ListViewColumn : public IListViewColumn
        {
        public:
            ListViewColumn(const std::string& text, int role, int width = 100);
            virtual ~ListViewColumn();

            const std::string& text() const override;
            void text(const std::string& value) override;

            int role() const override;
            void role(int value) override;

            int width() const override;
            void width(int value) override;

        private:
            struct listview_col_priv* _priv;
        };
    }
}
