#include "VolumeMixer.h"
#include <iostream>

void VolumeMixer::adjustVolumeOfProcess(DWORD processId, float adjustment)
{
    std::vector<IAudioSessionControl2*> sessions = this->getAudioSessionControlsForProcess(processId);
    for (IAudioSessionControl2* session : sessions) {
        ISimpleAudioVolume* volume;
        session->QueryInterface(&volume);
        float level = 0;
        volume->GetMasterVolume(&level);
        level += adjustment;
        if (level > 1) {
            level = 1;
        } else if (level < 0) {
            level = 0;
        }
        volume->SetMasterVolume(level, NULL);
        session->Release();
    }
}

std::vector<IAudioSessionControl2*> VolumeMixer::getAudioSessionControlsForProcess(DWORD processId)
{
    std::vector<IAudioSessionControl2*> sessions = this->getAudioSessionControls();
    std::remove_if(sessions.begin(), sessions.end(), [processId](IAudioSessionControl2* session) {
        DWORD pId = 0;
        session->GetProcessId(&pId);
        if (pId != processId || true) {
            session->Release();
            return true;
        }
        return false;
    });
    return sessions;
}

std::vector<IAudioSessionControl2 *> VolumeMixer::getAudioSessionControls()
{
	std::vector<IAudioSessionControl2 *> sessions;

    IMMDeviceCollection* devices = this->getDevices();
    UINT numDevices = 0;
    devices->GetCount(&numDevices);
    for (UINT i = 0; i < numDevices; i++) {
        IMMDevice* device = NULL;
        devices->Item(i, &device);

        IAudioSessionManager2* sessionManager = NULL;
        device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&sessionManager);

        IAudioSessionEnumerator* enumerator = NULL;
        sessionManager->GetSessionEnumerator(&enumerator);

        int numSessions = 0;
        enumerator->GetCount(&numSessions);
        for (int i = 0; i < numSessions; i++) {
            IAudioSessionControl* session = NULL;
            enumerator->GetSession(i, &session);
            IAudioSessionControl2* session2 = NULL;
            session->QueryInterface(&session2);
            session->Release();

            sessions.push_back(session2);
        }
        device->Release();
    }
    devices->Release();

    return sessions;
}

IMMDeviceCollection *VolumeMixer::getDevices()
{
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);

    IMMDeviceCollection* devices = NULL;
    deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
    deviceEnumerator->Release();

    return devices;
}
