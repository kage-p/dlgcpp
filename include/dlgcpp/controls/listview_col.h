#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/property.h"
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
            virtual IProperty<std::string, ISharedListViewColumn>& text() = 0;
            virtual IProperty<int, ISharedListViewColumn>& width() = 0;
        };

        class ListViewColumn :
            public IListViewColumn,
            public std::enable_shared_from_this<ListViewColumn>
        {
        public:
            ListViewColumn(const std::string& text = std::string(), int width = 100);
            ListViewColumn(const ListViewColumn& other);
            virtual ~ListViewColumn() = default;

            ListViewColumn& operator=(const ListViewColumn& other);
            bool operator==(const ListViewColumn& other) const;
            bool operator!=(const ListViewColumn& other) const;

            // IListViewColumn impl.
            IProperty<std::string, ISharedListViewColumn>& text() override;
            IProperty<int, ISharedListViewColumn>& width() override;

        private:
            Property<std::string, ISharedListViewColumn> _text;
            Property<int, ISharedListViewColumn> _width;
        };
    }
}
