#pragma once
#include <map>
#include "../MiniBlink/wke.h"

enum JS_RESULT_TYPE {
    JS_UNDEFINED,
    JS_INT,
    JS_DOUBLE,
    JS_CHAR,
    JS_WCHAR,
    JS_BOOL
};

struct JS_ARG {
    const char* js;
    void* result;
    JS_RESULT_TYPE type;
    void* param;
    wkeWebFrameHandle frameId;
    wkeWebView webView;
};

enum MB_ACTION_SENDER {
    MOUSE,
    MENU,
    WHEEL,
    KEY
};

struct MB_ACTION_MOUSE_DATA {
    unsigned message;
    int x;
    int y;
    unsigned flags;
};

struct MB_ACTION_MENU_DATA {
    int x;
    int y;
    unsigned flags;
};

struct MB_ACTION_WHEEL_DATA {
    int delta;
    int x;
    int y;
    unsigned flags;
};

enum MB_ACTION_KEY_EVENT {
    UP,
    DOWN,
    PRESS
};

struct MB_ACTION_KEY_DATA {
    MB_ACTION_KEY_EVENT event;
    unsigned code;
    unsigned flags;
    bool systemKey;
};

struct MB_ACTION_ITEM {
    MB_ACTION_SENDER sender;
    void* data = NULL;
    bool async = false;
    ~MB_ACTION_ITEM() {
        if (data != NULL) {
            if (sender == MB_ACTION_SENDER::KEY) {
                MB_ACTION_KEY_DATA* d = (MB_ACTION_KEY_DATA*) data;
                delete d;
                d = NULL;
                data = NULL;
            } else if (sender == MB_ACTION_SENDER::MENU) {
                MB_ACTION_MENU_DATA* d = (MB_ACTION_MENU_DATA*) data;
                delete d;
                d = NULL;
                data = NULL;
            } else if (sender == MB_ACTION_SENDER::MOUSE) {
                MB_ACTION_MOUSE_DATA* d = (MB_ACTION_MOUSE_DATA*) data;
                delete d;
                d = NULL;
                data = NULL;
            } else if (sender == MB_ACTION_SENDER::WHEEL) {
                MB_ACTION_WHEEL_DATA* d = (MB_ACTION_WHEEL_DATA*) data;
                delete d;
                d = NULL;
                data = NULL;
            }
        }
    }
};

struct MB_ACTION {
    MB_ACTION_ITEM* action = NULL;
    MB_ACTION* next = NULL;
    int sleep = 0;
    ~MB_ACTION() {
        if (action != NULL) {
            delete action;
            action = NULL;
        }
        if (next != NULL) {
            delete next;
            next = NULL;
        }
    }
};

typedef struct _rend_data {
    RECT rt; //��Ⱦ�ľ�������
    void* pixels; //��Ⱦ���ڴ�����
    HDC hDC; //�ڴ��豸
    HBITMAP hBitmap; //λͼ

    _rend_data() {
        memset(&rt, 0, sizeof(RECT));
        pixels = NULL;
        hDC = ::CreateCompatibleDC(0);
        hBitmap = NULL;
    }

    ~_rend_data() {
        if (hDC)
            DeleteDC(hDC);

        if (hBitmap)
            DeleteObject(hBitmap);
    }
} REND_DATA, * PRENDDATA;

class CWkeWebkitUI;

class IWkeCallback {
public:
    virtual void OnWkeTitleChanged(CWkeWebkitUI* webView, LPCTSTR title) {}
    virtual void OnWkeURLChanged(CWkeWebkitUI* webView, LPCTSTR url) {}
    virtual void OnWkeAlertBox(CWkeWebkitUI* webView, LPCTSTR msg) {}
    virtual bool OnWkeNavigation(CWkeWebkitUI* webView, wkeNavigationType navigationType, LPCTSTR url) { return false; }

    virtual wkeWebView OnWkeCreateView(CWkeWebkitUI* webView, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures) { return NULL; }

    virtual void OnWkeDocumentReady(CWkeWebkitUI* webView) {}

