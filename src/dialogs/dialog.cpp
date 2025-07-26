#include "controls/control_p.h"
#include "dialogs/dialog_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::menus;

Dialog::Dialog(DialogType type, ISharedDialog parent)
{
    _impl = std::make_shared<DialogImpl>(*this, type, parent);
}

Dialog::Dialog(std::shared_ptr<DialogImpl> impl) : _impl(impl)
{
}

std::shared_ptr<DialogImpl> Dialog::impl()
{
    return _impl;
}

int Dialog::exec()
{
    return _impl->exec();
}

bool Dialog::enabled() const
{
    return _impl->enabled();
}

void Dialog::enabled(bool value)
{
    if (_impl->enabled() == value)
        return;
    _impl->enabled(value);
}

bool Dialog::visible() const
{
    return _impl->visible();
}

void Dialog::visible(bool value)
{
    if (_impl->visible() == value)
        return;

    _impl->visible(value);
}

DisplayState Dialog::displayState() const
{
    return _impl->displayState();
}

void Dialog::displayState(DisplayState value)
{
    if (_impl->displayState() == value)
        return;
    _impl->displayState(value);
}

const Position& Dialog::p() const
{
    return _impl->p();
}

void Dialog::p(const Position& p)
{
    if (_impl->p() == p)
        return;

    _impl->p(p);
}

DialogType Dialog::type() const
{
    return _impl->type();
}

bool Dialog::showHelp() const
{
    return _impl->showHelp();
}

void Dialog::showHelp(bool value)
{
    _impl->showHelp(value);
}

const std::string& Dialog::title() const
{
    return _impl->title();
}

void Dialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

const ImageSource& Dialog::image() const
{
    return _impl->image();
}

void Dialog::image(const ImageSource& image)
{
    // must be an icon
    if (!image.isIcon)
        return;

    _impl->image(image);
}

std::shared_ptr<Menu> Dialog::menu() const
{
    return _impl->menu();
}

void Dialog::menu(std::shared_ptr<Menu> menu)
{
    if (_impl->menu() == menu)
        return;

    _impl->menu(menu);
}

Color Dialog::color() const
{
    return _impl->color();
}

void Dialog::color(Color value)
{
    if (_impl->color() == value)
        return;
    _impl->color(value);
}

Cursor Dialog::cursor() const
{
    return _impl->cursor();
}

void Dialog::cursor(Cursor value)
{
    _impl->cursor(value);
}

bool Dialog::dropTarget() const
{
    return _impl->dropTarget();
}

void Dialog::dropTarget(bool value)
{
    _impl->dropTarget(value);
}

bool Dialog::mouseCapture() const
{
    return _impl->mouseCapture();
}

void Dialog::mouseCapture(bool value)
{
    _impl->mouseCapture(value);
}

void* Dialog::handle() const
{
    return _impl->handle();
}

void* Dialog::user() const
{
    return _impl->user();
}

void Dialog::user(void* value)
{
    _impl->user(value);
}

ISharedDialog Dialog::parent() const
{
    return _impl->parent();
}

/// <summary>
/// Constructs and shows the dialog
/// </summary>
void Dialog::show()
{
    _impl->show();
}

/// <summary>
/// Close the dialog, removing it from view.
/// </summary>
void Dialog::close(int result)
{
    _impl->close(result);
}

void Dialog::setFocus()
{
    _impl->setFocus();
}

void Dialog::bringToFront()
{
    _impl->bringToFront();
}

void Dialog::sendToBack()
{
    _impl->sendToBack();
}

void Dialog::move(const Point& point)
{
    if (_impl->p().point() == point)
        return;

    _impl->move(point);
}

void Dialog::resize(const Size& size)
{
    if (_impl->p().size() == size)
        return;

    _impl->resize(size);
}

void Dialog::center()
{
    _impl->center();
}

void Dialog::message(
    const std::string& message,
    const std::string& title)
{
    _impl->message(message, title);
}

void Dialog::sendUserEvent(int param)
{
    _impl->sendUserEvent(param);
}

void Dialog::timer(int timeout)
{
    _impl->timer(timeout);
}

void Dialog::add(std::shared_ptr<Control> child)
{
    _impl->add(child);
}

void Dialog::remove(std::shared_ptr<Control> child)
{
    _impl->remove(child);
}

std::vector<std::shared_ptr<Control>> Dialog::controls() const
{
    return _impl->controls();
}

void Dialog::add(std::shared_ptr<Dialog> child)
{
    _impl->add(child);
}

void Dialog::remove(std::shared_ptr<Dialog> child)
{
    _impl->remove(child);
}

std::vector<std::shared_ptr<Dialog>> Dialog::dialogs() const
{
    return _impl->dialogs();
}

void Dialog::redraw(bool drawChildren)
{
    _impl->redraw(drawChildren);
}

Point Dialog::toPixels(const Point& point) const
{
    return _impl->toPixels(point);
}

Size Dialog::toPixels(const Size& size) const
{
    return _impl->toPixels(size);
}

Position Dialog::toPixels(const Position& pos) const
{
    return _impl->toPixels(pos);
}

IEvent<ISharedDialog>& Dialog::ConfirmEvent()
{
    return _impl->ConfirmEvent();
}

IEvent<ISharedDialog>& Dialog::CancelEvent()
{
    return _impl->CancelEvent();
}

IEvent<ISharedDialog>& Dialog::HelpEvent()
{
    return _impl->HelpEvent();
}

IEvent<ISharedDialog>& Dialog::MoveEvent()
{
    return _impl->MoveEvent();
}

IEvent<ISharedDialog>& Dialog::SizeEvent()
{
    return _impl->SizeEvent();
}

IEvent<ISharedDialog, ISharedDrawingContext>& Dialog::PaintEvent()
{
    return _impl->PaintEvent();
}

IEvent<ISharedDialog>& Dialog::TimerEvent()
{
    return _impl->TimerEvent();
}

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyDownEvent()
{
    return _impl->KeyDownEvent();
}

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyUpEvent()
{
    return _impl->KeyUpEvent();
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDownEvent()
{
    return _impl->MouseDownEvent();
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseUpEvent()
{
    return _impl->MouseUpEvent();
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseMoveEvent()
{
    return _impl->MouseMoveEvent();
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDoubleClickEvent()
{
    return _impl->MouseDoubleClickEvent();
}

IEvent<ISharedDialog>& Dialog::MouseCaptureLostEvent()
{
    return _impl->MouseCaptureLostEvent();
}

IEvent<ISharedDialog, std::vector<std::string>>& Dialog::DropEvent()
{
    return _impl->DropEvent();
}

IEvent<ISharedDialog, int>& Dialog::UserEvent()
{
    return _impl->UserEvent();
}
