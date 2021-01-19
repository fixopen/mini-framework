#include "StdAfx.h"
#include <exdisp.h>
#include <comdef.h>
#include <ShellAPI.h>

#include "MainWnd.h"
#include "resource.h"
#include "utils/SqlData.h"
#include "utils/Folders.h"
#include "utils/Texts.h"
#include "utils/WebServer.h"
#include "utils/zip.h"

void InitResource() {
    // ��Դ����
#ifdef _DEBUG
    CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
    CPaintManagerUI::SetResourceType(UILIB_FILE);
    // CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
    // ��Դ·��
    CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
    // ������Դ
    switch (CPaintManagerUI::GetResourceType()) {
    case UILIB_FILE:
    {
        strResourcePath += _T("Skin\\");
        CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
        // ������Դ������
        CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
        break;
    }
    case UILIB_RESOURCE:
    {
        strResourcePath += _T("Skin\\");
        CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
        // ������Դ������
        CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
        break;
    }
    case UILIB_ZIP:
    {
        strResourcePath += _T("Skin\\");
        CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
        CPaintManagerUI::SetResourceZip(_T("WkeBrowser.zip"), true);
        // ������Դ������
        CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
        break;
    }
    case UILIB_ZIPRESOURCE:
    {
        strResourcePath += _T("Skin\\");
        CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

        HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
        if (hResource != NULL) {
            DWORD dwSize = 0;
            HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
            if (hGlobal != NULL) {
                dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
                if (dwSize > 0) {
                    CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
                    // ������Դ������
                    CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                }
            }
            ::FreeResource(hResource);
        }
    }
    break;
    }
    // ע��ؼ�
    REGIST_DUICONTROL(CWkeWebkitUI);
}

bool isOneInstance() {
    HANDLE mutex = CreateMutexW(NULL, TRUE, L"Miniblink browser");
    if ((mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) {
        ReleaseMutex(mutex);
        return false;
    }
    return true;
}

//#include <afx.h>

void zipNewInstaller(int organizationId) {
    // CWnd window;
    std::string newData = u8R"json({
        "name": "awd",
        "object": "a",
        "no": "awd",
        "abstract": "dwwad",
        "description": "",
        "control_file_id": null,
        "operator": "awd",
        "����id": 1,
        "time": 1608082658301
    })json";
    nlohmann::json data = nlohmann::basic_json<>::parse(newData);
    std::int64_t id = utils::SqlData::insert(L"histories", data);
}

void rwe(std::wstring const& fileName, std::wstring const& sheetname, std::wstring const& json);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow) {
    if (!isOneInstance()) {
        ::MessageBoxW(NULL, L"�ý����Ѿ�����", L"����", MB_OK);
        return 0;
    }
    // zipNewInstaller(1);
    // utils::SqlData::constructTestData();
    // void reallyWriteExcel(std::wstring const& fileName, std::wstring const& sheetname, std::wstring const& json);
    // rwe(L"abc", L"xyz", L"[\"id\",\"����\",\"�ͺ�\",\"�汾\"]");
    auto webServer = utils::WebServer("0.0.0.0", 8888, utils::Texts::toNative(utils::Folders::GetCurrentPath()));
    // ��ʼ��
    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;
    HRESULT hRes = ::OleInitialize(NULL);
    // ��ʼ��UI������
    CPaintManagerUI::SetInstance(hInstance);

    CWkeWebkitUI::InitializeWebkit();

    // utils::SqlData::InsertFile("C:\\Users\\fixop\\source\\repos\\productmanage\\AirPlane\\utils\\db\\air.db", "C:\\Users\\fixop\\source\\repos\\productmanage\\AirPlane\\utils\\db\\shell.c");
    // ��ʼ����Դ
    InitResource();

    // ����������
    CMainWnd* pMainWnd = new CMainWnd();
    if (pMainWnd == NULL) return 0;
    pMainWnd->Create(NULL, _T("�ɻ���������״̬����ϵͳ"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pMainWnd->CenterWindow();

    // ����exeͼ��
    //HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
    HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_N));
    ::SendMessage(pMainWnd->GetHWND(), STM_SETICON, IMAGE_ICON, (LPARAM)(UINT)hIcon);
    ::SendMessage(pMainWnd->GetHWND(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    // ������Ϣѭ��
    CPaintManagerUI::MessageLoop();

    // �ͷ���Դ
    CResourceManager::GetInstance()->Release();
    OleUninitialize();
    ::CoUninitialize();

    // CWkeWebkitUI::UninitializeWebkit();

    return 0;
}
