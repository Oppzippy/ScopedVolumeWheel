#pragma once
#include <atlbase.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <optional>
#include <vector>

class VolumeMixer {
public:
    std::optional<float> adjustVolumeOfProcess(DWORD processId, float adjustment);

private:
    std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControlsForProcess(DWORD processId);
    std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControls();
    CComPtr<IMMDeviceCollection> getDevices();
};
