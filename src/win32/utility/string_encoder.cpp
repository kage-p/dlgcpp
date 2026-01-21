#include "string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace dlgcpp;

std::wstring StringEncoder::toWide(const std::string& text)
{
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), NULL, 0);
    if (convertResult <= 0)
        return std::wstring();

    std::wstring buf;
    buf.resize(convertResult);
    convertResult = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), &buf[0], (int)buf.size());
    if (convertResult <= 0)
        return std::wstring();
    return buf;
}

std::string StringEncoder::toBytes(const wchar_t* str)
{
    int convertResult = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    if (convertResult <= 0)
        return std::string();

    std::string buf;
    buf.resize(convertResult);
    convertResult = WideCharToMultiByte(CP_UTF8, 0, str, -1, &buf[0], (int)buf.size(), NULL, NULL);
    if (convertResult <= 0)
        return std::string();
    buf.pop_back();
    return buf;
}