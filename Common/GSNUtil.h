#ifndef GSNUtilH
#define GSNUtilH

#include <windows.h>
#include <string.h>
#include <vector>
#include<iostream>

#define  _FILETIME_PER_SECOND_        1 * 10 * 1000 * 1000

#define  _SIZE_1K                    1024
#define  _SIZE_1M                    _SIZE_1K * 1024
#define     _SIZE_1G                    _SIZE_1M * 1024
#define     _SIZE_1T                    _SIZE_1G * 1024

#define arrayof(x)            ( sizeof((x)) / sizeof((x)[0]) )

namespace GSNUtil {
    // �ڴ�����ת16����
    unsigned long DataToHex(void* pMem, unsigned long ulMemSize, char* pHexStr, unsigned long ulHexSize);

    // �ڴ�ת16����
    std::string DataToHex(std::string strData);

    // 16����ת�ڴ�����
    unsigned long HexToData(char* pHexStr, unsigned long ulHexSize, void* pMem, unsigned long ulMemSize);

    // 16����ת�ڴ�
    std::string HexToData(std::string strHex);

    // ��ȡϵͳ���汾��
    int GetOsMajorVersion();

    // ��ȡϵͳ�ΰ汾��
    int GetOsMinorVersion();

    // ��ȡϵͳ�汾
    std::string GetOsVersion();

    // ���÷��ʿ���Ȩ��
    bool SetSecurityDescriptorByString(SECURITY_DESCRIPTOR* pSd, std::string strSdString = "");

    // ע���ļ�
    bool RegisterFile(std::string strFile, int nWait = INFINITE);

    bool RegisterFile(std::wstring strFile, int nWait = INFINITE);

    // ��ע���ļ�
    bool UnRegisterFile(std::string strFile, int nWait = INFINITE);

    bool UnRegisterFile(std::wstring strFile, int nWait = INFINITE);

    // ����Hash
    unsigned int BKDRHash(std::string strBuf);

    // ��������
    HANDLE
    RunProcess(std::string strExe, std::string strCmd = "", STARTUPINFOA* pSi = NULL, PROCESS_INFORMATION* pPi = NULL);

    // ��������
    unsigned long RunProcess2(std::string strExe, std::string strCmd = "", bool bHide = false, STARTUPINFOA* pSi = NULL,
                              PROCESS_INFORMATION* pPi = NULL);

    // ���ݽ���ID�жϽ����Ƿ����
    bool IsProcExist(unsigned long ulProcId);

    // ���ݽ��̾���жϽ����Ƿ����
    bool IsProcExistFromHandle(HANDLE hProc);

    // ���ݽ������жϽ����Ƿ����
    bool IsProcExist(std::string strProcName);

    // �˳�����
    bool KillProc(unsigned long ulProcId);

    // �˳�����
    bool KillProc(std::string strProcName);

    // ���г���
    bool RunProcess3(std::string strExe, std::string strCmd = "", bool bWait = false, bool bHide = false,
                     unsigned long* pulExitCode = NULL);

    // ���г���
    bool RunProcess5(std::string strExe, std::string strCmd = "", unsigned long ulWaitTime = 0, bool bHide = false,
                     unsigned long* pulExitCode = NULL);

    // ���г���
    bool RunProcess4(std::string strExe, std::string strCmd = "", std::string strCurDir = "",
                     bool bWait = false, bool bHide = false, unsigned long* pulExitCode = NULL);

    // ��̨���г���
    bool RunProcessBk(std::string strCmdLine, unsigned long* pExitCode, unsigned long ulTimeOut);

    // ���ݽ������ƻ�ȡ����ID
    bool GetProcIdFromName(std::string strProcName, std::vector<unsigned long>& vecProcId);

    // ���ݽ������ƻ�ȡ����ID
    unsigned long GetProcIdFromName(std::string strProcName);

    // ���tcp�˿��Ƿ�ռ��(true ��ռ��)
    bool CheckPortTcp(unsigned short usPort);

    // ���udp�˿��Ƿ�ռ��(true ��ռ��)
    bool CheckPortUdp(unsigned short usPort);

    //-------------Service

    // ��������
    bool CreateOneService(std::string strPath, std::string strName,
                          unsigned long ulServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
                          unsigned long ulStartType = SERVICE_AUTO_START,
                          std::string strDisplayName = "");

    // �������Ƿ����
    bool CheckServiceExist(std::string strName);

    // ��������
    bool StartUpService(std::string strName, int nWait = 10000);

    // ��ͣ����
    bool PauseService(std::string strName, bool bResume = false, int nWait = 10000);

    // ֹͣ����
    bool StopService(std::string strName, int nWait = 10000);

    // ɾ������
    bool DeleteService(std::string strName, int nWait = 10000);

    // �����Ƿ�����
    bool CheckServiceRunning(std::string strName);

    // ��ȡ����IP��ַ
    std::string GetLocalIp();

    // ����һ����������
    HANDLE GetJobObj();

    // �������̣����빤������
    unsigned long RunProcessEx(std::string strPath, std::string strCmd, HANDLE hJobObj,
                               bool bWait = false, bool bHide = false, int* pExitCode = NULL);

    // �Ƿ���64λϵͳ
    bool IsWow64();

    // �Ƿ�win7�����
    BOOL IsWin7OrGreater();

    // �û�����ԱȨ������
    bool RunAsAdmin(std::string strFile, std::string strCmd, bool bHide = false);
}

#endif
