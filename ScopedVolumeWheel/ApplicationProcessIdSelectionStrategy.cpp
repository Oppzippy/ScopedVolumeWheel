#include "ApplicationProcessIdSelectionStrategy.h"
#include <Psapi.h>
#include <optional>
#include "Win32Exception.h"

ApplicationProcessIdSelectionStrategy::ApplicationProcessIdSelectionStrategy(std::wstring applicationName)
{
	this->applicationName = applicationName;
}

DWORD ApplicationProcessIdSelectionStrategy::processId()
{
	DWORD processIds[2048]{};
	DWORD bytesNeeded = 0;
	bool success = EnumProcesses(processIds, sizeof(processIds), &bytesNeeded) != 0;
	if (!success) {
		throwWin32Exception("EnumProcesses", 0);
	}

	for (DWORD i = 0; i < bytesNeeded / sizeof(DWORD); i++) {
		HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
		if (processHandle == NULL) {
			throwWin32Exception("OpenProcess", 0);
		}
		TCHAR filePath[MAX_PATH]{};
		bool success = GetModuleFileNameEx(processHandle, NULL, filePath, MAX_PATH) != 0;
		CloseHandle(processHandle);
		if (success) {
			std::wstring fileName = this->getFileName(filePath);
			if (fileName == this->applicationName) {
				return processIds[i];
			}
		} else {
			throwWin32Exception("GetModuleFileNameEx", 0)
		}
	}

	return 0;
}

std::wstring ApplicationProcessIdSelectionStrategy::getFileNameOfProcess(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (processHandle != NULL) {
		TCHAR filePath[MAX_PATH]{};
		bool success = GetModuleFileNameEx(processHandle, NULL, filePath, MAX_PATH) != 0;
		CloseHandle(processHandle);
		if (success) {
			return this->getFileName(filePath);
		} else {
			throwWin32Exception("GetModuleFileNameEx", 0);
		}
	}
}

std::wstring ApplicationProcessIdSelectionStrategy::getFileName(const wchar_t *filePath) {
	wchar_t fileName[MAX_PATH]{};
	wchar_t fileExtension[MAX_PATH]{};
	errno_t result =  _wsplitpath_s(filePath, NULL, 0, NULL, 0, fileName, MAX_PATH, fileExtension, MAX_PATH);
	if (result != 0) {
		// TODO use a different exception
		throwWin32Exception("_wsplitpath_s", result);
	}

	std::wstring fileNameAndExtension(fileName);
	fileNameAndExtension += L".";
	fileNameAndExtension += fileExtension;
	return fileNameAndExtension;
}
