#pragma once

#include "dlgcpp/controls/listbox.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct listbox_props
        {
            Event<> selChangedEvent;
            Event<> selCancelEvent;
        };
    }
}
