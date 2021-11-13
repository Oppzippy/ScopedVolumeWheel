#pragma once
#include "SystemTrayIcon.h"
#include <Windows.h>
#include <memory>

class OptionsWindow {
public:
    OptionsWindow();
    ~OptionsWindow();
    void setMusicPlayerChangeHandler(std::function<void(const std::wstring& applicationName)>);
    void setSelectedMusicPlayer(const std::wstring& applicationName);
    const std::unique_ptr<SystemTrayIcon>& getSystemTrayIcon() noexcept;

private:
    HWND hWnd = NULL;
    std::unique_ptr<SystemTrayIcon> systemTrayIcon;
};
