#include "SystemTrayIcon.h"
#include <Windows.h>
#include <WinUser.h>
#include <shellapi.h>
#include <CommCtrl.h>
#include <strsafe.h>
#include "Win32Exception.h"
#include "resource.h"

SystemTrayIcon::SystemTrayIcon(HWND hWnd)
{
	this->iconData.cbSize = sizeof(this->iconData);
	this->iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	this->iconData.uID = 0;
	this->iconData.uCallbackMessage = CALLBACK_MESSAGE_ID;
	this->iconData.hWnd = hWnd;
	StringCchCopy(this->iconData.szTip, ARRAYSIZE(this->iconData.szTip), L"ScopedVolumeWheel");
	
	HANDLE icon = LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON), IMAGE_ICON, 32, 32, LR_SHARED);
	if (icon == NULL) {
		throwWin32Exception("LoadIconW", GetLastError());
	}
	this->iconData.hIcon = static_cast<HICON>(icon);

	Shell_NotifyIconW(NIM_ADD, &this->iconData);

	this->contextMenu = CreatePopupMenu();

	MENUITEMINFO exitMenuItem{};
	exitMenuItem.cbSize = sizeof(exitMenuItem);
	exitMenuItem.fType = MFT_STRING;
	exitMenuItem.fState = MFS_ENABLED;
	exitMenuItem.fMask = MIIM_ID | MIIM_STRING;
	exitMenuItem.wID = EXIT_MENU_ITEM_ID;
	wchar_t text[6] = L"Exit";
	exitMenuItem.dwTypeData = text;

	InsertMenuItem(this->contextMenu, 2, true, &exitMenuItem);

}

SystemTrayIcon::~SystemTrayIcon()
{
	DestroyMenu(this->contextMenu);
	Shell_NotifyIconW(NIM_DELETE, &this->iconData);
}

void SystemTrayIcon::showMenu()
{
	POINT cursorPosition{};
	GetCursorPos(&cursorPosition);

	SetForegroundWindow(this->iconData.hWnd);
	BOOL result = TrackPopupMenuEx(
		this->contextMenu,
		TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_HORIZONTAL | TPM_RETURNCMD,
		cursorPosition.x,
		cursorPosition.y,
		this->iconData.hWnd,
		NULL
	);
	
	switch (result) {
	case EXIT_MENU_ITEM_ID:
		PostQuitMessage(0);
		break;
	}
}