#pragma once
#include "dlgcpp/control.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct ctl_props
    {
        std::shared_ptr<IDialog> parent;        
        bool enabled = true;
        bool visible = true;
        Position p;
        std::string text;
        Color fgColor = Color::Default;
        Color bgColor = Color::Default;
        Font font;
        Cursor cursor = Cursor::Default;
        int id = -1;
        void* user = nullptr;
        Event<> commandEvent;
    };

    struct ctl_state
    {
        HWND hwnd = NULL;
        HFONT hFont = NULL;
        HBRUSH hbrBack = NULL;
    };
}
