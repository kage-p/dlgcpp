#pragma once

#include "dlgcpp/defs.h"
#include <string>

namespace dlgcpp
{
    class StringEncoder
    {
    public:
        // utf8 to utf16
        static std::wstring toWide(const std::string& text);
        // utf16 to utf8
        static std::string toBytes(const wchar_t* str);
    };
}
