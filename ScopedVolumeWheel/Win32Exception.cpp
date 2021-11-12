#include "Win32Exception.h"
#include "StringEncoding.h"

Win32Exception::Win32Exception(const char* file, int line, const std::string& cause, const HRESULT errorCode)
    : ExceptionWithLocation(file, line)
{
    this->createMessage(cause, errorCode);
}

Win32Exception::Win32Exception(const char* file, int line, const std::string& cause, const DWORD errorCode)
    : ExceptionWithLocation(file, line)
{
    this->createMessage(cause, errorCode);

    this->message += ": ";

    LPVOID errorMessageBuf = NULL;
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, NULL, (LPTSTR)&errorMessageBuf, 1024, NULL)) {
        this->message += StringEncoding::fromWideChar((LPCTSTR)errorMessageBuf);
        LocalFree(errorMessageBuf);
    } else {
        DWORD errorCode = GetLastError();
        this->message += "FormatMessage failed with error code ";
        this->message += std::to_string(errorCode);
    }
}

void Win32Exception::createMessage(const std::string& cause, const DWORD errorCode)
{
    this->message = ExceptionWithLocation::what();
    this->message += " ";
    this->message += "Error code ";
    this->message += std::to_string(errorCode);
    this->message += " in ";
    this->message += cause;
}

const char* Win32Exception::what() const
{
    return this->message.c_str();
}
