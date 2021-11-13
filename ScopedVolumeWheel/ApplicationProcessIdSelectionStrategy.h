#pragma once
#include "ProcessIdSelectionStrategy.h"
#include <string>

class ApplicationProcessIdSelectionStrategy : public ProcessIdSelectionStrategy {
public:
    ApplicationProcessIdSelectionStrategy(std::wstring applicationName);
    DWORD processId() const override;
    void setApplicationName(const std::wstring& applicationName);

private:
    std::wstring applicationName;
    std::wstring getFileNameOfProcess(DWORD processId) const;
    std::wstring getFileName(const wchar_t* filePath) const;
};
