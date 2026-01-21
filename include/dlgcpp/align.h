#pragma once

#include <ostream>

namespace dlgcpp
{
    enum class HorizontalAlign
    {
        Left = 0,
        Center,
        Right
    };

    inline std::ostream& operator<<(std::ostream& os, HorizontalAlign e)
    {
        switch (e) {
        case HorizontalAlign::Left:   return os << "Left";
        case HorizontalAlign::Center: return os << "Center";
        case HorizontalAlign::Right:  return os << "Right";
        }
        return os << static_cast<int>(e); // fallback
    }

    enum class VerticalAlign
    {
        Top = 0,
        Center,
        Bottom
    };

    inline std::ostream& operator<<(std::ostream& os, VerticalAlign e)
    {
        switch (e) {
        case VerticalAlign::Top:   return os << "Top";
        case VerticalAlign::Center: return os << "Center";
        case VerticalAlign::Bottom:  return os << "Bottom";
        }
        return os << static_cast<int>(e); // fallback
    }
}