#pragma once

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
        Apps, LWin, RWin, Menu,
    };

    struct KeyboardEvent
    {
        Key key = Key::Unknown;
    };
}
