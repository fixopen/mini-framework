#include <Windows.h>

#include "comm.h"

namespace utils::Comm {
    // 打开串口
    // 输入: port - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者
    //       baudRate - 波特率
    //       parity - 奇偶校验
    //       byteSize - 数据字节宽度
    //       stopBits - 停止位
    HANDLE OpenComm(wchar_t *port, int baudRate, int parity, int byteSize, int stopBits) {
        HANDLE result = CreateFileW(port,             // 串口名称或设备路径
                                   GENERIC_READ | GENERIC_WRITE,    // 读写方式
                                   0,                // 共享方式：独占
                                   NULL,             // 默认的安全描述符
                                   OPEN_EXISTING,    // 创建方式
                                   0,                // 不需设置文件属性
                                   NULL);            // 不需参照模板文件
        if (result != INVALID_HANDLE_VALUE) {
            DCB dcb;        // 串口控制块
            GetCommState(result, &dcb);        // 取DCB
            dcb.BaudRate = baudRate;
            dcb.ByteSize = byteSize;
            dcb.Parity = parity;
            dcb.StopBits = stopBits;
            SetCommState(result, &dcb);        // 设置DCB

            SetupComm(result, 4096, 1024);    // 设置输入输出缓冲区大小

            COMMTIMEOUTS timeouts = {    // 串口超时控制参数
                    100,                 // 读字符间隔超时时间: 100 ms
                    1,                   // 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
                    500,                 // 基本的(额外的)读超时时间: 500 ms
                    1,                   // 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
                    100                  // 基本的(额外的)写超时时间: 100 ms
            };
            SetCommTimeouts(result, &timeouts);    // 设置超时
        }
        return result;
    }

    // 关闭串口
    BOOL CloseComm(HANDLE serialPort) {
        return CloseHandle(serialPort);
    }

    // 写串口
    // 输入: data - 待写的数据缓冲区指针
    //       length - 待写的数据长度
    // 返回: 实际写入的数据长度
    DWORD WriteComm(HANDLE serialPort, void *data, int length) {
        DWORD writed = 0;    // 串口发出的数据长度
        WriteFile(serialPort, data, (DWORD) length, &writed, NULL);
        return (int) writed;
    }

    // 读串口
    // 输入: data - 待读的数据缓冲区指针
    //       length - 待读的最大数据长度
    // 返回: 实际读出的数据长度
    DWORD ReadComm(HANDLE serialPort, void *data, int length) {
        DWORD readed = 0;    // 串口收到的数据长度
        ReadFile(serialPort, data, (DWORD) length, &readed, NULL);
        return (int) readed;
    }
}
