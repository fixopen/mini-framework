#include "StdAfx.h"
#include "WkeWebkit.h"

#include <string>
// using namespace std;
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "imm32.lib")

//#include "../Common/String/NSString.h"
#include "utils/Url.h"
#include "utils/Texts.h"
#include "utils/Log.h"
#include "utils/SqlData.h"
#include "utils/Folders.h"
#include "utils/Process.h"
#include "utils/zip.h"
#include "utils/BasicExcel.hpp"
#include "nlohmann/json.hpp"

map<wkeWebView, CWkeWebkitUI*> CWkeWebkitUI::m_mapWke2UI;

LPCTSTR wkeGetStringT(wkeString str) {
#ifdef _UNICODE
    return wkeGetStringW(str);
#else
    return wkeGetString(str);
#endif
}

IMPLEMENT_DUICONTROL(CWkeWebkitUI)
CWkeWebkitUI::CWkeWebkitUI(void) {
    memset(m_chHomeUrl, 0, sizeof(m_chHomeUrl));
    memset(m_chCurPageUrl, 0, sizeof(m_chCurPageUrl));
    memset(m_chErrUrl, 0, sizeof(m_chErrUrl));

    m_pWebView = wkeCreateWebView();

    m_mapWke2UI[m_pWebView] = this;

    m_pListenObj = NULL;
    m_pWkeCallback = NULL;

    TCHAR modulePath[MAX_PATH] = { 0 };
    TCHAR curDir[MAX_PATH] = { 0 }, drive[_MAX_DRIVE] = { 0 }, dir[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    _wsplitpath(modulePath, drive, dir, NULL, NULL);
    _tcscpy(curDir, drive), _tcscat(curDir, dir);
    _tcscpy(m_chErrUrl, L"file:///"), _tcscat(m_chErrUrl, curDir), _tcscat(m_chErrUrl, L"//resources//error//error.html");
    _tcscpy(m_chHomeUrl, L"file:///"), _tcscat(m_chHomeUrl, curDir), _tcscat(m_chHomeUrl, L"//resources//test.html");

    m_cursor = -1;
}

CWkeWebkitUI::~CWkeWebkitUI(void) {
    m_released = true;

    if (m_pWebView != NULL) {
        map<wkeWebView, CWkeWebkitUI*>::iterator iter = m_mapWke2UI.find(m_pWebView);
        if (iter != m_mapWke2UI.end()) {
            m_mapWke2UI.erase(iter);
        }
        //wkeSetHandle(m_pWebView, NULL);
        //wkeDestroyWebView(m_pWebView);//销毁wkeWebView对应的所有数据结构，包括真实窗口等
        m_pWebView = NULL;
    }
    m_pManager->RemoveMessageFilter(this);
}

LPCTSTR CWkeWebkitUI::GetClass() const {
    return _T("CWkeWebkitUI");
}

LPVOID CWkeWebkitUI::GetInterface(LPCTSTR pstrName) {
    if (_tcscmp(pstrName, _T("WkeBrowser")) == 0)
        return static_cast<CWkeWebkitUI*>(this);
    else
        return CControlUI::GetInterface(pstrName);
}

void CWkeWebkitUI::DoInit() {
    CControlUI::DoInit();

    HWND hWnd = m_pManager->GetPaintWindow();
    wkeSetHandle(m_pWebView, hWnd);

    // 设置名称
    wkeSetName(m_pWebView, utils::Texts::toNative(static_cast<const wchar_t*>(GetName())).c_str());

    // 初始化后回调接口
    wkeSetTransparent(m_pWebView, false); //通知无窗口模式下，webview开启透明模式。

    wkeOnTitleChanged(m_pWebView, OnWkeTitleChanged, this);

    wkeOnURLChanged(m_pWebView, OnWkeURLChanged, this);

    wkeOnNavigation(m_pWebView, OnWkeNavigation, this);

    wkeOnCreateView(m_pWebView, OnWkeCreateView, this);

    wkeOnDocumentReady(m_pWebView, OnWkeDocumentReady, this);

    wkeOnLoadUrlBegin(m_pWebView, onLoadUrlBegin, this);

    wkeOnLoadingFinish(m_pWebView, OnWkeLoadingFinish, this);

    wkeOnDownload(m_pWebView, OnWkeDownload, this);

    wkeOnPaintUpdated(m_pWebView, OnWkePaintUpdate, this);

    wkeOnAlertBox(m_pWebView, OnWkeAlertBox, this);

    wkeOnWindowClosing(m_pWebView, onWkeWindowClosing, this);

    // 设置UA
    wkeSetUserAgent(m_pWebView, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.2228.0 Safari/537.36");

    m_pManager->AddMessageFilter(this);
}

void CWkeWebkitUI::SetPos(RECT rc, bool bNeedUpdate/* = true*/) {
    m_RendData.rt = rc;
    m_RendData.pixels = NULL;
    // 调整位置和尺寸
    CControlUI::SetPos(rc, bNeedUpdate);
    wkeResize(m_pWebView, rc.right - rc.left, rc.bottom - rc.top);
}

void CWkeWebkitUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
    if (_tcscmp(pstrName, _T("homepage")) == 0) {
        SetHomePage(pstrValue);
    }
    if (_tcscmp(pstrName, L"url") == 0) {
        this->Navigate(pstrValue);
        return;
    }
    CControlUI::SetAttribute(pstrName, pstrValue);
}

LRESULT CWkeWebkitUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) {
    if (m_released)
        return FALSE;


    CControlUI* current = m_pManager->FindControl(m_pManager->GetMousePos());
    if (current != this) {
        return FALSE;
    }
    //修改鼠标指向时候的样式
    if (uMsg == WM_SETCURSOR) {
        bHandled = true;
        return S_OK;
    }
    //修改输入法的位置
    else if (uMsg == WM_IME_STARTCOMPOSITION) {
        const RECT controlPos = this->GetPos();
        wkeRect rect = wkeGetCaretRect(m_pWebView);
        HIMC hImc = ::ImmGetContext(m_pManager->GetPaintWindow());
        COMPOSITIONFORM Composition = { 0 };
        Composition.dwStyle = CFS_POINT | CFS_FORCE_POSITION;
        Composition.ptCurrentPos.x = rect.x + controlPos.left;
        Composition.ptCurrentPos.y = rect.y + controlPos.top;
        ImmSetCompositionWindow(hImc, &Composition);
        ImmReleaseContext(m_pManager->GetPaintWindow(), hImc);
        bHandled = TRUE;
        return S_OK;
    }
    return S_OK;
}

bool CWkeWebkitUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {
    CControlUI::DoPaint(hDC, rcPaint, pStopControl);

    if (m_RendData.pixels == NULL) {
        BITMAPINFO bi;
        memset(&bi, 0, sizeof(bi));
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = int(m_RendData.rt.right - m_RendData.rt.left);
        bi.bmiHeader.biHeight = -int(m_RendData.rt.bottom - m_RendData.rt.top);
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        HBITMAP hbmp = ::CreateDIBSection(0, &bi, DIB_RGB_COLORS, &m_RendData.pixels, NULL, 0);
        SelectObject(m_RendData.hDC, hbmp);
        if (m_RendData.hBitmap) {
            DeleteObject(m_RendData.hBitmap);
        }
        m_RendData.hBitmap = hbmp;
    }
    wkePaint(m_pWebView, m_RendData.pixels, 0);
    ::BitBlt(hDC, m_RendData.rt.left, m_RendData.rt.top, m_RendData.rt.right - m_RendData.rt.left, m_RendData.rt.bottom - m_RendData.rt.top, m_RendData.hDC, 0, 0, SRCCOPY);
    updateCursor();
    return true;

    //if (hDC != NULL) {	
    //	HDC mb_hdc = wkeGetViewDC(m_pWebView);
    //	if (mb_hdc != NULL)
    //	{
    //		::BitBlt(hDC, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, mb_hdc, 0, 0, SRCCOPY);
    //		::ReleaseDC(NULL, mb_hdc);
    //	}
    //}
    updateCursor();
    return true;
}

