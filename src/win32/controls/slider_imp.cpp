#include "slider_imp.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <strsafe.h>

#define SLDR_MAX_TEXT 512
typedef struct TSLDR_DATA {                    // Custom data for the control
    HWND hwnd;                                 // Handle to the control
    TCHAR szText[SLDR_MAX_TEXT];               // Display text
    int lMin;                                  // Minimum value (can be negative)
    int lMax;                                  // Maximum value
    int lPos;                                  // Current position
    COLORREF clrText;                          // Text display color
    COLORREF clrBack;                          // Back color
    COLORREF clrBar;                           // Bar color
    HBITMAP hBackImage;                        // Background image. If NULL, no image used.
    char bPercent;                             // Display percentage instead of text
    char bDirection;                           // Block size
    char bLocked;                              // Control doesnt allow user changes
    LOGFONT lfFont;                            // Font style
} TSLDR_DATA;

#define SLDR_USERDATA 0

void sldRedraw(TSLDR_DATA* psd)
{
    RedrawWindow(psd->hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE);
}

void sldMoveIncrement(TSLDR_DATA* psd, bool redraw)
{
    // bounds check
    if (psd->lPos > psd->lMax)
        psd->lPos = psd->lMax;
    else if (psd->lPos < psd->lMin)
        psd->lPos = psd->lMin;

    if (redraw)
        sldRedraw(psd);

    // notify parent
    SendMessage(GetParent(psd->hwnd), (psd->bDirection == SLS_VERTICAL ? WM_VSCROLL : WM_HSCROLL), psd->lPos, (LPARAM)psd->hwnd);
}

void sldMoveWithMouse(TSLDR_DATA* psd)
{
    auto rc = RECT();
    auto pt = POINT();
    GetWindowRect(psd->hwnd, &rc);
    GetCursorPos(&pt);
    ScreenToClient(psd->hwnd, &pt);

    if (pt.x < 0) pt.x = 0;
    if (pt.y < 0) pt.y = 0;

    int range = (psd->lMax - (psd->lMin-1));

    // Change position according to horizontal/vertical mode...
    int prevPos = psd->lPos;
    if (psd->bDirection == SLS_VERTICAL)
    {
        // Avoid divide-by-zero
        float fStep = ((float)(rc.bottom-rc.top) / (float)range);
        if (fStep > 0) psd->lPos = (int)(pt.y / fStep); else psd->lPos = 0;
        psd->lPos = psd->lMax-psd->lPos;
    }
    else
    {
        // Avoid divide-by-zero
        float fStep = ((float)(rc.right-rc.left) / (float)range);
        if (fStep > 0) psd->lPos = (int)(pt.x / fStep); else psd->lPos = 0;
    }

    // bounds check
    if (psd->lPos > psd->lMax)
        psd->lPos = psd->lMax;
    else if (psd->lPos < psd->lMin)
        psd->lPos = psd->lMin;

    if (psd->lPos != prevPos)
        sldRedraw(psd);

    // notify parent
    SendMessage(GetParent(psd->hwnd), (psd->bDirection == SLS_VERTICAL ? WM_VSCROLL : WM_HSCROLL), psd->lPos, (LPARAM)psd->hwnd);
}


