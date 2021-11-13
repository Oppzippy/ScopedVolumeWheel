#include "ScopedVolumeWheel.h"
#include "SpdlogGlobalConfiguration.h"
#include "spdlog/spdlog.h"
#include <WinUser.h>
#include <Windows.h>
#include <iostream>
#include <stdexcept>

int main()
{
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
    } catch (std::exception e) {
        spdlog::critical("Unexpected exception: {}", e.what());
        std::string errorMessage = std::string("Unexpected exception: ") + e.what();
        std::wstring errorMessageW = std::wstring(errorMessage.begin(), errorMessage.end());
        MessageBox(
            NULL,
            errorMessageW.c_str(),
            L"ScopedVolumeWheel crashed!",
            MB_OK);
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