void CWkeWebkitUI::InitializeWebkit() {
    static bool isInitialized = ::wkeIsInitialize == NULL ? false : (::wkeIsInitialize());

    if (!isInitialized) {
        // 加载mb的资源
        CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
        CDuiString mbPath = strResourcePath + L"node.dll";
        //CDuiString mbPath = strResourcePath + L"node_v8_4_8.dll";
        if (!::PathFileExists(mbPath)) {
            ::MessageBoxW(NULL, L"请把node.dll放exe目录下", L"错误", MB_OK);
            return;
        }
        wkeSetWkeDllPath(mbPath);

        wkeInitialize();

        //绑定全局函数
        jsBindFunction("jsToNative", JsToNative, 2);
        jsBindFunction("db_select", Select, 3);
        jsBindFunction("db_insert", Insert, 2);
        jsBindFunction("db_update", Update, 2);
        jsBindFunction("db_fillFileContent", FillFileContent, 4);
        jsBindFunction("db_getFileContent", GetFileContent, 3);
        jsBindFunction("db_remove", Remove, 2);
        jsBindFunction("db_backup", Backup, 1);
        jsBindFunction("db_restore", Restore, 1);
        jsBindFunction("db_importPackage", ImportPackage, 2);
        jsBindFunction("db_exportPackage", ExportPackage, 2);
        jsBindFunction("db_exportTemplatePackage", ExportTemplatePackage, 1);
        jsBindFunction("db_exportOrganizationsPackage", ExportOrganizationsPackage, 0);
        jsBindFunction("db_openfile", OpenFile, 1);
        jsBindFunction("db_executeSql", ExecuteSql, 1);
        jsBindFunction("db_buildInstall", BuildInstall, 1);
        jsBindFunction("db_writeExcel", WriteExcel, 3);

        wkeJsBindFunction("eMsg", &onMsg, nullptr, 5);
        wkeJsBindFunction("eShellExec", &onShellExec, nullptr, 3);
    }
}

void CWkeWebkitUI::UninitializeWebkit() {
    // 清理
    wkeShutdown();
}

void CWkeWebkitUI::ExecuteJS(LPCTSTR lpJS) {
    wkeRunJS(m_pWebView, utils::Texts::toNative(lpJS).c_str());
}

void CWkeWebkitUI::updateCursor() {
    int cursorInfo = wkeGetCursorInfoType(m_pWebView);
    if (m_cursor != cursorInfo) {
        m_cursor = cursorInfo;
        HCURSOR curosr = ::LoadCursor(NULL, IDC_ARROW);
        switch (cursorInfo) {
        case WkeCursorInfoPointer:
            curosr = ::LoadCursor(NULL, IDC_ARROW);
            break;
        case WkeCursorInfoCross:
            curosr = ::LoadCursor(NULL, IDC_CROSS);
            break;
        case WkeCursorInfoHand:
            curosr = ::LoadCursor(NULL, IDC_HAND);
            break;
        case WkeCursorInfoIBeam:
            curosr = ::LoadCursor(NULL, IDC_IBEAM);
            break;
        case WkeCursorInfoWait:
            curosr = ::LoadCursor(NULL, IDC_WAIT);
            break;
        case WkeCursorInfoHelp:
            curosr = ::LoadCursor(NULL, IDC_HELP);
            break;
        case WkeCursorInfoEastResize:
            curosr = ::LoadCursor(NULL, IDC_SIZEWE);
            break;
        case WkeCursorInfoNorthResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENS);
            break;
        case WkeCursorInfoNorthEastResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENESW);
            break;
        case WkeCursorInfoNorthWestResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENWSE);
            break;
        case WkeCursorInfoSouthResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENS);
            break;
        case WkeCursorInfoSouthEastResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENWSE);
            break;
        case WkeCursorInfoSouthWestResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENESW);
            break;
        case WkeCursorInfoWestResize:
            curosr = ::LoadCursor(NULL, IDC_SIZEWE);
            break;
        case WkeCursorInfoNorthSouthResize:
            curosr = ::LoadCursor(NULL, IDC_SIZENS);
            break;
        case WkeCursorInfoEastWestResize:
            curosr = ::LoadCursor(NULL, IDC_SIZEWE);
            break;
        case WkeCursorInfoNorthEastSouthWestResize:
            curosr = ::LoadCursor(NULL, IDC_SIZEALL);
            break;
        case WkeCursorInfoNorthWestSouthEastResize:
            curosr = ::LoadCursor(NULL, IDC_SIZEALL);
            break;
        case WkeCursorInfoColumnResize:
        case WkeCursorInfoRowResize:
            curosr = ::LoadCursor(NULL, IDC_ARROW);
            break;
        default:
            break;
        }
        ::SetCursor(curosr);
    }
}

void CWkeWebkitUI::DoEvent(TEventUI& event) {
    if (!this->IsEnabled() || !this->IsVisible()) return;

    RECT rc = GetPos();
    POINT pt = { event.ptMouse.x - rc.left, event.ptMouse.y - rc.top };
    static WkeCursorInfoType cursorInfo = WkeCursorInfoType::WkeCursorInfoPointer;
    switch (event.Type) {
    case UIEVENT_MOUSEENTER:
    case UIEVENT_MOUSELEAVE:
    case UIEVENT_MOUSEMOVE:
    {
        unsigned int flags = 0;
        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;
        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;
        wkeFireMouseEvent(m_pWebView, WKE_MSG_MOUSEMOVE, pt.x, pt.y, flags);
        updateCursor();
    }
    break;
    case UIEVENT_BUTTONDOWN:
        wkeFireMouseEvent(m_pWebView, WKE_MSG_LBUTTONDOWN, pt.x, pt.y, event.wKeyState);
        break;
    case UIEVENT_BUTTONUP:
        wkeFireMouseEvent(m_pWebView, WKE_MSG_LBUTTONUP, pt.x, pt.y, event.wKeyState);
        break;
    case UIEVENT_RBUTTONDOWN:
        wkeFireMouseEvent(m_pWebView, WKE_MSG_RBUTTONDOWN, pt.x, pt.y, event.wKeyState);
        break;
    case UIEVENT_DBLCLICK:
        wkeFireMouseEvent(m_pWebView, WKE_MSG_LBUTTONDBLCLK, pt.x, pt.y, event.wKeyState);
        break;
    case UIEVENT_SCROLLWHEEL:
        wkeFireMouseWheelEvent(m_pWebView, pt.x, pt.y, event.wParam == SB_LINEUP ? 120 : -120, event.wKeyState);
        break;
    case UIEVENT_KEYDOWN:
        wkeFireKeyDownEvent(m_pWebView, event.chKey, event.lParam, false);
        break;
    case UIEVENT_KEYUP:
        wkeFireKeyUpEvent(m_pWebView, event.chKey, event.lParam, false);
        break;
    case UIEVENT_CHAR:
    {
        wkeFireKeyPressEvent(m_pWebView, event.chKey, event.lParam, false);
        break;
    }
    case UIEVENT_SETFOCUS:
        wkeSetFocus(m_pWebView);
        break;
    case UIEVENT_KILLFOCUS:
        wkeKillFocus(m_pWebView);
        break;
    case UIEVENT_SETCURSOR:
        return;


    case UIEVENT_CONTEXTMENU:
    {
        unsigned int flags = 0;
        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;
        wkeFireContextMenuEvent(m_pWebView, pt.x, pt.y, flags);
        break;
    }
    case UIEVENT_TIMER:
        /*	if (event.wParam == EVENT_TICK_TIEMER_ID)
            {
                Invalidate();
            }*/
        break;
    default: break;
    }
    CControlUI::DoEvent(event);
}

wkeWebView CWkeWebkitUI::GetWebView() {
    return m_pWebView;
}

void CWkeWebkitUI::Navigate(LPCTSTR lpUrl) {
    common::Url uri(utils::Texts::toNative(lpUrl).c_str());
    std::string strscheme = uri.GetScheme();
    if (_tcslen(lpUrl) >= 8 && (strscheme == "http" || strscheme == "https" || StrCmp(lpUrl, L"about:blank") == 0)
        ) {
#ifdef UNICODE
        ::wkeLoadURLW(m_pWebView, lpUrl);
#else
        ::wkeLoadURL(m_pWebView, lpUrl);
#endif
    } else if (strscheme == "file") {
#ifdef UNICODE
        ::wkeLoadFileW(m_pWebView, lpUrl);
#else
        ::wkeLoadFile(m_pWebView, lpUrl);
#endif
    } else {
#ifdef UNICODE
        ::wkeLoadURLW(m_pWebView, lpUrl);
#else
        ::wkeLoadURL(m_pWebView, lpUrl);
#endif
    }
}

void CWkeWebkitUI::LoadHtml(LPCTSTR lpHtml) {
#ifdef _UNICODE
    wkeLoadHTMLW(m_pWebView, lpHtml);
#else
    wkeLoadHTML(m_pWebView, lpHtml);
#endif	
}

void CWkeWebkitUI::Close() {}

void CWkeWebkitUI::Back() {
    if (wkeCanGoBack(m_pWebView))
        wkeGoBack(m_pWebView);
}

void CWkeWebkitUI::Forward() {
    if (wkeCanGoForward(m_pWebView))
        wkeGoForward(m_pWebView);
}

void CWkeWebkitUI::Stop() {
    wkeStopLoading(m_pWebView);
}

void CWkeWebkitUI::Reload() {
    wkeReload(m_pWebView);
}

void CWkeWebkitUI::SetHomePage(LPCTSTR lpHomeUrl) {
    _tcscpy(m_chHomeUrl, lpHomeUrl);
    Navigate(m_chHomeUrl);
}

void CWkeWebkitUI::SetErrorInfoPageUrl(LPCTSTR lpErrUrl) {
    _tcscpy(m_chErrUrl, lpErrUrl);
}

void CWkeWebkitUI::NavigateHomePage() {
    Navigate(m_chHomeUrl);
}

