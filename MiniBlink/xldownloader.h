#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include"../ThunderOpenSDK/xldl.h"
#include "../Common/DllLoader.h"

using namespace WisdomUtils;

typedef std::function<void(void* lpParam, HANDLE hTask, DownTaskInfo stTaskInfo)> CallBackFunc;

class XLDownloader {
public:
    XLDownloader();

    ~XLDownloader();

public:
    bool initXunLei();

    bool downloadWithXL(std::wstring url, std::wstring filepath, std::wstring filename);

    // ���ûص�
    void setCallback(CallBackFunc callbackfuc, void* lpParam = nullptr);

private:

    //��ʼ���������棬��ʹ��Ϊ���������������׼����
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ�һЩ��������ᵼ�³�ʼ��ʧ�ܣ��磺�������ļ�ȱʧ��
    //˵�����ýӿڱ�����XL_Unint(void)�ɶԳ��֡����øýӿڵ�ʱ��xldl.dll������MiniThunderPlatform.exe�ӽ��̡�
    std::function<BOOL()> XL_Init;

    //����ʼ���������棬�ͷ�ģ�������ڼ��������Դ��
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ�
    //˵�������øýӿں󣬻�֪ͨMiniThunderPlatform.exe�ӽ����˳���
    std::function<BOOL()> XL_UnInit;

    //�������񡣿����½�����Ҳ�����������񣬲�����IsResume���ڱ�ʾ�����Ƿ�����������������Ϣ��ο�DownTaskParam�Ķ��塣
    //������[in] stParam������������Ҫ�Ĳ�����
    //DownTaskParam�ο���
    //struct DownTaskParam {
    //int nReserved1; TCHAR szTaskUrl[2084]; // ����URL��֧��http,https,ftp,ftps��ʽ TCHAR szRefUrl[2084]; // �ɿգ�HTTP�������ڵ���ҳURL TCHAR szCookies[4096]; // �ɿգ������cookie TCHAR szFilename[MAX_PATH]; // ���ر����ļ���. TCHAR szReserved[MAX_PATH]; TCHAR szSavePath[MAX_PATH]; // �ļ�����Ŀ¼ HWND hReserved; BOOL bReserved1; TCHAR szReserved1[64]; TCHAR szReserved2[64]; BOOL IsOnlyOriginal; //�Ƿ�ֻ��ԭʼ��ַ���� UINT nReserved2; BOOL bReserved2; BOOL IsResume; // �Ƿ������� DWORD reserved[2048]; }
    //����ֵ����������ľ����
    //˵�������񴴽����ǲ������Ͼ����صģ���Ҫ����XL_StartTask���첽ִ�С�
    std::function<HANDLE(DownTaskParam & stParam)> XL_CreateTask;

    //ɾ����ʱ�ļ�
    std::function<BOOL(DownTaskParam & stParam)> XL_DelTempFile;

    //��ʼ���ء����񴴽��ɹ��󣬲������Ͽ�ʼ���أ�����ô˽ӿڲŻῪʼ���ء�
    //������[in]hTask��������
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ������񲻴���ʱ�����û�ʧ�ܡ�
    //˵�����첽ִ��, ����������pending״̬���ܵ��øýӿڡ�
    std::function<BOOL(HANDLE hTask)> XL_StartTask;

    //���������ͷ������������ڼ��������Դ��
    //������[in]hTask��������
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ������񲻴���ʱ�����û�ʧ�ܡ�
    //˵������������ͣ���ɹ��Լ�ɾ�������ʱ����Ҫ���øýӿڣ���֤��Դ������ͷš�
    //�첽ִ��, ����������pending״̬���ܵ��øýӿڡ�
    std::function<BOOL(HANDLE hTask)> XL_DeleteTask;

    //ֹͣ����
    //������[in]hTask��������
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ������񲻴���ʱ�����û�ʧ�ܡ�
    //˵������ȡ������Ϣ��ʹ����ѯ��ʽ�ģ��������������˶�������������ֹͣ���غ��ѯ������ȷ�ġ�
    //���ú����񲻻�����ֹͣ������ѯ������״̬ΪPAUSE֮�󣬲��ܻ�ȡ����
    std::function<BOOL(HANDLE hTask)> XL_StopTask;

    //ǿ����ͣ����

    //	������[in] hTask��������
    //	����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ������񲻴���ʱ�����û�ʧ��
    //	˵�����ڿ���UAC���ش��ļ�ʱ������XL_StopTask������Ҫ���ıȽϳ���ʱ�䡣
    //��������������Ҫ������ͣ���񣬿��Ե���XL_ForceStopTask��
    //�����п��ܵ����Ѿ����صĻ�ûд�����̵����ݶ�ʧ��
    std::function<BOOL(HANDLE hTask)> XL_ForceStopTask;

