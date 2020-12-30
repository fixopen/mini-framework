#include "AudioPlayer.h"

#include <functional>
#include <cmath>
#include <Digitalv.h>

//#include "../shared/Volume.h"

#pragma comment(lib, "winmm.lib")

namespace utils {
	namespace {
		template<typename T>
		void genericProcess(MCIDEVICEID device, UINT message, DWORD command, T const& params, std::function<void()> const& post = nullptr) {
			MCIERROR r = mciSendCommand(device, message, command, reinterpret_cast<DWORD_PTR>(&params));
			if (r == 0) {
                if (post != nullptr) {
                    post();
                }
			} else {
				wchar_t buffer[256] = {};
				mciGetErrorStringW(r, buffer, 256);
				//AfxMessageBox(buffer);
				MessageBoxW(nullptr, buffer, L"Info", MB_OK);
			}
		}
	}

	void AudioPlayer::Open(std::wstring const& filename) {
		MCI_OPEN_PARMSW openParams;
		openParams.lpstrElementName = filename.c_str();
		openParams.dwCallback = reinterpret_cast<DWORD_PTR>(window_);
		auto& dev = device_;
		genericProcess(static_cast<MCIDEVICEID>(NULL), static_cast<UINT>(MCI_OPEN), static_cast<DWORD>(MCI_OPEN_ELEMENT | MCI_WAIT), openParams, [&dev, &openParams] {
			dev = openParams.wDeviceID;
		});
	}

	DWORD_PTR AudioPlayer::Info(DWORD item) {
		DWORD_PTR result = 0;
		MCI_STATUS_PARMS statusParams;
		statusParams.dwReturn = 0;
		statusParams.dwItem = item;
		genericProcess(device_, static_cast<UINT>(MCI_STATUS), static_cast<DWORD>(MCI_STATUS_ITEM), statusParams, [&result, &statusParams] {
			result = statusParams.dwReturn;
		});
		return result;
	}

	void AudioPlayer::Play() {
		MCI_PLAY_PARMS playParams;
		playParams.dwCallback = reinterpret_cast<DWORD_PTR>(window_);
		playParams.dwFrom = 0;
		genericProcess(device_, static_cast<UINT>(MCI_PLAY), static_cast<DWORD>(MCI_NOTIFY), playParams);
	}

	void AudioPlayer::Pause() {
		genericProcess(device_, static_cast<UINT>(MCI_PAUSE), static_cast<DWORD>(0), nullptr);
	}

	void AudioPlayer::Resume() {
		genericProcess(device_, static_cast<UINT>(MCI_RESUME), static_cast<DWORD>(0), nullptr);
	}

	void AudioPlayer::Stop() {
		genericProcess(device_, static_cast<UINT>(MCI_STOP), static_cast<DWORD>(0), nullptr);
	}

	void AudioPlayer::Close() {
		genericProcess(device_, static_cast<UINT>(MCI_CLOSE), static_cast<DWORD>(0), nullptr);
	}

	void AudioPlayer::SeekTo(DWORD position) {
		MCI_SEEK_PARMS seekParams;
		seekParams.dwTo = position;
		genericProcess(device_, static_cast<UINT>(MCI_SEEK), static_cast<DWORD>(MCI_TO), seekParams);
	}

	DWORD AudioPlayer::GetVolume() {
		//int result = 0;
		//Volume::GetVoiceVolume(result);
		//return result;
		return static_cast<DWORD>(Info(MCI_DGV_STATUS_VOLUME));
	}

	void AudioPlayer::SetVolume(DWORD volume) {
		MCI_DGV_SETAUDIO_PARMS setVolumeParams;
		setVolumeParams.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		setVolumeParams.dwValue = volume;
		genericProcess(device_, static_cast<UINT>(MCI_SETAUDIO), static_cast<DWORD>(MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM), setVolumeParams);
	}

	void AudioPlayer::Mute() {
		MCI_SET_PARMS setParams;
		setParams.dwAudio = MCI_SET_AUDIO_ALL;
		genericProcess(device_, static_cast<UINT>(MCI_SETAUDIO), static_cast<DWORD>(MCI_WAIT | MCI_SET_AUDIO | MCI_SET_OFF), setParams);
	}

	void AudioPlayer::Unmute() {
		MCI_SET_PARMS setParams;
		setParams.dwAudio = MCI_SET_AUDIO_ALL;
		genericProcess(device_, static_cast<UINT>(MCI_SETAUDIO), static_cast<DWORD>(MCI_WAIT | MCI_SET_AUDIO | MCI_SET_ON), setParams, [] {});
	}
}
