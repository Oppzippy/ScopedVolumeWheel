#pragma once
#include "ExceptionWithLocation.h"
#include <Windows.h>
#include <string>

class ProcessNotFoundException : public ExceptionWithLocation {
public:
    ProcessNotFoundException(const char* file, int line, DWORD processId);
    const char* what() const noexcept override;

private:
    std::string message;
};
