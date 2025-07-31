#pragma once

#include "dlgcpp/controls/control.h"
#include "support/unit_test.h"

namespace dlgcpp
{
    namespace tests
    {
        class MockControl : public dlgcpp::controls::IControl
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
            MOCK_METHOD(dlgcpp::controls::BorderStyle, border, (), (const, override));
            MOCK_METHOD(void, border, (dlgcpp::controls::BorderStyle value), (override));
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
    }
}
