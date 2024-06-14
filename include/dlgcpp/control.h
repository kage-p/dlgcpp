#pragma once

#include "defs.h"
#include "child.h"
#include "event.h"

#include <memory>
#include <string>

namespace dlgcpp
{
    class IControl
    {
    public:
        virtual bool enabled() const = 0;
        virtual void enabled(bool value) = 0;
        virtual bool visible() const = 0;
        virtual void visible(bool value) = 0;
        virtual const Position& p() const = 0;
        virtual void p(const Position& p) = 0;
        virtual void move(const Point& point) = 0;
        virtual void resize(const Size& size) = 0;
        virtual BorderStyle border() const = 0;
        virtual void border(BorderStyle value) = 0;
        virtual const std::string& text() const = 0;
        virtual void text(const std::string& value) = 0;
        virtual std::pair<Color, Color> colors() const = 0;
        virtual void colors(Color fgColor, Color bgColor) = 0;
        virtual const Font& font() const = 0;
        virtual void font(const Font& value) = 0;
        virtual Cursor cursor() const = 0;
        virtual void cursor(Cursor value) = 0;
        virtual void* handle() const = 0;
        virtual void* user() const = 0;
        virtual void user(void* value) = 0;
        virtual ISharedDialog parent() const = 0;
        virtual void redraw() = 0;
        virtual void setFocus() = 0;

        virtual IEvent<ISharedControl>& ClickEvent() = 0;
        virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
        virtual IEvent<ISharedControl, bool>& FocusEvent() = 0;
        virtual IEvent<ISharedControl>& MoveEvent() = 0;
        virtual IEvent<ISharedControl>& SizeEvent() = 0;
    };

    class Control : public IChildControl,
                    public IControl,
                    public std::enable_shared_from_this<Control>
    {
    public:
        virtual void notify(struct ctl_message&);

        // IChildControl impl.
        ISharedDialog parent() const override;
        void parent(ISharedDialog) override;
        int id() const override;
        void id(int value) override;
        std::shared_ptr<IControl> control() override;
        struct ctl_state state() override;
        void notify(dlg_message&) override;
        void rebuild() override;

        // IControl impl.
        bool enabled() const override;
        void enabled(bool value) override;
        bool visible() const override;
        void visible(bool value) override;
        const Position& p() const override;
        void p(const Position& p) override;
        void move(const Point& point) override;
        void resize(const Size& size) override;
        BorderStyle border() const override;
        void border(BorderStyle value) override;
        const std::string& text() const override;
        void text(const std::string& value) override;
        std::pair<Color, Color> colors() const override;
        void colors(Color fgColor, Color bgColor) override;
        const Font& font() const override;
        void font(const Font& value) override;
        Cursor cursor() const override;
        void cursor(Cursor value) override;
        void* handle() const override;
        void* user() const override;
        void user(void* value) override;
        void redraw() override;
        void setFocus() override;

        IEvent<ISharedControl>& ClickEvent() override;
        IEvent<ISharedControl>& DoubleClickEvent() override;
        IEvent<ISharedControl, bool>& FocusEvent() override;
        IEvent<ISharedControl>& MoveEvent() override;
        IEvent<ISharedControl>& SizeEvent() override;

    protected:
        explicit Control(const std::string& text, const Position& p);
        virtual ~Control();

        // overridable by derived class
        virtual std::string className() const;
        virtual unsigned int styles() const;
        virtual unsigned int exStyles() const;

    private:
        struct ctl_props* _props;
        struct ctl_state* _state;

        void destruct();
    };
}
