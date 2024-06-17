#pragma once

#include "dlgcpp/controls/label.h"

namespace dlgcpp
{
    namespace controls
    {
        struct lbl_props
        {
            bool autoSize = false;
            bool elipsis = false;
            bool clickable = true;
            HorizontalAlign horzAlign = HorizontalAlign::Left;
            VerticalAlign vertAlign = VerticalAlign::Top;
        };
    }
}
