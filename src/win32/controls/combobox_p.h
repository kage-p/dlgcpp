#pragma once

#include "dlgcpp/controls/combobox.h"
#include "../event_p.h"
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct combobox_props
        {
            bool editable = true;
            bool dropDown = false;
            bool sorted = true;
            std::vector<std::string> items;
            int currentIndex = -1;            
            Event<ISharedControl> selChangedEvent;
            Event<ISharedControl> selCancelEvent;
            Event<ISharedControl> listCloseEvent;
            Event<ISharedControl> listOpenEvent;
            Event<ISharedControl> textChangedEvent;
        };
    }
}
