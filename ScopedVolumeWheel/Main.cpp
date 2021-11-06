#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <stdexcept>
#include "VolumeMixer.h"
#include "Win32Exception.h"
#include "VolumeDisplay.h"
#include "HotKeyRegistry.h"
#include "VolumeAdjustmentHotKeyHandler.h"
#include "FocusedWindowProcessIdSelectionStrategy.h"
#include "ApplicationProcessIdSelectionStrategy.h"
#include "OptionsWindow.h"

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
    OptionsWindow opts;
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK) {
        std::cerr << "CoInitializeEx failed with code " << std::to_string(result) << std::endl;
    }

    std::shared_ptr<HotKeyRegistry> registry(new HotKeyRegistry());
    std::shared_ptr<VolumeMixer> mixer(new VolumeMixer());
    std::shared_ptr<VolumeDisplay> display(new VolumeDisplay());

    registerHandlers(registry, mixer, display);

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
            // WM_QUIT will make nextMessage return 0
            break;
        }
        display->tick();
        Sleep(16);
    }

    CoUninitialize();
}

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
) {
    main();
}