#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    // load an image
    extern HANDLE loadImage(
        const ImageSource& image,
        Size& sizePx,
        dlgcpp::Color maskColor = Color::None);
}
