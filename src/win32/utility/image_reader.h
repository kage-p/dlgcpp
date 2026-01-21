#pragma once

#include "dlgcpp/defs.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    class ImageReader
    {
    public:
        // load an image
        static HANDLE load(
            const ImageSource& image,
            Size& sizePx,
            dlgcpp::Color maskColor = Color::None);

    private:
        static HBITMAP loadWithGDIPlus(
            const ImageSource& image,
            const dlgcpp::Size& desiredSize,
            dlgcpp::Color maskColor);
    };
}
