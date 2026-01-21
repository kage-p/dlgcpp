#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    class Convert
    {
    public:
        Convert(HWND hwnd = HWND_DESKTOP);

        // units to pixels;
        Position toPixels(const Position& p, bool client = true);
        // void toPixels(Point& point, bool client = true);
        // void toPixels(Size& point, bool client = true);

         // pixels to units
        Position toUnits(const Position& p, bool client = true);
        //void toUnits(Point& point, bool client = true);
        //void toUnits(Size& point, bool client = true);

    private:
        HWND _hwnd = HWND_DESKTOP;
        static void getFontDimensions(HFONT hFont, Size& fontDimensions);
    };
}
