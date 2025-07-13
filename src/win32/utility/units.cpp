#include "units.h"

using namespace dlgcpp;

void getFontDimensions(HFONT hFont, Size& fontDimensions)
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

Position dlgcpp::toPixels(HWND hwnd, const Position& p, bool client)
{
    auto rc = RECT();

    if (hwnd != NULL)
    {
        SetRect(&rc,
            p.x(),
            p.y(),
            p.x() + p.width(),
            p.y() + p.height());

        if (MapDialogRect(hwnd, &rc) == FALSE)
        {
            // error
            return Position();
        }

        if (!client)
        {
            // TODO: if we had IDialog::styles then we don't need HWND
            auto menu = GetMenu(hwnd) != NULL;
            DWORD style = GetWindowLong(hwnd, GWL_STYLE);
            DWORD exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
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

void dlgcpp::toPixels(HWND hwnd, Point& point, bool client)
{
    auto p = toPixels(hwnd, Position(point), client);
    point = p.point();
}

void dlgcpp::toPixels(HWND hwnd, Size& size, bool client)
{
    auto p = toPixels(hwnd, Position(size), client);
    size = p.size();
}

Position dlgcpp::toUnits(HWND hwnd, const Position& p)
{
    if (hwnd != NULL)
    {
        // map a fixed rectangle to pixels using the dialog box
        static const int multiplier = 1000;
        auto rc = RECT();
        SetRect(&rc, 0, 0, multiplier, multiplier);
        if (MapDialogRect(hwnd, &rc) == FALSE)
        {
            // error
            return Position();
        }

        // translate using mapped value
        int x = MulDiv(p.x(), multiplier, rc.right);
        int y = MulDiv(p.y(), multiplier, rc.bottom);
        int cx = MulDiv(p.width(), multiplier, rc.right);
        int cy = MulDiv(p.height(), multiplier, rc.bottom);
        return Position(x, y, cx, cy);
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

void dlgcpp::toUnits(HWND hwnd, Point& point)
{
    auto p = toUnits(hwnd, Position(point));
    point = p.point();
}

void dlgcpp::toUnits(HWND hwnd, Size& size)
{
    auto p = toUnits(hwnd, Position(size));
    size = p.size();
}
