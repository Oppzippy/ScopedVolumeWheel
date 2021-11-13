#pragma once
#include <Windows.h>
#include <functional>
#include <string>
#include <vector>

class SystemTrayIcon {
public:
    const static UINT CALLBACK_MESSAGE_ID = WM_APP + 1;
    SystemTrayIcon(HWND hwnd);
    ~SystemTrayIcon();
    void showMenu();
    void updateMusicPlayerMenu();
    void setSelectedMusicPlayer(const std::wstring& applicationName);
    void setMusicPlayerChangeHandler(std::function<void(const std::wstring& applicationName)> handler);

private:
    const std::vector<std::wstring> musicApplications = {
        L"Spotify.exe",
        L"vlc.exe",
        L"foobar2000.exe",
        L"wmplayer.exe",
        L"msedge.exe",
        L"chrome.exe",
        L"firefox.exe",
    };
    std::wstring selectedMusicPlayer = L"Spotify.exe";

    NOTIFYICONDATA iconData {};
    HMENU contextMenu {};
    HMENU musicPlayerMenu;
    // Start at 1 to distinguish TrackPopupMenuEx's 0 return from an id
    UINT nextMenuItemId = 1;
    UINT exitMenuItemId = 0;
    UINT musicPlayerItemIdStartIndex = 0;

    std::function<void(const std::wstring& applicationName)> musicPlayerChangeHandler;

    void addExitMenuItem(UINT index);
    void addMusicPlayerSelectionMenuItem(UINT index);
    HMENU musicPlayerSelectionMenu();
};
