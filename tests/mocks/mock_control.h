#pragma once

#include "dlgcpp/controls/control.h"
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
        class MockControl : public dlgcpp::controls::IControl
        {
        public:
            // properties
            MOCK_METHOD((IProperty<bool, ISharedControl>&), enabled, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), visible, (), (override));
            MOCK_METHOD((IProperty<Position, ISharedControl>&), p, (), (override));
            MOCK_METHOD((IProperty<dlgcpp::controls::BorderStyle, ISharedControl>&), border, (), (override));
            MOCK_METHOD((IProperty<std::pair<Color, Color>, ISharedControl>&), colors, (), (override));
            MOCK_METHOD((IProperty<Font, ISharedControl>&), font, (), (override));
            MOCK_METHOD((IProperty<Cursor, ISharedControl>&), cursor, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), focus, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), mouseCapture, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), wantKeyboardEvents, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), wantMouseEvents, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), wantPaintEvents, (), (override));
            MOCK_METHOD((IProperty<bool, ISharedControl>&), wantSizingEvents, (), (override));
            MOCK_METHOD((IProperty<void*, ISharedControl>&), handle, (), (override));
            MOCK_METHOD((IProperty<void*, ISharedControl>&), user, (), (override));

            // parent/child management
            MOCK_METHOD(IWeakDialog, parent, (), (const, override));

            // actions
            MOCK_METHOD(void, move, (const Point&), (override));
            MOCK_METHOD(void, resize, (const Size&), (override));
            MOCK_METHOD(void, bringToFront, (), (override));
            MOCK_METHOD(void, sendToBack, (), (override));
            MOCK_METHOD(void, redraw, (), (override));

            // events
            MOCK_METHOD((IEvent<ISharedControl>&), CreateEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), DestroyEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, KeyboardEvent>&), KeyDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, KeyboardEvent>&), KeyUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseDownEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseUpEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseMoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, MouseEvent>&), MouseDoubleClickEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), MouseCaptureLostEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), MoveEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl>&), SizeEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, ISharedDrawingContext>&), PaintEvent, (), (override));
            MOCK_METHOD((IEvent<ISharedControl, int>&), UserEvent, (), (override));

            // internal
            MOCK_METHOD((std::shared_ptr<dlgcpp::controls::ControlImpl>), impl, (), (override));
        };
    }
}

#pragma pop_macro("CreateEvent")