#pragma once
#include "dlgcpp/dialog.h"
#include "event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    static const int ChildStartId = 100;
    static const int MenuStartId = 10000;

    struct dlg_props
    {
        std::shared_ptr<IDialog> parent;
        bool enabled = true;
        bool visible = false;  // hidden by default
        bool dropTarget = false;
        DialogType type = DialogType::Application;
        Position p;
        std::string title;
        ImageSource image;
        std::shared_ptr<IChildMenu> menu;
        Color backColor = Color::Default;
        Cursor cursor = Cursor::Default;
        std::vector<std::shared_ptr<IChildControl>> children;
        int nextId = ChildStartId;
        bool execRunning = false;
        struct
        {
            int id = 0;
            int timeout = 0;
        } timer;
        void* user = nullptr;
        Event<> clickEvent;
        Event<> dblClickEvent;
        Event<std::vector<std::string>> dropEvent;
        Event<> focusEvent;
        Event<> moveEvent;
        Event<> sizeEvent;
        Event<> timerEvent;
    };

    struct dlg_state
    {
        HWND hwnd = NULL;
        HMENU hMenu = NULL;
        HBRUSH hbrBgColor = NULL;
        HGDIOBJ hImage = NULL;
    };
}
