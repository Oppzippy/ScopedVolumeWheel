#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <stdexcept>
#include "VolumeMixer.h"
#include "Win32Exception.h"
#include "VolumeDisplay.h"

const int VOLUME_UP_HOTKEY_ID = 1;
const int VOLUME_DOWN_HOTKEY_ID = 2;

DWORD getForegroundWindowProcessId();

int main()
{
    VolumeDisplay vd;
    
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK) {
        std::cerr << "CoInitializeEx failed with code " << std::to_string(result) << std::endl;
    }

    VolumeMixer mixer;
    
    RegisterHotKey(NULL, VOLUME_UP_HOTKEY_ID, 0, VK_VOLUME_UP);
    RegisterHotKey(NULL, VOLUME_DOWN_HOTKEY_ID, 0, VK_VOLUME_DOWN);

    MSG msg = { 0 };
    while (true) {
        BOOL available = PeekMessage(&msg, NULL, WM_HOTKEY, WM_HOTKEY, true);
        if (!available) {
            vd.tick();
            Sleep(vd.isVisible() ? 5 : 50);
            continue;
        }
        try {
            if (msg.message == WM_HOTKEY) {
                DWORD processId = getForegroundWindowProcessId();
                switch (msg.wParam) {
                case VOLUME_UP_HOTKEY_ID:
                    vd.show(mixer.adjustVolumeOfProcess(processId, 0.02f));
                    break;
                case VOLUME_DOWN_HOTKEY_ID:
                    vd.show(mixer.adjustVolumeOfProcess(processId, -0.02f));
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