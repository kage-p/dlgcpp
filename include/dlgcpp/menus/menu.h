#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/menus/menu_item.h"

#include <memory>
#include <string>
#include <vector>

namespace dlgcpp
{
    namespace menus
    {
        class IMenu
        {
        public:
            // item management
            virtual void add(ISharedMenuItem) = 0;
            virtual void remove(ISharedMenuItem) = 0;
            virtual void clear() = 0;
            virtual IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenu>& items() = 0;

            virtual void popup(ISharedDialog parent, const Point& coords) = 0;
            virtual void popup(ISharedControl parent, const Point& coords) = 0;

            virtual std::shared_ptr<MenuImpl> impl() = 0;
        };

        class Menu :
            public IMenu,
            public std::enable_shared_from_this<Menu>

        {
        public:
            explicit Menu();
            virtual ~Menu();

            // IMenu impl.
            void add(ISharedMenuItem item) override;
            void remove(ISharedMenuItem item) override;
            void clear() override;
            IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenu>& items() override;

            void popup(ISharedControl parent, const Point& coords) override;
            void popup(ISharedDialog parent, const Point& coords) override;

            std::shared_ptr<MenuImpl> impl() override;

        private:
            std::shared_ptr<MenuImpl> _impl;

            Property<std::vector<ISharedMenuItem>, ISharedMenu> _items;
        };
    }
}
