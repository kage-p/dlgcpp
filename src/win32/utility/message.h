#pragma once

#include "dlgcpp/defs.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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

    class MessageInhibitor
    {
    public:
        inline MessageInhibitor(bool& f)
            : _flag(f)
        {
            _flag = true;
        }

        inline ~MessageInhibitor()
        {
            _flag = false;
        }

    private:
        bool& _flag;
    };

    class MessageProcessor
    {
    public:
        static int beginLoop(HWND hwnd, HACCEL hAccel = nullptr);
    };
}
