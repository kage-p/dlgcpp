#pragma once

#include "dlgcpp/controls/checkbox.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct checkbox_props
        {
            bool checked = false;
            bool autoCheck = true;
            HorizontalAlign horzAlign = HorizontalAlign::Left;
            VerticalAlign vertAlign = VerticalAlign::Center;
        };
    }
}
