#include "ApplicationPaths.h"
#include <Windows.h>
#include <ShlObj.h>
#include <PathCch.h>
#include "Win32Exception.h"

std::wstring ApplicationPaths::getStoragePath()
{
    PWSTR appDataPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath);
    throwWin32ExceptionIfNotOk("SHGetFolderPathW", result);

    PWSTR filePath = NULL;
    result = PathAllocCombine(appDataPath, L"ScopedVolumeWheel", PATHCCH_NONE, &filePath);
    throwWin32ExceptionIfNotOk("PathAllocCombine", result);
    CoTaskMemFree(appDataPath);

    std::wstring filePathString = std::wstring(filePath);
    CoTaskMemFree(filePath);

    return filePathString;
}