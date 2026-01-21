#pragma once

#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IListBox : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& highlight() = 0;
            virtual IProperty<bool, ISharedControl>& multiselect() = 0;
            virtual IProperty<bool, ISharedControl>& sorted() = 0;
            virtual IProperty<std::vector<std::string>, ISharedControl>& items() = 0;
            virtual IProperty<int, ISharedControl>& selectedIndex() = 0;
            virtual IProperty<std::vector<int>, ISharedControl>& selectedIndexes() = 0;

            // events
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelCancelEvent() = 0;
        };

        typedef std::shared_ptr<IListBox> ISharedListBox;

        class ListBoxImpl;

        class ListBox :
            public Control,
            public IListBox
        {
        public:
            explicit ListBox(const Position& p = Position());
            ~ListBox() override;

            // IListBox impl.
            IProperty<bool, ISharedControl>& highlight() override;
            IProperty<bool, ISharedControl>& multiselect() override;
            IProperty<bool, ISharedControl>& sorted() override;
            IProperty<std::vector<std::string>, ISharedControl>& items() override;
            IProperty<int, ISharedControl>& selectedIndex() override;
            IProperty<std::vector<int>, ISharedControl>& selectedIndexes() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& SelCancelEvent() override;

            // compatibility setters
            void selectedIndex(int value);
            void selectedIndexes(const std::vector<int>& indexes);
            void highlight(bool value);
            void multiselect(bool value);
            void sorted(bool value);
            void items(const std::vector<std::string>& items);

        private:
            ListBox(std::shared_ptr<ListBoxImpl> impl, const Position& p);

            std::shared_ptr<ListBoxImpl> _impl;

            Property<bool, ISharedControl> _highlight;
            Property<bool, ISharedControl> _multiselect;
            Property<bool, ISharedControl> _sorted;
            Property<std::vector<std::string>, ISharedControl> _items;
            Property<int, ISharedControl> _selectedIndex;
            Property<std::vector<int>, ISharedControl> _selectedIndexes;

            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl> _selCancelEvent;
        };
    }
}
