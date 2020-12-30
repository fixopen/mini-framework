#include <Windows.h>

#include "comm.h"

namespace utils::Comm {
    // �򿪴���
    // ����: port - �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���
    //       baudRate - ������
    //       parity - ��żУ��
    //       byteSize - �����ֽڿ��
    //       stopBits - ֹͣλ
    HANDLE OpenComm(wchar_t *port, int baudRate, int parity, int byteSize, int stopBits) {
        HANDLE result = CreateFileW(port,             // �������ƻ��豸·��
                                   GENERIC_READ | GENERIC_WRITE,    // ��д��ʽ
                                   0,                // ����ʽ����ռ
                                   NULL,             // Ĭ�ϵİ�ȫ������
                                   OPEN_EXISTING,    // ������ʽ
                                   0,                // ���������ļ�����
                                   NULL);            // �������ģ���ļ�
        if (result != INVALID_HANDLE_VALUE) {
            DCB dcb;        // ���ڿ��ƿ�
            GetCommState(result, &dcb);        // ȡDCB
            dcb.BaudRate = baudRate;
            dcb.ByteSize = byteSize;
            dcb.Parity = parity;
            dcb.StopBits = stopBits;
            SetCommState(result, &dcb);        // ����DCB

            SetupComm(result, 4096, 1024);    // �������������������С

            COMMTIMEOUTS timeouts = {    // ���ڳ�ʱ���Ʋ���
                    100,                 // ���ַ������ʱʱ��: 100 ms
                    1,                   // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
                    500,                 // ������(�����)����ʱʱ��: 500 ms
                    1,                   // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
                    100                  // ������(�����)д��ʱʱ��: 100 ms
            };
            SetCommTimeouts(result, &timeouts);    // ���ó�ʱ
        }
        return result;
    }

    // �رմ���
    BOOL CloseComm(HANDLE serialPort) {
        return CloseHandle(serialPort);
    }

    // д����
    // ����: data - ��д�����ݻ�����ָ��
    //       length - ��д�����ݳ���
    // ����: ʵ��д������ݳ���
    DWORD WriteComm(HANDLE serialPort, void *data, int length) {
        DWORD writed = 0;    // ���ڷ��������ݳ���
        WriteFile(serialPort, data, (DWORD) length, &writed, NULL);
        return (int) writed;
    }

    // ������
    // ����: data - ���������ݻ�����ָ��
    //       length - ������������ݳ���
    // ����: ʵ�ʶ��������ݳ���
    DWORD ReadComm(HANDLE serialPort, void *data, int length) {
        DWORD readed = 0;    // �����յ������ݳ���
        ReadFile(serialPort, data, (DWORD) length, &readed, NULL);
        return (int) readed;
    }
}
