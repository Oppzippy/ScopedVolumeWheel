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

const int VOLUME_UP_HOTKEY_ID = 1;
const int VOLUME_DOWN_HOTKEY_ID = 2;

void registerHandlers(
    std::shared_ptr<HotKeyRegistry> registry,
    std::shared_ptr<VolumeMixer> mixer,
    std::shared_ptr<VolumeDisplay> display
) {
    std::shared_ptr<const ProcessIdSelectionStrategy> focusedWindowStrategy(new FocusedWindowProcessIdSelectionStrategy());
    std::shared_ptr<const ProcessIdSelectionStrategy> spotifyStrategy(new ApplicationProcessIdSelectionStrategy(L"Spotify.exe"));

    std::shared_ptr<HotKeyHandler> volumeUpHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, focusedWindowStrategy, 0.02f));
    std::shared_ptr<HotKeyHandler> volumeDownHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, focusedWindowStrategy, -0.02f));
    std::shared_ptr<HotKeyHandler> spotifyVolumeUpHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, spotifyStrategy, 0.02f));
    std::shared_ptr<HotKeyHandler> spotifyVolumeDownHandler(new VolumeAdjustmentHotKeyHandler(mixer, display, spotifyStrategy, -0.02f));

    registry->registerHotKey(HotKey(VK_VOLUME_UP, 0), volumeUpHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, 0), volumeDownHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_UP, MOD_SHIFT), spotifyVolumeUpHandler);
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, MOD_SHIFT), spotifyVolumeDownHandler);
}

BOOL nextMessage(MSG* msg, bool wait) {
    if (wait) {
        std::cout << "WAIT" << std::endl;
        return GetMessage(msg, NULL, WM_HOTKEY, WM_HOTKEY);
    } else {
        std::cout << "PEEK" << std::endl;
        return PeekMessage(msg, NULL, WM_HOTKEY, WM_HOTKEY, true);
    }
}

int main()
{
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK) {
        std::cerr << "CoInitializeEx failed with code " << std::to_string(result) << std::endl;
    }

    std::shared_ptr<HotKeyRegistry> registry(new HotKeyRegistry());
    std::shared_ptr<VolumeMixer> mixer(new VolumeMixer());
    std::shared_ptr<VolumeDisplay> display(new VolumeDisplay());

    registerHandlers(registry, mixer, display);

    MSG msg = { 0 };
    while (true) {
        while (nextMessage(&msg, !display->isVisible()) != 0) {
            if (msg.message == WM_HOTKEY) {
                registry->handle(msg);
            }
        }
        display->tick();
        if (display->isVisible()) {
            Sleep(16);
        }
    }

    CoUninitialize();
}

