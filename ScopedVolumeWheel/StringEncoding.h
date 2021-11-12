#pragma once
#include <string>
class StringEncoding {
public:
    static std::string fromWideChar(const std::wstring& str);
    static std::string fromWideChar(const wchar_t* str);
    static std::wstring toWideChar(const std::string& str);
    static std::wstring toWideChar(const char* str);
};