    virtual bool  onLoadUrlBegin(CWkeWebkitUI* webView, void* param, const char* url, void* job) { return false; }
    //ҳ�������¼��ص������ĳЩ���ӣ��������ػ����
    virtual bool  OnWkeDownload(CWkeWebkitUI* webView, const char* url) { return false; }

    virtual void OnWkeLoadingFinish(CWkeWebkitUI* webView, const LPCTSTR url, wkeLoadingResult result, LPCTSTR failedReason) {}

    virtual LPCTSTR OnJS2Native(CWkeWebkitUI* webView, LPCTSTR lpMethod, LPCTSTR lpContent, void* pListenObj) { return NULL; }

    virtual void  OnWkeNetGetFavicon(CWkeWebkitUI* webView, const char* url, wkeMemBuf* buf) {};
};

class CWkeWebkitUI : public CControlUI, public IMessageFilterUI {
public:
    DECLARE_DUICONTROL(CWkeWebkitUI)

public:
    CWkeWebkitUI(void);
    ~CWkeWebkitUI(void);

protected:
    virtual LPCTSTR	GetClass() const;

    virtual LPVOID GetInterface(LPCTSTR pstrName);

    //���ؼ�����ӵ����������������õĺ�����
    //���������Duilib�������Ѿ���ɣ�����Ҫ��һЩ����ĳ�ʼ����ʱ������д�˺�����
    //�������÷������ڴ˽���Win32�Ӵ��岢�ҷ�װ����
    virtual void DoInit();

    virtual void SetPos(RECT rc, bool bNeedUpdate = true);
    //�ؼ��ĺ��ĺ��������ǿؼ��Ļ��ƴ���������Duilib�ײ�Ҫ���»�������ؼ���
    //���߿ؼ��Լ�����Invalidata����ǿ���Լ�ˢ��ʱ����������ͻᱻ������
    //���������������˸���״̬�µı���ǰ�����ƣ�����ɫ���ƣ��ı����ƣ��߿���ơ���������������PaintBkColor��PaintBkImage��PaintStatusImage��PaintText��PaintBorder�Ⱥ�������ɸ������Ʋ��衣��������Ը���������������дDoPaint����ֻ��дĳ����PaintXXX������DoPaint����������DoEvent�������ʹ�ã�DoEvent�õ���ĳ����Ϣ�󣬸ı�ؼ���״̬��Ȼ�����Invalidate�����ÿؼ��ػ档
    virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    //�ؼ��ĺ��ĺ�����������Ϣ����������������Duilib��װ���ĸ�����Ϣ��
    //�������������Ƴ������ֵ���ͣ������˫�����һ������ֻ�������ȡ���㡢���ù��ȵȡ�
    //���������Ŀؼ���Ҫ�޸���Щ��Ϊ��������д�������������Ĵ��������Բο�Duilib���еĿؼ���
    virtual void DoEvent(TEventUI& event);

    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:
    //��ʼ��webkit
    static void InitializeWebkit();

    //����ʼ��webkit
    static void UninitializeWebkit();

    //ִ��js����
    void ExecuteJS(LPCTSTR lpJS);

    //���������ʽ
    void updateCursor();

public:
    wkeWebView GetWebView();

    //����url
    void Navigate(LPCTSTR lpUrl);

    //����html����
    void LoadHtml(LPCTSTR lpHtml);

    //�ر�webkit����
    void Close();

    //ҳ����������ˡ�ǰ����ֹͣ��ˢ��
    void Back();
    //ҳ��ǰ��
    void Forward();
    //ֹͣ����
    void Stop();
    //ҳ�������ˢ��
    void Reload();

    //������ҳ
    void SetHomePage(LPCTSTR lpHomeUrl);

    //�����ҳ
    void NavigateHomePage();

    //����ҳ�潹��
    void SetPageFocus();

public:
    //���ü�������
    void SetListenObj(void* pListenObj);

    // ���ûص��ӿ�
    void SetWkeCallback(IWkeCallback* pWkeCallback);

    //����������ԭ����ҳ�����ʧ�ܺ����ʾҳ��
    void SetErrorInfoPageUrl(LPCTSTR lpErrUrl);

private:
    // ���ñ���仯��֪ͨ�ص�
    static void WKE_CALL_TYPE OnWkeTitleChanged(wkeWebView webView, void* param, wkeString title);

