#pragma once

#include <string>

namespace dlgcpp
{
    typedef struct Font
    {
        std::string family;
        int pointSize = 8;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool symbolType = false;
    } Font;
}