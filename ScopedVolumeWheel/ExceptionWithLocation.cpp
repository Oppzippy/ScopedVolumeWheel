#include "ExceptionWithLocation.h"

ExceptionWithLocation::ExceptionWithLocation(const char* file, int line)
{
    this->message += file;
    this->message += ":";
    this->message += std::to_string(line);
}

ExceptionWithLocation::ExceptionWithLocation(const char* file, int line, const char* message)
    : ExceptionWithLocation(file, line)
{
    this->message += " ";
    this->message += message;
}

const char* ExceptionWithLocation::what() const
{
    return this->message.c_str();
}
