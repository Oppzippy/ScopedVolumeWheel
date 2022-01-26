#pragma once
#include <atlbase.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <optional>
#include <set>
#include <vector>

class VolumeMixer {
public:
    std::optional<float> adjustVolumeOfProcess(DWORD processId, float adjustment);
    std::optional<float> adjustVolumeOfProcesses(const std::set<DWORD>& processId, float adjustment);

private:
    std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControlsForProcesses(const std::set<DWORD>& processId);
    std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControls();
    CComPtr<IMMDeviceCollection> getDevices();
};
