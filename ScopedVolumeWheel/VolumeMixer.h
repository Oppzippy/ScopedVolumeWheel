#pragma once
#include <vector>
#include <mmdeviceapi.h>
#include <audiopolicy.h>

class VolumeMixer
{
public:
	void adjustVolumeOfProcess(DWORD processId, float adjustment);
private:
	std::vector<IAudioSessionControl2*> getAudioSessionControlsForProcess(DWORD processId);
	std::vector<IAudioSessionControl2*> getAudioSessionControls();
	IMMDeviceCollection* getDevices();
};
