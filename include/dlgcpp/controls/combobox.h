#pragma once
#include "dlgcpp/controls/control.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IComboBox : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<int, ISharedControl>& selectedIndex() = 0;
            virtual IProperty<bool, ISharedControl>& dropDown() = 0;
            virtual IProperty<bool, ISharedControl>& editable() = 0;
            virtual IProperty<bool, ISharedControl>& sorted() = 0;
            virtual IProperty<std::vector<std::string>, ISharedControl>& items() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

            // events
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
            virtual IEvent<ISharedControl>& SelCancelEvent() = 0;
            virtual IEvent<ISharedControl>& ListOpenEvent() = 0;
            virtual IEvent<ISharedControl>& ListCloseEvent() = 0;
        };

        typedef std::shared_ptr<IComboBox> ISharedComboBox;

        class ComboBoxImpl;

        class ComboBox :
            public Control,
            public IComboBox
        {
        public:
            explicit ComboBox(const Position& p = Position());
            ~ComboBox() override;

            // IComboBox impl;
            IProperty<int, ISharedControl>& selectedIndex() override;
            IProperty<bool, ISharedControl>& dropDown() override;
            IProperty<bool, ISharedControl>& editable() override;
            IProperty<bool, ISharedControl>& sorted() override;
            IProperty<std::vector<std::string>, ISharedControl>& items() override;
            IProperty<std::string, ISharedControl>& text() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& SelChangedEvent() override;
            IEvent<ISharedControl>& SelCancelEvent() override;
            IEvent<ISharedControl>& ListOpenEvent() override;
            IEvent<ISharedControl>& ListCloseEvent() override;

            // compatibility setters
            void selectedIndex(int value);
            void dropDown(bool value);
            void editable(bool value);
            void sorted(bool value);
            void items(const std::vector<std::string>& items);
            void text(const std::string& value);

        private:
            ComboBox(std::shared_ptr<ComboBoxImpl> impl, const Position& p);

            std::shared_ptr<ComboBoxImpl> _impl;

            Property<bool, ISharedControl> _editable;
            Property<bool, ISharedControl> _dropDown;
            Property<bool, ISharedControl> _sorted;
            Property<int, ISharedControl> _selectedIndex;
            Property<std::vector<std::string>, ISharedControl> _items;
            Property<std::string, ISharedControl> _text;

            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _selChangedEvent;
            Event<ISharedControl> _selCancelEvent;
            Event<ISharedControl> _listCloseEvent;
            Event<ISharedControl> _listOpenEvent;
        };
    }
}
