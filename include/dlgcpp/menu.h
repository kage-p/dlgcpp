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

    class IMenuItemList
    {
    public:
        virtual void add(std::shared_ptr<IMenuItem>) = 0;
        virtual void remove(std::shared_ptr<IMenuItem>) = 0;
        virtual void clear() = 0;
        virtual const std::vector<std::shared_ptr<IMenuItem>>& items() const = 0;
    };

    class IMenuItem : public IMenuItemList
    {
    public:
        virtual const std::string& text() const = 0;
        virtual void text(const std::string& text) = 0;
        virtual bool separator() const = 0;
        virtual void seperator(bool value) = 0;
        virtual const std::vector<std::shared_ptr<IMenuItem>>& items() const = 0;
        virtual IEvent<>& Clicked() = 0;
    };

    class IMenu : public IMenuItemList
    {
    public:
        virtual void popup(int x, int y) = 0;
    };

    class MenuItem : public IMenuItem
    {
    public:
        explicit MenuItem(const std::string& text = std::string());
        virtual ~MenuItem();

        // IMenuItem impl.
        const std::string& text() const override;
        void text(const std::string& text) override;
        bool separator() const override;
        void seperator(bool value) override;
        void add(std::shared_ptr<IMenuItem> item) override;
        void remove(std::shared_ptr<IMenuItem> item) override;
        void clear() override;
        const std::vector<std::shared_ptr<IMenuItem>>& items() const override;

        // events
        IEvent<>& Clicked() override;

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
        std::shared_ptr<IDialog> parent() const override;
        void parent(std::shared_ptr<IDialog>) override;
        int id() const override;
        void id(int value) override;
        void notify(dlg_message&) override;
        std::shared_ptr<IMenu> menu() override;

        // IMenu impl.
        void add(std::shared_ptr<IMenuItem> item) override;
        void remove(std::shared_ptr<IMenuItem> item) override;
        void clear() override;
        const std::vector<std::shared_ptr<IMenuItem>>& items() const override;
        void popup(int x, int y) override;

    private:
        struct menu_props* _props;
        struct menu_state* _state;

        void dump();
        void rebuild();
    };
}
