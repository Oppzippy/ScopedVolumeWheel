#include "ApplicationPaths.h"
#include "Win32Exception.h"
#include <PathCch.h>
#include <ShlObj.h>
#include <Windows.h>

std::wstring ApplicationPaths::getStoragePath()
{
    PWSTR appDataPath = NULL;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath);
    throwWin32ExceptionIfError("SHGetFolderPathW", result);

    PWSTR filePath = NULL;
    result = PathAllocCombine(appDataPath, L"ScopedVolumeWheel", PATHCCH_NONE, &filePath);
    throwWin32ExceptionIfError("PathAllocCombine", result);
    CoTaskMemFree(appDataPath);

    std::wstring filePathString = std::wstring(filePath);
    CoTaskMemFree(filePath);

    return filePathString;
}
