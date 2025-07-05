#pragma once

#include "child.h"
#include "defs.h"
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
        virtual void setFocus() = 0;
        virtual void bringToFront() = 0;
        virtual void sendToBack() = 0;
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
        virtual bool wantKeyboardEvents() const = 0;
        virtual void wantKeyboardEvents(bool value) = 0;
        virtual bool wantMouseEvents() const = 0;
        virtual void wantMouseEvents(bool value) = 0;
        virtual bool wantSizingEvents() const = 0;
        virtual void wantSizingEvents(bool value) = 0;
        virtual void* handle() const = 0;
        virtual void* user() const = 0;
        virtual void user(void* value) = 0;
        virtual ISharedDialog parent() const = 0;
        virtual void redraw() = 0;

        virtual IEvent<ISharedControl>& ClickEvent() = 0;
        virtual IEvent<ISharedControl>& RightClickEvent() = 0;
        virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
        virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
        virtual IEvent<ISharedControl, bool>& FocusEvent() = 0;
        virtual IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent() = 0;
        virtual IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent() = 0;
        virtual IEvent<ISharedControl, MouseEvent>& MouseDownEvent() = 0;
        virtual IEvent<ISharedControl, MouseEvent>& MouseUpEvent() = 0;
        virtual IEvent<ISharedControl, MouseEvent>& MouseMoveEvent() = 0;
        virtual IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent() = 0;
        virtual IEvent<ISharedControl>& MouseCaptureLostEvent() = 0;
        virtual IEvent<ISharedControl>& MoveEvent() = 0;
        virtual IEvent<ISharedControl>& SizeEvent() = 0;
    };

    class Control : public IChildControl,
        public IControl,
        public std::enable_shared_from_this<Control>
    {
    public:
        // control specific
        virtual void notify(struct ctl_message&);

        // IChildControl impl.
        ISharedDialog parent() const override;
        void parent(ISharedDialog) override;
        int id() const override;
        void id(int value) override;
        int idRange() const override;
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
        void setFocus() override;
        void bringToFront() override;
        void sendToBack() override;
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
        bool wantKeyboardEvents() const override;
        void wantKeyboardEvents(bool value) override;
        bool wantMouseEvents() const override;
        void wantMouseEvents(bool value) override;
        bool wantSizingEvents() const override;
        void wantSizingEvents(bool value) override;
        void* handle() const override;
        void* user() const override;
        void user(void* value) override;
        void redraw() override;

        IEvent<ISharedControl>& ClickEvent() override;
        IEvent<ISharedControl>& RightClickEvent() override;
        IEvent<ISharedControl>& DoubleClickEvent() override;
        IEvent<ISharedControl>& DoubleRightClickEvent() override;
        IEvent<ISharedControl, bool>& FocusEvent() override;
        IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent() override;
        IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent() override;
        IEvent<ISharedControl, MouseEvent>& MouseDownEvent() override;
        IEvent<ISharedControl, MouseEvent>& MouseUpEvent() override;
        IEvent<ISharedControl, MouseEvent>& MouseMoveEvent() override;
        IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent() override;
        IEvent<ISharedControl>& MouseCaptureLostEvent() override;
        IEvent<ISharedControl>& MoveEvent() override;
        IEvent<ISharedControl>& SizeEvent() override;

    protected:
        explicit Control(const std::string& text, const Position& p);
        virtual ~Control();

        // exposed to derived class only
        bool wantInternalEvents() const;
        void wantInternalEvents(bool value);

        // overridable by derived class
        virtual std::string className() const;
        virtual unsigned int styles() const;
        virtual unsigned int exStyles() const;
        const struct ctl_priv* priv() const;

    private:
        struct ctl_priv* _pi = nullptr;
    };
}
