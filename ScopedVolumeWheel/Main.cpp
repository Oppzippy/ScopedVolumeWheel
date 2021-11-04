#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include "VolumeMixer.h"
#include "Win32Exception.h"
#include <stdexcept>

const int VOLUME_UP_HOTKEY_ID = 1;
const int VOLUME_DOWN_HOTKEY_ID = 2;

DWORD getForegroundWindowProcessId();

int main()
{
    HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (result != S_OK) {
        std::cerr << "CoInitializeEx failed with code " << std::to_string(result) << std::endl;
    }

    VolumeMixer mixer;
    
    RegisterHotKey(NULL, VOLUME_UP_HOTKEY_ID, 0, VK_VOLUME_UP);
    RegisterHotKey(NULL, VOLUME_DOWN_HOTKEY_ID, 0, VK_VOLUME_DOWN);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        try {
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
        } catch (Win32Exception& e) {
            std::cerr << e.what() << std::endl;
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