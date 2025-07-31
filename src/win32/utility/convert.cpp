#include "convert.h"

using namespace dlgcpp;

Convert::Convert(HWND hwnd) : _hwnd(hwnd)
{
    if (_hwnd != HWND_DESKTOP &&
        !IsWindow(_hwnd))
    {
        _hwnd = HWND_DESKTOP;
    }
}

Position Convert::toPixels(const Position& p, bool client)
{
    auto rc = RECT();

    if (_hwnd != NULL)
    {
        SetRect(&rc,
            p.x(),
            p.y(),
            p.x() + p.width(),
            p.y() + p.height());

        if (MapDialogRect(_hwnd, &rc) == FALSE)
        {
            // error
            return Position();
        }

        if (!client)
        {
            // TODO: if we had IDialog::styles then we don't need HWND
            auto menu = GetMenu(_hwnd) != NULL;
            DWORD style = GetWindowLong(_hwnd, GWL_STYLE);
            DWORD exStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);
            AdjustWindowRectEx(&rc, style, menu, exStyle);
        }
    }
    else
    {
        if (!client)
        {
            // cant do window position
            return Position();
        }
        // map units to pixels without a dialog box
        Size fontDimensions;
        getFontDimensions(NULL, fontDimensions);
        rc.left = MulDiv(p.x(), fontDimensions.width(), 4);
        rc.top = MulDiv(p.y(), fontDimensions.height(), 8);
        rc.right = rc.left + MulDiv(p.width(), fontDimensions.width(), 4);
        rc.bottom = rc.top + MulDiv(p.height(), fontDimensions.height(), 8);
    }

    return Position{ rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Position Convert::toUnits(const Position& p, bool client)
{
    if (_hwnd != NULL)
    {
        RECT map = { 0, 0, 1000, 1000 };
        if (!MapDialogRect(_hwnd, &map))
        {
            // error
            return Position();
        }

        int x = p.x();
        int y = p.y();
        int cx = p.width();
        int cy = p.height();

        if (!client)
        {
            // Simulate client rect at (0,0) to get border sizes
            RECT nc = { 0, 0, cx, cy };
            DWORD style = GetWindowLong(_hwnd, GWL_STYLE);
            DWORD exStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);
            BOOL hasMenu = GetMenu(_hwnd) != NULL;

            if (!AdjustWindowRectEx(&nc, style, hasMenu, exStyle))
            {
                // error
                return Position();
            }

            // Calculate border offsets
            int borderLeft = -nc.left;
            int borderTop = -nc.top;
            int borderRight = nc.right - cx;
            int borderBottom = nc.bottom - cy;

            // Adjust origin and size
            x += borderLeft;
            y += borderTop;
            cx -= (borderLeft + borderRight);
            cy -= (borderTop + borderBottom);
        }

        // Convert pixel coords to dialog units
        int unitsX = MulDiv(x, 1000, map.right);
        int unitsY = MulDiv(y, 1000, map.bottom);
        int unitsCX = MulDiv(cx, 1000, map.right);
        int unitsCY = MulDiv(cy, 1000, map.bottom);
        return Position(unitsX, unitsY, unitsCX, unitsCY);
    }
    else
    {
        // map pixels to units without a dialog box
        Size fontDimensions;
        getFontDimensions(NULL, fontDimensions);
        int x = MulDiv(p.x(), 4, fontDimensions.width());
        int y = MulDiv(p.y(), 8, fontDimensions.height());
        int cx = MulDiv(p.width(), 4, fontDimensions.width());
        int cy = MulDiv(p.height(), 8, fontDimensions.height());
        return Position(x, y, cx, cy);
    }
}

void Convert::getFontDimensions(HFONT hFont, Size& fontDimensions)
{
    fontDimensions = Size();

    HDC hdc = CreateCompatibleDC(NULL);

    bool usingDefaultFont = (hFont == NULL);

    if (usingDefaultFont)
    {
        auto ncm = NONCLIENTMETRICS();
        ncm.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
        hFont = CreateFontIndirect(&ncm.lfMessageFont);
        usingDefaultFont = true;
    }
    HGDIOBJ hOldFont = SelectObject(hdc, hFont);

    SIZE sz;
    TEXTMETRICW tm;
    static const WCHAR alphabet[] =
    {
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
        'r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',0
    };

    if (GetTextMetricsW(hdc, &tm) &&
        GetTextExtentPointW(hdc, alphabet, 52, &sz))
    {
        fontDimensions = Size((sz.cx / 26 + 1) / 2,
            tm.tmHeight);
    }

    SelectObject(hdc, hOldFont);
    DeleteDC(hdc);
    if (usingDefaultFont)
        DeleteObject(hFont);
}
