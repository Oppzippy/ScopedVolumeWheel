#include "ScopedVolumeWheel.h"
#include "ApplicationPaths.h"
#include "ApplicationProcessIdSelectionStrategy.h"
#include "FocusedWindowProcessIdSelectionStrategy.h"
#include "VolumeAdjustmentHotKeyHandler.h"
#include "Win32Exception.h"
#include "spdlog/spdlog.h"
#include <PathCch.h>
#include <Windows.h>
#include <memory>
#include <string>

ScopedVolumeWheel::ScopedVolumeWheel()
{
    this->config = std::make_unique<Configuration>(this->getConfigFilePath());
    spdlog::info("Loaded Configuration");

    this->optionsWindow = std::make_unique<OptionsWindow>();
    spdlog::info("Initialized OptionsWindow");

    this->registry = std::make_unique<HotKeyRegistry>();
    spdlog::info("Initialized HotKeyRegistry");

    this->mixer = std::make_unique<VolumeMixer>();
    spdlog::info("Initialized VolumeMixer");

    this->display = std::make_unique<VolumeDisplay>();
    spdlog::info("Initialized VolumeDisplay");

    this->optionsWindow->setSelectedMusicPlayer(this->config->getMusicPlayer());

    this->focusedWindowStrategy = std::make_unique<FocusedWindowProcessIdSelectionStrategy>();
    this->musicPlayerStrategy = std::make_unique<ApplicationProcessIdSelectionStrategy>(this->config->getMusicPlayer());

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
    MSG msg {};
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
        if (!this->display->isVisible()) {
            const BOOL result = WaitMessage();
            throwWin32ExceptionIfNotSuccess("WaitMessage", result);
        }
    }
}

void ScopedVolumeWheel::setMusicPlayer(const std::wstring& applicationName)
{
    this->musicPlayerStrategy->setApplicationName(applicationName);
    this->config->setMusicPlayer(applicationName);
}

std::wstring ScopedVolumeWheel::getConfigFilePath()
{
    std::wstring storagePath = ApplicationPaths::getStoragePath();

    PWSTR filePath = NULL;
    const HRESULT result = PathAllocCombine(storagePath.c_str(), L"config.toml", PATHCCH_NONE, &filePath);
    throwWin32ExceptionIfError("PathAllocCombine", result);

    std::wstring filePathString = std::wstring(filePath);
    CoTaskMemFree(filePath);

    return filePathString;
}
