#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/tab_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ITabs : public virtual IControl
        {
        public:
            virtual IProperty<Position, ISharedControl>& area() = 0;
            virtual IProperty<int, ISharedControl>& selectedIndex() = 0;
            virtual IProperty<std::vector<ISharedTabItem>, ISharedControl>& items() = 0;

            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
        };

        typedef std::shared_ptr<ITabs> ISharedTabs;

        class TabsImpl;

        class Tabs :
            public Control,
            public ITabs
        {
        public:
            explicit Tabs(
                const Position& p = Position());

            ~Tabs() override;

            // ITabs impl.
            IProperty<Position, ISharedControl>& area() override;
            IProperty<int, ISharedControl>& selectedIndex() override;
            IProperty<std::vector<ISharedTabItem>, ISharedControl>& items() override;

            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;

            // compatibility setters
            void selectedIndex(int value);
            void items(const std::vector<ISharedTabItem>& items);

        private:
            Tabs(std::shared_ptr<TabsImpl> impl, const Position& p);

            std::shared_ptr<TabsImpl> _impl;

            Property<Position, ISharedControl> _area;
            Property<int, ISharedControl> _selectedIndex;
            Property<std::vector<ISharedTabItem>, ISharedControl> _items;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
        };
    }
}
