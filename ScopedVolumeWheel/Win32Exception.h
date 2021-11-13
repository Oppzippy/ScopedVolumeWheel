#pragma once
#include "ExceptionWithLocation.h"
#include <Windows.h>
#include <exception>
#include <string>

class Win32Exception : public ExceptionWithLocation {
public:
    Win32Exception(const char* file, int line, const std::string& cause, const HRESULT errorCode);
    // errorCode is from GetLastError()
    Win32Exception(const char* file, int line, const std::string&, const DWORD errorCode);
    const char* what() const noexcept override;

private:
    std::string message;

    void createMessage(const std::string& cause, const DWORD errorCode);
};

#define win32Exception(what, why) exceptionWithLocation(Win32Exception, what, why);
#define throwWin32ExceptionIfError(what, errorCode) \
    if (errorCode != S_OK)                          \
        throw win32Exception(what, errorCode);
#define throwWin32ExceptionIfNotSuccess(what, isSuccess) \
    if (!isSuccess)                                      \
        throw win32Exception(what, GetLastError());
