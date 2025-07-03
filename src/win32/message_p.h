#pragma once

#include "dlgcpp/defs.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace dlgcpp
{
    class MessageProcessor
    {
    public:
        static int beginLoop(HWND hwnd, HACCEL hAccel = nullptr)
        {
            auto msg = MSG();
            while (GetMessage(&msg, NULL, 0, 0))
            {
                // dialog handle may change
                if (hAccel == NULL || !TranslateAccelerator(hwnd, hAccel, &msg))
                {
                    if (!IsDialogMessage(hwnd, &msg))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
            }
            return (int)msg.wParam;
        }
    };
}