    //��ѯ����ǰ��Ϣ��
    //������[in]hTask��������[out] stTaskInfo, ������Ϣ��
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ�
    //˵����IsOriginUsable��ԭʼ��Դ�Ƿ���ã���һ�������͵Ĳ���
    //	����ԭʼ����ԭʼ��Դ��һ����ʱ�Ĳ�������ζ���м��ж�ʱ��ԭʼ��Դ�Ƿ���Ч��δ֪�ġ�
    //��������ĳ�ʼ̬��false�����Ծʹ���ԭʼ��Դ����Ч����������տ�ʼʱ���������false�����⡣
    //�ϲ���ʹ�ø�ֵ��ʱ����Ҫ����ҵ���߼���������
    std::function<BOOL(HANDLE hTask, DownTaskInfo& stTaskInfo)> XL_QueryTaskInfoEx;

    //�򵥷�װ��XL_CreateTask�ӿ�
    std::function<HANDLE(const wchar_t* url, const wchar_t* path, const wchar_t* fileName, BOOL IsResume)> XL_CreateTaskByURL;

    //������������ٶȡ�
    //������[in]nKBps, �ٶ�����ֵ
    //˵�����첽ִ��
    std::function<void(INT32 nKBps)> XL_SetSpeedLimit;

    //�����ϴ��ٶ�
    //�ɸ���ʵ����������������������ϴ��ٶȡ�
    //	������[in] nTcpKBps, �����ϴ��ٶȣ���λΪKB / s[in] nOtherKBps, �����ϴ��ٶȣ���λΪKB / s
    //	����ֵ����
    std::function<void(INT32 nTcpKBps, INT32 nOtherKBps)> XL_SetUploadSpeedLimit;

    //�������ش���ȫ�����������Ϣ��
    //	������[in]stProxyIfno ������Ϣ
    //����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ������Ƿ�����FALSE
    std::function<BOOL(DOWN_PROXY_INFO & stProxyInfo)> XL_SetProxy;

    //������ԭʼ��Դ��Http���������UserAgent��
    //	������[in]pszUserAgent���ͻ���UserAgent�ַ���
    //	˵�����ڵ��ø÷����󴴽�������ĲŻ�ʹ���µ�UserAgent��
    std::function<void(const wchar_t* pszUserAgent)> XL_SetUserAgent;

    //�򵥷�װ�˽�Ѹ��ר����ת��ʵ��URL
    //Ѹ��ר�����Ǹ���һ���߼�����ͨURLת�ɵġ�
    //	������[in] pszThunderUrl��Ѹ��ר����URL[out] normalUrlBuffer���洢ת�������ͨURL�Ļ�����[out] bufferLen��normalUrlBuffer�Ļ�������С����λ�ַ���
    //	����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
    //	˵�����˺������ܶ���������������û��������ϵ������ʱ����ʹ��
    std::function<BOOL(const wchar_t* pszThunderUrl, wchar_t* normalUrlBuffer, INT32 bufferLen)> XL_ParseThunderPrivateUrl;

    //��ȡ���ص��ļ���С
    std::function<BOOL(const wchar_t* lpURL, INT64& iFileSize)> XL_GetFileSizeWithUrl;

    std::function<BOOL(HANDLE hTask, char szFileId[40], unsigned __int64 nFileSize)> XL_SetFileIdAndSize;

    std::function<BOOL(HANDLE task_id, WSAPROTOCOL_INFOW* sock_info, CHAR* http_resp_buf, LONG buf_len)> XL_SetAdditionInfo;

    // ����Ѹ��7������������
    //ͨ������URL���ļ�������Ϣ����Ѹ��7���½���崴����������
    //	������[in] pszUrl������URL[in] pszFileName�����ر�����ļ���[in] pszReferUrl������ҳURL[in] pszCharSet����ǰ��ҳ���ַ���[in] pszCookie���������������cookie
    //	����ֵ��0�ɹ�������Ϊʧ��
    std::function<LONG(wchar_t * pszUrl, wchar_t * pszFileName, wchar_t * pszReferUrl, wchar_t * pszCharSet,
                       wchar_t * pszCookie)> XL_CreateTaskByThunder;

    std::function<LONG(const wchar_t* pszPath)> XL_CreateBTTaskByThunder;
private:
    WisdomUtils::CDllLoader* pDllloader;
    void* m_pCallbackParam; // �ص�����ָ��
    CallBackFunc m_callbackfuc; // �ص�����
    std::thread* m_QueryTaskInfoThread;
    std::atomic <BOOL> m_bRun;
    std::vector <HANDLE> m_vecTask;
};
