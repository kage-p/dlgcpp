#pragma once

#include "dlgcpp/defs.h"
#include "dlgcpp/event.h"
#include "dlgcpp/gfx/context.h"
#include "dlgcpp/property.h"

#include <memory>
#include <ostream>
#include <string>

namespace dlgcpp
{
    namespace controls
    {
        enum class BorderStyle
        {
            None = 0,
            Thin,
            Sunken,
            Raised
        };

        inline std::ostream& operator<<(std::ostream& os, BorderStyle e)
        {
            switch (e) {
            case BorderStyle::None:   return os << "None";
            case BorderStyle::Thin:   return os << "Thin";
            case BorderStyle::Sunken: return os << "Sunken";
            case BorderStyle::Raised:  return os << "Raised";
            }
            return os << static_cast<int>(e); // fallback
        }

        class IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& enabled() = 0;
            virtual IProperty<bool, ISharedControl>& visible() = 0;
            virtual IProperty<Position, ISharedControl>& p() = 0;
            virtual IProperty<BorderStyle, ISharedControl>& border() = 0;
            virtual IProperty<std::pair<Color, Color>, ISharedControl>& colors() = 0;
            virtual IProperty<Font, ISharedControl>& font() = 0;
            virtual IProperty<Cursor, ISharedControl>& cursor() = 0;
            virtual IProperty<bool, ISharedControl>& focus() = 0;
            virtual IProperty<bool, ISharedControl>& mouseCapture() = 0;
            virtual IProperty<bool, ISharedControl>& wantKeyboardEvents() = 0;
            virtual IProperty<bool, ISharedControl>& wantMouseEvents() = 0;
            virtual IProperty<bool, ISharedControl>& wantPaintEvents() = 0;
            virtual IProperty<bool, ISharedControl>& wantSizingEvents() = 0;
            virtual IReadOnlyProperty<void*, ISharedControl>& handle() = 0;
            virtual IProperty<void*, ISharedControl>& user() = 0;

            // parent/child management
            virtual IWeakDialog parent() const = 0;

            // actions
            virtual void move(const Point& point) = 0;
            virtual void resize(const Size& size) = 0;
            virtual void bringToFront() = 0;
            virtual void sendToBack() = 0;
            virtual void redraw() = 0;

            // events
            virtual IEvent<ISharedControl>& CreateEvent() = 0;
            virtual IEvent<ISharedControl>& DestroyEvent() = 0;
            virtual IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent() = 0;
            virtual IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent() = 0;
            virtual IEvent<ISharedControl, MouseEvent>& MouseDownEvent() = 0;
            virtual IEvent<ISharedControl, MouseEvent>& MouseUpEvent() = 0;
            virtual IEvent<ISharedControl, MouseEvent>& MouseMoveEvent() = 0;
            virtual IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& MouseCaptureLostEvent() = 0;
            virtual IEvent<ISharedControl>& MoveEvent() = 0;
            virtual IEvent<ISharedControl>& SizeEvent() = 0;
            virtual IEvent<ISharedControl, ISharedDrawingContext>& PaintEvent() = 0;
            virtual IEvent<ISharedControl, int>& UserEvent() = 0;

            // internal
            virtual std::shared_ptr<ControlImpl> impl() = 0;
        };

