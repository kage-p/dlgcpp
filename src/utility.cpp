#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;

std::wstring dlgcpp::toWide(const std::string& text)
{
#if defined(_WIN32) || defined(WIN32)
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), NULL, 0);
    if (convertResult <= 0)
        return std::wstring();

    std::wstring buf;
    buf.resize(convertResult);
    convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), &buf[0], (int)buf.size());
    if (convertResult <= 0)
        return std::wstring();
    return buf;
#else
    return std::wstring();
#endif
}
