#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    struct dlg_message
    {
        UINT wMsg = 0;
        WPARAM wParam = 0;
        LPARAM lParam = 0;
        LRESULT result = 0;
    };
}
