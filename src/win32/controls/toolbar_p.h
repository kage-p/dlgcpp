#pragma once

#include "dlgcpp/controls/toolbar.h"
#include "../event_p.h"
#include <vector>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        struct toolbar_props
        {
            Size buttonSize;
            std::vector<ISharedToolBarItem> items;
        };
    }
}
