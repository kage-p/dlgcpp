#pragma once

#include "../event_p.h"
#include "dlgcpp/controls/trackbar.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct trackbar_props
        {
            bool vertical = false;
            int value = 0;
            std::pair<int, int> range = { 0,100 };
            int pageSize = 1;
            TrackBarTickMark tickMarks = TrackBarTickMark::Above;
            Event<ISharedControl> changedEvent;
        };
    }
}
