#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <chrono>
#include <optional>
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
    DWORD processId() override;
    std::optional<DWORD> getCachedProcessId() noexcept;
    void setApplicationName(const std::wstring& applicationName);

private:
    std::wstring applicationName;
    std::optional<DWORD> cachedProcessId;
    std::chrono::steady_clock::time_point cachedAt;

    std::optional<std::wstring>
    getFileNameOfProcess(DWORD processId) const;
    std::wstring getFileName(const wchar_t* filePath) const;
};
