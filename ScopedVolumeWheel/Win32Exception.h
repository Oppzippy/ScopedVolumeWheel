#pragma once
#include <Windows.h>
#include <exception>
#include <string>

class Win32Exception : public std::exception {
public:
    Win32Exception(const std::string& what, const HRESULT why, const char* file, int line);
    virtual const char* what() const;

private:
    std::string message;
};

#define throwWin32Exception(what, why) throw Win32Exception(what, why, __FILE__, __LINE__);
#define throwWin32ExceptionIfNotOk(what, why) \
    if (why != S_OK)                          \
        throwWin32Exception(what, why);