#include <Windows.h>

#include "Volume.h"
#include "Utils.h"

#include <cmath>
//#include <WTypesbase.h>
#include <objbase.h>
#include <winerror.h>
#include <mmdeviceapi.h>
//#include <combaseapi.h>
#include <endpointvolume.h>
#include <Audioclient.h>

namespace utils::Volume {
    namespace {
        bool volumeProcess(std::function<bool(IAudioEndpointVolume* audioEndpointVolume)>&& volumeProc) {
            return Utils::COMProcess<IMMDeviceEnumerator>(__uuidof(MMDeviceEnumerator), CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), [&volumeProc](IMMDeviceEnumerator* deviceEnumerator) {
                auto result = false;
                IMMDevice* device = nullptr;
                HRESULT hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
                if (device) {
                    IAudioEndpointVolume* audioEndpointVolume = nullptr;
                    hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)& audioEndpointVolume);
                    if (SUCCEEDED(hr)) {
                        // @@ why??
                        IAudioClient* audioClient = nullptr;
                        hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)& audioClient);
                        if (SUCCEEDED(hr)) {
                            result = volumeProc(audioEndpointVolume);
                            audioClient->Release();
                        }
                        audioEndpointVolume->Release();
                    }
                    device->Release();
                }
                return result;
            });
        }
    }
    bool GetVoiceVolume(int& volume) {
        return volumeProcess([&volume](IAudioEndpointVolume* audioEndpointVolume) {
            bool result = false;
            float volumeValue;
            HRESULT hr = audioEndpointVolume->GetMasterVolumeLevelScalar(&volumeValue);
            volume = static_cast<int>(std::ceil(volumeValue * 100));
            if (SUCCEEDED(hr)) {
                result = true;
            }
            return result;
        });
    }

    bool SetVoiceVolume(int level) {
        return volumeProcess([level](IAudioEndpointVolume* audioEndpointVolume) -> bool {
            auto result = false;
            if (level == -2) {
                HRESULT hr = audioEndpointVolume->SetMute(FALSE, NULL);
                if (SUCCEEDED(hr)) {
                    result = true;
                }
            } else if (level == -1) {
                HRESULT hr = audioEndpointVolume->SetMute(TRUE, NULL);
                if (SUCCEEDED(hr)) {
                    result = true;
                }
            } else {
                if (level >= 0 && level <= 100) {
                    float fVolume = level / 100.0f;
                    HRESULT hr = audioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &GUID_NULL);
                    if (SUCCEEDED(hr)) {
                        result = true;
                    }
                } else {
                    HRESULT hr = E_INVALIDARG;
                    // throw "Invalid Arg";
                }
            }
            return result;
        });
    }
}
