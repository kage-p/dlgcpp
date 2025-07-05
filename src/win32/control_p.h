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
        bool wantInternalEvents = false;
        bool wantKeyboardEvents = false;
        bool wantMouseEvents = false;
        bool wantSizingEvents = false;
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
        Event<ISharedControl> rightClickEvent;
        Event<ISharedControl> dblClickEvent;
        Event<ISharedControl> dblRightClickEvent;
        Event<ISharedControl, bool> focusEvent;
        Event<ISharedControl, KeyboardEvent> keyDownEvent;
        Event<ISharedControl, KeyboardEvent> keyUpEvent;
        Event<ISharedControl, MouseEvent> mouseDownEvent;
        Event<ISharedControl, MouseEvent> mouseUpEvent;
        Event<ISharedControl, MouseEvent> mouseMoveEvent;
        Event<ISharedControl, MouseEvent> mouseDblClickEvent;
        Event<ISharedControl> mouseCaptureLost;
        Event<ISharedControl> moveEvent;
        Event<ISharedControl> sizeEvent;
    };

    struct ctl_state
    {
        HWND hwnd = NULL;
        HFONT hFont = NULL;
        HBRUSH hbrBack = NULL;
        WNDPROC prevWndProc = NULL;
    };

    struct ctl_message
    {
        UINT wMsg = 0;
        WPARAM wParam = 0;
        LPARAM lParam = 0;
        LRESULT result = 0;
        WNDPROC orgWndProc = NULL;
    };

    struct ctl_priv
    {
        ctl_props props;
        ctl_state state;
    };
}
