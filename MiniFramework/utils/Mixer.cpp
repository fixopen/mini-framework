#include "Mixer.h"

#pragma comment(lib, "winmm.lib")

namespace utils {
	BOOL Mixer::Open(DWORD type, HWND callbackWindow) {
		BOOL result = FALSE;
		if (hMixer_) {
			mixerClose(reinterpret_cast<HMIXER>(hMixer_));
			hMixer_ = nullptr;
		}
		if (mixerGetNumDevs() > 0) {
			MMRESULT r = mixerOpen(reinterpret_cast<LPHMIXER>(&hMixer_), 0, reinterpret_cast<DWORD_PTR>(callbackWindow), 0, MIXER_OBJECTF_MIXER | CALLBACK_WINDOW);
			if (r == MMSYSERR_NOERROR) {
				MIXERLINE mixerLine = {};
				mixerLine.cbStruct = sizeof(MIXERLINE);
				mixerLine.dwComponentType = type;
				r = mixerGetLineInfo(hMixer_, &mixerLine, MIXER_OBJECTF_MIXER | MIXER_GETLINEINFOF_COMPONENTTYPE);
				if (r == MMSYSERR_NOERROR) {
					MIXERLINECONTROLS mixerLineControls = {};
					mixerLineControls.cbStruct = sizeof(MIXERLINECONTROLS);
					mixerLineControls.dwLineID = mixerLine.dwLineID;
					mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
					mixerLineControls.cControls = 1;
					mixerLineControls.cbmxctrl = sizeof(MIXERCONTROL);
					MIXERCONTROL mixerControl = {};
					mixerControl.cbStruct = sizeof(MIXERCONTROL);
					mixerLineControls.pamxctrl = &mixerControl;
					r = mixerGetLineControls(hMixer_, &mixerLineControls, MIXER_OBJECTF_MIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
					if (r == MMSYSERR_NOERROR) {
						minVolume_ = mixerControl.Bounds.dwMinimum;
						maxVolume_ = mixerControl.Bounds.dwMaximum;
						volumeControlId_ = mixerControl.dwControlID;
						result = TRUE;
					}
					mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
					r = mixerGetLineControls(hMixer_, &mixerLineControls, MIXER_OBJECTF_MIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
					if (r == MMSYSERR_NOERROR) {
						muteControlId_ = mixerControl.dwControlID;
						result = TRUE;
					}
					//mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_SIGNEDMETER;
					//r = mixerGetLineControls(hMixer_, &mixerLineControls, MIXER_OBJECTF_MIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
					//if (r == MMSYSERR_NOERROR) {
					//	signedMeterControlId_ = mixerControl.dwControlID;
					//	result = TRUE;
					//}
				}
			}
		}
		return result;
	}

	BOOL Mixer::GetVolume(DWORD& volume) {
		BOOL result = FALSE;
		MIXERCONTROLDETAILS mixerControlDetails = {};
		mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mixerControlDetails.dwControlID = volumeControlId_;
		mixerControlDetails.cChannels = 1;
		mixerControlDetails.cMultipleItems = 0;
		mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		MIXERCONTROLDETAILS_UNSIGNED value = {};
		mixerControlDetails.paDetails = &value;
		auto r = mixerGetControlDetails(hMixer_, &mixerControlDetails, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
		if (r == MMSYSERR_NOERROR) {
			volume = value.dwValue;
			result = TRUE;
		}
		return result;
	}

	BOOL Mixer::SetVolume(DWORD volume) {
		BOOL result = FALSE;
		MIXERCONTROLDETAILS mixerControlDetails = {};
		mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mixerControlDetails.dwControlID = volumeControlId_;
		mixerControlDetails.cChannels = 1;
		mixerControlDetails.cMultipleItems = 0;
		mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		MIXERCONTROLDETAILS_UNSIGNED value = {volume};
		mixerControlDetails.paDetails = &value;
		auto r = mixerSetControlDetails(hMixer_, &mixerControlDetails, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
		if (r == MMSYSERR_NOERROR) {
			result = TRUE;
		}
		return result;
	}

	BOOL Mixer::IsMute(BOOL& mute) {
		BOOL result = FALSE;
		MIXERCONTROLDETAILS mixerControlDetails = {};
		mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mixerControlDetails.dwControlID = muteControlId_;
		mixerControlDetails.cChannels = 1;
		mixerControlDetails.cMultipleItems = 0;
		mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		MIXERCONTROLDETAILS_BOOLEAN value = {};
		mixerControlDetails.paDetails = &value;
		auto r = mixerGetControlDetails(hMixer_, &mixerControlDetails, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
		if (r == MMSYSERR_NOERROR) {
			mute = value.fValue;
			result = TRUE;
		}
		return result;
	}

	BOOL Mixer::SetMute(BOOL mute) {
		BOOL result = FALSE;
		MIXERCONTROLDETAILS mixerControlDetails = {};
		mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mixerControlDetails.dwControlID = muteControlId_;
		mixerControlDetails.cChannels = 1;
		mixerControlDetails.cMultipleItems = 0;
		mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		MIXERCONTROLDETAILS_BOOLEAN value = {mute};
		mixerControlDetails.paDetails = &value;
		auto r = mixerSetControlDetails(hMixer_, &mixerControlDetails, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
		if (r == MMSYSERR_NOERROR) {
			result = TRUE;
		}
		return result;
	}
}
