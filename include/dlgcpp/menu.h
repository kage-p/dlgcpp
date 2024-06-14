#pragma once

#include "defs.h"
#include "child.h"
#include "event.h"

#include <memory>
#include <string>
#include <vector>

namespace dlgcpp
{
    class IMenuItem;
    typedef std::shared_ptr<IMenuItem> ISharedMenuItem;

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
        virtual void seperator(bool value) = 0;
        virtual const std::vector<ISharedMenuItem>& items() const = 0;
        virtual IEvent<ISharedMenuItem>& ClickEvent() = 0;
    };

    class IMenu : public IMenuItemList
    {
    public:
        virtual void popup(ISharedDialog parent, const Point& coords) = 0;
    };

    class MenuItem : public IMenuItem
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
        void seperator(bool value) override;        
        void add(ISharedMenuItem item) override;
        void remove(ISharedMenuItem item) override;
        void clear() override;
        const std::vector<ISharedMenuItem>& items() const override;

        // events
        IEvent<ISharedMenuItem>& ClickEvent() override;

    private:
        struct menui_props* _props;
    };

    class Menu : public IChildMenu,
                 public IMenu,
                 public std::enable_shared_from_this<Menu>

    {
    public:
        explicit Menu();
        virtual ~Menu();

        // IChildMenu impl.
        ISharedDialog parent() const override;
        void parent(ISharedDialog) override;
        int id() const override;
        void id(int value) override;
        void notify(dlg_message&) override;
        void rebuild() override;
        std::shared_ptr<IMenu> menu() override;

        // IMenu impl.
        void add(ISharedMenuItem item) override;
        void remove(ISharedMenuItem item) override;
        void clear() override;
        const std::vector<ISharedMenuItem>& items() const override;
        void popup(ISharedDialog parent, const Point& coords) override;

    private:
        struct menu_props* _props;
        struct menu_state* _state;

        void destruct();
    };
}
