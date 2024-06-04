#pragma once

#include "dlgcpp/controls/textbox.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        struct tb_props
        {
            bool readOnly = false;
            bool multiline = false;
            bool wrapText = false;
        };
    }
}
