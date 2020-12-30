#pragma once

#include <WTypes.h>
#include <mmsystem.h>

namespace utils {
	class Mixer {
	public:
		// MIXERLINE_COMPONENTTYPE_SRC_LINE
		// MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE
		// MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
		// MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER
		// MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC
		// MIXERLINE_COMPONENTTYPE_DST_WAVEIN
		// MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
		BOOL Open(DWORD type = MIXERLINE_COMPONENTTYPE_SRC_LINE, HWND callbackWindow = nullptr);
		BOOL GetVolume(DWORD& volume);
		BOOL SetVolume(DWORD volume);
		BOOL IsMute(BOOL& mute);
		BOOL SetMute(BOOL mute);
	private:
		HMIXEROBJ hMixer_ = nullptr;
		DWORD muteControlId_ = 0;
		DWORD volumeControlId_ = 0;
		DWORD minVolume_ = 0;
		DWORD maxVolume_ = 0;
		//DWORD signedMeterControlId_ = 0;
	};
}
