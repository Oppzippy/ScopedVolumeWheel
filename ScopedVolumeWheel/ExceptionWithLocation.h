#pragma once
#include <stdexcept>
#include <string>

class ExceptionWithLocation : std::exception {
public:
    ExceptionWithLocation(const char* file, int line);
    ExceptionWithLocation(const char* file, int line, const char* message);
    const char* what() const noexcept override;

private:
    std::string message;
};

#define exceptionWithLocation(exception, ...) exception(__FILE__, __LINE__, __VA_ARGS__);
