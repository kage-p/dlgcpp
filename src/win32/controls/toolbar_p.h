#pragma once

#include "../event_p.h"
#include "dlgcpp/controls/toolbar.h"
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
            Size buttonSize = Size(24, 24);
            Size imageSize = Size(20, 20);
            std::vector<ISharedToolBarItem> items;
            std::wstring tooltipBuf;
        };
    }
}