void CWkeWebkitUI::SetPageFocus() {
    wkeSetFocus(m_pWebView);
}

void CWkeWebkitUI::SetListenObj(void* pListenObj) {
    m_pListenObj = pListenObj;
}

void CWkeWebkitUI::SetWkeCallback(IWkeCallback* pWkeCallback) {
    m_pWkeCallback = pWkeCallback;
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkePaintUpdate(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;
    pWkeUI->Invalidate();
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeTitleChanged(wkeWebView webView, void* param, wkeString title) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;
    if (pWkeUI->m_pWkeCallback) {
#ifdef UNICODE
        return pWkeUI->m_pWkeCallback->OnWkeTitleChanged(pWkeUI, wkeGetStringW(title));
#else
        return pWkeUI->m_pWkeCallback->OnWkeTitleChanged(pWkeUI, wkeGetStringT(title));
#endif // UNICODE
    }
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeURLChanged(wkeWebView webView, void* param, wkeString url) {
    wkeTempCallbackInfo* temInfo = wkeGetTempCallbackInfo(webView);
    if (::wkeIsMainFrame(webView, temInfo->frame)) {
        CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
        if (!pWkeUI || pWkeUI->m_released)	return;
        if (pWkeUI->m_pWkeCallback) {
            return pWkeUI->m_pWkeCallback->OnWkeURLChanged(pWkeUI, wkeGetStringT(url));
        }
    }
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeAlertBox(wkeWebView webView, void* param, wkeString msg) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;

    if (pWkeUI->m_pWkeCallback) {
        return pWkeUI->m_pWkeCallback->OnWkeAlertBox(pWkeUI, wkeGetStringT(msg));
    }
}

bool WKE_CALL_TYPE CWkeWebkitUI::OnWkeNavigation(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return false;

    LPCTSTR pStrUrl = wkeGetStringT(url);
    if (!_tcsstr(pStrUrl, _T("error.html"))) {
        _tcscpy(pWkeUI->m_chCurPageUrl, pStrUrl);
    }

    if (pWkeUI->m_pWkeCallback) {
        return pWkeUI->m_pWkeCallback->OnWkeNavigation(pWkeUI, navigationType, pStrUrl);
    }

    return true;
}

wkeWebView WKE_CALL_TYPE CWkeWebkitUI::OnWkeCreateView(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return NULL;

    if (pWkeUI->m_pWkeCallback) {
        return pWkeUI->m_pWkeCallback->OnWkeCreateView(pWkeUI, navigationType, url, windowFeatures);
    }

    return NULL;
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeDocumentReady(wkeWebView webView, void* param) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;
    if (pWkeUI->m_pWkeCallback) {
        return pWkeUI->m_pWkeCallback->OnWkeDocumentReady(pWkeUI);
    }
}

void readJsFile(const wchar_t* path, std::vector<char>* buffer) {
    HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        DebugBreak();
        return;
    }

    DWORD fileSizeHigh;
    const DWORD bufferSize = ::GetFileSize(hFile, &fileSizeHigh);

    DWORD numberOfBytesRead = 0;
    buffer->resize(bufferSize);
    BOOL b = ::ReadFile(hFile, &buffer->at(0), bufferSize, &numberOfBytesRead, nullptr);
    ::CloseHandle(hFile);
    b = b;
}

//任何网络请求发起前会触发此回调
//注意：
//1，此回调功能强大，在回调里，如果对job设置了wkeNetHookRequest，则表示mb会缓存获取到的网络数据，并在这次网络请求 结束后调用wkeOnLoadUrlEnd设置的回调，同时传递缓存的数据。在此期间，mb不会处理网络数据。
//2，如果在wkeLoadUrlBeginCallback里没设置wkeNetHookRequest，则不会触发wkeOnLoadUrlEnd回调。
//3，如果wkeLoadUrlBeginCallback回调里返回true，表示mb不处理此网络请求（既不会发送网络请求）。返回false，表示mb依然会发送网络请求。
bool  WKE_CALL_TYPE CWkeWebkitUI::onLoadUrlBegin(wkeWebView webView, void* param, const char* url, void* job) {
    const char kPreHead[] = "http://hook.test/";
    const char* pos = strstr(url, kPreHead);
    if (pos) {
        const utf8* decodeURL = wkeUtilDecodeURLEscape(url);
        if (!decodeURL)
            return false;
        std::string urlString(decodeURL);
        std::string localPath = urlString.substr(sizeof(kPreHead) - 1);

        std::wstring path = CPaintManagerUI::GetInstancePath().GetData() + utils::Texts::fromUtf8(localPath);
        std::vector<char> buffer;

        readJsFile(path.c_str(), &buffer);

        wkeNetSetData(job, buffer.data(), buffer.size());

        return true;
    } else if (strncmp(url, "http://localhost:12222", 22) == 0) {
        wkeNetSetMIMEType(job, (char*) "text/html");
        wkeNetSetData(job, (char*) "\"test1111\"", 10);
        return true;
    } else if (strcmp(url, "http://www.baidu.com/") == 0) {
        wkeNetHookRequest(job);
    }

    return false;
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeLoadingFinish(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;

    //页面加载失败
    if (result == WKE_LOADING_FAILED) {
        pWkeUI->Navigate(pWkeUI->m_chErrUrl);
    }

    wkeTempCallbackInfo* temInfo = wkeGetTempCallbackInfo(webView);
    if (::wkeIsMainFrame(webView, temInfo->frame)) {
        ::wkeNetGetFavicon(webView, OnWkeNetGetFavicon, param);
    }

    if (pWkeUI->m_pWkeCallback) {
        pWkeUI->m_pWkeCallback->OnWkeLoadingFinish(pWkeUI, wkeGetStringT(url), result, wkeGetStringT(failedReason));
    }
}

void WKE_CALL_TYPE CWkeWebkitUI::OnWkeNetGetFavicon(wkeWebView webView, void* param, const utf8* url, wkeMemBuf* buf) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return;

    if (pWkeUI->m_pWkeCallback && buf != nullptr && url != nullptr) {
        pWkeUI->m_pWkeCallback->OnWkeNetGetFavicon(pWkeUI, url, buf);
    }

    return;
}

bool WKE_CALL_TYPE CWkeWebkitUI::onWkeWindowClosing(wkeWebView webView, void* param) {
    return false;
}

bool WKE_CALL_TYPE CWkeWebkitUI::OnWkeDownload(wkeWebView webView, void* param, const char* url) {
    CWkeWebkitUI* pWkeUI = (CWkeWebkitUI*) param;
    if (!pWkeUI || pWkeUI->m_released)	return false;

    if (pWkeUI->m_pWkeCallback) {
        return pWkeUI->m_pWkeCallback->OnWkeDownload(pWkeUI, url);
    }
    return true;
}

jsValue WKE_CALL_TYPE  CWkeWebkitUI::onMsg(jsExecState es, void* param) {
    int argCount = jsArgCount(es);
    if (argCount < 1)
        return jsUndefined();

    jsType type = jsArgType(es, 0);
    if (JSTYPE_STRING != type)
        return jsUndefined();

    jsValue arg0 = jsArg(es, 0);
    std::string msgOutput = "eMsg:";
    std::string msg = jsToTempString(es, arg0);
    msgOutput = msgOutput + msg;
    msgOutput += "\n";
    OutputDebugStringA(msgOutput.c_str());

    //查找UI对象
    CWkeWebkitUI* pWkeUI = NULL;
    wkeWebView pWke = jsGetWebView(es);
    if (pWke) {
        map<wkeWebView, CWkeWebkitUI*>::const_iterator iter = m_mapWke2UI.find(pWke);
        if (iter != m_mapWke2UI.end()) {
            pWkeUI = iter->second;
        }
    }
    if (pWkeUI) {
        int nArg = jsArgCount(es);
        if (nArg > 0) {
            jsValue arg1 = jsArg(es, 0);

            if (jsIsString(arg1)) {

#ifdef _UNICODE 
                wchar_t buf1[16 * 1024] = { 0 };
                wcsncpy(buf1, jsToTempStringW(es, arg1), 16 * 1024 - 1);

#else
                char buf1[16 * 1024] = { 0 };
                strncpy(buf1, jsToTempString(es, arg1), 16 * 1024 - 1);

#endif

                LPCTSTR lpArg1 = buf1;

                if (wcscmp(lpArg1, L"refresh") == 0) {
                    //本地刷新
                    pWkeUI->Navigate(pWkeUI->m_chCurPageUrl);
                    return jsUndefined();
                }

                if (pWkeUI->m_pWkeCallback) {
                    LPCTSTR lpRet = pWkeUI->m_pWkeCallback->OnJS2Native(pWkeUI, lpArg1, L"", pWkeUI->m_pListenObj);
                    if (lpRet) {
#ifdef _UNICODE
                        return jsStringW(es, lpRet);
#else
                        return jsString(es, lpRet);
#endif
                    }
                }

            }
        }
    }

    return jsUndefined();
}

jsValue WKE_CALL_TYPE CWkeWebkitUI::onShellExec(jsExecState es, void* param) {
    if (0 == jsArgCount(es))
        return jsUndefined();
    jsValue arg0 = jsArg(es, 0);
    if (!jsIsString(arg0))
        return jsUndefined();

    jsValue arg1 = jsArg(es, 1);
    if (!jsIsString(arg1))
        return jsUndefined();

    std::string strName;
    strName = jsToTempString(es, arg0);

    std::string path;
    path = jsToTempString(es, arg1);

    if ("runEchars" == path) {
        //createECharsTest();
    } else if ("wkeBrowser" == path) {
        //wkeBrowserMain(nullptr, nullptr, nullptr, TRUE);
    } else {
        //system(path.c_str());
        WinExec(strName.c_str(), SW_SHOW);
    }

    path += "\n";
    OutputDebugStringA(path.c_str());

    return jsUndefined();
}

jsValue JS_CALL CWkeWebkitUI::JsToNative(jsExecState es) {
    //查找UI对象
    CWkeWebkitUI* pWkeUI = NULL;
    wkeWebView pWke = jsGetWebView(es);
    if (pWke) {
        map<wkeWebView, CWkeWebkitUI*>::const_iterator iter = m_mapWke2UI.find(pWke);
        if (iter != m_mapWke2UI.end()) {
            pWkeUI = iter->second;
        }
    }
    if (pWkeUI) {
        int nArg = jsArgCount(es);
        if (nArg == 2) {
            jsValue arg1 = jsArg(es, 0);
            jsValue arg2 = jsArg(es, 1);
            int int0 = jsToInt(es, arg1);
            int int1 = jsToInt(es, arg2);


            return jsInt(int0 + int1);
            if (jsIsString(arg1) && jsIsString(arg2)) {
                //需要保证两个参数都为字符串
#ifdef _UNICODE 
                wchar_t buf1[16 * 1024] = { 0 }, buf2[16 * 1024] = { 0 };
                wcsncpy(buf1, jsToTempStringW(es, arg1), 16 * 1024 - 1);
                wcsncpy(buf2, jsToTempStringW(es, arg2), 16 * 1024 - 1);
#else
                char buf1[16 * 1024] = { 0 }, buf2[16 * 1024] = { 0 };
                strncpy(buf1, jsToTempString(es, arg1), 16 * 1024 - 1);
                strncpy(buf2, jsToTempString(es, arg2), 16 * 1024 - 1);
#endif

                LPCTSTR lpArg1 = buf1;
                LPCTSTR lpArg2 = buf2;

                if (wcscmp(lpArg1, L"refresh") == 0) {
                    //本地刷新
                    pWkeUI->Navigate(pWkeUI->m_chCurPageUrl);
                    return jsUndefined();
                }

                if (pWkeUI->m_pWkeCallback) {
                    LPCTSTR lpRet = pWkeUI->m_pWkeCallback->OnJS2Native(pWkeUI, lpArg1, lpArg2, pWkeUI->m_pListenObj);
                    if (lpRet) {
#ifdef _UNICODE
                        return jsStringW(es, lpRet);
#else
                        return jsString(es, lpRet);
#endif
                    }
                }

            }
        }
    }

    return jsUndefined();
}

namespace {
    std::map<std::wstring, std::vector<std::wstring>> const TABLES{
            { L"templates", { L"id", L"name", L"state", L"description" } },
            { L"types", { L"id", L"no", L"template_id", L"name", L"type", L"description" } },
            { L"codes", { L"id", L"template_id", L"parent_no", L"no", L"value", L"description" } },
            { L"schemas", { L"id", L"template_id", L"parent_no", L"no", L"name", L"type_no", L"is_required", L"restriction", L"description" } },
            { L"lists", { L"id", L"template_id", L"parent_no", L"element_type_no", L"min", L"max", L"group_by", L"description" } },
            { L"unit_types", { L"id", L"name", L"description" } },
            { L"units", { L"id", L"type_id", L"name", L"symbol", L"description" } },
            { L"组织", { L"id", L"编号", L"名称", L"类型", L"地址", L"电话", L"联系人", L"管理员id", L"备注" } },
            { L"用户", { L"id", L"登录名", L"姓名", L"性别", L"生日", L"组织id", L"住址", L"电话", L"email", L"website", L"密码", L"role", L"备注" } },
            { L"机型", { L"id", L"模板id", L"类型", L"名称", L"工厂id", L"state", L"extended", L"备注" } },
            { L"状态", { L"id", L"机型id", L"编号", L"使命任务", L"state", L"extended", L"备注" } },
            { L"批次", { L"id", L"状态id", L"编号", L"飞机编号范围", L"发动机编号范围", L"布局", L"主要技术参数", L"主要配备", L"喷涂方案", L"state", L"extended", L"备注" } },
            { L"基线", { L"id", L"适用目标", L"目标id", L"state", L"extended", L"备注" } },
            { L"飞机", { L"id", L"批次id", L"编号", L"承制单位id", L"接装单位id", L"移交时间", L"移交编号", L"使用寿命", L"剩余寿命", L"引擎型号", L"引擎编号", L"引擎使用寿命", L"引擎剩余寿命", L"state",  L"extended", L"备注" } },
            { L"构型", { L"id", L"基线id", L"parent_id", L"编号", L"序号", L"名称", L"型号", L"单机定额", L"装机部位", L"承制单位id", L"重要度", L"状态", L"主管", L"版本", L"extended", L"备注" } },
            { L"support_configs", { L"id", L"name", L"table_name" } },
            { L"随机设备工具", { L"id", L"基线id", L"parent_id", L"专业", L"序号", L"名称", L"型号", L"用途", L"配套比例", L"定价", L"承制单位id", L"extended", L"备注" } },
            { L"机械专业随机备件", { L"id", L"基线id", L"parent_id", L"序号", L"名称", L"型号(图号)", L"单机数量", L"比例及数量", L"所属系统", L"安装使用部位", L"承制单位id", L"extended", L"备注" } },
            { L"随机资料目录", { L"id", L"基线id", L"parent_id", L"序号", L"资料编号", L"手册名称", L"册数", L"单册页数", L"密级", L"extended", L"备注" } },
            { L"随机资料配套目录", { L"id", L"基线id", L"parent_id", L"序号", L"资料名称", L"手册编制的参考标准", L"主要用途/使用时机/使用地点", L"主要使用对象", L"extended", L"备注" } },
            { L"技术状态文件", { L"id", L"基线id", L"类型", L"previous_version_id", L"path", L"编号", L"名称", L"版本", L"修订时间", L"构型节点id", L"附件", L"digest", L"state", L"extended", L"备注" } },
            { L"有寿件", { L"id", L"基线id", L"extended" } },
            { L"双流水", { L"id", L"基线id", L"extended" } },
            { L"交付遗留问题及处置情况", { L"id", L"基线id", L"extended" } },
            { L"change_histories", { L"id", L"基线id", L"time", L"no", L"control_file_id", L"description" } },
            { L"change_history_details", { L"id", L"change_history_id", L"from_file_id", L"to_file_id", L"description" } }
    };
}

jsValue JS_CALL CWkeWebkitUI::Select(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 3) {
        jsValue tn = jsArg(es, 0);
        jsValue w = jsArg(es, 1);
        jsValue o = jsArg(es, 2);
        if (jsIsString(tn) && jsIsString(w) && jsIsString(o)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            std::wstring whereCause = jsToTempStringW(es, w);
            std::wstring orderBy = jsToTempStringW(es, o);
            std::wstring r = utils::SqlData::query(tableName, whereCause, orderBy);
            result = jsStringW(es, r.c_str());
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::Insert(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 2) {
        jsValue tn = jsArg(es, 0);
        jsValue d = jsArg(es, 1);
        if (jsIsString(tn) && jsIsString(d)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            std::wstring dataJson = jsToTempStringW(es, d);
            nlohmann::json data = nlohmann::basic_json<>::parse(utils::Texts::toUtf8(dataJson));
            int64_t id = utils::SqlData::insert(tableName, data);
            // utils::Log::log("insert finally, id is " + std::to_string(id));
            result = jsInt(id);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::Update(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 2) {
        jsValue tn = jsArg(es, 0);
        jsValue d = jsArg(es, 1);
        if (jsIsString(tn) && jsIsString(d)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            std::wstring dataJson = jsToTempStringW(es, d);
            nlohmann::json data = nlohmann::json::parse(utils::Texts::toUtf8(dataJson));
            utils::SqlData::update(tableName, data);
            result = jsNull();
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::Remove(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 2) {
        jsValue tn = jsArg(es, 0);
        jsValue d = jsArg(es, 1);
        if (jsIsString(tn) && jsIsNumber(d)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            int id = jsToInt(es, d);
            utils::SqlData::remove(tableName, id);
            result = jsNull();
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::FillFileContent(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 4) {
        jsValue tn = jsArg(es, 0);
        jsValue id = jsArg(es, 1);
        jsValue c = jsArg(es, 2);
        jsValue d = jsArg(es, 3);
        if (jsIsString(tn) && jsIsString(c)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            // auto it = TABLES.find(tableName);
            // if (it != TABLES.end()) {
                int ID = jsToInt(es, id);
                std::wstring columnName = jsToTempStringW(es, c);
                // jsType t = jsArgType(es, 3);
                // void* data = jsToV8Value(es, d);
                wkeMemBuf* buffer = jsGetArrayBuffer(es, d);
                // jsData* data = jsGetData(es, d);
                utils::SqlData::fillBlob(tableName, ID, columnName, buffer->data, buffer->length); // --or-- buffer->length
                result = jsNull();
            // }
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::GetFileContent(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 4) {
        jsValue tn = jsArg(es, 0);
        jsValue id = jsArg(es, 1);
        jsValue c = jsArg(es, 2);
        if (jsIsString(tn) && jsIsString(c)) {
            std::wstring tableName = jsToTempStringW(es, tn);
            // auto it = TABLES.find(tableName);
            // if (it != TABLES.end()) {
                int ID = jsToInt(es, id);
                std::wstring columnName = jsToTempStringW(es, c);
                std::string data = utils::SqlData::readBlob(tableName, ID, columnName); // --or-- buffer->length
                // TODO fill blob content
                // wkeMemBuf* buffer = jsGetArrayBuffer(es, d);
                result = jsArrayBuffer(es, data.data(), data.length());
            // }
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::Backup(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue name = jsArg(es, 0);
        if (jsIsString(name)) {
            std::wstring fileName = jsToTempStringW(es, name);
            int r = utils::SqlData::backup(fileName);
            result = jsInt(r);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::Restore(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue name = jsArg(es, 0);
        if (jsIsString(name)) {
            std::wstring fileName = jsToTempStringW(es, name);
            int r = utils::SqlData::restore(fileName);
            result = jsInt(r);
        }
    }
    return result;
}

namespace {
    void importBaseline(std::wstring const& workPath, nlohmann::json& data, int64_t baselineId) {
        map<int64_t, int64_t> configurationIdMaps;
        for (auto& configuration : data["configurations"]) {
            configuration[u8"baseline_id"] = baselineId;
            int64_t oldId = configuration["id"].get<std::int64_t>();
            configuration.erase("id");
            if (configuration.contains("parent_id") && !configuration["parent_id"].is_null()) {
                int64_t oldParentId = configuration["parent_id"].get<std::int64_t>();
                configuration["parent_id"] = configurationIdMaps[oldParentId];
            }
            int64_t newId = utils::SqlData::insert(L"data", configuration);
            configurationIdMaps.insert(std::make_pair(oldId, newId));
        }
        /*
        for (auto& auxiliary : data["auxiliaries"]) {
            auxiliary[u8"基线id"] = baselineId;
            auxiliary.erase("id");
            utils::SqlData::insert(L"随机设备工具", auxiliary);
        }
        for (auto& machineAux : data["machineAux"]) {
            machineAux[u8"基线id"] = baselineId;
            machineAux.erase("id");
            utils::SqlData::insert(L"机械专业随机备件", machineAux);
        }
        for (auto& resourceAux : data["resourceAux"]) {
            resourceAux[u8"基线id"] = baselineId;
            resourceAux.erase("id");
            utils::SqlData::insert(L"随机资料目录", resourceAux);
        }
        for (auto& resourceDirectoryAux : data["resourceDirectoryAux"]) {
            resourceDirectoryAux[u8"基线id"] = baselineId;
            resourceDirectoryAux.erase("id");
            utils::SqlData::insert(L"随机资料配套目录", resourceDirectoryAux);
        }
        for (auto& technologyStateFile : data["technologyStateFiles"]) {
            if (data.contains("files")) {
                int64_t fileId = technologyStateFile[u8"附件"].get<std::int64_t>();
                for (auto& file : data["files"]) {
                    if (file["id"].get<std::int64_t>() == fileId) {
                        std::string type = "application/json";
                        if (file.contains("type") && !file["type"].is_null()) {
                            type = file["type"].get<std::string>();
                        }
                        std::string ext;
                        if (file.contains("ext") && !file["ext"].is_null()) {
                            ext = file["ext"].get<std::string>();
                        }
                        std::string json;
                        if (ext.empty()) {
                            json = R"json({"type": "application/json", "ext": null})json";
                        } else {
                            json = "{\"type\": \"" + type + "\", \"ext\": \"" + ext + "\"}";
                        }
                        int64_t newFileId = utils::SqlData::insert(L"files", nlohmann::basic_json<>::parse(json));
                        if (!ext.empty()) {
                            std::string fileContent = utils::Folders::ReadFile(workPath + L"\\技术状态文件\\"
                                + std::to_wstring(technologyStateFile[u8"类型"].get<std::int64_t>()) + L"\\"
                                + utils::Texts::fromUtf8(technologyStateFile[u8"名称"].get<std::string>())
                                + L"." + utils::Texts::fromUtf8(file["ext"].get<std::string>()));
                            utils::SqlData::fillBlob(L"files", newFileId, L"content", fileContent.data(), fileContent.length());
                        }
                        technologyStateFile[u8"附件"] = newFileId;
                        break;
                    }
                }
            }
            technologyStateFile[u8"基线id"] = baselineId;
            technologyStateFile.erase("id");
            if (technologyStateFile.contains(u8"构型节点id") && !technologyStateFile[u8"构型节点id"].is_null()) {
                int64_t oldId = technologyStateFile[u8"构型节点id"].get<std::int64_t>();
                technologyStateFile[u8"构型节点id"] = configurationIdMaps[oldId];
            }
            utils::SqlData::insert(L"技术状态文件", technologyStateFile);
        }
        for (auto& lifespan : data["lifespans"]) {
            lifespan[u8"基线id"] = baselineId;
            lifespan.erase("id");
            utils::SqlData::insert(L"有寿件", lifespan);
        }
        for (auto& doubleCurrent : data["doubleCurrents"]) {
            doubleCurrent[u8"基线id"] = baselineId;
            doubleCurrent.erase("id");
            utils::SqlData::insert(L"双流水", doubleCurrent);
        }
        for (auto& problem : data["problems"]) {
            problem[u8"基线id"] = baselineId;
            problem.erase("id");
            utils::SqlData::insert(L"交付遗留问题及处置情况", problem);
        }
        */
    }
}

jsValue JS_CALL CWkeWebkitUI::ImportPackage(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 2) {
        jsValue c = jsArg(es, 0);
        wkeMemBuf* buffer = jsGetArrayBuffer(es, c);
        if (buffer) {
            std::wstring destPath = utils::Folders::GetCurrentPath();
            std::wstring encFilename = destPath + L"\\temp.zip.enc";
            utils::Folders::WriteFile(encFilename, std::string(reinterpret_cast<char*>(buffer->data), buffer->length));
            std::wstring filename = destPath + L"\\temp.zip";
            utils::Folders::DecryptFileContent(encFilename, filename);
            std::wstring workPath = destPath + L"\\working";
            utils::Folders::RecurisiveMakeDir(workPath);
            utils::zip::UnzipViaZlib(filename, workPath);
            // jsValue type = jsArg(es, 1);
            jsValue id = jsArg(es, 1);
            if (jsIsNumber(id)) {
                int idValue = jsToInt(es, id);
                nlohmann::json data = nlohmann::basic_json<>::parse(utils::Folders::ReadFile(workPath + L"\\meta.json"));
                if (data.contains("air")) {
                    data["air"][u8"batch_id"] = idValue;
                    data["air"]["state"] = 0;
                    data["air"].erase("id");
                    int64_t airId = utils::SqlData::insert(L"飞机", data["air"]);
                    data["baseline"][u8"object_id"] = airId;
                    data["baseline"].erase("id");
                    int64_t baselineId = utils::SqlData::insert(L"baselines", data["baseline"]);
                    importBaseline(workPath, data, baselineId);
                } else {
                    data["batch"][u8"state_id"] = idValue;
                    data["batch"]["state"] = 0;
                    data["batch"].erase("id");
                    int64_t batchId = utils::SqlData::insert(L"batchs", data["batch"]);
                    data["baseline"][u8"object_id"] = batchId;
                    data["baseline"].erase("id");
                    int64_t baselineId = utils::SqlData::insert(L"baselines", data["baseline"]);
                    importBaseline(workPath, data, baselineId);
                }
                result = jsInt(0);
            }
        }
    }
    return result;
}

namespace {
    struct Result {
        int code;
        std::wstring ext;
    };

    Result saveFile(int64_t idValue, std::wstring const& fileName) {
        Result result = { 0, L"" };
        // int result = 0;
        std::wstring meta = utils::SqlData::query(L"files", L"id = " + std::to_wstring(idValue), L"");
        nlohmann::json data = nlohmann::basic_json<>::parse(utils::Texts::toUtf8(meta));
        if (data.is_array() && !data.empty()) {
            auto e = data[0];
            if (e.contains("type") && e.contains("ext") && !e["type"].is_null() && !e["ext"].is_null() && e["type"].is_string() && e["ext"].is_string()) {
                // std::string type = e["type"].get<std::string>();
                // std::wstring ext = utils::Folders::GetExt(utils::Texts::fromUtf8(type));
                std::string ext = e["ext"].get<std::string>();
                std::string c = utils::SqlData::readBlob(L"files", idValue, L"content");
                //bstd::wstring fileName = utils::Folders::GetCurrentPath() + L"\\temp." + utils::Texts::fromUtf8(ext);
                result.ext = utils::Texts::fromUtf8(ext);
                utils::Folders::WriteFile(fileName + L"." + result.ext, c);
                result.code = 0;
            }
        } else {
            result.code = -1;
        }
        return result;
    }

    std::vector<std::optional<std::wstring>> getType(int templateId, std::wstring const& typeNo) {
        std::vector<std::optional<std::wstring>> result;
        std::vector<std::vector<std::optional<std::wstring>>> types = utils::SqlData::ExecuteQueryTable(L"SELECT "
            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"types"))
            + L" FROM \"types\" WHERE \"no\" = " + typeNo + L";");
        if (types.size() == 1) {
            result = types[0];
        }
        return result;
    }

    std::vector<std::optional<std::wstring>> getList(int templateId, std::wstring const& typeNo) {
        std::vector<std::optional<std::wstring>> result;
        std::vector<std::vector<std::optional<std::wstring>>> lists = utils::SqlData::ExecuteQueryTable(L"SELECT "
            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"lists"))
            + L" FROM \"lists\" WHERE \"parent_no\" = " + typeNo + L";");
        if (lists.size() == 1) {
            result = lists[0];
        }
        return result;
    }

    std::vector<std::vector<std::optional<std::wstring>>> getComponents(int templateId, std::wstring const& typeNo) {
        return utils::SqlData::ExecuteQueryTable(L"SELECT "
            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"schemas"))
            + L" FROM \"schemas\" WHERE \"parent_no\" = " + typeNo + L";");
    }

    std::vector<std::wstring> getFilePath(std::wstring const& typeNo, std::vector<std::vector<std::wstring>>& ret) {
        std::vector<std::wstring> path;

        if (typeNo.length() == 1) {
            if (typeNo == L"8" || typeNo == L"9") {
               ret.push_back(path);
            }
        } else {
            auto type = getType(0, typeNo);
            if (!type.empty()) {
                if (type[4] == L"2") {
                    auto components = getComponents(0, typeNo);
                    for (auto const& component : components) {
                        auto rpath = getFilePath(component[5].value(), ret);
                        if (!rpath.empty()) {
                            path.push_back(component[4].value());
                            for (auto part : rpath) {
                                path.push_back(part);
                            }
                        }
                    }
                } else if (type[4] == L"3") {
                    auto list = getList(0, type[1].value());
                    if (!list.empty()) {
                        auto rpath = getFilePath(list[3].value(), ret);
                        if (!rpath.empty()) {
                            path.push_back(L"[]");
                            for (auto part : rpath) {
                                path.push_back(part);
                            }
                        }
                    }
                }
            }
        }

        return path;
    }
}

jsValue JS_CALL CWkeWebkitUI::ExportPackage(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 2) {
        jsValue type = jsArg(es, 0); // type: 0 batch 1 air
        jsValue id = jsArg(es, 1); // id
        if (jsIsNumber(type) && jsIsNumber(id)) {
            int typeValue = jsToInt(es, type);
            int idValue = jsToInt(es, id);

            std::vector<std::optional<std::wstring>> baseline;
            auto baselines = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"baselines"))
                + L" FROM \"baselines\" WHERE \"object_type\" = "
                + std::to_wstring(typeValue)
                + L" AND \"object_id\" = "
                + std::to_wstring(idValue));
            if (baselines.size() == 1) {
                baseline = baselines[0];
            }

            std::vector<std::vector<std::optional<std::wstring>>> batchs;
            std::vector<std::optional<std::wstring>> batch;
            std::vector<std::vector<std::optional<std::wstring>>> airs;
            std::vector<std::optional<std::wstring>> air;
            switch (typeValue) {
                case 0:
                    batchs = utils::SqlData::ExecuteQueryTable(L"SELECT "
                        + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"batchs"))
                        + L" FROM \"batchs\" WHERE id = "
                        + std::to_wstring(idValue));
                    if (batchs.size() == 1) {
                        batch = batchs[0];
                    }
                    break;
                case 1:
                    airs = utils::SqlData::ExecuteQueryTable(L"SELECT "
                        + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"airs"))
                        + L" FROM \"airs\" WHERE id = "
                        + std::to_wstring(idValue));
                    if (airs.size() == 1) {
                        air = airs[0];
                        batchs = utils::SqlData::ExecuteQueryTable(L"SELECT "
                            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"batchs"))
                            + L" FROM \"batchs\" WHERE id = "
                            + air[1].value());
                        if (batchs.size() == 1) {
                            batch = batchs[0];
                        }
                    }
                    break;
            }

            std::vector<std::optional<std::wstring>> state;
            std::vector<std::vector<std::optional<std::wstring>>> states = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"states"))
                + L" FROM \"states\" WHERE id = "
                + batch[1].value());
            if (states.size() == 1) {
                state = states[0];
            }

            std::vector<std::optional<std::wstring>> model;
            std::vector<std::vector<std::optional<std::wstring>>> models = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"models"))
                + L" FROM \"models\" WHERE id = "
                + state[1].value());
            if (models.size() == 1) {
                model = models[0];
            }
            
            std::vector<std::vector<std::optional<std::wstring>>> configurations = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"data"))
                + L" FROM \"data\" WHERE \"ref_id\" = "
                + baseline[0].value());
            /*
            std::vector<std::vector<std::optional<std::wstring>>> auxiliaries = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"随机设备工具"))
                + L" FROM \"随机设备工具\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> machineAux = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"机械专业随机备件"))
                + L" FROM \"机械专业随机备件\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> resourceAux = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"随机资料目录"))
                + L" FROM \"随机资料目录\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> resourceDirectoryAux = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"随机资料配套目录"))
                + L" FROM \"随机资料配套目录\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> technologyStateFiles = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"技术状态文件"))
                + L" FROM \"技术状态文件\" WHERE \"基线id\" = "
                + baseline[0].value());
            std::vector<std::vector<std::optional<std::wstring>>> files;
            for (auto const& techFile : technologyStateFiles) {
                std::vector<std::vector<std::optional<std::wstring>>> queryFiles = utils::SqlData::ExecuteQueryTable(L"SELECT "
                    + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"files"))
                    + L" FROM \"files\" WHERE \"id\" = "
                    + techFile[10].value());
                if (!queryFiles.empty()) {
                    files.push_back(queryFiles[0]);
                }
            }

            std::vector<std::vector<std::optional<std::wstring>>> lifespans = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"有寿件"))
                + L" FROM \"有寿件\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> doubleCurrents = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"双流水"))
                + L" FROM \"双流水\" WHERE \"基线id\" = "
                + baseline[0].value());

            std::vector<std::vector<std::optional<std::wstring>>> problems = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"交付遗留问题及处置情况"))
                + L" FROM \"交付遗留问题及处置情况\" WHERE \"基线id\" = "
                + baseline[0].value());
            */
            std::wstring r = L"{\"baseline\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"baselines"), baseline)
                + L", \"batch\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"batchs"), batch)
                + L", \"state\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"states"), state)
                + L", \"model\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"models"), model)
                + L", \"data\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"data"), configurations)
                //+ L", \"configurations\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"构型"), configurations)
                //+ L", \"auxiliaries\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"随机设备工具"), auxiliaries)
                //+ L", \"machineAux\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"机械专业随机备件"), machineAux)
                //+ L", \"resourceAux\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"随机资料目录"), resourceAux)
                //+ L", \"resourceDirectoryAux\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"随机资料配套目录"), resourceDirectoryAux)
                //+ L", \"technologyStateFiles\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"技术状态文件"), technologyStateFiles)
                //+ L", \"files\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"files"), files)
                //+ L", \"lifespans\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"有寿件"), lifespans)
                //+ L", \"doubleCurrents\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"双流水"), doubleCurrents)
                //+ L", \"problems\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"交付遗留问题及处置情况"), problems)
                + (air.empty() ? L"" : L", \"air\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"飞机"), air))
                + L"}";
            std::wstring rootPath = utils::Folders::GetCurrentPath() + L"\\export";
            std::wstring destPath = rootPath + L"\\working";
            utils::Folders::RecurisiveMakeDir(destPath);
            //if (!utils::Folders::IsDirectoryExist(destPath)) {
            //    utils::Folders::MakeDir(destPath);
            //}
            std::wstring fileName = destPath + L"\\meta.json";
            utils::Folders::WriteFile(fileName, utils::Texts::toUtf8(r));
            
            // batch[5] // 布局
            // 布局\\图片\\name.ext ...
            if (batch[5].has_value()) {
                std::wstring layout = batch[5].value();
                nlohmann::json data = nlohmann::basic_json<>::parse(utils::Texts::toUtf8(layout));

                std::string name = utils::Texts::toUtf8(L"图片");
                std::string nameField = utils::Texts::toUtf8(L"名称");
                if (data.contains(name) && !data[name].is_null() && data[name].is_array()) {
                    for (auto& image : data[name]) {
                        if (image.contains(nameField) && !image[nameField].is_null() && image[nameField].is_string() && image.contains(name) && !image[name].is_null() && image[name].is_number()) {
                            std::string imageName = image[nameField].get<std::string>();
                            std::int64_t imageId = image[name].get<std::int64_t>();
                            saveFile(imageId, destPath + L"\\布局\\图片\\" + utils::Texts::fromUtf8(imageName));
                        }
                    }
                }
            }
            /*
            // batch[8] // 喷涂方案
            if (batch[8].has_value()) {
                std::wstring bid = batch[8].value();
                saveFile(std::stoi(bid), destPath + L"\\喷涂方案");
            }

            for (auto const& technologyStateFile : technologyStateFiles) {
                if (technologyStateFile[10].has_value()) {
                    std::wstring type = technologyStateFile[2].value();
                    //if (technologyStateFile[4].has_value()) {
                    //    std::wstring path = technologyStateFile[4].value();
                    //}
                    std::wstring name = technologyStateFile[0].value();
                    if (technologyStateFile[6].has_value()) {
                        name = technologyStateFile[6].value();
                    }
                    std::wstring blob = technologyStateFile[10].value();
                    std::wstring typePath = destPath + L"\\技术状态文件\\" + type;
                    utils::Folders::RecurisiveMakeDir(typePath);
                    saveFile(std::stoi(blob), typePath + L"\\" + name);
                }
            }
            */
            // zip it
            std::wstring zipFilename;
            // model[3] // 名称
            // state[2] // 编号
            // batch[2] // 编号
            // air[3] // 编号
            switch (typeValue) {
            case 0:
                // \\export\\批次包-modelNo-stateNo-batchNo.zip
                zipFilename = L"\\批次包-" + model[3].value() + L"-" + state[2].value() + L"-" + batch[2].value() + L".zip";
                break;
            case 1:
                // \\export\\单机包-modelNo-stateNo-batchNo-airNo.zip
                zipFilename = L"\\单机包-" + model[3].value() + L"-" + state[2].value() + L"-" + batch[2].value() + L"-" + air[2].value() + L".zip";
                break;
            }
            utils::zip::ZipViaZlib(rootPath + zipFilename + L".plain", destPath);
            // encrypt it
            utils::Folders::EncryptFileContent(rootPath + zipFilename + L".plain", rootPath + zipFilename);
            utils::Folders::FileDelete(rootPath + zipFilename + L".plain");
            // utils::Folders::FileCopy(destPath + zipFilename, rootPath + zipFilename);
            utils::Folders::DeleteDir(destPath);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::ExportTemplatePackage(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue id = jsArg(es, 0); // id
        if (jsIsNumber(id)) {
            int idValue = jsToInt(es, id);
            std::vector<std::optional<std::wstring>> theTemplate;
            auto templates = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"templates"))
                + L" FROM \"templates\" WHERE \"id\" = "
                + std::to_wstring(idValue));
            if (templates.size() == 1) {
                theTemplate = templates[0];
            }

            auto types = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"types"))
                + L" FROM \"types\" WHERE \"template_id\" = "
                + std::to_wstring(idValue));

            auto codes = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"codes"))
                + L" FROM \"codes\" WHERE \"template_id\" = "
                + std::to_wstring(idValue));

            auto schemas = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"schemas"))
                + L" FROM \"schemas\" WHERE \"template_id\" = "
                + std::to_wstring(idValue));

            auto lists = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"lists"))
                + L" FROM \"lists\" WHERE \"template_id\" = "
                + std::to_wstring(idValue));

            auto supportConfigs = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"info_configs"))
                + L" FROM \"info_configs\" WHERE \"template_id\" = "
                + std::to_wstring(idValue));

            std::wstring r = L"{\"template\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"templates"), theTemplate)
                + L", \"types\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"types"), types)
                + L", \"codes\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"codes"), codes)
                + L", \"schemas\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"schemas"), schemas)
                + L", \"lists\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"lists"), lists)
                + L", \"supportConfigs\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"info_configs"), supportConfigs)
                + L"}";
            std::wstring destPath = utils::Folders::GetCurrentPath() + L"\\export";
            if (!utils::Folders::IsDirectoryExist(destPath)) {
                utils::Folders::MakeDir(destPath);
            }
            // \\export\\template-templateId.json
            std::wstring fileName = destPath + L"\\template-" + std::to_wstring(idValue) + L".json";
            utils::Folders::WriteFile(fileName, utils::Texts::toUtf8(r));
            result = jsInt(0);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::ExportOrganizationsPackage(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 0) {
        auto organizations = utils::SqlData::ExecuteQueryTable(L"SELECT "
            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"organizations"))
            + L" FROM \"organizations\"");

        auto users = utils::SqlData::ExecuteQueryTable(L"SELECT "
            + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"users"))
            + L" FROM \"users\"");

        std::wstring r = L"{\"organizations\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"organizations"), organizations)
            + L", \"users\": " + utils::SqlData::toJson(utils::SqlData::getColumns(L"users"), users)
            + L"}";

        std::wstring destPath = utils::Folders::GetCurrentPath() + L"\\export";
        if (!utils::Folders::IsDirectoryExist(destPath)) {
            utils::Folders::MakeDir(destPath);
        }
        // \\export\\template-templateId.json
        std::wstring fileName = destPath + L"\\organizations.json";
        utils::Folders::WriteFile(fileName, utils::Texts::toUtf8(r));
        result = jsInt(0);
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::OpenFile(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue id = jsArg(es, 0);
        if (jsIsNumber(id)) {
            int idValue = jsToInt(es, id);
            std::wstring filename = utils::Folders::GetCurrentPath() + L"\\temp";
            auto r = saveFile(idValue, filename);
            result = jsInt(r.code);
            utils::Process::OpenUserFile(filename + L"." + r.ext);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::ExecuteSql(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue command = jsArg(es, 0);
        if (jsIsString(command)) {
            std::wstring cmd = jsToTempStringW(es, command);
            utils::SqlData::ExecuteNonQuery(cmd);
        }
    }
    return result;
}

jsValue JS_CALL CWkeWebkitUI::BuildInstall(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 1) {
        jsValue orgId = jsArg(es, 0);
        if (jsIsNumber(orgId)) {
            int organizationId = jsToInt(es, orgId);
            // query organization via organizationId
            std::vector<std::vector<std::optional<std::wstring>>> organizations = utils::SqlData::ExecuteQueryTable(L"SELECT "
                + utils::SqlData::constructColumnList(utils::SqlData::getColumns(L"organizations"))
                + L" FROM \"organizations\" WHERE \"id\" = "
                + std::to_wstring(organizationId));
            if (organizations.size() == 1) {
                std::wstring type = L"admin.bin";
                std::vector<std::optional<std::wstring>> organization = organizations[0];
                switch (std::stoi(organization[3].value())) {
                case 0:
                    type = L"admin.bin";
                    break;
                case 1:
                    type = L"producer.bin";
                    break;
                case 2:
                    type = L"user.bin";
                    break;
                }
                // query users via organizationId
                // create new db
                // insert organization & users
                // copy all other files to temp
                // zip temp directory contents to zipfile
                auto currentPath = utils::Folders::GetCurrentPath();
                auto workPath = currentPath + L"\\working";
                auto binPath = workPath + L"\\Bin";
                if (!utils::Folders::IsDirectoryExist(workPath)) {
                    utils::Folders::MakeDir(workPath);
                }
                utils::Folders::DeleteDir(workPath);
                utils::Folders::MakeDir(binPath);
                utils::Folders::FileCopy(currentPath + L"\\air.db", binPath + L"\\air.db");
                utils::Folders::FileCopy(currentPath + L"\\DuiLib.dll", binPath + L"\\DuiLib.dll");
                utils::Folders::FileCopy(currentPath + L"\\libcrypto-1_1.dll", binPath + L"\\libcrypto-1_1.dll");
                utils::Folders::FileCopy(currentPath + L"\\libcurl.dll", binPath + L"\\libcurl.dll");
                utils::Folders::FileCopy(currentPath + L"\\libssl-1_1.dll", binPath + L"\\libssl-1_1.dll");
                utils::Folders::FileCopy(currentPath + L"\\MiniBlinkBrowser.exe", binPath + L"\\MiniBlinkBrowser.exe");
                utils::Folders::FileCopy(currentPath + L"\\msvcr110.dll", binPath + L"\\msvcr110.dll");
                utils::Folders::FileCopy(currentPath + L"\\node.dll", binPath + L"\\node.dll");
                utils::Folders::FileCopy(currentPath + L"\\AirPlaneSetup.exe", binPath + L"\\AirPlaneSetup.exe");
                utils::Folders::FileCopy(currentPath + L"\\AirUninstall.exe", binPath + L"\\AirUninstall.exe");
                utils::Folders::FileCopy(currentPath + L"\\producer.bin", binPath + L"\\producer.bin");
                utils::Folders::FileCopy(currentPath + L"\\user.bin", binPath + L"\\user.bin");
                utils::Folders::FileCopy(currentPath + L"\\admin.bin", binPath + L"\\admin.bin");
                utils::Folders::MakeDir(binPath + L"\\favicon");
                utils::Folders::CopyDir(currentPath + L"\\favicon", binPath + L"\\favicon");
                utils::Folders::MakeDir(binPath + L"\\LocalStorage");
                utils::Folders::CopyDir(currentPath + L"\\LocalStorage", binPath + L"\\LocalStorage");
                utils::Folders::MakeDir(binPath + L"\\resources");
                utils::Folders::CopyDir(currentPath + L"\\resources", binPath + L"\\resources");
                utils::Folders::MakeDir(binPath + L"\\Skin");
                utils::Folders::CopyDir(currentPath + L"\\Skin", binPath + L"\\Skin");

                auto destPath = currentPath + L"\\export";
                if (!utils::Folders::IsDirectoryExist(destPath)) {
                    utils::Folders::MakeDir(destPath);
                }
                utils::zip::ZipViaZlib(destPath + L"\\program.zip", workPath);

                // utils::Folders::DeleteDir(destPath);
                // utils::Folders::CopyDir(L"E:\\test", L"E:\\next-test");
                // utils::zip::UnzipViaZlib(L"abc.zip", L"E:\\test");
                // remove unneed data from new db
                // utils::zip::Zip(currentPath + L"\\install.zip", destPath);
                std::wstring command = L"copy /b " + binPath + L"\\AirPlaneSetup.exe + " + binPath + L"\\" + type + L" + " + binPath + L"\\AirUninstall.exe + " + destPath + L"\\program.zip " + destPath + L"\\AirPlaneSetup.exe";
                // utils::Log::log(command);
                system(utils::Texts::toNative(command).c_str());
                // 计算机\HKEY_CURRENT_USER\SOFTWARE\AirPlaneAdmin
            }
            /*
            SetCurrentDirectory("c:\\docs\\stuff");
            HZIP hz = OpenZip("c:\\stuff.zip", 0);
            ZIPENTRY ze;
            GetZipItem(hz, -1, &ze);
            int numitems = ze.index;
            for (int i = 0; i < numitems; i++) {
                GetZipItem(hz, i, &ze);
                UnzipItem(hz, i, ze.name);
            }
            CloseZip(hz);
            */
        }
    }
    return result;
}

