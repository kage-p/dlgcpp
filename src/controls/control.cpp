#include "controls/control_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Control::Control(std::shared_ptr<ControlImpl> impl)
    : _impl(impl)
{
}

Control::~Control()
{
    _impl.reset();
}

std::shared_ptr<ControlImpl> Control::impl()
{
    return _impl;
}

ISharedDialog Control::parent() const
{
    return _impl->parent();
}

bool Control::enabled() const
{
    return _impl->enabled();
}

void Control::enabled(bool value)
{
    _impl->enabled(value);
}

bool Control::visible() const
{
    return _impl->visible();
}

void Control::visible(bool value)
{
    _impl->visible(value);
}

const Position& Control::p() const
{
    return _impl->p();
}

void Control::p(const Position& p)
{
    _impl->p(p);
}

void Control::move(const Point& point)
{
    _impl->move(point);
}

void Control::resize(const Size& size)
{
    _impl->resize(size);
}

void Control::setFocus()
{
    _impl->setFocus();
}

void Control::bringToFront()
{
    _impl->bringToFront();
}

void Control::sendToBack()
{
    _impl->sendToBack();
}

BorderStyle Control::border() const
{
    return _impl->border();
}

void Control::border(BorderStyle value)
{
    if (_impl->border() == value)
        return;
    _impl->border(value);
}

const std::string& Control::text() const
{
    return _impl->text();
}

void Control::text(const std::string& value)
{
    if (_impl->text() == value)
        return;

    _impl->text(value);
}

std::pair<Color, Color> Control::colors() const
{
    return _impl->colors();
}

void Control::colors(Color fgColor, Color bgColor)
{
    _impl->colors(fgColor, bgColor);
}

const Font& Control::font() const
{
    return _impl->font();
}

void Control::font(const Font& value)
{
    _impl->font(value);
}

Cursor Control::cursor() const
{
    return _impl->cursor();
}

void Control::cursor(Cursor value)
{
    _impl->cursor(value);
}

bool Control::mouseCapture() const
{
    return _impl->mouseCapture();
}

void Control::mouseCapture(bool value)
{
    _impl->mouseCapture(value);
}

/// <summary>
/// Checks whether the control receives all keyboard events.
/// </summary>
bool Control::wantKeyboardEvents() const
{
    return _impl->wantKeyboardEvents();
}

/// <summary>
/// Enables the control to receive all keyboard events.
/// </summary>
void Control::wantKeyboardEvents(bool value)
{
    if (_impl->wantKeyboardEvents() == value)
        return;
    _impl->wantKeyboardEvents(value);
}

/// <summary>
/// Checks whether the control receives all mouse events.
/// </summary>
bool Control::wantMouseEvents() const
{
    return _impl->wantMouseEvents();
}

/// <summary>
/// Enables the control to receive all mouse events.
/// </summary>
void Control::wantMouseEvents(bool value)
{
    if (_impl->wantMouseEvents() == value)
        return;
    _impl->wantMouseEvents(value);
}


/// <summary>
/// Checks whether the control receives paint events.
/// </summary>
bool Control::wantPaintEvents() const
{
    return _impl->wantPaintEvents();
}

/// <summary>
/// Enables the control to receive paint events.
/// </summary>
void Control::wantPaintEvents(bool value)
{
    if (_impl->wantPaintEvents() == value)
        return;
    _impl->wantPaintEvents(value);
}

/// <summary>
/// Checks whether the control receives move/size events.
/// </summary>
bool Control::wantSizingEvents() const
{
    return _impl->wantSizingEvents();
}

/// <summary>
/// Enables the control to receive move/size events.
/// </summary>
/// <param name="value"></param>
void Control::wantSizingEvents(bool value)
{
    if (_impl->wantSizingEvents() == value)
        return;
    _impl->wantSizingEvents(value);
}

void* Control::handle() const
{
    return _impl->handle();
}

void* Control::user() const
{
    return _impl->user();
}

void Control::user(void* value)
{
    _impl->user(value);
}

void Control::redraw()
{
    _impl->redraw();
}

IEvent<ISharedControl>& Control::ClickEvent()
{
    return _impl->ClickEvent();
}

IEvent<ISharedControl>& Control::RightClickEvent()
{
    return _impl->RightClickEvent();
}

IEvent<ISharedControl>& Control::DoubleClickEvent()
{
    return _impl->DoubleClickEvent();
}

IEvent<ISharedControl>& Control::DoubleRightClickEvent()
{
    return _impl->DoubleRightClickEvent();
}

IEvent<ISharedControl, bool>& Control::FocusEvent()
{
    return _impl->FocusEvent();
}

IEvent<ISharedControl, KeyboardEvent>& Control::KeyDownEvent()
{
    return _impl->KeyDownEvent();
}

IEvent<ISharedControl, KeyboardEvent>& Control::KeyUpEvent()
{
    return _impl->KeyUpEvent();
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDownEvent()
{
    return _impl->MouseDownEvent();
}

IEvent<ISharedControl, MouseEvent>& Control::MouseUpEvent()
{
    return _impl->MouseUpEvent();
}

IEvent<ISharedControl, MouseEvent>& Control::MouseMoveEvent()
{
    return _impl->MouseMoveEvent();
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDoubleClickEvent()
{
    return _impl->MouseDoubleClickEvent();
}

IEvent<ISharedControl>& Control::MouseCaptureLostEvent()
{
    return _impl->MouseCaptureLostEvent();
}

IEvent<ISharedControl>& Control::MoveEvent()
{
    return _impl->MoveEvent();
}

IEvent<ISharedControl>& Control::SizeEvent()
{
    return _impl->SizeEvent();
}

IEvent<ISharedControl, ISharedDrawingContext>& Control::PaintEvent()
{
    return _impl->PaintEvent();
}

IEvent<int>& Control::UserEvent()
{
    return _impl->UserEvent();
}
