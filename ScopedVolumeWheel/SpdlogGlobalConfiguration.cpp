#include "SpdlogGlobalConfiguration.h"
#include "ApplicationPaths.h"
#include "Win32Exception.h"
#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <PathCch.h>
#include <ShlObj.h>
#include <Windows.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <stdexcept>

void SpdlogGlobalConfiguration::configureDebug()
{
    spdlog::set_level(spdlog::level::debug);
}

void SpdlogGlobalConfiguration::configure()
{
    auto consoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_st>();
    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(logPath(), LOG_FILE_MAX_SIZE, 5, true);

    auto distSink = std::make_shared<spdlog::sinks::dist_sink_st>();
    distSink->add_sink(consoleSink);
    distSink->add_sink(fileSink);

    auto logger = std::make_shared<spdlog::logger>("main", distSink);
    logger->flush_on(spdlog::level::err);

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
}

std::wstring SpdlogGlobalConfiguration::logPath()
{
    std::wstring storagePath = ApplicationPaths::getStoragePath();

    PWSTR filePath = NULL;
    const HRESULT result = PathAllocCombine(storagePath.c_str(), L"logs\\log.txt", PATHCCH_NONE, &filePath);
    throwWin32ExceptionIfError("PathAllocCombine", result);

    std::wstring filePathString = std::wstring(filePath);
    CoTaskMemFree(filePath);

    return filePathString;
}
