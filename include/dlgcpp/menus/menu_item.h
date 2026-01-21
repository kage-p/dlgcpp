#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/property.h"

#include <memory>
#include <string>
#include <vector>

namespace dlgcpp
{
    namespace menus
    {
        class IMenuItem
        {
        public:
            // properties
            virtual IProperty<std::string, ISharedMenuItem>& text() = 0;
            virtual IProperty<bool, ISharedMenuItem>& enabled() = 0;
            virtual IProperty<bool, ISharedMenuItem>& checked() = 0;
            virtual IProperty<bool, ISharedMenuItem>& defaultItem() = 0;
            virtual IProperty<bool, ISharedMenuItem>& separator() = 0;

            // item management
            virtual void add(ISharedMenuItem) = 0;
            virtual void remove(ISharedMenuItem) = 0;
            virtual void clear() = 0;
            virtual IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenuItem>& items() = 0;

            // events
            virtual IEvent<ISharedMenuItem>& ClickEvent() = 0;

            // internal
            virtual std::shared_ptr<MenuItemImpl> impl() = 0;
        };

        class MenuItem :
            public IMenuItem,
            public std::enable_shared_from_this<MenuItem>
        {
        public:
            explicit MenuItem(const std::string& text = std::string());
            virtual ~MenuItem();

            // IMenuItem impl.
            IProperty<std::string, ISharedMenuItem>& text() override;
            IProperty<bool, ISharedMenuItem>& enabled() override;
            IProperty<bool, ISharedMenuItem>& checked() override;
            IProperty<bool, ISharedMenuItem>& defaultItem() override;
            IProperty<bool, ISharedMenuItem>& separator() override;
            IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenuItem>& items() override;
            void add(ISharedMenuItem item) override;
            void remove(ISharedMenuItem item) override;
            void clear() override;
            std::shared_ptr<MenuItemImpl> impl() override;
            IEvent<ISharedMenuItem>& ClickEvent() override;

        private:
            std::shared_ptr<MenuItemImpl> _impl;

            Property<std::string, ISharedMenuItem> _text;
            Property<bool, ISharedMenuItem> _enabled;
            Property<bool, ISharedMenuItem> _checked;
            Property<bool, ISharedMenuItem> _defaultItem;
            Property<bool, ISharedMenuItem> _separator;
            Property<std::vector<ISharedMenuItem>, ISharedMenuItem> _items;

            Event<ISharedMenuItem> _clickEvent;
        };
    }
}
