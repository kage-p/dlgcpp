#pragma once

#include "dlgcpp/defs.h"
#include <string>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    // utf8 to utf16
    extern std::wstring toWide(const std::string& text);
    // utf16 to utf8
    extern std::string toBytes(const wchar_t* str);
    // units to pixels;
    extern Position toPixels(HWND hwnd, const Position& p, bool client = true);
    extern void toPixels(HWND hwnd, Point& point, bool client = true);
    extern void toPixels(HWND hwnd, Size& point, bool client = true);
    // pixels to units
    extern Position toUnits(HWND hwnd, const Position& p);
    extern void toUnits(HWND hwnd, Point& point);
    extern void toUnits(HWND hwnd, Size& point);
    // font from string
    extern HFONT makeFont(const Font& font);
    // load an image
    extern HANDLE loadImage(const ImageSource& image, Size& sizePx);
}
