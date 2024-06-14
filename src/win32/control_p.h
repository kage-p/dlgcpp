#pragma once
#include "dlgcpp/control.h"
#include "event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct ctl_props
    {
        ISharedDialog parent;
        bool enabled = true;
        bool visible = true;
        Position p;
        BorderStyle borderStyle = BorderStyle::None;
        std::string text;
        Color fgColor = Color::Default;
        Color bgColor = Color::Default;
        Font font;
        Cursor cursor = Cursor::Default;
        int id = -1;
        void* user = nullptr;

        Event<ISharedControl> clickEvent;
        Event<ISharedControl> dblClickEvent;
        Event<ISharedControl, bool> focusEvent;
    };

    struct ctl_state
    {
        HWND hwnd = NULL;
        HFONT hFont = NULL;
        HBRUSH hbrBack = NULL;
    };
}
