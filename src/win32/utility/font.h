#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    /// <summary>
    /// Create a font GDI object from Font() object
    /// </summary>
    extern HFONT toGdiFont(const Font& font);

    /// <summary>
    /// Create a Font() object from a font GDI object
    /// </summary>
    extern Font toFont(HFONT gdiFont);
}
