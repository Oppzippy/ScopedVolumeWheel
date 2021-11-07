#pragma once
#include <string>

#define SPDLOG_WCHAR_FILENAMES

class SpdlogGlobalConfiguration
{
public:
	static void configure();
private:
	static const int LOG_FILE_MAX_SIZE = 1024 * 1024 * 10; // 10MiB
	static std::wstring logPath();
};

