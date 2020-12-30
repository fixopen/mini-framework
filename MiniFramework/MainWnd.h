#pragma once

//////////////////////////////////////////////////////////////////////////
///
//#include "TroyControls.h"
#include "WkeWebkit.h"
#include "TagControl.h"

typedef struct _tagTabInfo {
    UINT64 nID;
    TCHAR szUrl[2048];
    TCHAR szTitle[256];
    TCHAR szFavUrl[2048];
    TCHAR szFavName[256];
    CBrowserTabUI* pTab;

    CWkeWebkitUI* pWebBrowser;
    ~_tagTabInfo() {
        if (pWebBrowser != NULL) {
            pWebBrowser->Close();
            pWebBrowser = NULL;
        }
        if (pTab != NULL) {
            pTab->RemoveAll();
            delete pTab;
            pTab = NULL;
        }
    }
} TabInfo;

class CDownloadWnd;
class XLDownloader;

class CMainWnd : public WindowImplBase, public IWkeCallback {
public:
    CMainWnd(void);
    ~CMainWnd(void);

public:
    // ��ʼ����Դ������
    virtual void InitWindow();

    virtual void OnFinalMessage(HWND);
    virtual CDuiString GetSkinFile();
    virtual LPCTSTR GetWindowClassName(void) const;

    virtual CControlUI* CreateControl(LPCTSTR pstrClass);

    DUI_DECLARE_MESSAGE_MAP()

    virtual void OnClick(TNotifyUI& msg);
    virtual void OnSelectChanged(TNotifyUI& msg);
    virtual void OnTabIndexChanged(TNotifyUI& msg);
    virtual void OnTabClosed(TNotifyUI& msg);
    virtual void OnReturn(TNotifyUI& msg);

    virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    int CreateNewTab(int nIndex, LPCTSTR pstrUrl = NULL);
    int CreateNewTabAndGo(LPCTSTR pstrUrl = NULL);
    CWkeWebkitUI* GetCurWeb();
    void AddressGo();
    void SearchGo();
    void Back();
    void Forward();
    void Home();
    void Refresh();

public:
    virtual void OnWkeTitleChanged(CWkeWebkitUI* webView, LPCTSTR title);
    virtual void OnWkeURLChanged(CWkeWebkitUI* webView, LPCTSTR url);
    virtual void OnWkeAlertBox(CWkeWebkitUI* webView, LPCTSTR msg);
    virtual bool OnWkeNavigation(CWkeWebkitUI* webView, wkeNavigationType navigationType, LPCTSTR url);

    virtual wkeWebView OnWkeCreateView(CWkeWebkitUI* webView, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);
    virtual void OnWkeDocumentReady(CWkeWebkitUI* webView);

    //�κ�����������ǰ�ᴥ���˻ص�
    virtual bool  onLoadUrlBegin(CWkeWebkitUI* webView, void* param, const char* url, void* job);

    virtual void OnWkeLoadingFinish(CWkeWebkitUI* webView, const LPCTSTR url, wkeLoadingResult result, LPCTSTR failedReason);

    virtual LPCTSTR OnJS2Native(CWkeWebkitUI* pWeb, LPCTSTR lpMethod, LPCTSTR lpContent, void* pListenObj);

    //ҳ�������¼��ص������ĳЩ���ӣ��������ػ����
    virtual bool  OnWkeDownload(CWkeWebkitUI* webView, const char* url);

    //��ȡfavicon��
    virtual void  OnWkeNetGetFavicon(CWkeWebkitUI* webView, const char* url, wkeMemBuf* buf);

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    CButtonUI* m_pMenuBtn;
    // CEditUI* m_pAddressEdit;
    // CEditUI* m_pSearchEdit;
    CTabLayoutUI* m_pBrowserTabBody;
    // CTabLayoutUI* m_pModeMainTab;
    // std::vector<TabInfo*> m_vTabs;
    // int m_nTabID;
    // CDownloadWnd* m_pDownloadWnd;
    // XLDownloader* m_pDownloader;
    // CHorizontalLayoutUI* m_pBrowserTabBar;
    CWkeWebkitUI* pWeb;
};
