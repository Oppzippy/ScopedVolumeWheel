#include "SpdlogGlobalConfiguration.h"
#include <memory>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <Windows.h>
#include <ShlObj.h>
#include <PathCch.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/dist_sink.h"
#include "Win32Exception.h"

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
}

std::wstring SpdlogGlobalConfiguration::logPath()
{
    PWSTR appDataPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath);
    throwWin32ExceptionIfNotOk("SHGetFolderPathW", result);

    PWSTR filePath = NULL;
    result = PathAllocCombine(appDataPath, L"ScopedVolumeWheel\\logs\\log.txt", PATHCCH_NONE, &filePath);
    throwWin32ExceptionIfNotOk("PathAllocCombine", result);
    CoTaskMemFree(appDataPath);

    std::wstring filePathString = std::wstring(filePath);
    CoTaskMemFree(filePath);

    return filePathString;
}
