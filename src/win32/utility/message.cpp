#include "message.h"

using namespace dlgcpp;

int MessageProcessor::beginLoop(
    HWND hwnd,
    HACCEL hAccel)
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

void MessageProcessor::doEvents()
{
    auto msg = MSG();
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
