#pragma once

#include "point.h"

namespace dlgcpp
{
    enum class MouseButton
    {
        None = 0,
        Left,
        Right,
        Middle
    };

    struct MouseEvent
    {
        MouseButton button = MouseButton::None;
        Point point = Point(0, 0);
    };

    enum class Cursor
    {
        Default = 0,
        Arrow,
        ArrowBusy,
        Busy,
        Cross,
        Hand,
        Help,
        Text,
        Unavailable
    };
}
