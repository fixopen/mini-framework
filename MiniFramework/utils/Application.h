#pragma once

#include <string>
#include <map>
#include <functional>
#include <wtypes.h> // for HWND HINSTANCE
#include <CommCtrl.h> // for HTREEITEM

namespace utils::Application {
    int Run(HINSTANCE hInstance);
    /*
    void LoadLibraries(); // use boost::dll & cef & miniblink
    void LoadUIResources(); // use unzip & expat
    void CreateMainWindow(); // ControlFactory
    void MainMessageLoop();
    void DestroyMainWindow();
    void UnloadUIResources();
    void UnloadLibraries();
    */

    bool IsOsVistaExt();
    void AutoHideTaskbar(bool isAutoHide = true);
    //std::map<std::wstring, std::wstring> EnumerateWindows();

    std::wstring MyGetClassName(HWND const& win);
    std::wstring MyGetWindowText(HWND const& win);
    DWORD GetProcessIdByHWND(HWND const& win);

    template<typename T>
    struct EnumWindowParam {
        EnumWindowParam(std::function<bool(HWND const& win, T const&)> const& accept, T const& params): filter(accept), filterParams(params), foundWindow(nullptr){}
        std::function<bool(HWND const& win, T const&)> const& filter;
        T const& filterParams;
        HWND foundWindow;
    };

    template<typename T>
    HWND MyFindWindow(std::function<bool(HWND const& win, T const&)> const& accept, T const& filterParams) {
        EnumWindowParam<T> param(accept, filterParams);
        EnumWindows([](HWND win, LPARAM lParam) {
            BOOL result = TRUE; // continue
            EnumWindowParam<T>& param = *reinterpret_cast<EnumWindowParam<T>*>(lParam);
            if (param.filter(win, param.filterParams)) {
                param.foundWindow = win;
                result = FALSE; // break
            }
            return result;
        }, reinterpret_cast<LPARAM>(&param));
        return param.foundWindow;
    }

    HWND FindWindowByTitle(std::wstring const& title);
    HWND FindWindowByName(std::wstring const& titleName);
    HWND FindWindowByProcessId(DWORD processId);
    HWND FindWindowByTitleAndProcessId(std::wstring const& title, DWORD processId);

    HWND DirectFindWindowByTitle(std::wstring const& title);
    HWND DirectFindWindowByClass(std::wstring const& className);
    HWND DirectFindWindowByClassAndTitle(std::wstring const& className, std::wstring const& title);

    bool const ActiveWindowViaAPI(HWND window);
    //bool const ActiveWindowViaMessage(HWND window);

    //int RecursiveEnumerateTreeItem(CTreeCtrl* tree, HTREEITEM parentItem, std::function<int(CTreeCtrl* tree, HTREEITEM item, bool isLeaf)>&& process);
}
