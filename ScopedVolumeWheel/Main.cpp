#include "ScopedVolumeWheel.h"
#include "SpdlogGlobalConfiguration.h"
#include <WinUser.h>
#include <Windows.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <stdexcept>

DWORD mainThreadId;

BOOL WINAPI consoleCtrlHandler(DWORD dwCtrlType)
{
    switch (dwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
        const BOOL success = PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
        if (success == 0) {
            spdlog::critical("PostThreadMessage failed to send WM_QUIT to main thread with error code {}: ", GetLastError());
        }
        return true;
    }
    return false;
}

int main()
{
    mainThreadId = GetCurrentThreadId();
    SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);

    try {
        SpdlogGlobalConfiguration::configure();
#ifdef DEBUG
        SpdlogGlobalConfiguration::configureDebug();
#endif

        HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        if (result != S_OK) {
            spdlog::critical("CoInitializeEx failed with code {}", result);
            return 1;
        }

        ScopedVolumeWheel application;
        application.run();

        CoUninitialize();
    } catch (const std::exception& e) {
        spdlog::critical("Unexpected exception: {}", e.what());
        std::string errorMessage = std::string("Unexpected exception: ") + e.what();
        std::wstring errorMessageW = std::wstring(errorMessage.begin(), errorMessage.end());
        const int result = MessageBox(
            NULL,
            errorMessageW.c_str(),
            L"ScopedVolumeWheel crashed!",
            MB_OK);
        if (result == 0) {
            spdlog::error("MessageBox failed with error code {}", GetLastError());
        }
        return 1;
    }

    return 0;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    return main();
}
