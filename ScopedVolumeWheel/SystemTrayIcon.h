#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <functional>

class SystemTrayIcon 
{
public:
	const static UINT CALLBACK_MESSAGE_ID = WM_APP + 1;
	SystemTrayIcon(HWND hwnd);
	~SystemTrayIcon();
	void showMenu();
	void setMusicPlayerChangeHandler(std::function<void(const std::wstring& applicationName)> handler);
private:
	const std::vector<std::wstring> musicApplications = {
		L"Spotify.exe",
		L"vlc.exe",
		L"foobar2000.exe",
		L"wmplayer.exe",
		L"msedge.exe",
		L"chrome.exe"
		L"firefox.exe",
	};
	NOTIFYICONDATA iconData{};
	HMENU contextMenu{};
	HMENU musicPlayerMenu;
	UINT nextMenuItemId = 0;
	UINT exitMenuItemId = 0;
	UINT musicPlayerItemIdStartIndex = 0;
	std::function<void(const std::wstring& applicationName)> musicPlayerChangeHandler;

	void addExitMenuItem(UINT index);
	void addMusicPlayerSelectionMenuItem(UINT index);
	HMENU musicPlayerSelectionMenu();
};

