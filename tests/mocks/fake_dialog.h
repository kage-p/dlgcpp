#pragma once

#include "dlgcpp/dialogs/dialog.h"
#include "unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        // the fake dialog has concrete properties/events with mockable action methods.
        class FakeDialog :
            public dlgcpp::dialogs::IDialog,
            public std::enable_shared_from_this<FakeDialog>
        {

        public:
            FakeDialog()
            {
                // initialize events with default binder so they know their owner
                auto ownerFn = [this]() -> ISharedDialog { return std::static_pointer_cast<IDialog>(shared_from_this()); };

                _enabled.reset(true, nullptr, ownerFn, "enabled");
                _visible.reset(false, nullptr, ownerFn, "visible");
                _display.reset(dlgcpp::dialogs::DisplayState::Normal, nullptr, ownerFn, "display");
                _p.reset(Position(0, 0, 600, 400), nullptr, ownerFn, "p");
                _type.reset(dlgcpp::dialogs::DialogType::Application, nullptr, ownerFn, "type");
                _showHelp.reset(false, nullptr, ownerFn, "showHelp");
                _title.reset(std::string(), nullptr, ownerFn, "title");
                _image.reset(ImageSource{}, nullptr, ownerFn, "image");
                _menu.reset(nullptr, nullptr, ownerFn, "menu");
                _color.reset(Color::Default, nullptr, ownerFn, "color");
                _cursor.reset(Cursor::Default, nullptr, ownerFn, "cursor");
                _active.reset(false, nullptr, ownerFn, "active");
                _dropTarget.reset(false, nullptr, ownerFn, "dropTarget");
                _mouseCapture.reset(false, nullptr, ownerFn, "mouseCapture");
                _handle.reset(nullptr, nullptr, ownerFn, "handle");
                _user.reset(nullptr, nullptr, ownerFn, "user");

                _confirmEvent.reset(ownerFn, "ConfirmEvent");
                _cancelEvent.reset(ownerFn, "CancelEvent");
                _closeEvent.reset(ownerFn, "CloseEvent");
                _createEvent.reset(ownerFn, "CreateEvent");
                _destroyEvent.reset(ownerFn, "DestroyEvent");
                _helpEvent.reset(ownerFn, "HelpEvent");
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
                _dropEvent.reset(ownerFn, "DropEvent");
                _userEvent.reset(ownerFn, "UserEvent");
            }

            // properties (concrete)
            IProperty<bool, ISharedDialog>& enabled() override { return _enabled; }
            IProperty<bool, ISharedDialog>& visible() override { return _visible; }
            IProperty<dlgcpp::dialogs::DisplayState, ISharedDialog>& display() override { return _display; }
            IProperty<Position, ISharedDialog>& p() override { return _p; }
            IProperty<dlgcpp::dialogs::DialogType, ISharedDialog>& type() override { return _type; }
            IProperty<bool, ISharedDialog>& showHelp() override { return _showHelp; }
            IProperty<std::string, ISharedDialog>& title() override { return _title; }
            IProperty<ImageSource, ISharedDialog>& image() override { return _image; }
            IProperty<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog>& menu() override { return _menu; }
            IProperty<Color, ISharedDialog>& color() override { return _color; }
            IProperty<Cursor, ISharedDialog>& cursor() override { return _cursor; }
            IProperty<bool, ISharedDialog>& active() override { return _active; }
            IProperty<bool, ISharedDialog>& dropTarget() override { return _dropTarget; }
            IProperty<bool, ISharedDialog>& mouseCapture() override { return _mouseCapture; }
            IProperty<void*, ISharedDialog>& handle() override { return _handle; }
            IProperty<void*, ISharedDialog>& user() override { return _user; }

            // actions
            MOCK_METHOD(int, exec, (), (override));
            MOCK_METHOD(void, show, (), (override));
            MOCK_METHOD(void, close, (int result), (override));
            MOCK_METHOD(void, redraw, (bool), (override));
            MOCK_METHOD(void, bringToFront, (), (override));
            MOCK_METHOD(void, sendToBack, (), (override));
            MOCK_METHOD(void, move, (const Point& point), (override));
            MOCK_METHOD(void, resize, (const Size& size), (override));
            MOCK_METHOD(void, center, (), (override));
            MOCK_METHOD(void, message, (const std::string& message, const std::string& title), (override));
            MOCK_METHOD(void, timer, (int timeout), (override));
            MOCK_METHOD(void, sendUserEvent, (int param), (override));

            // child management
            MOCK_METHOD(IWeakDialog, parent, (), (const, override));
            MOCK_METHOD(void, add, (ISharedControl child), (override));
            MOCK_METHOD(void, remove, (ISharedControl child), (override));
            MOCK_METHOD(std::vector<ISharedControl>, controls, (), (const, override));
            MOCK_METHOD(void, add, (ISharedDialog child), (override));
            MOCK_METHOD(void, remove, (ISharedDialog child), (override));
            MOCK_METHOD(std::vector<ISharedDialog>, dialogs, (), (const, override));

            // events (concrete)
            IEvent<ISharedDialog>& ConfirmEvent() override { return _confirmEvent; }
            IEvent<ISharedDialog>& CancelEvent() override { return _cancelEvent; }
            IEvent<ISharedDialog, dlgcpp::dialogs::CloseDialogEvent>& CloseEvent() override { return _closeEvent; }
            IEvent<ISharedDialog>& CreateEvent() override { return _createEvent; }
            IEvent<ISharedDialog>& DestroyEvent() override { return _destroyEvent; }
            IEvent<ISharedDialog>& HelpEvent() override { return _helpEvent; }
            IEvent<ISharedDialog>& MoveEvent() override { return _moveEvent; }
            IEvent<ISharedDialog>& SizeEvent() override { return _sizeEvent; }
            IEvent<ISharedDialog>& TimerEvent() override { return _timerEvent; }
            IEvent<ISharedDialog, KeyboardEvent>& KeyDownEvent() override { return _keyDownEvent; }
            IEvent<ISharedDialog, KeyboardEvent>& KeyUpEvent() override { return _keyUpEvent; }
            IEvent<ISharedDialog, MouseEvent>& MouseDownEvent() override { return _mouseDownEvent; }
            IEvent<ISharedDialog, MouseEvent>& MouseUpEvent() override { return _mouseUpEvent; }
            IEvent<ISharedDialog, MouseEvent>& MouseMoveEvent() override { return _mouseMoveEvent; }
            IEvent<ISharedDialog, MouseEvent>& MouseDoubleClickEvent() override { return _mouseDoubleClickEvent; }
            IEvent<ISharedDialog>& MouseCaptureLostEvent() override { return _mouseCaptureLostEvent; }
            IEvent<ISharedDialog, ISharedDrawingContext>& PaintEvent() override { return _paintEvent; }
            IEvent<ISharedDialog, DropFilesEvent>& DropEvent() override { return _dropEvent; }
            IEvent<ISharedDialog, int>& UserEvent() override { return _userEvent; }

            // internal
            MOCK_METHOD(std::shared_ptr<dlgcpp::dialogs::DialogImpl>, impl, (), (override));

        private:
            // concrete properties
            Property<bool, ISharedDialog> _enabled;
            Property<bool, ISharedDialog> _visible;
            Property<dlgcpp::dialogs::DisplayState, ISharedDialog> _display;
            Property<Position, ISharedDialog> _p;
            Property<dlgcpp::dialogs::DialogType, ISharedDialog> _type;
            Property<bool, ISharedDialog> _showHelp;
            Property<std::string, ISharedDialog> _title;
            Property<ImageSource, ISharedDialog> _image;
            Property<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog> _menu;
            Property<Color, ISharedDialog> _color;
            Property<Cursor, ISharedDialog> _cursor;
            Property<bool, ISharedDialog> _active;
            Property<bool, ISharedDialog> _dropTarget;
            Property<bool, ISharedDialog> _mouseCapture;
            Property<void*, ISharedDialog> _handle;
            Property<void*, ISharedDialog> _user;

            // concrete events
            Event<ISharedDialog> _confirmEvent;
            Event<ISharedDialog> _cancelEvent;
            Event<ISharedDialog, dlgcpp::dialogs::CloseDialogEvent> _closeEvent;
            Event<ISharedDialog> _createEvent;
            Event<ISharedDialog> _destroyEvent;
            Event<ISharedDialog> _helpEvent;
            Event<ISharedDialog> _moveEvent;
            Event<ISharedDialog> _sizeEvent;
            Event<ISharedDialog> _timerEvent;
            Event<ISharedDialog, KeyboardEvent> _keyDownEvent;
            Event<ISharedDialog, KeyboardEvent> _keyUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseDownEvent;
            Event<ISharedDialog, MouseEvent> _mouseUpEvent;
            Event<ISharedDialog, MouseEvent> _mouseMoveEvent;
            Event<ISharedDialog, MouseEvent> _mouseDoubleClickEvent;
            Event<ISharedDialog> _mouseCaptureLostEvent;
            Event<ISharedDialog, ISharedDrawingContext> _paintEvent;
            Event<ISharedDialog, DropFilesEvent> _dropEvent;
            Event<ISharedDialog, int> _userEvent;
        };
    }
}