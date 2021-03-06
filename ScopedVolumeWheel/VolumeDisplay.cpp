#include "VolumeDisplay.h"
#include "Win32Exception.h"
#include <WinUser.h>
#include <Windows.h>
#include <iostream>
#include <thread>

using std::chrono::steady_clock;

const LPCWSTR WINDOW_CLASS_NAME = L"volumeDisplay";

VolumeDisplay::VolumeDisplay()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXW wc { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    };
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    const ATOM atom = RegisterClassExW(&wc);
    throwWin32ExceptionIfNotSuccess("RegisterClassExW", atom != 0);

    this->hWnd = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
        WINDOW_CLASS_NAME,
        L"no title",
        WS_POPUP,
        0, 0, // Position
        500, 500, // Size
        NULL,
        NULL,
        hInstance,
        NULL);
}

VolumeDisplay::~VolumeDisplay()
{
    DestroyWindow(this->hWnd);
    this->hWnd = NULL;
}

void VolumeDisplay::show(float level) noexcept
{
    this->startTime = steady_clock::now();
    this->level = level;
    this->visible = true;
    ShowWindow(this->hWnd, SW_SHOWNA);
    SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), 0xFF, LWA_ALPHA);
    this->renderBar();
    this->render();
}

void VolumeDisplay::setDuration(float duration) noexcept
{
    this->duration = static_cast<long long>(duration * 1e9);
}

void VolumeDisplay::setFadeTime(float fadeTime) noexcept
{
    this->fadeTime = static_cast<long long>(fadeTime * 1e9);
}

bool VolumeDisplay::isVisible() noexcept
{
    return this->visible;
}

void VolumeDisplay::tick() noexcept
{
    MSG msg;
    while (PeekMessageW(&msg, this->hWnd, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (visible) {
        this->render();
    }
}

void VolumeDisplay::render() noexcept
{
    const steady_clock::time_point now = steady_clock::now();
    const steady_clock::duration elapsed = now - this->startTime;
    const long long nanoSeconds = elapsed.count();

    if (nanoSeconds < this->duration) {
        // SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), 0xFF, LWA_ALPHA);
    } else if (nanoSeconds - this->duration < this->fadeTime) {
        const float opacity = 1.0f - (static_cast<float>(nanoSeconds) - this->duration) / this->fadeTime;
        const BYTE opacityByte = static_cast<BYTE>(opacity * 0xFF);
        SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), opacityByte, LWA_ALPHA);
    } else {
        ShowWindow(this->hWnd, SW_HIDE);
        this->visible = false;
    }
}

void VolumeDisplay::renderBar() noexcept
{
    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    const int width = screenWidth;
    const int height = screenHeight / 30;

    MoveWindow(this->hWnd, 0, 0, width, height, false);

    const int filledWidth = static_cast<int>(width * level);

    HDC dc = GetDC(this->hWnd);

    SelectObject(dc, GetStockObject(BLACK_PEN));
    SelectObject(dc, GetStockObject(BLACK_BRUSH));
    Rectangle(dc, filledWidth, 0, width, height);
    SelectObject(dc, GetStockObject(WHITE_PEN));
    SelectObject(dc, GetStockObject(WHITE_BRUSH));
    Rectangle(dc, 0, 0, filledWidth + 1, height);

    ReleaseDC(this->hWnd, dc);
}
