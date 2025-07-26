#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"

#include <memory>
#include <string>
#include <vector>

namespace dlgcpp
{
    namespace menus
    {
        class IMenuItemList
        {
        public:
            virtual void add(ISharedMenuItem) = 0;
            virtual void remove(ISharedMenuItem) = 0;
            virtual void clear() = 0;
            virtual const std::vector<ISharedMenuItem>& items() const = 0;
        };

        class IMenuItem : public IMenuItemList
        {
        public:
            virtual const std::string& text() const = 0;
            virtual void text(const std::string& value) = 0;
            virtual bool enabled() const = 0;
            virtual void enabled(bool value) = 0;
            virtual bool checked() const = 0;
            virtual void checked(bool value) = 0;
            virtual bool defaultItem() const = 0;
            virtual void defaultItem(bool value) = 0;
            virtual bool separator() const = 0;
            virtual void separator(bool value) = 0;
            virtual const std::vector<ISharedMenuItem>& items() const = 0;

            // events
            virtual IEvent<ISharedMenuItem>& ChangedEvent() = 0;
            virtual IEvent<ISharedMenuItem>& ClickEvent() = 0;
        };

        class MenuItem :
            public IMenuItem,
            public std::enable_shared_from_this<MenuItem>
        {
        public:
            explicit MenuItem(const std::string& text = std::string());
            virtual ~MenuItem();

            // IMenuItem impl.
            const std::string& text() const override;
            void text(const std::string& text) override;
            bool enabled() const override;
            void enabled(bool value) override;
            bool checked() const override;
            void checked(bool value) override;
            bool defaultItem() const override;
            void defaultItem(bool value) override;
            bool separator() const override;
            void separator(bool value) override;
            void add(ISharedMenuItem item) override;
            void remove(ISharedMenuItem item) override;
            void clear() override;
            const std::vector<ISharedMenuItem>& items() const override;

            // events
            IEvent<ISharedMenuItem>& ChangedEvent() override;
            IEvent<ISharedMenuItem>& ClickEvent() override;

        private:
            std::shared_ptr<MenuItemImpl> _impl;
        };
    }
}
