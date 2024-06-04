#pragma once
#include "dlgcpp/control.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct ctl_props
    {
        bool _enabled = true;
        bool _visible = true;
        Position _p;
        std::string _text;
        Color _fgColor = Color::Default;
        Color _bgColor = Color::Default;
        Font _font;
        Cursor _cursor = Cursor::Default;
        int _id = -1;
        void* _user = nullptr;
    };

    struct ctl_state
    {
        HWND _hwnd = NULL;
        HFONT hFont = NULL;
        HBRUSH hbrBack = NULL;
    };
}
