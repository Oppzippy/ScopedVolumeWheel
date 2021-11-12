#include "VolumeMixer.h"
#include "Win32Exception.h"

#define AUDCLNT_S_NO_SINGLE_PROCESS AUDCLNT_SUCCESS(0x00d)

float VolumeMixer::adjustVolumeOfProcess(DWORD processId, float adjustment)
{
    std::vector<CComPtr<IAudioSessionControl2>> sessions = this->getAudioSessionControlsForProcess(processId);
    for (CComPtr<IAudioSessionControl2> session : sessions) {
        CComPtr<ISimpleAudioVolume> volume;
        HRESULT result = session->QueryInterface(&volume);
        throwWin32ExceptionIfNotOk("IAudioSessionControl2::QueryInterface", result);

        float level = 0;
        result = volume->GetMasterVolume(&level);
        throwWin32ExceptionIfNotOk("ISimpleAudioVolume::GetMasterVolume", result);

        level += adjustment;
        if (level > 1) {
            level = 1;
        } else if (level < 0) {
            level = 0;
        }
        result = volume->SetMasterVolume(level, NULL);
        throwWin32ExceptionIfNotOk("ISimpleAudioVolume::SetMasterVolume", result);
        return level;
    }
}

std::vector<CComPtr<IAudioSessionControl2>> VolumeMixer::getAudioSessionControlsForProcess(DWORD processId)
{
    std::vector<CComPtr<IAudioSessionControl2>> sessions = this->getAudioSessionControls();
    auto thingsToRemove = std::remove_if(sessions.begin(), sessions.end(), [processId](CComPtr<IAudioSessionControl2> session) {
        DWORD pId = 0;
        HRESULT result = session->GetProcessId(&pId);
        if (result != AUDCLNT_S_NO_SINGLE_PROCESS) {
            throwWin32ExceptionIfNotOk("IAudioSessionControl2::GetProcessId", result);
        }
        return pId != processId;
    });
    sessions.erase(thingsToRemove, sessions.end());
    return sessions;
}

std::vector<CComPtr<IAudioSessionControl2>> VolumeMixer::getAudioSessionControls()
{
    std::vector<CComPtr<IAudioSessionControl2>> sessions;

    CComPtr<IMMDeviceCollection> devices = this->getDevices();
    UINT numDevices = 0;
    HRESULT result = devices->GetCount(&numDevices);
    throwWin32ExceptionIfNotOk("IMMDeviceCollection::GetCount", result)

        for (UINT i = 0; i < numDevices; i++)
    {
        CComPtr<IMMDevice> device;
        result = devices->Item(i, &device);
        throwWin32ExceptionIfNotOk("IMMDeviceCollection::Item", result);

        CComPtr<IAudioSessionManager2> sessionManager;
        result = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&sessionManager);
        throwWin32ExceptionIfNotOk("IMMDevice::Activate", result);

        CComPtr<IAudioSessionEnumerator> enumerator;
        result = sessionManager->GetSessionEnumerator(&enumerator);
        throwWin32ExceptionIfNotOk("IAudioSessionManager2::GetSessionEnumerator", result);

        int numSessions = 0;
        result = enumerator->GetCount(&numSessions);
        throwWin32ExceptionIfNotOk("IAudioSessionEnumerator::GetCount", result);

        for (int i = 0; i < numSessions; i++) {
            CComPtr<IAudioSessionControl> session;
            result = enumerator->GetSession(i, &session);
            throwWin32ExceptionIfNotOk("IAudioSessionEnumerator::GetSession", result);

            CComPtr<IAudioSessionControl2> session2;
            session->QueryInterface(&session2);
            throwWin32ExceptionIfNotOk("IAudioSessionControl::QueryInterface", result);

            sessions.push_back(session2);
        }
    }

    return sessions;
}

CComPtr<IMMDeviceCollection> VolumeMixer::getDevices()
{
    CComPtr<IMMDeviceEnumerator> deviceEnumerator;
    HRESULT result = deviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
    throwWin32ExceptionIfNotOk("CComPtr::CoCreateInstance", result);

    CComPtr<IMMDeviceCollection> devices;
    result = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
    throwWin32ExceptionIfNotOk("IMMDeviceEnumerator::EnumAudioEndpoints", result);

    return devices;
}
