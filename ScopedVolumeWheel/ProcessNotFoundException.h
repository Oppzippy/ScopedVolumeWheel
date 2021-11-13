#pragma once
#include "ExceptionWithLocation.h"
#include <Windows.h>
#include <string>

class ProcessNotFoundException : public ExceptionWithLocation {
public:
    ProcessNotFoundException(const char* file, int line, DWORD processId);
    virtual const char* what() const;

private:
    std::string message;
};
