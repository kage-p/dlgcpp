#pragma once

#include "dlgcpp/controls/control.h"
#include "utfw/unit_test.h"

#pragma push_macro("CreateEvent")

#ifdef CreateEvent
// conflicts with Windows.h macro
#undef CreateEvent
#endif

namespace dlgcpp
{
    namespace tests
    {
        // the fake control has concrete properties/events with mockable action methods.
        class FakeControl :
            public dlgcpp::controls::IControl,
            public std::enable_shared_from_this<FakeControl>
        {
        public:
            FakeControl()
            {
                // initialize events with default binder so they know their owner
                auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<IControl>(shared_from_this()); };

                _enabled.reset(true, nullptr, ownerFn, "enabled");
                _visible.reset(false, nullptr, ownerFn, "visible");
                _p.reset(Position(0, 0, 600, 400), nullptr, ownerFn, "p");
                _border.reset(dlgcpp::controls::BorderStyle::None, nullptr, ownerFn, "border");
                _colors.reset(std::make_pair(Color::Default, Color::Default), nullptr, ownerFn, "colors");
                _font.reset(Font(), nullptr, ownerFn, "font");
                _cursor.reset(Cursor::Default, nullptr, ownerFn, "cursor");
                _focus.reset(false, nullptr, ownerFn, "focus");
                _mouseCapture.reset(false, nullptr, ownerFn, "mouseCapture");
                _wantKeyboardEvents.reset(false, nullptr, ownerFn, "wantKeyboardEvents");
                _wantMouseEvents.reset(false, nullptr, ownerFn, "wantMouseEvents");
                _wantPaintEvents.reset(false, nullptr, ownerFn, "wantPaintEvents");
                _wantSizingEvents.reset(false, nullptr, ownerFn, "wantSizingEvents");
                _handle.reset(nullptr, nullptr, ownerFn, "handle");
                _user.reset(nullptr, nullptr, ownerFn, "user");

                _createEvent.reset(ownerFn, "CreateEvent");
                _destroyEvent.reset(ownerFn, "DestroyEvent");
                _moveEvent.reset(ownerFn, "MoveEvent");
                _sizeEvent.reset(ownerFn, "SizeEvent");
                _timerEvent.reset(ownerFn, "TimerEvent");
                _keyDownEvent.reset(ownerFn, "KeyDownEvent");
                _keyUpEvent.reset(ownerFn, "KeyUpEvent");
                _mouseDownEvent.reset(ownerFn, "MouseDownEvent");
                _mouseUpEvent.reset(ownerFn, "MouseUpEvent");
                _mouseMoveEvent.reset(ownerFn, "MouseMoveEvent");
                _mouseDoubleClickEvent.reset(ownerFn, "MouseDoubleClickEvent");
                _mouseCaptureLostEvent.reset(ownerFn, "MouseCaptureLostEvent");
                _paintEvent.reset(ownerFn, "PaintEvent");
                _userEvent.reset(ownerFn, "UserEvent");
            }

            // properties (concrete)
            IProperty<bool, ISharedControl>& enabled() override { return _enabled; }
            IProperty<bool, ISharedControl>& visible() override { return _visible; }
            IProperty<Position, ISharedControl>& p() override { return _p; }
            IProperty<dlgcpp::controls::BorderStyle, ISharedControl>& border() { return _border; }
            IProperty<std::pair<Color, Color>, ISharedControl>& colors() override { return _colors; }
            IProperty<Font, ISharedControl>& font() override { return _font; }
            IProperty<Cursor, ISharedControl>& cursor() override { return _cursor; }
            IProperty<bool, ISharedControl>& focus() override { return _focus; }
            IProperty<bool, ISharedControl>& mouseCapture() override { return _mouseCapture; }
            IProperty<bool, ISharedControl>& wantKeyboardEvents() override { return _wantKeyboardEvents; }
            IProperty<bool, ISharedControl>& wantMouseEvents() override { return _wantMouseEvents; }
            IProperty<bool, ISharedControl>& wantPaintEvents() override { return _wantPaintEvents; }
            IProperty<bool, ISharedControl>& wantSizingEvents() override { return _wantSizingEvents; }
            IProperty<void*, ISharedControl>& handle() override { return _handle; }
            IProperty<void*, ISharedControl>& user() override { return _user; }

