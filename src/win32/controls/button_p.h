#pragma once

#include "dlgcpp/controls/button.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct button_props
        {
            HorizontalAlign horzAlign = HorizontalAlign::Center;
            VerticalAlign vertAlign = VerticalAlign::Center;
        };
    }
}
