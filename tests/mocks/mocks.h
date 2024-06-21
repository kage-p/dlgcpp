#pragma once

#include "dlgcpp/dlgcpp.h"
#include "support/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class MockDialog : public IDialog
        {
        public:
            MOCK_METHOD(bool, enabled, (), (const, override));
            MOCK_METHOD(void, enabled, (bool), (override));
            MOCK_METHOD(bool, visible, (), (const, override));
            MOCK_METHOD(void, visible, (bool value), (override));
            MOCK_METHOD(const Position&, p, (), (const, override));
            MOCK_METHOD(void, move, (const Point& point), (override));
            MOCK_METHOD(void, resize, (const Size& size), (override));
            MOCK_METHOD(void, center, (), (override));
            MOCK_METHOD(DialogType, type, (), (const, override));
            MOCK_METHOD(bool, showHelp, (), (const, override));
            MOCK_METHOD(void, showHelp, (bool value), (override));
            MOCK_METHOD(const std::string&, title, (), (const, override));
            MOCK_METHOD(void, title, (const std::string& value), (override));
            MOCK_METHOD(const ImageSource&, image, (), (const, override));
            MOCK_METHOD(void, image, (const ImageSource& image), (override));
            MOCK_METHOD(std::shared_ptr<IMenu>, menu, (), (const, override));
            MOCK_METHOD(void, menu, (std::shared_ptr<IChildMenu> menu), (override));
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
            MOCK_METHOD(ISharedDialog, parent, (), (override));
            MOCK_METHOD(void, close, (int result), (override));
            MOCK_METHOD(void, message, (const std::string& message, const std::string& title), (override));
            MOCK_METHOD(void, timer, (int timeout), (override));

            // child management
            MOCK_METHOD(void, add, (std::shared_ptr<IChildControl> child), (override));
            MOCK_METHOD(void, remove, (std::shared_ptr<IChildControl> child), (override));
            MOCK_METHOD(std::vector<std::shared_ptr<IControl>>, controls, (), (const, override));
            MOCK_METHOD(void, add, (std::shared_ptr<IChildDialog> child), (override));
            MOCK_METHOD(void, remove, (std::shared_ptr<IChildDialog> child), (override));
            MOCK_METHOD(std::vector<ISharedDialog>, dialogs, (), (const, override));

            // events
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseMoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, MouseEvent>&), MouseDoubleClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), MouseCaptureLostEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog, std::vector<std::string>>&), DropEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), HelpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), MoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), SizeEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedDialog>&), TimerEvent, (), (override));
        };

        class MockControl : public IControl
        {
        public:
            MOCK_METHOD(bool, enabled, (), (const, override));
            MOCK_METHOD(void, enabled, (bool value), (override));
            MOCK_METHOD(bool, visible, (), (const, override));
            MOCK_METHOD(void, visible, (bool value), (override));
            MOCK_METHOD(const Position&, p, (), (const, override));
            MOCK_METHOD(void, p, (const Position& position), (override));
            MOCK_METHOD(void, move, (const Point& point), (override));
            MOCK_METHOD(void, resize, (const Size& size), (override));
            MOCK_METHOD(BorderStyle, border, (), (const, override));
            MOCK_METHOD(void, border, (BorderStyle value), (override));
            MOCK_METHOD(const std::string&, text, (), (const, override));
            MOCK_METHOD(void, text, (const std::string& value), (override));
            MOCK_METHOD((std::pair<Color, Color>), colors, (), (const, override));
            MOCK_METHOD(void, colors, (Color fgColor, Color bgColor), (override));
            MOCK_METHOD(const Font&, font, (), (const, override));
            MOCK_METHOD(void, font, (const Font& value), (override));
            MOCK_METHOD(Cursor, cursor, (), (const, override));
            MOCK_METHOD(void, cursor, (Cursor value), (override));
            MOCK_METHOD(void*, handle, (), (const, override));
            MOCK_METHOD(void*, user, (), (const, override));
            MOCK_METHOD(void, user, (void* value), (override));
            MOCK_METHOD(ISharedDialog, parent, (), (const, override));
            MOCK_METHOD(void, redraw, (), (override));
            MOCK_METHOD(void, setFocus, (), (override));

            MOCK_METHOD((IEvent<ISharedControl>&), ClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), DoubleClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, bool>&), FocusEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseMoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseDoubleClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), MouseCaptureLostEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), MoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), SizeEvent, (), (override));
        };

        class MockMenu : public IMenu
        {
        public:
            MOCK_METHOD(void, add, (ISharedMenuItem), (override));
            MOCK_METHOD(void, remove, (ISharedMenuItem), (override));
            MOCK_METHOD(void, clear, (), (override));
            MOCK_METHOD(const std::vector<ISharedMenuItem>&, items, (), (const, override));

            MOCK_METHOD(void, popup, (ISharedDialog, const Point&), (override));
        };

        class MockChildControl : public IChildControl
        {
        public:
            MOCK_METHOD(ISharedDialog, parent, (), (const, override));
            MOCK_METHOD(void, parent, (ISharedDialog), (override));
            MOCK_METHOD(int, id, (), (const, override));
            MOCK_METHOD(void, id, (int value), (override));
            MOCK_METHOD(void, notify, (dlg_message&), (override));
            MOCK_METHOD(void, rebuild, (), (override));

            MOCK_METHOD(std::shared_ptr<IControl>, control, (), (override));

            //MOCK_METHOD(ctl_state, state, (), (override));
        };

        class MockChildDialog : public IChildDialog
        {
        public:
            MOCK_METHOD(ISharedDialog, parent, (), (const, override));
            MOCK_METHOD(void, parent, (ISharedDialog), (override));
            MOCK_METHOD(int, id, (), (const, override));
            MOCK_METHOD(void, id, (int value), (override));
            MOCK_METHOD(void, notify, (dlg_message&), (override));
            MOCK_METHOD(void, rebuild, (), (override));

            MOCK_METHOD(std::shared_ptr<IDialog>, dialog, (), (override));
        };

        class MockChildMenu : public IChildMenu
        {
        public:
            MOCK_METHOD(ISharedDialog, parent, (), (const, override));
            MOCK_METHOD(void, parent, (ISharedDialog), (override));
            MOCK_METHOD(int, id, (), (const, override));
            MOCK_METHOD(void, id, (int value), (override));
            MOCK_METHOD(void, notify, (dlg_message&), (override));
            MOCK_METHOD(void, rebuild, (), (override));

            MOCK_METHOD(std::shared_ptr<IMenu>, menu, (), (override));
        };

        template <class... Args>
        class MockEvent : public IEvent<Args...> {
        public:
           // MOCK_METHOD((IEvent<Args...>&), (operator+=), (std::function<void(Args... args)> fn), (override));
           // MOCK_METHOD((IEvent<Args...>&), operator+=, (std::function<void()> fn), (override));
            MOCK_METHOD(size_t, count, (), (override));
            MOCK_METHOD(void, clear, (), (override));
            MOCK_METHOD(void, invoke, (Args... args), (override));
            MOCK_METHOD(void, invoke, (), (override));
        };

    }
}
