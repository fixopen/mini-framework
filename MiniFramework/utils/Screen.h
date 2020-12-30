#pragma once

#include <WinUser.h> // for GetSystemMetrics

namespace utils::Screens {
    // ÆÁÄ»³ß´ç
    inline int constexpr const ORG_WIDTH = 1920;
    inline int constexpr const ORG_HEIGHT = 1080;

    // inline int constexpr const SCREEN_WIDTH = 1920;
    // inline int constexpr const SCREEN_HEIGHT = 1080;

    inline int REAL_WIDTH;
    inline int REAL_HEIGHT;

    inline void Init() {
        REAL_WIDTH = GetSystemMetrics(SM_CXSCREEN);
        REAL_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    }

    inline constexpr int wScale(int x) {
        return x / ORG_WIDTH;
    }

    inline constexpr int hScale(int y) {
        return y / ORG_HEIGHT;
    }

    inline constexpr int GetScaleX(int x) {
        return x * ORG_WIDTH / REAL_WIDTH;
    }

    inline constexpr int GetScaleY(int y) {
        return y * ORG_HEIGHT / REAL_HEIGHT;
    }

    inline constexpr int SetScaleX(int x) {
        return x * wScale(REAL_WIDTH);
    }

    inline constexpr int SetScaleY(int y) {
        return y * hScale(REAL_HEIGHT);
    }

    void SetScreenNet(int type);
}
