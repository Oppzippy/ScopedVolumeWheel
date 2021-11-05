#pragma once
#include <Windows.h>

class SystemTrayIcon 
{
public:
	const static UINT CALLBACK_MESSAGE_ID = WM_APP + 1;
	SystemTrayIcon(HWND hwnd);
	~SystemTrayIcon();
	void showMenu();
private:
	const static UINT EXIT_MENU_ITEM_ID = 1;
	NOTIFYICONDATA iconData{};
	HMENU contextMenu{};
};

