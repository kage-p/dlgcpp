#pragma once

#include "dlgcpp/controls/listbox.h"
#include "../event_p.h"
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct listbox_props
        {
            bool highlight = true;
            bool multiselect = false;
            bool sorted = true;
            std::vector<std::string> items;
            int currentIndex = -1;
            std::vector<int> currentIndexes;
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl> selCancelEvent;
        };
    }
}
