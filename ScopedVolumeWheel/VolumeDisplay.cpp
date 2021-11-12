#include "VolumeDisplay.h"
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
    wc.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    };
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    auto x = RegisterClassExW(&wc);

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

void VolumeDisplay::show(float level)
{
    this->startTime = steady_clock::now();
    this->level = level;
    this->visible = true;
    ShowWindow(this->hWnd, SW_SHOWNA);
    SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), 0xFF, LWA_ALPHA);
    this->renderBar();
    this->render();
}

void VolumeDisplay::setDuration(float duration)
{
    this->duration = static_cast<long long>(duration * 1e9);
}

void VolumeDisplay::setFadeTime(float fadeTime)
{
    this->fadeTime = static_cast<long long>(fadeTime * 1e9);
}

bool VolumeDisplay::isVisible()
{
    return this->visible;
}

void VolumeDisplay::tick()
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

void VolumeDisplay::render()
{
    steady_clock::time_point now = steady_clock::now();
    steady_clock::duration elapsed = now - this->startTime;
    long long nanoSeconds = elapsed.count();

    if (nanoSeconds < this->duration) {
        // SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), 0xFF, LWA_ALPHA);
    } else if (nanoSeconds - this->duration < this->fadeTime) {
        float opacity = 1.0f - (static_cast<float>(nanoSeconds) - this->duration) / this->fadeTime;
        BYTE opacityByte = static_cast<BYTE>(opacity * 0xFF);
        SetLayeredWindowAttributes(this->hWnd, RGB(0, 0, 0), opacityByte, LWA_ALPHA);
    } else {
        ShowWindow(this->hWnd, SW_HIDE);
        this->visible = false;
    }
}

void VolumeDisplay::renderBar()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int width = screenWidth;
    int height = screenHeight / 30;

    MoveWindow(this->hWnd, 0, 0, width, height, false);

    int filledWidth = width * level;

    HDC dc = GetDC(this->hWnd);

    SelectObject(dc, GetStockObject(BLACK_PEN));
    SelectObject(dc, GetStockObject(BLACK_BRUSH));
    Rectangle(dc, filledWidth, 0, width, height);
    SelectObject(dc, GetStockObject(WHITE_PEN));
    SelectObject(dc, GetStockObject(WHITE_BRUSH));
    Rectangle(dc, 0, 0, filledWidth + 1, height);

    ReleaseDC(this->hWnd, dc);
}