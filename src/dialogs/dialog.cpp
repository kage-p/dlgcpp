#include "controls/control_impl.h"
#include "dialogs/dialog_impl.h"
#include "dlgcpp/dialogs/dialog.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::menus;

Dialog::Dialog(DialogType type, ISharedDialog parent) :
    Dialog::Dialog(
        std::make_shared<DialogImpl>(),
        type,
        parent)
{
}

Dialog::Dialog(
    std::shared_ptr<DialogImpl> impl,
    DialogType type,
    ISharedDialog parent) :
    _impl(impl)
{

    auto initPos = Position(0, 0, 600, 400);
    if (parent != nullptr)
    {
        // this dialog has a parent; but is not a child of the parent.
        // default position off-parent
        Point offset = parent->p()->point();
        initPos = Position(
            offset.x(),
            offset.y(),
            initPos.width(),
            initPos.height());

        _parent = parent;
    }

    auto ownerFn = [this]() -> ISharedDialog { return shared_from_this(); };

    _active.reset(false, nullptr, ownerFn, "active");
    _color.reset(Color::Default, nullptr, ownerFn, "color");
    _cursor.reset(Cursor::Default, nullptr, ownerFn, "cursor");
    _display.reset(DisplayState::Normal, nullptr, ownerFn, "display");
    _dropTarget.reset(false, nullptr, ownerFn, "dropTarget");
    _enabled.reset(true, nullptr, ownerFn, "enabled");
    _image.reset(ImageSource{}, nullptr, ownerFn, "image");
    _menu.reset(nullptr, nullptr, ownerFn, "menu");
    _mouseCapture.reset(false, nullptr, ownerFn, "mouseCapture");
    _p.reset(initPos, nullptr, ownerFn, "p");
    _showHelp.reset(false, nullptr, ownerFn, "showHelp");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _type.reset(type, nullptr, ownerFn, "type");
    _user.reset(nullptr, nullptr, ownerFn, "user");
    _visible.reset(false, nullptr, ownerFn, "visible");

    // changed internally only  
    _handle.reset(nullptr, nullptr, ownerFn, "handle");

    // events
    _cancelEvent.reset(ownerFn, "CancelEvent");
    _confirmEvent.reset(ownerFn, "ConfirmEvent");
    _closeEvent.reset(ownerFn, "CloseEvent");
    _createEvent.reset(ownerFn, "CreateEvent");
    _destroyEvent.reset(ownerFn, "DestroyEvent");
    _dropEvent.reset(ownerFn, "DropEvent");
    _helpEvent.reset(ownerFn, "HelpEvent");
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
    _timerEvent.reset(ownerFn, "TimerEvent");
    _userEvent.reset(ownerFn, "UserEvent");

    _impl->owner(this);

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

Dialog::~Dialog()
{
    // stop properties and event handlers from firing
    _enabled.event().clear();
    _visible.event().clear();
    _type.event().clear();
    _title.event().clear();
    _image.event().clear();
    _display.event().clear();
    _p.event().clear();
    _color.event().clear();
    _cursor.event().clear();
    _menu.event().clear();
    _active.event().clear();
    _dropTarget.event().clear();
    _mouseCapture.event().clear();
    _showHelp.event().clear();
    _handle.event().clear();
    _user.event().clear();

    _confirmEvent.clear();
    _cancelEvent.clear();
    _createEvent.clear();
    _destroyEvent.clear();
    _keyDownEvent.clear();
    _keyUpEvent.clear();
    _mouseDownEvent.clear();
    _mouseUpEvent.clear();
    _mouseMoveEvent.clear();
    _mouseDblClickEvent.clear();
    _mouseCaptureLostEvent.clear();
    _dropEvent.clear();
    _focusEvent.clear();
    _helpEvent.clear();
    _moveEvent.clear();
    _sizeEvent.clear();
    _paintEvent.clear();
    _timerEvent.clear();
    _userEvent.clear();

    _impl.reset();
}

std::shared_ptr<DialogImpl> Dialog::impl()
{
    return _impl;
}

int Dialog::exec()
{
    return _impl->exec();
}

IProperty<bool, ISharedDialog>& Dialog::enabled()
{
    return _enabled;
}

void Dialog::enabled(bool value)
{
    _enabled = value;
}

IProperty<bool, ISharedDialog>& Dialog::visible()
{
    return _visible;
}

void Dialog::visible(bool value)
{
    _visible = value;
}

IProperty<DisplayState, ISharedDialog>& Dialog::display()
{
    return _display;
}

void Dialog::display(DisplayState value)
{
    _display = value;
}

IProperty<Position, ISharedDialog>& Dialog::p()
{
    return _p;
}

void Dialog::p(const Position& p)
{
    _p = p;
}

IProperty<DialogType, ISharedDialog>& Dialog::type()
{
    return _type;
}

IProperty<bool, ISharedDialog>& Dialog::showHelp()
{
    return _showHelp;
}

void Dialog::showHelp(bool value)
{
    _showHelp = value;
}

IProperty<std::string, ISharedDialog>& Dialog::title()
{
    return _title;
}

void Dialog::title(const std::string& value)
{
    _title = value;
}

IProperty<ImageSource, ISharedDialog>& Dialog::image()
{
    return _image;
}

void Dialog::image(const ImageSource& image)
{
    _image = image;
}

IProperty<std::shared_ptr<Menu>, ISharedDialog>& Dialog::menu()
{
    return _menu;
}

void Dialog::menu(std::shared_ptr<Menu> menu)
{
    _menu = menu;
}

IProperty<Color, ISharedDialog>& Dialog::color()
{
    return _color;
}

void Dialog::color(Color value)
{
    _color = value;
}

IProperty<Cursor, ISharedDialog>& Dialog::cursor()
{
    return _cursor;
}

void Dialog::cursor(Cursor value)
{
    _cursor = value;
}

IProperty<bool, ISharedDialog>& Dialog::dropTarget()
{
    return _dropTarget;
}

void Dialog::dropTarget(bool value)
{
    _dropTarget = value;
}

IProperty<bool, ISharedDialog>& Dialog::active()
{
    return _active;
}

void Dialog::active(bool value)
{
    _active = value;
}

IProperty<bool, ISharedDialog>& Dialog::mouseCapture()
{
    return _mouseCapture;
}

void Dialog::mouseCapture(bool value)
{
    _mouseCapture = value;
}

IReadOnlyProperty<void*, ISharedDialog>& Dialog::handle()
{
    return _handle;
}

IProperty<void*, ISharedDialog>& Dialog::user()
{
    return _user;
}

void Dialog::user(void* value)
{
    _user = value;
}

IWeakDialog Dialog::parent() const
{
    return _parent;
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
    _p = Position(point.x(), point.y(), _p->width(), _p->height());
}

void Dialog::resize(const Size& size)
{
    _p = Position(_p->x(), _p->y(), size.width(), size.height());
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

void Dialog::add(ISharedControl control)
{
    if (control == nullptr)
    {
        DLGCPP_CERR("Attempt to add null control.");
        return;
    }

    if (!control->parent().expired())
    {
        // already has a parent
        DLGCPP_CERR("Control already has a parent.");
        return;
    }

    auto it = std::find(_controls.begin(), _controls.end(), control);
    if (it != _controls.end())
    {
        DLGCPP_CERR("Control is already owned by this dialog.");
        return;
    }

    auto actual = std::dynamic_pointer_cast<Control>(control);
    actual->_parent = shared_from_this();

    _controls.push_back(control);

    _impl->add(control);
}

void Dialog::remove(ISharedControl control)
{
    auto it = std::find(_controls.begin(), _controls.end(), control);
    if (it == _controls.end())
        return;

    auto actual = std::dynamic_pointer_cast<Control>(control);
    actual->_parent.reset();

    _controls.erase(it);

    _impl->remove(control);
}

std::vector<ISharedControl> Dialog::controls() const
{
    return _controls;
}

void Dialog::add(ISharedDialog dialog)
{
    if (dialog == nullptr)
    {
        DLGCPP_CERR("Attempt to add null dialog.");
        return;
    }

    if (!dialog->parent().expired())
    {
        // already has a parent
        DLGCPP_CERR("Dialog already has a parent.");
        return;
    }

    auto it = std::find(_dialogs.begin(), _dialogs.end(), dialog);
    if (it != _dialogs.end())
    {
        DLGCPP_CERR("Dialog is already owned by this dialog.");
        return;
    }

    auto actual = std::dynamic_pointer_cast<Dialog>(dialog);
    actual->_parent = shared_from_this();

    _dialogs.push_back(dialog);

    _impl->add(dialog);
}

void Dialog::remove(ISharedDialog dialog)
{
    auto it = std::find(_dialogs.begin(), _dialogs.end(), dialog);
    if (it == _dialogs.end())
        return;

    auto actual = std::dynamic_pointer_cast<Dialog>(dialog);
    actual->_parent.reset();

    _dialogs.erase(it);

    _impl->remove(dialog);
}

std::vector<ISharedDialog> Dialog::dialogs() const
{
    return _dialogs;
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
    return _confirmEvent;
}

IEvent<ISharedDialog>& Dialog::CancelEvent()
{
    return _cancelEvent;
}

IEvent<ISharedDialog, CloseDialogEvent>& Dialog::CloseEvent()
{
    return _closeEvent;
}

IEvent<ISharedDialog>& Dialog::CreateEvent()
{
    return _createEvent;
}

IEvent<ISharedDialog>& Dialog::DestroyEvent()
{
    return _destroyEvent;
}

IEvent<ISharedDialog>& Dialog::HelpEvent()
{
    return _helpEvent;
}

IEvent<ISharedDialog>& Dialog::MoveEvent()
{
    return _moveEvent;
}

IEvent<ISharedDialog>& Dialog::SizeEvent()
{
    return _sizeEvent;
}

IEvent<ISharedDialog, ISharedDrawingContext>& Dialog::PaintEvent()
{
    return _paintEvent;
}

IEvent<ISharedDialog>& Dialog::TimerEvent()
{
    return _timerEvent;
}

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyDownEvent()
{
    return _keyDownEvent;
}

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyUpEvent()
{
    return _keyUpEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDownEvent()
{
    return _mouseDownEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseUpEvent()
{
    return _mouseUpEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseMoveEvent()
{
    return _mouseMoveEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDoubleClickEvent()
{
    return _mouseDblClickEvent;
}

IEvent<ISharedDialog>& Dialog::MouseCaptureLostEvent()
{
    return _mouseCaptureLostEvent;
}

IEvent<ISharedDialog, DropFilesEvent>& Dialog::DropEvent()
{
    return _dropEvent;
}

IEvent<ISharedDialog, int>& Dialog::UserEvent()
{
    return _userEvent;
}
