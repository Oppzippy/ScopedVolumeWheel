#include "StringEncoding.h"
#include "Win32Exception.h"
#include <Windows.h>
#include <memory>
#include <vector>

std::string StringEncoding::fromWideChar(const std::wstring& str)
{
    return fromWideChar(str.c_str());
}

std::string StringEncoding::fromWideChar(const wchar_t* str)
{
    const int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    throwWin32ExceptionIfNotSuccess("WideCharToMultiByte", bytesNeeded != 0);

    std::vector<char> buffer(bytesNeeded);
    const int result = WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer.data(), bytesNeeded, NULL, NULL);
    throwWin32ExceptionIfNotSuccess("WideCharToMultiByte", result != 0);

    return std::string(buffer.data());
}

std::wstring StringEncoding::toWideChar(const std::string& str)
{
    return toWideChar(str.c_str());
}

std::wstring StringEncoding::toWideChar(const char* str)
{
    const int bytesNeeded = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    throwWin32ExceptionIfNotSuccess("MultiByteToWideChar", bytesNeeded != 0);

    std::vector<wchar_t> buffer(bytesNeeded);
    const int result = MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer.data(), bytesNeeded);
    throwWin32ExceptionIfNotSuccess("MultiByteToWideChar", result != 0);

    return std::wstring(buffer.data());
}
