#pragma once

namespace utils::Volume {
    bool GetVoiceVolume(int &volume);

    //参数:
    //	-2 取消静音
    //	-1 静音
    //	0~100:音量比例
    bool SetVoiceVolume(int level);
}
