#include "ApplicationProcessIdSelectionStrategy.h"
#include "Win32Exception.h"
#include <Psapi.h>
#include <optional>

ApplicationProcessIdSelectionStrategy::ApplicationProcessIdSelectionStrategy()
{
}

ApplicationProcessIdSelectionStrategy::ApplicationProcessIdSelectionStrategy(std::wstring applicationName)
{
    this->applicationName = applicationName;
}

std::optional<ProcessSelection> ApplicationProcessIdSelectionStrategy::processId()
{
    std::optional<ProcessSelection> cachedProcessId = this->getCachedProcessId();
    if (cachedProcessId.has_value()) {
        return cachedProcessId.value();
    }

    DWORD processIds[2048] {};
    DWORD bytesNeeded = 0;
    const BOOL success = EnumProcesses(processIds, sizeof(processIds), &bytesNeeded);
    throwWin32ExceptionIfNotSuccess("EnumProcesses", success);

    std::set<DWORD> filteredProcessIds;
    for (DWORD i = 0; i < bytesNeeded / sizeof(DWORD); i++) {
        const DWORD processId = processIds[i];
        if (processId == 0) {
            continue;
        }
        try {
            std::optional<std::wstring> fileName = this->getFileNameOfProcess(processId);
            if (fileName.has_value() && fileName.value() == this->applicationName) {
                filteredProcessIds.insert(processId);
            }
        } catch (const Win32Exception& e) {
            throw e;
        }
    }

    if (!filteredProcessIds.empty()) {
        ProcessSelection selection {
            .preferredProcess = 0,
            .fallbackProcesses = filteredProcessIds,
        };
        this->cachedProcessId = std::make_optional<ProcessSelection>(selection);
        this->cachedAt = std::chrono::steady_clock::now();
        return selection;
    }

    return std::nullopt;
}

std::optional<ProcessSelection> ApplicationProcessIdSelectionStrategy::getCachedProcessId() noexcept
{
    if (this->cachedProcessId.has_value() && std::chrono::steady_clock::now() - this->cachedAt < std::chrono::seconds(1)) {
        return this->cachedProcessId.value();
    }
    return std::nullopt;
}

void ApplicationProcessIdSelectionStrategy::setApplicationName(const std::wstring& applicationName)
{
    this->applicationName = applicationName;
    this->cachedProcessId = std::nullopt;
}

void ApplicationProcessIdSelectionStrategy::setApplicationNameFromProcessId(DWORD processId)
{
    std::optional<std::wstring> name = this->getFileNameOfProcess(processId);
    this->setApplicationName(name.value_or(L""));
}

std::optional<std::wstring> ApplicationProcessIdSelectionStrategy::getFileNameOfProcess(DWORD processId) const
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle == NULL) {
        if (GetLastError() == ERROR_INVALID_PARAMETER || GetLastError() == ERROR_ACCESS_DENIED) {
            return std::nullopt;
        }
    }
    throwWin32ExceptionIfNotSuccess("OpenProcess", processHandle != NULL);

    TCHAR filePath[MAX_PATH] {};
    bool success = GetModuleFileNameEx(processHandle, NULL, filePath, MAX_PATH) != 0;
    if (!success) {
        // Try to close but throwing if it does isn't a priority
        const DWORD lastError = GetLastError();
        switch (lastError) {
        case ERROR_ACCESS_DENIED:
            success = CloseHandle(processHandle);
            throwWin32ExceptionIfNotSuccess("CloseHandle", success);
            return std::nullopt;
        default:
            CloseHandle(processHandle);
            throw win32Exception("GetModuleFileNameEx", GetLastError());
        }
    }

    success = CloseHandle(processHandle);
    throwWin32ExceptionIfNotSuccess("CloseHandle", success);

    return this->getFileName(filePath);
}

std::wstring ApplicationProcessIdSelectionStrategy::getFileName(const wchar_t* filePath) const
{
    wchar_t fileName[MAX_PATH] {};
    wchar_t fileExtension[MAX_PATH] {};
    const errno_t result = _wsplitpath_s(filePath, NULL, 0, NULL, 0, fileName, MAX_PATH, fileExtension, MAX_PATH);
    if (result != 0) {
        std::string errorMessage = "_wsplitpath_s failed : ";
        errorMessage += std::to_string(result);
        throw exceptionWithLocation(ExceptionWithLocation, errorMessage.c_str());
    }

    std::wstring fileNameAndExtension(fileName);
    fileNameAndExtension += fileExtension;
    return fileNameAndExtension;
}
