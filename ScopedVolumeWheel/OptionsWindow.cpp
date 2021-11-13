#include "OptionsWindow.h"
#include "Win32Exception.h"
#include <iostream>
#include <string>
#include <strsafe.h>

const LPCWSTR WINDOW_CLASS_NAME = L"optionsWindow";

OptionsWindow::OptionsWindow()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXW wc { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        LONG_PTR selfPointer = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        OptionsWindow* self = reinterpret_cast<OptionsWindow*>(selfPointer);

        switch (lParam) {
        case WM_RBUTTONUP:
            self->getSystemTrayIcon()->showMenu();
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    };
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    const ATOM atom = RegisterClassExW(&wc);
    throwWin32ExceptionIfNotSuccess("RegisterClassExW", atom != 0);

    this->hWnd = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW,
        WINDOW_CLASS_NAME,
        L"no title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, // Position
        CW_USEDEFAULT, CW_USEDEFAULT, // Size
        NULL,
        NULL,
        hInstance,
        NULL);
    SetWindowLongPtr(this->hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    this->systemTrayIcon = std::make_unique<SystemTrayIcon>(this->hWnd);
}

OptionsWindow::~OptionsWindow()
{
    DestroyWindow(this->hWnd);
}

void OptionsWindow::setMusicPlayerChangeHandler(std::function<void(const std::wstring& applicationName)> handler)
{
    this->systemTrayIcon->setMusicPlayerChangeHandler(handler);
}

void OptionsWindow::setSelectedMusicPlayer(const std::wstring& applicationName)
{
    this->systemTrayIcon->setSelectedMusicPlayer(applicationName);
}

const std::unique_ptr<SystemTrayIcon>& OptionsWindow::getSystemTrayIcon() noexcept
{
    return this->systemTrayIcon;
}