            // parent/child management
            MOCK_METHOD(IWeakDialog, parent, (), (const, override));

            // actions
            MOCK_METHOD(void, move, (const Point&), (override));
            MOCK_METHOD(void, resize, (const Size&), (override));
            MOCK_METHOD(void, bringToFront, (), (override));
            MOCK_METHOD(void, sendToBack, (), (override));
            MOCK_METHOD(void, redraw, (), (override));

            // events (concrete)
            IEvent<ISharedControl>& CreateEvent() override { return _createEvent; }
            IEvent<ISharedControl>& DestroyEvent() override { return _destroyEvent; }
            IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent() override { return _keyDownEvent; }
            IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent() override { return _keyUpEvent; }
            IEvent<ISharedControl, MouseEvent>& MouseDownEvent() override { return _mouseDownEvent; }
            IEvent<ISharedControl, MouseEvent>& MouseUpEvent() override { return _mouseUpEvent; }
            IEvent<ISharedControl, MouseEvent>& MouseMoveEvent() override { return _mouseMoveEvent; }
            IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent() override { return _mouseDoubleClickEvent; }
            IEvent<ISharedControl>& MouseCaptureLostEvent() override { return _mouseCaptureLostEvent; }
            IEvent<ISharedControl>& MoveEvent() override { return _moveEvent; }
            IEvent<ISharedControl>& SizeEvent() override { return _sizeEvent; }
            IEvent<ISharedControl, ISharedDrawingContext>& PaintEvent() override { return _paintEvent; }
            IEvent<ISharedControl, int>& UserEvent() override { return _userEvent; }

            // internal
            MOCK_METHOD((std::shared_ptr<dlgcpp::controls::ControlImpl>), impl, (), (override));

        private:
            // concrete properties
            Property<bool, ISharedControl> _enabled;
            Property<bool, ISharedControl> _visible;
            Property<Position, ISharedControl> _p;
            Property<dlgcpp::controls::BorderStyle, ISharedControl> _border;
            Property<std::pair<Color, Color>, ISharedControl> _colors;
            Property<Font, ISharedControl> _font;
            Property<Cursor, ISharedControl> _cursor;
            Property<bool, ISharedControl> _focus;
            Property<bool, ISharedControl> _mouseCapture;
            Property<bool, ISharedControl> _wantKeyboardEvents;
            Property<bool, ISharedControl> _wantMouseEvents;
            Property<bool, ISharedControl> _wantPaintEvents;
            Property<bool, ISharedControl> _wantSizingEvents;
            Property<void*, ISharedControl> _handle;
            Property<void*, ISharedControl> _user;

            // concrete events
            Event<ISharedControl> _createEvent;
            Event<ISharedControl> _destroyEvent;
            Event<ISharedControl> _moveEvent;
            Event<ISharedControl> _sizeEvent;
            Event<ISharedControl> _timerEvent;
            Event<ISharedControl, KeyboardEvent> _keyDownEvent;
            Event<ISharedControl, KeyboardEvent> _keyUpEvent;
            Event<ISharedControl, MouseEvent> _mouseDownEvent;
            Event<ISharedControl, MouseEvent> _mouseUpEvent;
            Event<ISharedControl, MouseEvent> _mouseMoveEvent;
            Event<ISharedControl, MouseEvent> _mouseDoubleClickEvent;
            Event<ISharedControl> _mouseCaptureLostEvent;
            Event<ISharedControl, ISharedDrawingContext> _paintEvent;
            Event<ISharedControl, int> _userEvent;
        };
    }
}

#pragma pop_macro("CreateEvent")
