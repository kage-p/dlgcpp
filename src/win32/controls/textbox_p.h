#pragma once

#include "dlgcpp/controls/textbox.h"
#include "../event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct textbox_props
        {
            int maxChars = 0;
            bool password = false;
            bool readOnly = false;
            bool multiline = false;
            bool wrapText = false;

            HorizontalAlign horzAlign = HorizontalAlign::Left;

            Event<ISharedControl> changedEvent;
        };
    }
}