    //url�ı�ص�
    static void WKE_CALL_TYPE OnWkeURLChanged(wkeWebView webView, void* param, wkeString url);

    // ��ҳ����alert���ߵ�����ӿ�����Ļص�
    static void WKE_CALL_TYPE OnWkeAlertBox(wkeWebView webView, void* param, wkeString msg);

    //wkeNavigationCallback�ص��ķ���ֵ�������true����ʾ���Լ������������false��ʾ��ֹ���������
    static bool WKE_CALL_TYPE OnWkeNavigation(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url);

    //��ҳ���a��ǩ�����´���ʱ�������ص�
    static wkeWebView(WKE_CALL_TYPE OnWkeCreateView)(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);

    static void WKE_CALL_TYPE OnWkeDocumentReady(wkeWebView webView, void* param);
    //�κ�����������ǰ�ᴥ���˻ص�
    static bool WKE_CALL_TYPE onLoadUrlBegin(wkeWebView webView, void* param, const char* url, void* job);
    //������ɻص�
    static void WKE_CALL_TYPE OnWkeLoadingFinish(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);

    //ҳ�������¼��ص������ĳЩ���ӣ��������ػ����
    static bool  WKE_CALL_TYPE  OnWkeDownload(wkeWebView webView, void* param, const char* url);

    //����һ��js�뱾�صĺ���ӳ��
    static jsValue JS_CALL JsToNative(jsExecState es);
    static jsValue JS_CALL Select(jsExecState es);
    static jsValue JS_CALL Insert(jsExecState es);
    static jsValue JS_CALL Update(jsExecState es);
    static jsValue JS_CALL Remove(jsExecState es);
    static jsValue JS_CALL FillFileContent(jsExecState es);
    static jsValue JS_CALL GetFileContent(jsExecState es);
    static jsValue JS_CALL Backup(jsExecState es);
    static jsValue JS_CALL Restore(jsExecState es);
    static jsValue JS_CALL ImportPackage(jsExecState es);
    static jsValue JS_CALL ExportPackage(jsExecState es);
    static jsValue JS_CALL ExportTemplatePackage(jsExecState es);
    static jsValue JS_CALL ExportOrganizationsPackage(jsExecState es);
    static jsValue JS_CALL ExportUnitsPackage(jsExecState es);
    static jsValue JS_CALL OpenFile(jsExecState es);
    static jsValue JS_CALL ExecuteSql(jsExecState es);
    static jsValue JS_CALL BuildInstall(jsExecState es);
    static jsValue JS_CALL WriteExcel(jsExecState es);
    static jsValue JS_CALL GetRoot(jsExecState es);

    static jsValue WKE_CALL_TYPE onMsg(jsExecState es, void* param);

    static jsValue WKE_CALL_TYPE onShellExec(jsExecState es, void* param);

    static void WKE_CALL_TYPE OnWkePaintUpdate(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy);

    //��ȡfavicon��
    static void WKE_CALL_TYPE OnWkeNetGetFavicon(wkeWebView webView, void* param, const utf8* url, wkeMemBuf* buf);

    //wkeWebView������洰��ģʽ�������յ�WM_CLODE��Ϣʱ�����˻ص�������ͨ���ڻص��з���false�ܾ��رմ���
    static bool WKE_CALL_TYPE onWkeWindowClosing(wkeWebView webView, void* param);

private:
    REND_DATA m_RendData;

    wkeWebView	 m_pWebView = NULL;;

    TCHAR m_chHomeUrl[1024]; //��ҳ��url

    TCHAR m_chCurPageUrl[1024]; //��ǰҳ���url

    TCHAR m_chErrUrl[1024]; //������ʾҳ���url

private:
    void* m_pListenObj; //��������

    IWkeCallback* m_pWkeCallback;	// �ص��ӿ�

    static std::map<wkeWebView, CWkeWebkitUI*> m_mapWke2UI; //����Wke���ĵ�WebkitUI��ӳ���ϵ

    int  m_cursor;
    bool m_released = false;
};
