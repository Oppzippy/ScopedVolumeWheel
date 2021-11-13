#include "ProcessNotFoundException.h"

ProcessNotFoundException::ProcessNotFoundException(const char* file, int line, DWORD processId)
    : ExceptionWithLocation(file, line)
{
    this->message = ExceptionWithLocation::what();
    this->message += " Process id ";
    this->message += std::to_string(processId);
    this->message += " not found.";
}

const char* ProcessNotFoundException::what() const
{
    return this->message.c_str();
}
