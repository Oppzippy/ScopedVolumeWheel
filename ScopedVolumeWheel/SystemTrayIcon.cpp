#include "SystemTrayIcon.h"
#include "Win32Exception.h"
#include "resource.h"
#include <CommCtrl.h>
#include <WinUser.h>
#include <Windows.h>
#include <shellapi.h>
#include <spdlog/spdlog.h>
#include <strsafe.h>
#include <vector>

SystemTrayIcon::SystemTrayIcon(HWND hWnd)
{
    this->iconData.cbSize = sizeof(this->iconData);
    this->iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    this->iconData.uID = 0;
    this->iconData.uCallbackMessage = CALLBACK_MESSAGE_ID;
    this->iconData.hWnd = hWnd;
    const HRESULT hresult = StringCchCopy(this->iconData.szTip, ARRAYSIZE(this->iconData.szTip), L"ScopedVolumeWheel");
    throwWin32ExceptionIfError("StringCchCopy", hresult);

    HANDLE icon = LoadImageW(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON), IMAGE_ICON, 32, 32, LR_SHARED);
    throwWin32ExceptionIfNotSuccess("LoadIconW", icon);

    this->iconData.hIcon = static_cast<HICON>(icon);

    const BOOL result = Shell_NotifyIconW(NIM_ADD, &this->iconData);
    throwWin32ExceptionIfNotSuccess("Shell_NotifyIconW", result);

    this->contextMenu = CreatePopupMenu();

    int i = 0;
    this->addMusicPlayerSelectionMenuItem(i++);
    this->addExitMenuItem(i++);
}

SystemTrayIcon::~SystemTrayIcon()
{
    try {
        BOOL result = DestroyMenu(this->musicPlayerMenu);
        throwWin32ExceptionIfNotSuccess("DestroyMenu", result);
        result = DestroyMenu(this->contextMenu);
        throwWin32ExceptionIfNotSuccess("DestroyMenu", result);
        result = Shell_NotifyIconW(NIM_DELETE, &this->iconData);
        throwWin32ExceptionIfNotSuccess("Shell_NotifyIconW", result);
    } catch (const Win32Exception& e) {
        // spdlog::error isn't noexcept, suppress this warning
#pragma warning(push)
#pragma warning(disable : 26447)
        spdlog::error(e.what());
#pragma warning(pop)
    }
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

    const BOOL result = InsertMenuItem(this->contextMenu, index, true, &item);
    throwWin32ExceptionIfNotSuccess("InsertMenuItem", result);
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

    const BOOL result = InsertMenuItem(this->contextMenu, index, true, &item);
    throwWin32ExceptionIfNotSuccess("InsertMenuItem", result);
}

HMENU SystemTrayIcon::musicPlayerSelectionMenu()
{
    HMENU menu = CreatePopupMenu();
    throwWin32ExceptionIfNotSuccess("CreatePopupMenu", menu != NULL);

    musicPlayerItemIdStartIndex = this->nextMenuItemId;
    int i = 0;
    for (const std::wstring& application : this->musicApplications) {
        MENUITEMINFO item {};
        item.cbSize = sizeof(item);
        item.fType = MFT_RADIOCHECK;
        item.fState = application == this->selectedMusicPlayer ? MFS_CHECKED : MFS_UNCHECKED;
        item.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_STATE | MIIM_ID;
        item.wID = this->nextMenuItemId++;

        // InsertMenuItem doesn't modify the text so this is safe
        LPWSTR text = const_cast<wchar_t*>(application.c_str());
        item.dwTypeData = text;
        item.cch = application.size();

        const BOOL result = InsertMenuItem(menu, i++, true, &item);
        throwWin32ExceptionIfNotSuccess("InsertMenuItem", result);
    }

    return menu;
}

void SystemTrayIcon::showMenu()
{
    POINT cursorPosition {};
    BOOL result = GetCursorPos(&cursorPosition);
    throwWin32ExceptionIfNotSuccess("GetCursorPos", result);

    result = SetForegroundWindow(this->iconData.hWnd);
    throwWin32ExceptionIfNotSuccess("SetForegroundWindow", result);

    const UINT selectionId = TrackPopupMenuEx(
        this->contextMenu,
        TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_HORIZONTAL | TPM_RETURNCMD,
        cursorPosition.x,
        cursorPosition.y,
        this->iconData.hWnd,
        NULL);

    if (selectionId == this->exitMenuItemId) {
        PostQuitMessage(0);
    } else if (
        selectionId >= this->musicPlayerItemIdStartIndex
        && selectionId < this->musicPlayerItemIdStartIndex + this->musicApplications.size()) {

        this->selectedMusicPlayer = this->musicApplications[static_cast<UINT>(selectionId) - this->musicPlayerItemIdStartIndex];

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
        const UINT itemId = i + this->musicPlayerItemIdStartIndex;
        MENUITEMINFO item {};
        item.cbSize = sizeof(item);
        item.fState = this->musicApplications[i] == this->selectedMusicPlayer ? MFS_CHECKED : MFS_UNCHECKED;
        item.fMask = MIIM_STATE;

        const BOOL result = SetMenuItemInfo(this->musicPlayerMenu, itemId, false, &item);
        throwWin32ExceptionIfNotSuccess("SetMenuItemInfo", result);
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
