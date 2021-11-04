#pragma once
#include <vector>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <atlbase.h>

class VolumeMixer
{
public:
	float adjustVolumeOfProcess(DWORD processId, float adjustment);
private:
	std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControlsForProcess(DWORD processId);
	std::vector<CComPtr<IAudioSessionControl2>> getAudioSessionControls();
	CComPtr<IMMDeviceCollection> getDevices();
};
