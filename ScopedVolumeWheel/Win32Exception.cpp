#include "Win32Exception.h"

Win32Exception::Win32Exception(const std::string &what, const HRESULT why, const char* file, int line)
{
	this->message = file;
	this->message += ":";
	this->message += std::to_string(line);
	this->message += " ";
	this->message += what;
	this->message += ": HRESULT (";
	this->message += std::to_string(why);
	this->message += ")";
}

const char* Win32Exception::what() const throw()
{
	return this->message.c_str();
}
