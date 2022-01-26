#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <chrono>
#include <optional>
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    ApplicationProcessIdSelectionStrategy();
    ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
    std::optional<ProcessSelection> processId() override;
    std::optional<ProcessSelection> getCachedProcessId() noexcept;
    void setApplicationName(const std::wstring& applicationName);
    void setApplicationNameFromProcessId(DWORD processId);

private:
    std::wstring applicationName;
    std::optional<ProcessSelection> cachedProcessId;
    std::chrono::steady_clock::time_point cachedAt;

    std::optional<std::wstring>
    getFileNameOfProcess(DWORD processId) const;
    std::wstring getFileName(const wchar_t* filePath) const;
};
