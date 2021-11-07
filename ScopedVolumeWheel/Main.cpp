#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <stdexcept>
#include "spdlog/spdlog.h"
#include "VolumeMixer.h"
#include "Win32Exception.h"
#include "VolumeDisplay.h"
#include "HotKeyRegistry.h"
#include "VolumeAdjustmentHotKeyHandler.h"
#include "FocusedWindowProcessIdSelectionStrategy.h"
#include "ApplicationProcessIdSelectionStrategy.h"
#include "OptionsWindow.h"
#include "SpdlogGlobalConfiguration.h"

void registerHandlers(
    std::shared_ptr<HotKeyRegistry> registry,
    std::shared_ptr<VolumeMixer> mixer,
    std::shared_ptr<VolumeDisplay> display
) {
    std::shared_ptr<const ProcessIdSelectionStrategy> focusedWindowStrategy(new FocusedWindowProcessIdSelectionStrategy());
    std::shared_ptr<const ProcessIdSelectionStrategy> spotifyStrategy(new ApplicationProcessIdSelectionStrategy(L"Spotify.exe"));

    std::unique_ptr<HotKeyHandler> volumeUpHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, focusedWindowStrategy, 0.02f));
    std::unique_ptr<HotKeyHandler> volumeDownHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, focusedWindowStrategy, -0.02f));
    std::unique_ptr<HotKeyHandler> spotifyVolumeUpHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, spotifyStrategy, 0.02f));
    std::unique_ptr<HotKeyHandler> spotifyVolumeDownHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, spotifyStrategy, -0.02f));

    registry->registerHotKey(HotKey(VK_VOLUME_UP, 0), volumeUpHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, 0), volumeDownHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_UP, MOD_SHIFT), spotifyVolumeUpHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, MOD_SHIFT), spotifyVolumeDownHandler);
}

int main()
{
    SpdlogGlobalConfiguration::configure();

    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (result != S_OK) {
        spdlog::critical("CoInitializeEx failed with code {}", result);
        return 1;
    }

    try {
        std::shared_ptr<OptionsWindow> opts(new OptionsWindow());
        spdlog::info("Initialized OptionsWindow");
        std::shared_ptr<HotKeyRegistry> registry(new HotKeyRegistry());
        spdlog::info("Initialized HotKeyRegistry");
        std::shared_ptr<VolumeMixer> mixer(new VolumeMixer());
        spdlog::info("Initialized VolumeMixer");
        std::shared_ptr<VolumeDisplay> display(new VolumeDisplay());
        spdlog::info("Initialized VolumeDisplay");

        registerHandlers(registry, mixer, display);
        spdlog::info("Registered handlers");

        MSG msg{};
        while (true) {
            while (PeekMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY, true) != 0) {
                switch (msg.message) {
                case WM_HOTKEY:
                    registry->handle(msg);
                    break;
                }
            }
            if (msg.message == WM_QUIT) {
                break;
            }
            display->tick();
            Sleep(16);
        }

        CoUninitialize();
    } catch (std::exception e) {
        spdlog::critical("Unexpected exception: {}", e.what());
        std::string errorMessage = std::string("Unexpected exception: ") + e.what();
        std::wstring errorMessageW = std::wstring(errorMessage.begin(), errorMessage.end());
        MessageBox(
            NULL,
            errorMessageW.c_str(),
            L"ScopedVolumeWheel crashed!",
            MB_OK
        );
        return 1;
    }

    return 0;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nShowCmd
) {
    return main();
}
