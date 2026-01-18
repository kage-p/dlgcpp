#include "controls/control_impl.h"
#include "dlgcpp/controls/control.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Control::Control(
    std::shared_ptr<ControlImpl> impl,
    const Position& p = Position{})
    : _impl(impl)
{
    auto ownerFn = [this]() -> ISharedControl { return shared_from_this(); };

    // properties
    _border.reset(BorderStyle::None, nullptr, ownerFn, "border");
    _colors.reset(std::make_pair(Color::Default, Color::Default), nullptr, ownerFn, "colors");
    _cursor.reset(Cursor::Default, nullptr, ownerFn, "cursor");
    _enabled.reset(true, nullptr, ownerFn, "enabled");
    _focus.reset(false, nullptr, ownerFn, "focus");
    _font.reset(Font(), nullptr, ownerFn, "font");
    _mouseCapture.reset(false, nullptr, ownerFn, "mouseCapture");
    _p.reset(p, nullptr, ownerFn, "p");
    _user.reset(nullptr, nullptr, ownerFn, "user");
    _visible.reset(true, nullptr, ownerFn, "visible");
    _wantKeyboardEvents.reset(false, nullptr, ownerFn, "wantKeyboardEvents");
    _wantMouseEvents.reset(false, nullptr, ownerFn, "wantMouseEvents");
    _wantPaintEvents.reset(false, nullptr, ownerFn, "wantPaintEvents");
    _wantSizingEvents.reset(false, nullptr, ownerFn, "wantSizingEvents");

    // changed internally only
    _handle.reset(nullptr, nullptr, ownerFn, "handle");

    // events
    _createEvent.reset(ownerFn, "CreateEvent");
    _destroyEvent.reset(ownerFn, "DestroyEvent");
    _keyDownEvent.reset(ownerFn, "KeyDownEvent");
    _keyUpEvent.reset(ownerFn, "KeyUpEvent");
    _mouseCaptureLostEvent.reset(ownerFn, "MouseCaptureLostEvent");
    _mouseDblClickEvent.reset(ownerFn, "MouseDoubleClickEvent");
    _mouseDownEvent.reset(ownerFn, "MouseDownEvent");
    _mouseMoveEvent.reset(ownerFn, "MouseMoveEvent");
    _mouseUpEvent.reset(ownerFn, "MouseUpEvent");
    _moveEvent.reset(ownerFn, "MoveEvent");
    _paintEvent.reset(ownerFn, "PaintEvent");
    _sizeEvent.reset(ownerFn, "SizeEvent");
    _userEvent.reset(ownerFn, "UserEvent");

    // forward handle property change
    _impl->handle().event() +=
        [&](auto)
        {
            if (_impl)
                _handle = _impl->handle();
            else
                _handle = nullptr;
        };
}

Control::~Control()
{
    _impl.reset();
}

std::shared_ptr<ControlImpl> Control::impl()
{
    return _impl;
}

IProperty<bool, ISharedControl>& Control::enabled()
{
    return _enabled;
}

void Control::enabled(bool value)
{
    _enabled = value;
}

IProperty<bool, ISharedControl>& Control::visible()
{
    return _visible;
}

void Control::visible(bool value)
{
    _visible = value;
}

IProperty<Position, ISharedControl>& Control::p()
{
    return _p;
}

void Control::p(const Position& p)
{
    _p = p;
}

IProperty<BorderStyle, ISharedControl>& Control::border()
{
    return _border;
}

void Control::border(BorderStyle value)
{
    _border = value;
}

IProperty<std::pair<Color, Color>, ISharedControl>& Control::colors()
{
    return _colors;
}

void Control::colors(Color fgColor, Color bgColor)
{
    _colors = std::make_pair(fgColor, bgColor);
}

IProperty<Font, ISharedControl>& Control::font()
{
    return _font;
}

void Control::font(const Font& value)
{
    _font = value;
}

IProperty<Cursor, ISharedControl>& Control::cursor()
{
    return _cursor;
}

void Control::cursor(Cursor value)
{
    _cursor = value;
}

IProperty<bool, ISharedControl>& Control::focus()
{
    return _focus;
}

void Control::focus(bool value)
{
    _focus = value;
}

IProperty<bool, ISharedControl>& Control::mouseCapture()
{
    return _mouseCapture;
}

void Control::mouseCapture(bool value)
{
    _mouseCapture = value;
}

/// <summary>
/// Enables the control to receive all keyboard events.
/// </summary>
IProperty<bool, ISharedControl>& Control::wantKeyboardEvents()
{
    return _wantKeyboardEvents;
}

void Control::wantKeyboardEvents(bool value)
{
    _wantKeyboardEvents = value;
}

/// <summary>
/// Enables the control to receive all mouse events.
/// </summary>
IProperty<bool, ISharedControl>& Control::wantMouseEvents()
{
    return _wantMouseEvents;
}

void Control::wantMouseEvents(bool value)
{
    _wantMouseEvents = value;
}

/// <summary>
/// Enables the control to receive paint events.
/// </summary>
IProperty<bool, ISharedControl>& Control::wantPaintEvents()
{
    return _wantPaintEvents;
}

void Control::wantPaintEvents(bool value)
{
    _wantPaintEvents = value;
}

/// <summary>
/// Enables the control to receive move/size events.
/// </summary>
IProperty<bool, ISharedControl>& Control::wantSizingEvents()
{
    return _wantSizingEvents;
}

void Control::wantSizingEvents(bool value)
{
    _wantSizingEvents = value;
}

IReadOnlyProperty<void*, ISharedControl>& Control::handle()
{
    return _handle;
}

IProperty<void*, ISharedControl>& Control::user()
{
    return _user;
}

void Control::user(void* value)
{
    _user = value;
}

IWeakDialog Control::parent() const
{
    return _parent;
}

void Control::move(const Point& point)
{
    _p = Position(point.x(), point.y(), _p->width(), _p->height());
}

void Control::resize(const Size& size)
{
    _p = Position(_p->x(), _p->y(), size.width(), size.height());
}

void Control::bringToFront()
{
    _impl->bringToFront();
}

void Control::sendToBack()
{
    _impl->sendToBack();
}

void Control::redraw()
{
    _impl->redraw();
}

IEvent<ISharedControl>& Control::CreateEvent()
{
    return _createEvent;
}

IEvent<ISharedControl>& Control::DestroyEvent()
{
    return _destroyEvent;
}

IEvent<ISharedControl, KeyboardEvent>& Control::KeyDownEvent()
{
    return _keyDownEvent;
}

IEvent<ISharedControl, KeyboardEvent>& Control::KeyUpEvent()
{
    return _keyUpEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDownEvent()
{
    return _mouseDownEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseUpEvent()
{
    return _mouseUpEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseMoveEvent()
{
    return _mouseMoveEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDoubleClickEvent()
{
    return _mouseDblClickEvent;
}

IEvent<ISharedControl>& Control::MouseCaptureLostEvent()
{
    return _mouseCaptureLostEvent;
}

IEvent<ISharedControl>& Control::MoveEvent()
{
    return _moveEvent;
}

IEvent<ISharedControl>& Control::SizeEvent()
{
    return _sizeEvent;
}

IEvent<ISharedControl, ISharedDrawingContext>& Control::PaintEvent()
{
    return _paintEvent;
}

IEvent<ISharedControl, int>& Control::UserEvent()
{
    return _userEvent;
}
