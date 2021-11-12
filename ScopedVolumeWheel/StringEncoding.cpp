#include "StringEncoding.h"
#include <Windows.h>
#include <memory>

std::string StringEncoding::fromWideChar(const std::wstring& str)
{
    return fromWideChar(str.c_str());
}

std::string StringEncoding::fromWideChar(const wchar_t* str)
{
    int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> buffer(new char[bytesNeeded]);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer.get(), bytesNeeded, NULL, NULL);

    return std::string(buffer.get());
}

std::wstring StringEncoding::toWideChar(const std::string& str)
{
    return toWideChar(str.c_str());
}

std::wstring StringEncoding::toWideChar(const char* str)
{
    int bytesNeeded = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    std::unique_ptr<wchar_t[]> buffer(new wchar_t[bytesNeeded]);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer.get(), bytesNeeded);

    return std::wstring(buffer.get());
}
