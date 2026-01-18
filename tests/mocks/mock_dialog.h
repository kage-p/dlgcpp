#pragma once

#include "dlgcpp/dialogs/dialog.h"
#include "support/unit_test.h"

#pragma push_macro("CreateEvent")

#ifdef CreateEvent
// conflicts with Windows.h macro
#undef CreateEvent
#endif

namespace dlgcpp
{
    namespace tests
    {
        class MockDialog : public dlgcpp::dialogs::IDialog
        {
        public:
            // properties
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), enabled, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), visible, (), (override));
            MOCK_METHOD((IProperty<dlgcpp::dialogs::DisplayState, ISharedDialog>&), display, (), (override));
            MOCK_METHOD((IProperty<Position, ISharedDialog>&), p, (), (override));
            MOCK_METHOD((IProperty<dlgcpp::dialogs::DialogType, ISharedDialog>&), type, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), showHelp, (), (override));
            MOCK_METHOD((IProperty<std::string, ISharedDialog>&), title, (), (override));
            MOCK_METHOD((IProperty<ImageSource, ISharedDialog>&), image, (), (override));
            MOCK_METHOD((IProperty<std::shared_ptr<dlgcpp::menus::Menu>, ISharedDialog>&), menu, (), (override));
            MOCK_METHOD((IProperty<Color, ISharedDialog>&), color, (), (override));
            MOCK_METHOD((IProperty<Cursor, ISharedDialog>&), cursor, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), active, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), dropTarget, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedDialog>&), mouseCapture, (), (override));
            MOCK_METHOD((IProperty<void*, ISharedDialog>&), handle, (), (override));
            MOCK_METHOD((IProperty<void*, ISharedDialog>&), user, (), (override));

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

            // parent/child management
            MOCK_METHOD(IWeakDialog, parent, (), (const, override));
            MOCK_METHOD(void, add, (ISharedControl child), (override));
            MOCK_METHOD(void, remove, (ISharedControl child), (override));
            MOCK_METHOD(std::vector<ISharedControl>, controls, (), (const, override));
            MOCK_METHOD(void, add, (ISharedDialog child), (override));
            MOCK_METHOD(void, remove, (ISharedDialog child), (override));
            MOCK_METHOD(std::vector<ISharedDialog>, dialogs, (), (const, override));

            // events
            MOCK_METHOD((IEvent<ISharedDialog>&), ConfirmEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), CancelEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, dlgcpp::dialogs::CloseDialogEvent>&), CloseEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), CreateEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), DestroyEvent, (), (override));
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
            MOCK_METHOD((IEvent<ISharedDialog, DropFilesEvent>&), DropEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, int>&), UserEvent, (), (override));

            // internal
            MOCK_METHOD(std::shared_ptr<dlgcpp::dialogs::DialogImpl>, impl, (), (override));
        };
    }
}

#pragma pop_macro("CreateEvent")

