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
        class IMenu : public IMenuItemList
        {
        public:
            virtual void popup(ISharedDialog parent, const Point& coords) = 0;
            virtual void popup(ISharedControl parent, const Point& coords) = 0;
        };

        class Menu :
            public IMenu,
            public std::enable_shared_from_this<Menu>

        {
            friend class dlgcpp::dialogs::DialogImpl;

        public:
            explicit Menu();
            virtual ~Menu();

            // IMenu impl.
            void add(ISharedMenuItem item) override;
            void remove(ISharedMenuItem item) override;
            void clear() override;
            const std::vector<ISharedMenuItem>& items() const override;
            void popup(ISharedControl parent, const Point& coords) override;
            void popup(ISharedDialog parent, const Point& coords) override;

        protected:
            std::shared_ptr<MenuImpl> impl();

        private:
            std::shared_ptr<MenuImpl> _impl;
        };
    }
}
