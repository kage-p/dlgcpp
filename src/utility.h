#pragma once

#include <string>

namespace dlgcpp
{
    typedef struct Position
    {
        int _x = 0;
        int _y = 0;
        int _cx = 0;
        int _cy = 0;
    } Position;

    extern std::wstring toWide(const std::string& text);
}
