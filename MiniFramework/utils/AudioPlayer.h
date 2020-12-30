#pragma once

#include <string>
#include <wtypes.h>
#include <MMSystem.h> // for MCIDEVICEID
// #include <mciapi.h>

namespace utils {
	class AudioPlayer {
	public:
		AudioPlayer(HWND window): window_(window){}
		void Open(std::wstring const& filename);
		DWORD_PTR Info(DWORD item);
		void Play();
		void Pause();
		void Resume();
		void Stop();
		void Close();
		void SeekTo(DWORD position);
		DWORD GetVolume();
		void SetVolume(DWORD volume);
		void Mute();
		void Unmute();
	private:
		HWND window_ = nullptr;
		MCIDEVICEID device_ = 0;
	};
}
