#pragma once
#include "dlgcpp/menus/menu_item.h"
#include "utility/event.h"
#include "utility/message.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace menus
    {
        class MenuItemImpl
        {
        public:
            explicit MenuItemImpl(
                MenuItem& menuItem,
                const std::string& text = std::string());

            const std::string& text() const;
            void text(const std::string& text);
            bool enabled() const;
            void enabled(bool value);
            bool checked() const;
            void checked(bool value);
            bool defaultItem() const;
            void defaultItem(bool value);
            bool separator() const;
            void separator(bool value);
            void add(ISharedMenuItem item);
            void remove(ISharedMenuItem item);
            void clear();
            const std::vector<ISharedMenuItem>& items() const;

            ISharedDialog parent() const;
            void parent(ISharedDialog);
            int id() const;
            void id(int value);
            int idRange() const { return 1; } // TBD
            void notify(DialogMessage&);
            void rebuild();
            std::shared_ptr<IMenu> menu();

            // events
            IEvent<ISharedMenuItem>& ChangedEvent();
            IEvent<ISharedMenuItem>& ClickEvent();

        private:
            MenuItem& _menuItem;
            int _id = 0;
            std::string _text;
            bool _enabled = true;
            bool _checked = false;
            bool _defaultItem = false;
            bool _separator = false;
            std::vector<ISharedMenuItem> _items;

            Event<ISharedMenuItem> _changedEvent;
            Event<ISharedMenuItem> _clickEvent;
        };
    }
}
