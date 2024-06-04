#pragma once
#include "dlgcpp/dialog.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    static const int DefaultStartId = 100;

    struct dlg_props
    {
        std::shared_ptr<IDialog> parent;
        bool enabled = true;
        bool visible = false;  // hidden by default
        DialogType type = DialogType::Application;
        Position p;
        std::string text;
        Color backColor = Color::Default;
        Cursor cursor = Cursor::Default;
        std::vector<std::shared_ptr<IChild>> children;
        int nextId = DefaultStartId;
        bool execRunning = false;
        struct
        {
            int id = 0;
            int timeout = 0;
            std::function<void(void)> handler;
        } timer;
        void* user = nullptr;
        Event clickEvent;
    };

    struct dlg_state
    {
        HWND hwnd = NULL;
        HBRUSH hbrBgColor = NULL;
    };
}
