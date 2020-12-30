#include <Windows.h>

#include "Screen.h"

//#include <wingdi.h> // for DEVMODE
//#include <WinUser.h> // for ChangeDisplaySettings

namespace utils::Screens {
    void SetScreenNet(int type) {
        if (wScale(REAL_WIDTH) == 1 && hScale(REAL_HEIGHT) == 1) {
            DEVMODE devMode;
            devMode.dmBitsPerPel = 32;
            if (type == 0) {
                devMode.dmPelsWidth = 1280;
                devMode.dmPelsHeight = 800;
            } else {
                devMode.dmPelsWidth = 1920;
                devMode.dmPelsHeight = 1080;
            }

            devMode.dmSize = sizeof(devMode);
            devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
            LONG result = ChangeDisplaySettings(&devMode, 0);
            if (result == DISP_CHANGE_SUCCESSFUL) {
                ChangeDisplaySettings(&devMode, CDS_UPDATEREGISTRY); // 使用CDS_UPDATEREGISTRY表示次修改是持久的，并在注册表中写入了相关的数据
            } else {
                ChangeDisplaySettings(NULL, 0);
            }
        }
    }
}