        class Control :
            public virtual IControl,
            public std::enable_shared_from_this<Control>
        {
            friend class dlgcpp::dialogs::Dialog;

        public:
            // IControl impl.

            // properties
            IProperty<bool, ISharedControl>& enabled() override;
            IProperty<bool, ISharedControl>& visible() override;
            IProperty<Position, ISharedControl>& p() override;
            IProperty<BorderStyle, ISharedControl>& border() override;
            IProperty<std::pair<Color, Color>, ISharedControl>& colors() override;
            IProperty<Font, ISharedControl>& font() override;
            IProperty<Cursor, ISharedControl>& cursor() override;
            IProperty<bool, ISharedControl>& focus() override;
            IProperty<bool, ISharedControl>& mouseCapture() override;
            IProperty<bool, ISharedControl>& wantKeyboardEvents() override;
            IProperty<bool, ISharedControl>& wantMouseEvents() override;
            IProperty<bool, ISharedControl>& wantPaintEvents() override;
            IProperty<bool, ISharedControl>& wantSizingEvents() override;
            IProperty<void*, ISharedControl>& user() override;
            IReadOnlyProperty<void*, ISharedControl>& handle() override;

            // parent/child management
            IWeakDialog parent() const override;

            // compatibility setters
            void enabled(bool value);
            void visible(bool value);
            void p(const Position& p);
            void border(BorderStyle value);
            void colors(Color fgColor, Color bgColor);
            void font(const Font& value);
            void cursor(Cursor value);
            void focus(bool value);
            void mouseCapture(bool value);
            void wantKeyboardEvents(bool value);
            void wantMouseEvents(bool value);
            void wantPaintEvents(bool value);
            void wantSizingEvents(bool value);
            void user(void* value);

            // actions
            void move(const Point& point) override;
            void resize(const Size& size) override;
            void bringToFront() override;
            void sendToBack() override;
            void redraw() override;

            // events
            IEvent<ISharedControl>& CreateEvent() override;
            IEvent<ISharedControl>& DestroyEvent() override;
            IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent() override;
            IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent() override;
            IEvent<ISharedControl, MouseEvent>& MouseDownEvent() override;
            IEvent<ISharedControl, MouseEvent>& MouseUpEvent() override;
            IEvent<ISharedControl, MouseEvent>& MouseMoveEvent() override;
            IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent() override;
            IEvent<ISharedControl>& MouseCaptureLostEvent() override;
            IEvent<ISharedControl>& MoveEvent() override;
            IEvent<ISharedControl>& SizeEvent() override;
            IEvent<ISharedControl, ISharedDrawingContext>& PaintEvent() override;
            IEvent<ISharedControl, int>& UserEvent() override;

            // internal
            std::shared_ptr<ControlImpl> impl() override;

        protected:
            explicit Control(std::shared_ptr<ControlImpl> impl, const Position& p);
            virtual ~Control();

        private:
            std::shared_ptr<ControlImpl> _impl;
            IWeakDialog _parent;

            Property<bool, ISharedControl> _enabled;
            Property<bool, ISharedControl> _visible;
            Property<bool, ISharedControl> _mouseCapture;
            Property<bool, ISharedControl> _wantKeyboardEvents;
            Property<bool, ISharedControl> _wantMouseEvents;
            Property<bool, ISharedControl> _wantPaintEvents;
            Property<bool, ISharedControl> _wantSizingEvents;
            Property<Position, ISharedControl> _p;
            Property<BorderStyle, ISharedControl> _border;
            Property<std::pair<Color, Color>, ISharedControl> _colors;
            Property<Font, ISharedControl> _font;
            Property<Cursor, ISharedControl> _cursor;
            Property<bool, ISharedControl> _focus;
            Property<void*, ISharedControl> _handle;
            Property<void*, ISharedControl> _user;

            Event<ISharedControl> _createEvent;
            Event<ISharedControl> _destroyEvent;
            Event<ISharedControl, KeyboardEvent> _keyDownEvent;
            Event<ISharedControl, KeyboardEvent> _keyUpEvent;
            Event<ISharedControl, MouseEvent> _mouseDownEvent;
            Event<ISharedControl, MouseEvent> _mouseUpEvent;
            Event<ISharedControl, MouseEvent> _mouseMoveEvent;
            Event<ISharedControl, MouseEvent> _mouseDblClickEvent;
            Event<ISharedControl> _mouseCaptureLostEvent;
            Event<ISharedControl> _moveEvent;
            Event<ISharedControl> _sizeEvent;
            Event<ISharedControl, ISharedDrawingContext> _paintEvent;
            Event<ISharedControl, int> _userEvent;
        };
    }
}
