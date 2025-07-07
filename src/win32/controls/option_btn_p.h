#pragma once

#include "dlgcpp/controls/option_btn.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct optionbtn_props
        {
            bool checked = false;
            HorizontalAlign horzAlign = HorizontalAlign::Left;
            VerticalAlign vertAlign = VerticalAlign::Center;
        };
    }
}