namespace {
    using namespace YExcel;
    std::wstring reallyWriteExcel(std::wstring const& fileName, std::wstring const& sheetname, std::wstring const& json) {
        nlohmann::json data = nlohmann::json::parse(utils::Texts::toUtf8(json));
        BasicExcel e;
        e.New(1);
        e.RenameWorksheet(0u, sheetname.c_str());
        BasicExcelWorksheet* sheet = e.GetWorksheet(0u);
        if (sheet) {
            auto count = data.size();
            for (size_t column = 0; column < count; ++column) {
                sheet->Cell(1, column + 1)->SetWString(utils::Texts::fromUtf8(data[column].get<std::string>()).c_str());
            }
        }
        std::wstring destPath = utils::Folders::GetCurrentPath() + L"\\export";
        if (!utils::Folders::IsDirectoryExist(destPath)) {
            utils::Folders::MakeDir(destPath);
        }
        std::wstring destFileName = destPath + L"\\" + fileName + L".xls";
        e.SaveAs(utils::Texts::toNative(destFileName).c_str());
        return destFileName;
    }
}

void rwe(std::wstring const& fileName, std::wstring const& sheetname, std::wstring const& json) {
    reallyWriteExcel(fileName, sheetname, json);
}

jsValue JS_CALL CWkeWebkitUI::WriteExcel(jsExecState es) {
    jsValue result = jsUndefined();
    int argCount = jsArgCount(es);
    if (argCount == 3) {
        jsValue filename = jsArg(es, 0);
        jsValue sheetName = jsArg(es, 1);
        jsValue dataJson = jsArg(es, 2);
        if (jsIsString(filename) && jsIsString(sheetName) && jsIsString(dataJson)) {
            std::wstring fileName = jsToTempStringW(es, filename);
            std::wstring sheetname = jsToTempStringW(es, sheetName);
            std::wstring json = jsToTempStringW(es, dataJson);
            auto fn = reallyWriteExcel(fileName, sheetname, json);
            utils::Process::OpenUserFile(fn);
            result = jsNull();
        }
    }
    return result;
}

