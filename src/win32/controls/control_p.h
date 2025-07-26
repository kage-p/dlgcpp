#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/gfx/context.h"
#include "utility/event.h"
#include "utility/message.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct ControlMessage
        {
            UINT wMsg = 0;
            WPARAM wParam = 0;
            LPARAM lParam = 0;
            LRESULT result = 0;
            WNDPROC orgWndProc = NULL;
        };

        class ControlImpl
        {
        public:
            ControlImpl(
                Control& control,
                const std::string& text,
                const Position& p);

            virtual ~ControlImpl();

            // properties
            virtual bool enabled() const;
            virtual void enabled(bool value);
            virtual bool visible() const;
            virtual void visible(bool value);
            virtual const Position& p() const;
            virtual void p(const Position& p);
            virtual void move(const Point& point);
            virtual void resize(const Size& size);
            virtual void setFocus();
            virtual void bringToFront();
            virtual void sendToBack();
            virtual BorderStyle border() const;
            virtual void border(BorderStyle value);
            virtual const std::string& text() const;
            virtual void text(const std::string& value);
            virtual std::pair<Color, Color> colors() const;
            virtual void colors(Color fgColor, Color bgColor);
            virtual const Font& font() const;
            virtual void font(const Font& value);
            virtual Cursor cursor() const;
            virtual void cursor(Cursor value);
            virtual bool mouseCapture() const;
            virtual void mouseCapture(bool value);
            virtual bool wantKeyboardEvents() const;
            virtual void wantKeyboardEvents(bool value);
            virtual bool wantMouseEvents() const;
            virtual void wantMouseEvents(bool value);
            virtual bool wantPaintEvents() const;
            virtual void wantPaintEvents(bool value);
            virtual bool wantSizingEvents() const;
            virtual void wantSizingEvents(bool value);
            ISharedDialog parent() const;
            virtual void* handle() const;
            virtual void* user() const;
            virtual void user(void* value);

            // actions
            virtual void redraw();

            // events
            virtual IEvent<ISharedControl>& ClickEvent();
            virtual IEvent<ISharedControl>& RightClickEvent();
            virtual IEvent<ISharedControl>& DoubleClickEvent();
            virtual IEvent<ISharedControl>& DoubleRightClickEvent();
            virtual IEvent<ISharedControl, bool>& FocusEvent();
            virtual IEvent<ISharedControl, KeyboardEvent>& KeyDownEvent();
            virtual IEvent<ISharedControl, KeyboardEvent>& KeyUpEvent();
            virtual IEvent<ISharedControl, MouseEvent>& MouseDownEvent();
            virtual IEvent<ISharedControl, MouseEvent>& MouseUpEvent();
            virtual IEvent<ISharedControl, MouseEvent>& MouseMoveEvent();
            virtual IEvent<ISharedControl, MouseEvent>& MouseDoubleClickEvent();
            virtual IEvent<ISharedControl>& MouseCaptureLostEvent();
            virtual IEvent<ISharedControl>& MoveEvent();
            virtual IEvent<ISharedControl>& SizeEvent();
            virtual IEvent<ISharedControl, ISharedDrawingContext>& PaintEvent();
            virtual IEvent<int>& UserEvent();

            // accessible from dialog internal and derived internal class
            int id() const;
            void id(int value);
            virtual int idRange() const;
            std::shared_ptr<IControl> control();
            virtual void notify(DialogMessage&);
            virtual void rebuild();
            void parent(ISharedDialog);
            virtual bool isHandleEqual(void* otherHandle) const;
            HBRUSH backgroundBrush() const;

        protected:
            // shared with derived internal class only
            bool wantInternalEvents() const;
            void wantInternalEvents(bool value);

            // overridable by derived internal class
            virtual std::string className() const;
            virtual unsigned int styles() const;
            virtual unsigned int exStyles() const;
            virtual void notify(ControlMessage&);

        private:
            Control& _control;
            ISharedDialog _parent;
            bool _enabled = true;
            bool _visible = true;
            bool _wantInternalEvents = false;
            bool _wantKeyboardEvents = false;
            bool _wantMouseEvents = false;
            bool _wantPaintEvents = false;
            bool _wantSizingEvents = false;
            Position _p;
            BorderStyle _borderStyle = BorderStyle::None;
            std::string _text;
            Color _fgColor = Color::Default;
            Color _bgColor = Color::Default;
            Font _font;
            Cursor _cursor = Cursor::Default;
            int _id = -1;
            void* _user = nullptr;

            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
            Event<ISharedControl, bool> _focusEvent;
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
            Event<int> _userEvent;

            // state properties
            HWND _hwnd = NULL;
            HFONT _hFont = NULL;
            HBRUSH _hbrBack = NULL;
            WNDPROC _prevWndProc = NULL;

            void destruct();
            bool mustBeSubclassed() const;
            static LRESULT CALLBACK controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
        };
    }
}
