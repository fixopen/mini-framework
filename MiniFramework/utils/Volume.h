#pragma once

namespace utils::Volume {
    bool GetVoiceVolume(int &volume);

    //����:
    //	-2 ȡ������
    //	-1 ����
    //	0~100:��������
    bool SetVoiceVolume(int level);
}
