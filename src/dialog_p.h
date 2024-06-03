#pragma once
#include "dialog.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct dlg_props
    {
        bool _enabled = true;
        bool _visible = false;  // hidden by default
        DialogType _type = DialogType::Application;
        Position _p;
        std::string _text;
        Color _backColor = Color::Default;
        Cursor _cursor = Cursor::Default;
        struct
        {
            int id = 0;
            int timeout = 0;
            std::function<void(void)> handler;
        } _timer;
        void* _user = nullptr;
    };

    struct dlg_state
    {
        HWND _hwnd = NULL;
        HBRUSH _hbrBgColor = NULL;
    };
}