//------------------------------------------------------------------------------
// Window procedure for the custom control.
//------------------------------------------------------------------------------
LRESULT WINAPI sldWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    switch (wMsg)
    {
    case WM_CREATE:
    {
        auto psd = new TSLDR_DATA();

        SetWindowLongPtr(hwnd, SLDR_USERDATA, (LONG_PTR)psd);
        psd->hwnd = hwnd;

        SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &psd->lfFont, 0);

        GetWindowText(hwnd, psd->szText, SLDR_MAX_TEXT);
        // Set defaults (other params are zeroed on allocation)...
        psd->clrText    = GetSysColor(COLOR_BTNTEXT);
        psd->clrBack    = GetSysColor(COLOR_BTNFACE);
        psd->clrBar     = GetSysColor(COLOR_HIGHLIGHT);
        psd->lMax       = 100;
        psd->bDirection = SLS_HORIZONTAL;
        break;
    }

    case SLM_SETBKCOLOR:
    {
        // Set back color for the control...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->clrBack = (COLORREF)lParam;
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_SETTEXTCOLOR:
    {
        // Set text color for the control...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->clrText = (COLORREF)lParam;
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_SETBARCOLOR:
    {
        // Set back color for the control...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->clrBar = (COLORREF)lParam;
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_SETIMAGE:
    {
        // Set background image for the control...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->hBackImage = (HBITMAP)lParam;
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_GETIMAGE:
    {
        // Return background image for the control...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        return (LRESULT)psd->hBackImage;
    }

    case SLM_SETRANGE:
    {
        // Set min\max values (wParam is typecasted)...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->lMin = (int)wParam;
        psd->lMax = (int)lParam;
        if (psd->lMin > psd->lMax)
        {
            // Swaps if in wrong order
            int prevMin = psd->lMin;
            psd->lMin = psd->lMax;
            psd->lMax = prevMin;
        }
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_GETRANGE:
    {
        // Return min or max depending on wParam value...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        return (wParam ? psd->lMin : psd->lMax);
    }

    case SLM_SETPOS:
    {
        // Set position (and send notification)...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        int prevPos = psd->lPos;
        psd->lPos = (int)wParam;
        if (psd->lPos < psd->lMin) psd->lPos = psd->lMin;
        if (psd->lPos > psd->lMax) psd->lPos = psd->lMax;
        // Update new trackbar position if changed...
        if (prevPos != psd->lPos) sldRedraw(psd);
        if (!lParam)
        {
            // notify parent
            SendMessage(GetParent(hwnd), (psd->bDirection == SLS_VERTICAL? WM_VSCROLL: WM_HSCROLL), psd->lPos, (LPARAM)hwnd);
        }
        return (LRESULT)prevPos;
    }

    case SLM_GETPOS:
    {
        // Return position...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        return (LRESULT)psd->lPos;
    }

    case SLM_SETDIRECTION:
    {
        // Set scrolling direction...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->bDirection = (lParam == SLS_VERTICAL ? SLS_VERTICAL : SLS_HORIZONTAL);
        // Redraw...
        sldRedraw(psd);
        return TRUE;
    }

    case SLM_SETLOCK:
    {
        // Set lock mode...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->bLocked = (lParam ? TRUE : FALSE);
        return TRUE;
    }

    case SLM_USEPERCENT:
    {
        // Set percentage display...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        psd->bPercent = (lParam ? TRUE : FALSE);
        sldRedraw(psd);
        return TRUE;
    }

    case WM_SETTEXT:
    {
        // Grab the text to our buffer...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        if (lParam)
            StringCchCopy(psd->szText, SLDR_MAX_TEXT, (TCHAR*)lParam);
        else
            psd->szText[0] = 0; // Reset

        // Redraw...
        sldRedraw(psd);
        break;
    }

    case WM_SETFONT:
    {
        // Save new font...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        GetObject((HANDLE)wParam, sizeof(LOGFONT), &psd->lfFont);
        // Redraw...
        if (lParam) sldRedraw(psd);
        break;
    }

    case WM_PAINT:
    {
        // Draw text and style...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        auto ps = PAINTSTRUCT();
        auto rc = RECT();
        GetClientRect(hwnd, &rc);
        BeginPaint(hwnd, &ps);
        HDC hMemDC = CreateCompatibleDC(ps.hdc);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, CreateCompatibleBitmap(ps.hdc, rc.right, rc.bottom));

        if (!ps.fErase)
        {
            // Get background from other source
            // Note: background includes bar position color
            SendMessage(hwnd, WM_ERASEBKGND, (WPARAM)hMemDC, 0);
        }
        else
        {
            // Draw background...
            HBRUSH hBack = CreateSolidBrush(psd->clrBack);
            FillRect(hMemDC, &rc, hBack);
            DeleteObject(hBack);

            // Draw background image, if specified...
            if (psd->hBackImage)
            {
                auto bm = BITMAP();
                GetObject(psd->hBackImage, sizeof(BITMAP), &bm);
                if ((bm.bmWidth > 0) && (bm.bmHeight > 0))
                {
                    HDC hTmpDC = CreateCompatibleDC(ps.hdc);
                    SelectObject(hTmpDC, psd->hBackImage);
                    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hTmpDC, 0, 0, SRCCOPY);
                    DeleteDC(hTmpDC);
                }
            }

            // Draw bar...
            if (psd->bDirection == SLS_HORIZONTAL)
                rc.right = (int) ((float)psd->lPos * (float)rc.right/(float)psd->lMax);
            else
                rc.top = rc.bottom - (int) (((float)rc.bottom/(float)psd->lMax)*(float)psd->lPos);

            // Paint the bar color...
            hBack = CreateSolidBrush(psd->clrBar);
            FillRect(hMemDC, &rc, hBack);
            DeleteObject(hBack);

            //TODO: draw edge line (optional instead of bar?)
        }

        static const int bufSize = 512;
        TCHAR szText[bufSize] = {0};
        if (psd->bPercent)
        {
            int curPos = 0+(psd->lPos * (psd->lMin+psd->lMax)/100);
            StringCchPrintf(szText, bufSize, TEXT("%d%%"), curPos);
        }
        else
            StringCchCopy(szText, bufSize, psd->szText);

        if (szText[0])
        {
            GetClientRect(hwnd, &rc);
            HFONT hFont = (HFONT)SelectObject(hMemDC, CreateFontIndirect(&psd->lfFont));
            SetBkMode(hMemDC, TRANSPARENT);
            SetTextColor(hMemDC, psd->clrText);
            DrawText(hMemDC, szText, -1, &rc, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_CENTER);
            DeleteObject(SelectObject(hMemDC, hFont));
        }

        // Display the updated graphics...
        BitBlt(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, hMemDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

        // Clean up...
        DeleteObject(SelectObject(hMemDC, hOldBmp));
        DeleteDC(hMemDC);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
    {
        return FALSE;
    }

    case WM_MOUSEWHEEL:
    {
        // Fake single arrow key with mouse wheel movement...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        if (psd->bLocked)
            return 0;
        int wheelDir = (int)(short)HIWORD(wParam); // Works
        psd->lPos += (wheelDir > 0 ? 1 : -1);
        sldMoveIncrement(psd, true);
        break;
    }

    case WM_KEYDOWN:
    {
        // Trap control keys if enabled...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));

        if (psd->bLocked)
            return 0;

        int prevPos = psd->lPos;
        // Track keys according to horizontal/vertical mode...
        if (psd->bDirection == SLS_VERTICAL)
        {
            switch (wParam)
            {
            case VK_UP: case VK_LEFT: psd->lPos++; break;
            case VK_DOWN: case VK_RIGHT: psd->lPos--; break;
            case VK_HOME: psd->lPos = psd->lMax; break;
            case VK_END: psd->lPos = psd->lMin; break;
            case VK_NEXT: psd->lPos = psd->lPos - 10; break;// Page down
            case VK_PRIOR: psd->lPos = psd->lPos + 10; break;// Page up
            default: return 0;
            }
        }
        else
        {
            switch (wParam)
            {
            case VK_LEFT: case VK_UP: psd->lPos--; break;
            case VK_RIGHT: case VK_DOWN: psd->lPos++; break;
            case VK_HOME: psd->lPos = psd->lMin; break;
            case VK_END: psd->lPos = psd->lMax; break;
            case VK_NEXT: psd->lPos = psd->lPos + 10; break; // Page down
            case VK_PRIOR: psd->lPos = psd->lPos - 10; break; // Page up
            default: return 0;
            }
        }
        sldMoveIncrement(psd, prevPos != psd->lPos);
        break;
    }

    case WM_GETDLGCODE:
    {
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        if (!psd->bLocked)
            return DLGC_WANTARROWS;
        break;
    }

    case WM_LBUTTONDOWN:
    {
        // Left mouse button down...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));

        // Always send notification even if locked...
        SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), STN_CLICKED), (LPARAM)hwnd);

        // Track mouse if not locked...
        if (psd->bLocked)
            return 0;
        SetFocus(hwnd);
        SetCapture(hwnd);
        sldMoveWithMouse(psd);
    }
    case WM_MOUSEMOVE:
    {
        // Traps mouse movement
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        if (psd->bLocked)
            return 0;

        if (GetCapture() != hwnd)
            return 0;

        sldMoveWithMouse(psd);

        break;
    }
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
    {
        // Repaint control when focus changes...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        sldRedraw(psd);
        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    {
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        if (!psd->bLocked) ReleaseCapture();
        break;
    }
    case WM_DESTROY:
    {
        // Free control//s data block...
        auto psd = reinterpret_cast<TSLDR_DATA*>(GetWindowLongPtr(hwnd, SLDR_USERDATA));
        delete psd;
        SetWindowLongPtr(hwnd, SLDR_USERDATA, 0);
        break;
    }
    }

    return DefWindowProc(hwnd, wMsg, wParam, lParam);
}

//------------------------------------------------------------------------------
// Registers the library for use.
//------------------------------------------------------------------------------
int sldRegister()
{
    static bool init = false;
    if (init)
        return TRUE;

    WNDCLASS wc = WNDCLASS();

    TCHAR szClass[64];
    StringCchCopy(szClass, 64, TEXT(SLDR_CLASS));
    wc.lpfnWndProc = &sldWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.cbWndExtra = sizeof(LPVOID); // Correct
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszClassName = szClass;
    if (!RegisterClass(&wc))
        return FALSE;

    init = true;
    return TRUE;
}

//------------------------------------------------------------------------------
// Unregisters the control class.
//------------------------------------------------------------------------------
int sldUnregister()
{
    return UnregisterClass(TEXT(SLDR_CLASS), GetModuleHandle(NULL));
}