/*
std::wstring params = lpContent;
auto j = nlohmann::json::parse(params);
if (strMethod.CompareNoCase(_T("select")) == 0) {
    std::wstring tableName, whereCause, orderBy;

    for (auto& item : j.items()) {
        if (item.key() == "tableName") {
            tableName = utils::Texts::fromUtf8(item.value());
        } else if (item.key() == "whereCause") {
            whereCause = utils::Texts::fromUtf8(item.value());
        } else if (item.key() == "orderBy") {
            orderBy = utils::Texts::fromUtf8(item.value());
        }
    }

    auto it = TABLES.find(tableName);
    if (it != TABLES.end()) {
        std::vector<std::wstring> const& columns = it->second;
        static std::wstring res;
        res = L"[";
        std::wstring query = utils::SqlData::constructQuery(tableName, whereCause, orderBy);
        std::vector<std::vector<std::wstring>> r = utils::SqlData::ExecuteQueryTable(query);
        bool first = true;
        for (auto& row : r) {
            if (first) {
                first = false;
            } else {
                res += L", ";
            }
            res += utils::SqlData::toJson(columns, row);
        }
        res += L"]";
        result = res.c_str();
    }
} else if (strMethod.CompareNoCase(_T("insert")) == 0) {
    result = L"-1";
    // ` ~ 1 ! 2 @ 3 # 4 $ 5 % 6 ^ 7 & 8 * 9 ( 0 ) - _ = +
    // q Q w W e E r R t T y Y u U i I o O p P [ { ] } \ |
    // a A s S d D f F g G h H j J k K l L ; : ' "
    // z Z x X c C v V b B n N m M , < . > / ?
    std::wstring tableName;
    nlohmann::json data;
    for (auto& item : j.items()) {
        if (item.key() == "tableName") {
            tableName = utils::Texts::fromUtf8(item.value());
        } else if (item.key() == "data") {
            data = utils::Texts::fromUtf8(item.value());
        }
    }

    auto it = TABLES.find(tableName);
    if (it != TABLES.end()) {
        std::vector<std::wstring> const& columns = it->second;
        std::map<std::wstring, std::wstring> values;
        for (auto& item : data.items()) {
            for (auto& name : columns) {
                if (utils::Texts::fromUtf8(item.key()) == name) {
                    values.insert(std::make_pair(name, item.value().get<std::wstring>()));
                    break;
                }
            }
        }
        int id = 0;
        std::wstring insert = utils::SqlData::constructInsert(tableName, values, id);
        std::wstring idStr = std::to_wstring(id);
        static wchar_t idStatic[32] = {};
        wcscpy_s(idStatic, idStr.c_str());
        result = idStatic;
        utils::SqlData::ExecuteNonQuery(insert);
    }
} else if (strMethod.CompareNoCase(_T("update")) == 0) {
    std::wstring tableName;
    nlohmann::json data;
    for (auto& item : j.items()) {
        if (item.key() == "tableName") {
            tableName = utils::Texts::fromUtf8(item.value());
        } else if (item.key() == "data") {
            data = utils::Texts::fromUtf8(item.value());
        }
    }

    auto it = TABLES.find(tableName);
    if (it != TABLES.end()) {
        std::vector<std::wstring> const& columns = it->second;
        result = L"0";
        std::map<std::wstring, std::wstring> values;
        for (auto& item : data.items()) {
            for (auto& name : columns) {
                if (utils::Texts::fromUtf8(item.key()) == name) {
                    values.insert(std::make_pair(name, item.value().get<std::wstring>()));
                    break;
                }
            }
        }
        std::wstring update = utils::SqlData::constructUpdate(tableName, values);
        utils::SqlData::ExecuteNonQuery(update);
    }
} else if (strMethod.CompareNoCase(_T("remove")) == 0) {
    std::wstring tableName;
    int id = 0;
    for (auto& item : j.items()) {
        if (item.key() == "tableName") {
            tableName = utils::Texts::fromUtf8(item.value());
        } else if (item.key() == "id") {
            id = item.value().get<int>();
        }
    }

    auto it = TABLES.find(tableName);
    if (it != TABLES.end()) {
        std::wstring remove = utils::SqlData::constructRemove(tableName, id);
        utils::SqlData::ExecuteNonQuery(remove);
    }
}
return result;
*/
