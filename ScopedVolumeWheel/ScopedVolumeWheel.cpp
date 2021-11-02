#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include "VolumeMixer.h"

const int VOLUME_UP_HOTKEY_ID = 1;
const int VOLUME_DOWN_HOTKEY_ID = 2;

DWORD getForegroundWindowProcessId();

int main()
{
    HRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    std::cout << res << std::endl;

    VolumeMixer mixer;
    
    RegisterHotKey(NULL, VOLUME_UP_HOTKEY_ID, 0, VK_VOLUME_UP);
    RegisterHotKey(NULL, VOLUME_DOWN_HOTKEY_ID, 0, VK_VOLUME_DOWN);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (msg.message == WM_HOTKEY) {
            DWORD processId = getForegroundWindowProcessId();
            switch (msg.wParam) {
            case VOLUME_UP_HOTKEY_ID:
                mixer.adjustVolumeOfProcess(processId, 0.02f);
                break;
            case VOLUME_DOWN_HOTKEY_ID:
                mixer.adjustVolumeOfProcess(processId, -0.02f);
                break;
            }
        }
    }

    CoUninitialize();
}

DWORD getForegroundWindowProcessId() {
    HWND hwnd = GetForegroundWindow();
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    return processId;
}