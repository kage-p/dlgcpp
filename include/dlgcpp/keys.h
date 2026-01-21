#pragma once

#include <ostream>

namespace dlgcpp
{
    enum class Key
    {
        Unknown,

        // Letters
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // Numbers
        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,

        // Function keys
        F1, F2, F3, F4, F5, F6,
        F7, F8, F9, F10, F11, F12,
        F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

        // Control keys
        Escape, Tab, CapsLock, Shift, Ctrl, Alt,
        LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt,
        Space, Enter, Backspace,

        // Arrows
        Left, Up, Right, Down,

        // Navigation
        Insert, Delete, Home, End, PageUp, PageDown,

        // Symbols
        Apostrophe, Comma, Minus, Period, Slash,
        Semicolon, Equal, BracketLeft, Backslash, BracketRight, Grave,

        // Numpad
        NumPad0, NumPad1, NumPad2, NumPad3, NumPad4,
        NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
        NumPadMultiply, NumPadAdd, NumPadSubtract,
        NumPadDecimal, NumPadDivide, NumPadEnter, NumLock,

        // Other
        PrintScreen, ScrollLock, Pause,
        Apps, LWin, RWin, Menu
    };

    inline std::ostream& operator<<(std::ostream& os, Key key)
    {
        switch (key)
        {
        case Key::Unknown: return os << "Unknown";

            // Letters  
        case Key::A: return os << "A";
        case Key::B: return os << "B";
        case Key::C: return os << "C";
        case Key::D: return os << "D";
        case Key::E: return os << "E";
        case Key::F: return os << "F";
        case Key::G: return os << "G";
        case Key::H: return os << "H";
        case Key::I: return os << "I";
        case Key::J: return os << "J";
        case Key::K: return os << "K";
        case Key::L: return os << "L";
        case Key::M: return os << "M";
        case Key::N: return os << "N";
        case Key::O: return os << "O";
        case Key::P: return os << "P";
        case Key::Q: return os << "Q";
        case Key::R: return os << "R";
        case Key::S: return os << "S";
        case Key::T: return os << "T";
        case Key::U: return os << "U";
        case Key::V: return os << "V";
        case Key::W: return os << "W";
        case Key::X: return os << "X";
        case Key::Y: return os << "Y";
        case Key::Z: return os << "Z";

            // Numbers  
        case Key::Num0: return os << "Num0";
        case Key::Num1: return os << "Num1";
        case Key::Num2: return os << "Num2";
        case Key::Num3: return os << "Num3";
        case Key::Num4: return os << "Num4";
        case Key::Num5: return os << "Num5";
        case Key::Num6: return os << "Num6";
        case Key::Num7: return os << "Num7";
        case Key::Num8: return os << "Num8";
        case Key::Num9: return os << "Num9";

            // Function keys  
        case Key::F1: return os << "F1";
        case Key::F2: return os << "F2";
        case Key::F3: return os << "F3";
        case Key::F4: return os << "F4";
        case Key::F5: return os << "F5";
        case Key::F6: return os << "F6";
        case Key::F7: return os << "F7";
        case Key::F8: return os << "F8";
        case Key::F9: return os << "F9";
        case Key::F10: return os << "F10";
        case Key::F11: return os << "F11";
        case Key::F12: return os << "F12";
        case Key::F13: return os << "F13";
        case Key::F14: return os << "F14";
        case Key::F15: return os << "F15";
        case Key::F16: return os << "F16";
        case Key::F17: return os << "F17";
        case Key::F18: return os << "F18";
        case Key::F19: return os << "F19";
        case Key::F20: return os << "F20";
        case Key::F21: return os << "F21";
        case Key::F22: return os << "F22";
        case Key::F23: return os << "F23";
        case Key::F24: return os << "F24";

            // Control keys  
        case Key::Escape: return os << "Escape";
        case Key::Tab: return os << "Tab";
        case Key::CapsLock: return os << "CapsLock";
        case Key::Shift: return os << "Shift";
        case Key::Ctrl: return os << "Ctrl";
        case Key::Alt: return os << "Alt";
        case Key::LeftShift: return os << "LeftShift";
        case Key::RightShift: return os << "RightShift";
        case Key::LeftCtrl: return os << "LeftCtrl";
        case Key::RightCtrl: return os << "RightCtrl";
        case Key::LeftAlt: return os << "LeftAlt";
        case Key::RightAlt: return os << "RightAlt";
        case Key::Space: return os << "Space";
        case Key::Enter: return os << "Enter";
        case Key::Backspace: return os << "Backspace";

            // Arrows  
        case Key::Left: return os << "Left";
        case Key::Up: return os << "Up";
        case Key::Right: return os << "Right";
        case Key::Down: return os << "Down";

            // Navigation  
        case Key::Insert: return os << "Insert";
        case Key::Delete: return os << "Delete";
        case Key::Home: return os << "Home";
        case Key::End: return os << "End";
        case Key::PageUp: return os << "PageUp";
        case Key::PageDown: return os << "PageDown";

            // Symbols  
        case Key::Apostrophe: return os << "Apostrophe";
        case Key::Comma: return os << "Comma";
        case Key::Minus: return os << "Minus";
        case Key::Period: return os << "Period";
        case Key::Slash: return os << "Slash";
        case Key::Semicolon: return os << "Semicolon";
        case Key::Equal: return os << "Equal";
        case Key::BracketLeft: return os << "BracketLeft";
        case Key::Backslash: return os << "Backslash";
        case Key::BracketRight: return os << "BracketRight";
        case Key::Grave: return os << "Grave";

            // Numpad  
        case Key::NumPad0: return os << "NumPad0";
        case Key::NumPad1: return os << "NumPad1";
        case Key::NumPad2: return os << "NumPad2";
        case Key::NumPad3: return os << "NumPad3";
        case Key::NumPad4: return os << "NumPad4";
        case Key::NumPad5: return os << "NumPad5";
        case Key::NumPad6: return os << "NumPad6";
        case Key::NumPad7: return os << "NumPad7";
        case Key::NumPad8: return os << "NumPad8";
        case Key::NumPad9: return os << "NumPad9";
        case Key::NumPadMultiply: return os << "NumPadMultiply";
        case Key::NumPadAdd: return os << "NumPadAdd";
        case Key::NumPadSubtract: return os << "NumPadSubtract";
        case Key::NumPadDecimal: return os << "NumPadDecimal";
        case Key::NumPadDivide: return os << "NumPadDivide";
        case Key::NumPadEnter: return os << "NumPadEnter";
        case Key::NumLock: return os << "NumLock";

            // Other  
        case Key::PrintScreen: return os << "PrintScreen";
        case Key::ScrollLock: return os << "ScrollLock";
        case Key::Pause: return os << "Pause";
        case Key::Apps: return os << "Apps";
        case Key::LWin: return os << "LWin";
        case Key::RWin: return os << "RWin";
        case Key::Menu: return os << "Menu";

        default: return os << "Unknown";
        }
    }

    struct KeyboardEvent
    {
        Key key = Key::Unknown;

        friend std::ostream& operator<<(std::ostream& os, const KeyboardEvent& obj)
        {
            return os <<
                "{ key: " << obj.key << " }";
        }
    };
}
