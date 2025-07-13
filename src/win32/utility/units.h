#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    // units to pixels;
    extern Position toPixels(HWND hwnd, const Position& p, bool client = true);
    extern void toPixels(HWND hwnd, Point& point, bool client = true);
    extern void toPixels(HWND hwnd, Size& point, bool client = true);
    // pixels to units
    extern Position toUnits(HWND hwnd, const Position& p);
    extern void toUnits(HWND hwnd, Point& point);
    extern void toUnits(HWND hwnd, Size& point);
}
