#pragma once

#include "dlgcpp/defs.h"
#include <map>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace dlgcpp
{
    static const std::map<UINT, Key> vkToKeyMap = {
        // Letters
        {'A', Key::A}, {'B', Key::B}, {'C', Key::C}, {'D', Key::D}, {'E', Key::E},
        {'F', Key::F}, {'G', Key::G}, {'H', Key::H}, {'I', Key::I}, {'J', Key::J},
        {'K', Key::K}, {'L', Key::L}, {'M', Key::M}, {'N', Key::N}, {'O', Key::O},
        {'P', Key::P}, {'Q', Key::Q}, {'R', Key::R}, {'S', Key::S}, {'T', Key::T},
        {'U', Key::U}, {'V', Key::V}, {'W', Key::W}, {'X', Key::X}, {'Y', Key::Y},
        {'Z', Key::Z},

        // Numbers
        {'0', Key::Num0}, {'1', Key::Num1}, {'2', Key::Num2}, {'3', Key::Num3},
        {'4', Key::Num4}, {'5', Key::Num5}, {'6', Key::Num6}, {'7', Key::Num7},
        {'8', Key::Num8}, {'9', Key::Num9},

        // Function keys
        {VK_F1, Key::F1}, {VK_F2, Key::F2}, {VK_F3, Key::F3}, {VK_F4, Key::F4},
        {VK_F5, Key::F5}, {VK_F6, Key::F6}, {VK_F7, Key::F7}, {VK_F8, Key::F8},
        {VK_F9, Key::F9}, {VK_F10, Key::F10}, {VK_F11, Key::F11}, {VK_F12, Key::F12},
        {VK_F13, Key::F13}, {VK_F14, Key::F14}, {VK_F15, Key::F15}, {VK_F16, Key::F16},
        {VK_F17, Key::F17}, {VK_F18, Key::F18}, {VK_F19, Key::F19}, {VK_F20, Key::F20},
        {VK_F21, Key::F21}, {VK_F22, Key::F22}, {VK_F23, Key::F23}, {VK_F24, Key::F24},

        // Control
        {VK_ESCAPE, Key::Escape}, {VK_TAB, Key::Tab}, {VK_CAPITAL, Key::CapsLock},
        {VK_SHIFT, Key::Shift}, {VK_CONTROL, Key::Ctrl}, {VK_MENU, Key::Alt},
        {VK_LSHIFT, Key::LeftShift}, {VK_RSHIFT, Key::RightShift},
        {VK_LCONTROL, Key::LeftCtrl}, {VK_RCONTROL, Key::RightCtrl},
        {VK_LMENU, Key::LeftAlt}, {VK_RMENU, Key::RightAlt},
        {VK_SPACE, Key::Space}, {VK_RETURN, Key::Enter}, {VK_BACK, Key::Backspace},

        // Arrows
        {VK_LEFT, Key::Left}, {VK_RIGHT, Key::Right},
        {VK_UP, Key::Up}, {VK_DOWN, Key::Down},

        // Navigation
        {VK_INSERT, Key::Insert}, {VK_DELETE, Key::Delete},
        {VK_HOME, Key::Home}, {VK_END, Key::End},
        {VK_PRIOR, Key::PageUp}, {VK_NEXT, Key::PageDown},

        // Symbols
        {VK_OEM_1, Key::Semicolon},        // ;:
        {VK_OEM_PLUS, Key::Equal},         // =+
        {VK_OEM_COMMA, Key::Comma},        // ,<
        {VK_OEM_MINUS, Key::Minus},        // -_
        {VK_OEM_PERIOD, Key::Period},      // .>
        {VK_OEM_2, Key::Slash},            // /?
        {VK_OEM_3, Key::Grave},            // `~
        {VK_OEM_4, Key::BracketLeft},      // [{
        {VK_OEM_5, Key::Backslash},        // \|
        {VK_OEM_6, Key::BracketRight},     // ]}
        {VK_OEM_7, Key::Apostrophe},       // '"

        // Numpad
        {VK_NUMPAD0, Key::NumPad0}, {VK_NUMPAD1, Key::NumPad1}, {VK_NUMPAD2, Key::NumPad2},
        {VK_NUMPAD3, Key::NumPad3}, {VK_NUMPAD4, Key::NumPad4}, {VK_NUMPAD5, Key::NumPad5},
        {VK_NUMPAD6, Key::NumPad6}, {VK_NUMPAD7, Key::NumPad7}, {VK_NUMPAD8, Key::NumPad8},
        {VK_NUMPAD9, Key::NumPad9},
        {VK_MULTIPLY, Key::NumPadMultiply}, {VK_ADD, Key::NumPadAdd},
        {VK_SUBTRACT, Key::NumPadSubtract}, {VK_DECIMAL, Key::NumPadDecimal},
        {VK_DIVIDE, Key::NumPadDivide}, {VK_RETURN, Key::NumPadEnter}, // same as main Enter
        {VK_NUMLOCK, Key::NumLock},

        // Other
        {VK_PRINT, Key::PrintScreen}, {VK_SCROLL, Key::ScrollLock}, {VK_PAUSE, Key::Pause},
        {VK_APPS, Key::Apps}, {VK_LWIN, Key::LWin}, {VK_RWIN, Key::RWin}, {VK_MENU, Key::Menu},
    };

    inline Key TranslateKey(UINT vk)
    {
        auto it = vkToKeyMap.find(vk);
        return (it != vkToKeyMap.end()) ? it->second : Key::Unknown;
    }
}