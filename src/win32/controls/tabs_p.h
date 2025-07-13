#pragma once

#include "dlgcpp/controls/tabs.h"
#include "../event_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct tabs_props
        {
            std::vector<ISharedTabItem> items;
            int currentIndex = -1;
            Event<ISharedControl> selChangedEvent;
        };
    }
}
