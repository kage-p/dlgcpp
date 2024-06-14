#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <strsafe.h>

using namespace dlgcpp;

std::wstring dlgcpp::toWide(const std::string& text)
{
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), NULL, 0);
    if (convertResult <= 0)
        return std::wstring();

    std::wstring buf;
    buf.resize(convertResult);
    convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), &buf[0], (int)buf.size());
    if (convertResult <= 0)
        return std::wstring();
    return buf;
}

std::string dlgcpp::toBytes(const wchar_t* str)
{
    int convertResult = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    if (convertResult <= 0)
        return std::string();

    std::string buf;
    buf.resize(convertResult);
    convertResult = WideCharToMultiByte(CP_UTF8, 0, str, -1, &buf[0], (int)buf.size(), NULL, NULL);
    if (convertResult <= 0)
        return std::string();
    buf.pop_back();
    return buf;
}

void getFontDimensions(HFONT hFont, Size& fontDimensions)
{
    fontDimensions = Size();

    HDC hdc = CreateCompatibleDC(NULL);

    bool usingDefaultFont = (hFont == NULL);

    if (usingDefaultFont)
    {
        NONCLIENTMETRICS ncm;
        ncm.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
        hFont = CreateFontIndirect(&ncm.lfMessageFont);
        //hFont = makeFont(Font{"MS Shell Dlg",8});
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

// void getCharDimensions_Test()
// {
//     LOGFONT logfont = {-11, 0, 0, 0, 400,
//                        0, 0, 0, 0, 0, 0, 0, 0,
//                        "MS Shell Dlg 2"};
//     HFONT hFont, hOldFont;
//     HDC hdc;
//     LONG x, y, x2;
//     TEXTMETRICW tm;
//     SIZE size;
//     static const WCHAR alphabet[] = {
//                                      'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
//                                      'r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H',
//                                      'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',0};

//     hFont = CreateFontIndirect(&logfont);
//     hdc = CreateCompatibleDC(NULL);
//     hOldFont = SelectObject(hdc, hFont);

//     x = GdiGetCharDimensions(hdc, &tm, &y);
//     GetTextExtentPointW(hdc, alphabet, 52, &size);
//     x2 = (size.cx / 26 + 1) / 2;

//     ok(x == x2, "x=%ld, x2=%ld\n", x, x2);
//     ok(y == tm.tmHeight, "y = %ld, tm.tmHeight = %ld\n", y, tm.tmHeight);

//     SelectObject(hdc, hOldFont);
//     DeleteObject(hFont);
//     DeleteDC(hdc);
// }

void getFontDimensions_OLD(HFONT hFont, Size& fontDimensions)
{
    // Obtain the handle to the system font typically used in dialogs
    bool usingDefaultFont = (hFont == NULL);

    if (usingDefaultFont)
    {
        NONCLIENTMETRICS ncm;
        ncm.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
        hFont = CreateFontIndirect(&ncm.lfMessageFont);
        //hFont = makeFont(Font{"MS Shell Dlg",8});
        usingDefaultFont = true;
    }

    HDC hdc = CreateCompatibleDC(NULL);
    HGDIOBJ oldFont = SelectObject(hdc, hFont);

    auto tm = TEXTMETRIC();
    GetTextMetrics(hdc, &tm);

    SelectObject(hdc, oldFont);
    DeleteDC(hdc);
    if (usingDefaultFont)
        DeleteObject(hFont);

    fontDimensions = Size(tm.tmAveCharWidth, tm.tmHeight);
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

    return Position{rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top};
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
        int x  = MulDiv(p.x(), multiplier, rc.right);
        int y  = MulDiv(p.y(), multiplier, rc.bottom);
        int cx = MulDiv(p.width(), multiplier, rc.right);
        int cy = MulDiv(p.height(), multiplier, rc.bottom);
        return Position(x,y,cx,cy);
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
        return Position(x,y,cx,cy);
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

HFONT dlgcpp::makeFont(const Font& font)
{
    auto lf = LOGFONTW();
    StringCchCopyW(lf.lfFaceName, LF_FACESIZE, toWide(font.faceName).c_str());
    auto hdc = GetDC(NULL);
    lf.lfWeight = font.bold ? FW_BOLD : FW_NORMAL;
    lf.lfUnderline = font.underline;
    lf.lfHeight = -MulDiv(abs(font.pointSize), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(NULL, hdc);
    lf.lfCharSet = font.symbolType ? SYMBOL_CHARSET : DEFAULT_CHARSET;
    return CreateFontIndirectW(&lf);
}

HANDLE dlgcpp::loadImage(const ImageSource& image,
                         Size& sizePx)
{
    if (image.id.empty())
        return NULL;

    auto imageType = (image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);

    UINT loadFlags = LR_CREATEDIBSECTION;
    if (image.isFile)
        loadFlags |= LR_LOADFROMFILE;

    if (sizePx.empty())
        loadFlags |= LR_DEFAULTSIZE;

    auto hInstRes = GetModuleHandle(NULL);

    HANDLE hImage = LoadImage(hInstRes,
                              image.id.c_str(),
                              imageType,
                              sizePx.width(),
                              sizePx.height(),
                              loadFlags);

    sizePx = Size();
    if (hImage == NULL)
        return NULL;

    if (!image.isIcon)
    {
        BITMAP bitmap;
        if (GetObject(hImage, sizeof(BITMAP), &bitmap) == FALSE)
        {
            DeleteObject(hImage);
            return NULL;
        }
        else
        {
            sizePx = Size(bitmap.bmWidth, bitmap.bmHeight);
        }
    }
    else
    {
        ICONINFO iconInfo;
        if (GetIconInfo((HICON)hImage, &iconInfo) == FALSE)
        {
            DeleteObject(hImage);
            return NULL;
        }

        BITMAP bitmap;
        if (GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitmap))
        {
            sizePx = Size(bitmap.bmWidth, bitmap.bmHeight);
        }
        DeleteObject(iconInfo.hbmColor);
        DeleteObject(iconInfo.hbmMask);
    }
    return hImage;
}
