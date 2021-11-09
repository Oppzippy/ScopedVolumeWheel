#include "ScopedVolumeWheel.h"
#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "ApplicationProcessIdSelectionStrategy.h"
#include "FocusedWindowProcessIdSelectionStrategy.h"
#include "VolumeAdjustmentHotKeyHandler.h"

ScopedVolumeWheel::ScopedVolumeWheel()
{
    this->optionsWindow = std::make_unique<OptionsWindow>();
    spdlog::info("Initialized OptionsWindow");

    this->registry = std::make_unique<HotKeyRegistry>();
    spdlog::info("Initialized HotKeyRegistry");

    this->mixer = std::make_unique<VolumeMixer>();
    spdlog::info("Initialized VolumeMixer");

    this->display = std::make_unique<VolumeDisplay>();
    spdlog::info("Initialized VolumeDisplay");

    this->focusedWindowStrategy = std::make_unique<FocusedWindowProcessIdSelectionStrategy>();
    this->musicPlayerStrategy = std::make_unique<ApplicationProcessIdSelectionStrategy>(std::wstring(L"Spotify.exe"));

    auto volumeUpHandler = std::make_unique<VolumeAdjustmentHotKeyHandler>(*this->mixer, *this->display, *this->focusedWindowStrategy, VOLUME_ADJUSTMENT);
    auto volumeDownHandler = std::make_unique<VolumeAdjustmentHotKeyHandler>(*this->mixer, *this->display, *this->focusedWindowStrategy, -VOLUME_ADJUSTMENT);
    auto spotifyVolumeUpHandler = std::make_unique<VolumeAdjustmentHotKeyHandler>(*this->mixer, *this->display, *this->musicPlayerStrategy, VOLUME_ADJUSTMENT);
    auto spotifyVolumeDownHandler = std::make_unique<VolumeAdjustmentHotKeyHandler>(*this->mixer, *this->display, *this->musicPlayerStrategy, -VOLUME_ADJUSTMENT);

    registry->registerHotKey(HotKey(VK_VOLUME_UP, 0), std::move(volumeUpHandler));
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, 0), std::move(volumeDownHandler));
    registry->registerHotKey(HotKey(VK_VOLUME_UP, MOD_SHIFT), std::move(spotifyVolumeUpHandler));
    registry->registerHotKey(HotKey(VK_VOLUME_DOWN, MOD_SHIFT), std::move(spotifyVolumeDownHandler));

    this->optionsWindow->setMusicPlayerChangeHandler([this](const std::wstring& applicationName) {
        this->setMusicPlayer(applicationName);
    });
}

void ScopedVolumeWheel::run()
{
    MSG msg{};
    while (true) {
        while (PeekMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY, true) != 0) {
            switch (msg.message) {
            case WM_HOTKEY:
                this->registry->handle(msg);
                break;
            }
        }
        if (msg.message == WM_QUIT) {
            break;
        }
        this->display->tick();
        Sleep(16);
    }
}

void ScopedVolumeWheel::setMusicPlayer(const std::wstring& applicationName) {
    this->musicPlayerStrategy->setApplicationName(applicationName);
}