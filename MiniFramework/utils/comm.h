#pragma once

#include <wtypes.h> // for HANDLE BOOL DWORD

namespace utils::Comm {
    HANDLE OpenComm(wchar_t *port, int baudRate, int parity, int byteSize, int stopBits);

    BOOL CloseComm(HANDLE serialPort);

    DWORD ReadComm(HANDLE serialPort, void *data, int length);

    DWORD WriteComm(HANDLE serialPort, void *data, int length);
}
