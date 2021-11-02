#include <iostream>
#include "VolumeMixer.h"

int main()
{
    HRESULT res = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    std::cout << res << std::endl;

    VolumeMixer mixer;
    mixer.adjustVolumeOfProcess(0, -0.01);
}
