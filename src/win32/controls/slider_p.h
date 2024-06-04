#pragma once

#include "dlgcpp/controls/slider.h"
#include "p_slider.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct slider_props
        {
            bool vertical = false;
            int value = 0;
            std::pair<int, int> range;
            Color barColor = Color::Blue;
        };
    }
}
