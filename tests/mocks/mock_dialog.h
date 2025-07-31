#pragma once

#include "dlgcpp/dialogs/dialog.h"
#include "support/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class MockDialog : public dlgcpp::dialogs::IDialog
        {
        public:
            // properties
            MOCK_METHOD(bool, enabled, (), (const, override));
            MOCK_METHOD(void, enabled, (bool), (override));
            MOCK_METHOD(bool, visible, (), (const, override));
            MOCK_METHOD(void, visible, (bool value), (override));
            MOCK_METHOD(dlgcpp::dialogs::DisplayState, displayState, (), (const, override));
            MOCK_METHOD(void, displayState, (dlgcpp::dialogs::DisplayState value), (override));
            MOCK_METHOD(const Position&, p, (), (const, override));
            MOCK_METHOD(void, p, (const Position&), (override));
            MOCK_METHOD(dlgcpp::dialogs::DialogType, type, (), (const, override));
            MOCK_METHOD(bool, showHelp, (), (const, override));
            MOCK_METHOD(void, showHelp, (bool value), (override));
            MOCK_METHOD(const std::string&, title, (), (const, override));
            MOCK_METHOD(void, title, (const std::string& value), (override));
            MOCK_METHOD(const ImageSource&, image, (), (const, override));
            MOCK_METHOD(void, image, (const ImageSource& image), (override));
            MOCK_METHOD(std::shared_ptr<dlgcpp::menus::Menu>, menu, (), (const, override));
            MOCK_METHOD(void, menu, (std::shared_ptr<dlgcpp::menus::Menu> menu), (override));
            MOCK_METHOD(Color, color, (), (const, override));
            MOCK_METHOD(void, color, (Color value), (override));
            MOCK_METHOD(Cursor, cursor, (), (const, override));
            MOCK_METHOD(void, cursor, (Cursor value), (override));
            MOCK_METHOD(bool, dropTarget, (), (const, override));
            MOCK_METHOD(void, dropTarget, (bool value), (override));
            MOCK_METHOD(bool, mouseCapture, (), (const, override));
            MOCK_METHOD(void, mouseCapture, (bool value), (override));
            MOCK_METHOD(void*, handle, (), (const, override));
            MOCK_METHOD(void*, user, (), (const, override));
            MOCK_METHOD(void, user, (void* value), (override));
            MOCK_METHOD(ISharedDialog, ptr, (), (override));
            MOCK_METHOD(ISharedDialog, parent, (), (const, override));

            // actions
            MOCK_METHOD(void, show, (), (override));
            MOCK_METHOD(void, close, (int result), (override));
            MOCK_METHOD(void, redraw, (bool), (override));
            MOCK_METHOD(void, setFocus, (), (override));
            MOCK_METHOD(void, bringToFront, (), (override));
            MOCK_METHOD(void, sendToBack, (), (override));
            MOCK_METHOD(void, move, (const Point& point), (override));
            MOCK_METHOD(void, resize, (const Size& size), (override));
            MOCK_METHOD(void, center, (), (override));
            MOCK_METHOD(void, message, (const std::string& message, const std::string& title), (override));
            MOCK_METHOD(void, timer, (int timeout), (override));
            MOCK_METHOD(void, sendUserEvent, (int param), (override));

            // child management
            MOCK_METHOD(void, add, (std::shared_ptr<dlgcpp::controls::Control> child), (override));
            MOCK_METHOD(void, remove, (std::shared_ptr<dlgcpp::controls::Control> child), (override));
            MOCK_METHOD(std::vector<std::shared_ptr<dlgcpp::controls::Control>>, controls, (), (const, override));
            MOCK_METHOD(void, add, (std::shared_ptr<dlgcpp::dialogs::Dialog> child), (override));
            MOCK_METHOD(void, remove, (std::shared_ptr<dlgcpp::dialogs::Dialog> child), (override));
            MOCK_METHOD(std::vector<std::shared_ptr<dlgcpp::dialogs::Dialog>>, dialogs, (), (const, override));

            // events
            MOCK_METHOD((IEvent<ISharedDialog>&), ConfirmEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), CancelEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), HelpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), MoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), SizeEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), TimerEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, KeyboardEvent>&), KeyDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, KeyboardEvent>&), KeyUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseMoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseDoubleClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), MouseCaptureLostEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, ISharedDrawingContext>&), PaintEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, std::vector<std::string>>&), DropEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, int>&), UserEvent, (), (override));
        };
    }
}
