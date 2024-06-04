#pragma once

#include "image.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct img_props
        {
            ImageSource image;
            bool autoSize = false;
        };

        struct img_state
        {
            HGDIOBJ hImage = NULL;
        };
    }
}
