#include "SystemTrayIcon.h"
#include "Win32Exception.h"
#include "resource.h"
#include "spdlog/spdlog.h"
#include <CommCtrl.h>
#include <WinUser.h>
#include <Windows.h>
#include <shellapi.h>
#include <strsafe.h>
#include <vector>

SystemTrayIcon::SystemTrayIcon(HWND hWnd)
{
    this->iconData.cbSize = sizeof(this->iconData);
    this->iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    this->iconData.uID = 0;
    this->iconData.uCallbackMessage = CALLBACK_MESSAGE_ID;
    this->iconData.hWnd = hWnd;
    StringCchCopy(this->iconData.szTip, ARRAYSIZE(this->iconData.szTip), L"ScopedVolumeWheel");

    HANDLE icon = LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON), IMAGE_ICON, 32, 32, LR_SHARED);
    throwWin32ExceptionIfNotSuccess("LoadIconW", icon);

    this->iconData.hIcon = static_cast<HICON>(icon);

    Shell_NotifyIconW(NIM_ADD, &this->iconData);

    this->contextMenu = CreatePopupMenu();

    int i = 0;
    this->addMusicPlayerSelectionMenuItem(i++);
    this->addExitMenuItem(i++);
}

SystemTrayIcon::~SystemTrayIcon()
{
    DestroyMenu(this->musicPlayerMenu);
    DestroyMenu(this->contextMenu);
    Shell_NotifyIconW(NIM_DELETE, &this->iconData);
}

void SystemTrayIcon::addExitMenuItem(UINT index)
{
    MENUITEMINFO item {};
    item.cbSize = sizeof(item);
    item.fState = MFS_ENABLED;
    item.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
    this->exitMenuItemId = this->nextMenuItemId++;
    item.wID = this->exitMenuItemId;
    wchar_t text[] = L"Exit";
    item.cch = sizeof(text) / sizeof(wchar_t) - 1;
    item.dwTypeData = text;

    InsertMenuItem(this->contextMenu, index, true, &item);
}

void SystemTrayIcon::addMusicPlayerSelectionMenuItem(UINT index)
{

    MENUITEMINFO item {};
    item.cbSize = sizeof(item);
    item.fState = MFS_ENABLED;
    item.fMask = MIIM_STRING | MIIM_SUBMENU | MIIM_STATE;
    item.hSubMenu = this->musicPlayerSelectionMenu();
    wchar_t text[] = L"Music Player";
    item.cch = sizeof(text) / sizeof(wchar_t) - 1;
    item.dwTypeData = text;

    this->musicPlayerMenu = item.hSubMenu;
    InsertMenuItem(this->contextMenu, index, true, &item);
}

HMENU SystemTrayIcon::musicPlayerSelectionMenu()
{
    HMENU menu = CreatePopupMenu();

    musicPlayerItemIdStartIndex = this->nextMenuItemId;
    int i = 0;
    for (const std::wstring& application : this->musicApplications) {
        MENUITEMINFO item {};
        item.cbSize = sizeof(item);
        item.fType = MFT_RADIOCHECK;
        item.fState = application == this->selectedMusicPlayer ? MFS_CHECKED : MFS_UNCHECKED;
        item.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_STATE | MIIM_ID;
        item.wID = this->nextMenuItemId++;

        LPWSTR text = const_cast<wchar_t*>(application.c_str());
        item.dwTypeData = text;
        item.cch = application.size();

        InsertMenuItem(menu, i++, true, &item);
    }

    return menu;
}

void SystemTrayIcon::showMenu()
{
    POINT cursorPosition {};
    GetCursorPos(&cursorPosition);

    SetForegroundWindow(this->iconData.hWnd);
    BOOL result = TrackPopupMenuEx(
        this->contextMenu,
        TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_HORIZONTAL | TPM_RETURNCMD,
        cursorPosition.x,
        cursorPosition.y,
        this->iconData.hWnd,
        NULL);

    if (result == this->exitMenuItemId) {
        PostQuitMessage(0);
    } else if (
        result >= this->musicPlayerItemIdStartIndex && result < this->musicPlayerItemIdStartIndex + this->musicApplications.size()) {
        this->selectedMusicPlayer = this->musicApplications[result - this->musicPlayerItemIdStartIndex];

        this->updateMusicPlayerMenu();

        if (this->musicPlayerChangeHandler) {
            this->musicPlayerChangeHandler(this->selectedMusicPlayer);
        } else {
            spdlog::warn("musicPlayerChangeHandler is not set");
        }
    }
}

void SystemTrayIcon::updateMusicPlayerMenu()
{
    for (UINT i = 0; i < this->musicApplications.size(); i++) {
        UINT itemId = i + this->musicPlayerItemIdStartIndex;
        MENUITEMINFO item {};
        item.cbSize = sizeof(item);
        item.fState = this->musicApplications[i] == this->selectedMusicPlayer ? MFS_CHECKED : MFS_UNCHECKED;
        item.fMask = MIIM_STATE;

        SetMenuItemInfo(this->musicPlayerMenu, itemId, false, &item);
    }
}

void SystemTrayIcon::setSelectedMusicPlayer(const std::wstring& applicationName)
{
    this->selectedMusicPlayer = applicationName;
    this->updateMusicPlayerMenu();
}

void SystemTrayIcon::setMusicPlayerChangeHandler(std::function<void(const std::wstring& applicationName)> handler)
{
    this->musicPlayerChangeHandler = handler;
}
