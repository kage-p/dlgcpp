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

Position dlgcpp::toPixels(HWND hwnd, const Position& p, bool menu)
{
    if (hwnd == NULL)
        return Position();

    auto rc = RECT();
    SetRect(&rc,
            p._x,
            p._y,
            p._x + p._cx,
            p._y + p._cy);

    MapDialogRect(hwnd, &rc);

    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    AdjustWindowRect(&rc, style, menu);

    return Position{rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top};
}

Position dlgcpp::toUnits(HWND hwnd, const Position& p)
{
    if (hwnd == NULL)
        return Position();

    static const int multiplier = 1000;
    RECT rc = {0,0,multiplier,multiplier};
    MapDialogRect(hwnd, &rc);

    // translate using mapped value
    Position r;
    r._x = p._x * float(multiplier) / float(rc.right);
    r._y = p._y * float(multiplier) / float(rc.bottom);
    r._cx = p._cx * float(multiplier) / float(rc.right);
    r._cy = p._cy * float(multiplier) / float(rc.bottom);

    return r;
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
