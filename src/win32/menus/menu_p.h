#pragma once
#include "dlgcpp/menus/menu.h"
#include "utility/message.h"
#include <map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace menus
    {
        class MenuImpl : public IMenu
        {
        public:
            explicit MenuImpl();
            virtual ~MenuImpl();

            // IMenu impl.
            void add(ISharedMenuItem item);
            void remove(ISharedMenuItem item);
            void clear();
            const std::vector<ISharedMenuItem>& items() const;
            void popup(ISharedControl parent, const Point& coords);
            void popup(ISharedDialog parent, const Point& coords);

            ISharedDialog parent() const;
            void parent(ISharedDialog);
            int id() const;
            void id(int value);
            int idRange() const { return 1; } // TBD
            void notify(DialogMessage&);
            void rebuild();

        private:
            ISharedMenu _menu;
            ISharedDialog _parent;
            int _startId = MenuStartId;
            std::vector<ISharedMenuItem> _items;

            // state properties
            HMENU _hMenu = NULL;
            std::map<int, ISharedMenuItem> _idMap;

            void updateItem(ISharedMenuItem item);
            void destruct();

            void createMenuItems(
                HMENU hMenu,
                const std::vector<ISharedMenuItem>& items,
                int& nextItemId);

            bool insertMenuItem(
                ISharedMenuItem item,
                HMENU hMenu,
                int& nextItemId);

            MENUITEMINFOW menuItemToStruct(
                ISharedMenuItem item,
                int menuItemId,
                std::wstring& buffer);

            static constexpr int MenuStartId = 10000;
        };
    }
}
