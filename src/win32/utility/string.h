#pragma once

#include "dlgcpp/defs.h"
#include <string>

namespace dlgcpp
{
    // utf8 to utf16
    extern std::wstring toWide(const std::string& text);
    // utf16 to utf8
    extern std::string toBytes(const wchar_t* str);
}
