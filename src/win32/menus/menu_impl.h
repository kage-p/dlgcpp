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
        class MenuImpl
        {
        public:
            explicit MenuImpl();
            virtual ~MenuImpl();

            void owner(IMenu* menu);
            void parent(std::weak_ptr<dialogs::IDialog> dialog);

            void popup(ISharedControl parent, const Point& coords);
            void popup(ISharedDialog parent, const Point& coords);

            int id() const;
            void id(int value);
            int idRange() const { return 1; } // TBD
            HMENU handle() const { return _hMenu; }
            void notify(DialogMessage&);
            void rebuild();

        private:
            IMenu* _menu = nullptr;
            std::weak_ptr<dialogs::IDialog> _parent;
            int _startId = MenuStartId;

            // state properties
            HMENU _hMenu = NULL;
            std::map<int, ISharedMenuItem> _menuItemIdMap;

            void destruct();

            static constexpr int MenuStartId = 10000;
        };
    }
}
