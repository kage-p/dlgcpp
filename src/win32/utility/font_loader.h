#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    class FontLoader
    {
    public:
        /// <summary>
        /// Create a font GDI object from Font() object
        /// </summary>
        static HFONT toGdiFont(const Font& font);

        /// <summary>
        /// Create a Font() object from a font GDI object
        /// </summary>
        static Font toFont(HFONT gdiFont);

    private:
    };
}
