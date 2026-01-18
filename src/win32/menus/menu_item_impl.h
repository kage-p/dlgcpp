#pragma once
#include "dlgcpp/menus/menu_item.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace menus
    {
        // performs no actions; but retained for compatibility
        class MenuItemImpl
        {
        public:
            explicit MenuItemImpl();

            void owner(IMenuItem* owner);

            bool insert(
                HMENU hMenu,
                int& menuItemId,
                std::map<int, ISharedMenuItem>& menuItemIdMap);
            void update();
            void destruct();

        private:
            IMenuItem* _menuItem = nullptr;
            HMENU _hMenu = NULL;
            int _menuItemId = 0;

            MENUITEMINFOW menuItemToStruct(
                int menuItemId,
                std::wstring& buffer);
        };
    }
}
