#pragma once

#include "point.h"
#include <ostream>

namespace dlgcpp
{
    enum class MouseButton
    {
        None = 0,
        Left,
        Right,
        Middle
    };

    inline std::ostream& operator<<(std::ostream& os, MouseButton c) {
        switch (c) {
        case MouseButton::Left:   return os << "Left";
        case MouseButton::Middle: return os << "Middle";
        case MouseButton::Right:  return os << "Right";
        case MouseButton::None:  return os << "None";
        }
        return os << static_cast<int>(c); // fallback
    }

    struct MouseEvent
    {
        MouseButton button = MouseButton::None;
        Point point = Point(0, 0);

        friend std::ostream& operator<<(std::ostream& os, const MouseEvent& obj)
        {
            return os <<
                "{ button: " << obj.button <<
                ", point: " << obj.point <<
                " }";
        }
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

    inline std::ostream& operator<<(std::ostream& os, Cursor c)
    {
        switch (c) {
        case Cursor::Default:   return os << "Default";
        case Cursor::Arrow: return os << "Arrow";
        case Cursor::ArrowBusy:  return os << "ArrowBusy";
        case Cursor::Busy:  return os << "Busy";
        case Cursor::Cross:   return os << "Cross";
        case Cursor::Hand: return os << "Hand";
        case Cursor::Help:  return os << "Help";
        case Cursor::Text:  return os << "Text";
        case Cursor::Unavailable:  return os << "Unavailable";
        }
        return os << static_cast<int>(c); // fallback
    }
}
