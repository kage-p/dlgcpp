#pragma once

#include "dlgcpp/controls/progress.h"
#include "../event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct progress_props
        {
            bool vertical = false;
            int value = 0;
            std::pair<int, int> range = {0,100};
            Event<ISharedControl> changedEvent;
        };
    }
}
