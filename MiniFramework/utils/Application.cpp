#include <Windows.h>
#include <ShellAPI.h> // for APPBARDATA
// FIXME #include <afxcmn.h> // for CTreeCtrl
//#include <VersionHelpers.h>

#include "Application.h"
#include "Screen.h"
#include "Folders.h"
#include "Texts.h"

#include <map>

namespace {
    bool isOneInstance() {
        HANDLE mutex = CreateMutexW(NULL, TRUE, L"Miniblink browser");
        if ((mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) {
            ReleaseMutex(mutex);
            return false;
        }
        return true;
    }
}

/*
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE / *hPrevInstance* /, LPSTR / *lpCmdLine* /, int nCmdShow) {
    if (!isOneInstance()) {
        ::MessageBoxW(NULL, L"该进程已经启动", L"错误", MB_OK);
        return 0;
    }
    return utils::Application::Run(hInstance);
}
*/

namespace utils::Application {
    namespace {
        HINSTANCE hInstance_;
    }

    int Run(HINSTANCE hInstance) {
        hInstance_ = hInstance;
        /*
        HRESULT Hr = ::CoInitialize(NULL);
        if (FAILED(Hr)) return 0;

        CWkeWebkitUI::InitializeWebkit();

        // 初始化资源
        InitResource();

        // 创建主窗口
        CMainWnd* pMainWnd = new CMainWnd();
        if (pMainWnd == NULL) return 0;
        pMainWnd->Create(NULL, _T("miniblink使用例子"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
        pMainWnd->CenterWindow();

        //导入exe图标
        HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
        ::SendMessage(pMainWnd->GetHWND(), STM_SETICON, IMAGE_ICON, (LPARAM) (UINT) hIcon);

        // 启动消息循环
        MSG msg = { 0 };
        while (::GetMessage(&msg, NULL, 0, 0)) {
            if (!CPaintManagerUI::TranslateMessage(&msg)) {
                ::TranslateMessage(&msg);
                try {
                    ::DispatchMessage(&msg);
                } catch (...) {
                    DUITRACE(_T("EXCEPTION: %s(%d)\n"), __FILET__, __LINE__);
#ifdef _DEBUG
                    throw "CPaintManagerUI::MessageLoop";
#endif
                }
            }
        }

        //CWkeWebkitUI::UninitializeWebkit();

        // 释放资源
        CResourceManager::GetInstance()->Release();
        ::CoUninitialize();
        */
        return 0;
    }

    bool IsOsVistaExt() {
        //return IsWindowsVistaOrGreater();

        OSVERSIONINFO osinfo;
        osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&osinfo);

        //OSVERSIONINFOEXW versionInfo;
        //versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
        //VerifyVersionInfo(&versionInfo, dwTypeMask, dwlConditionMask);

        DWORD dwPlatformId = osinfo.dwPlatformId;
        DWORD dwMajorVersion = osinfo.dwMajorVersion;
        //Vista & Windows Server 2008         : 6.0
        //Windows 7 & Windows Server 2008 R2  : 6.1
        //Windows 8 & Windows Server 2012     : 6.2
        //Windows 8.1 & Windows Server 2012 R2: 6.3
        return ((dwPlatformId == 2) && (dwMajorVersion >= 6));
    }

    void AutoHideTaskbar(bool isAutoHide) {
        APPBARDATA appBar;
        memset(&appBar, 0, sizeof(appBar));
        appBar.cbSize = sizeof(appBar);
        appBar.hWnd = ::FindWindowW(L"Shell_TrayWnd", nullptr);
        if (appBar.hWnd) {
            appBar.lParam = isAutoHide ? ABS_AUTOHIDE : ABS_ALWAYSONTOP;
            SHAppBarMessage(ABM_SETSTATE, &appBar); // 设置任务栏自动隐藏
        }
    }

    //std::map<std::wstring, std::wstring> EnumerateWindows() {
    //    std::map<std::wstring, std::wstring> winClassTitle;
    //    CWnd* desktop = CWnd::GetDesktopWindow();
    //    if (desktop) {
    //        CWnd* win = desktop->GetWindow(GW_CHILD);
    //        while (win) {
    //            winClassTitle[MyGetClassName(win->GetSafeHwnd())] = MyGetWindowText(win->GetSafeHwnd());
    //            win = desktop->GetWindow(GW_HWNDNEXT);
    //        }
    //    }
    //    return winClassTitle;
    //}

    std::wstring MyGetClassName(HWND const& win) {
        wchar_t className[256] = {};
        GetClassNameW(win, className, 256);
        return className;
    }

    std::wstring MyGetWindowText(HWND const& win) {
        wchar_t title[256] = {};
        GetWindowTextW(win, title, 256);
        return title;
    }

    DWORD GetProcessIdByHWND(HWND const& win) {
        DWORD processId = 0;
        GetWindowThreadProcessId(win, &processId);
        return processId;
    }

    HWND FindWindowByTitle(std::wstring const& title) {
        return MyFindWindow<std::wstring>([](auto const& win, auto const& title) {
            auto result = false;
            auto const& currentTitle = MyGetWindowText(win);
            if (currentTitle == title) {
                result = true;
            }
            return result;
            }, title);
    }

    HWND FindWindowByName(std::wstring const& titleName) {
        struct TArgs {
            std::wstring titleName;
            HWND findWindow;
        };
        TArgs args;
        args.titleName = titleName;
        args.findWindow = (HWND)INVALID_HANDLE_VALUE;
        EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
            TArgs* args = (TArgs*)lParam;
            wchar_t wndName[256] = { 0 };
            ::GetWindowTextW(hWnd, wndName, sizeof(wndName));
            if (std::wstring(wndName) == args->titleName) {
                args->findWindow = hWnd;
                return FALSE;
            }
            return TRUE;
            }, (LPARAM)& args); // 枚举所有运行的窗口
        return args.findWindow;
    }

    HWND FindWindowByProcessId(DWORD processId) {
        return MyFindWindow<DWORD>([](auto const& win, auto const& processId) {
            auto result = false;
            auto procId = GetProcessIdByHWND(win);
            if (procId == processId) {
                result = true;
            }
            return result;
            }, processId);
    }

    HWND FindWindowByTitleAndProcessId(std::wstring const& title, DWORD processId) {
        struct FilterParam {
            std::wstring title;
            DWORD processId;
        };
        FilterParam params;
        params.title = title;
        params.processId = processId;
        return MyFindWindow<FilterParam>([](auto const& win, auto const& filterParams) {
            auto result = false;
            auto procId = GetProcessIdByHWND(win);
            auto const& title = MyGetWindowText(win);
            if (title == filterParams.title && procId == filterParams.processId) {
                result = true;
            }
            return result;
            }, params);
    }

    HWND DirectFindWindowByTitle(std::wstring const& title) {
        return FindWindowW(nullptr, title.c_str());
    }

    HWND DirectFindWindowByClass(std::wstring const& className) {
        return FindWindowW(className.c_str(), nullptr);
    }

    HWND DirectFindWindowByClassAndTitle(std::wstring const& className, std::wstring const& title) {
        return FindWindowW(className.c_str(), title.c_str());
    }

    bool const ActiveWindowViaAPI(HWND window) {
        bool result = false;
        if (window != INVALID_HANDLE_VALUE) {
            ::ShowWindow(window, SW_SHOWMAXIMIZED); // 激活找到的前一个程序
            ::SetForegroundWindow(window); // 把它设为前景窗口
            result = true;
        }
        return result;
    }

    //bool const ActiveWindowViaMessage(HWND window) {
    //    bool result = false;
    //    if (window != INVALID_HANDLE_VALUE) {
    //        SetWindowPos(window, CWnd::wndTop.GetSafeHwnd(), 0, 0, 1920 * utils::Screens::wScale(utils::Screens::REAL_WIDTH), 1080 * utils::Screens::hScale(utils::Screens::REAL_HEIGHT), 0);
    //        SendMessage(window, WM_SYSCOMMAND, SC_RESTORE, 0);
    //        result = true;
    //    }
    //    return result;
    //}

    //int EnumerateTreeItem(CTreeCtrl* tree, HTREEITEM parentItem, std::function<int(CTreeCtrl* tree, HTREEITEM item, bool isLeaf)>&& process) {
    //    int result = 0;
    //    HTREEITEM parent = parentItem ? parentItem : tree->GetRootItem();
    //    if (parent) {
    //        HTREEITEM child = tree->GetChildItem(parent);
    //        while (child) {
    //            HTREEITEM next = tree->GetNextSiblingItem(child);
    //            if (tree->GetChildItem(child)) { // has grandson
    //                result = process(tree, child, false); // branch
    //            } else {
    //                result = process(tree, child, true); // leaf
    //            }
    //            if (result) {
    //                break;
    //            }
    //            child = next;
    //        }
    //    }
    //    return result;
    //}

    //int RecursiveEnumerateTreeItem(CTreeCtrl* tree, HTREEITEM parentItem, std::function<int(CTreeCtrl* tree, HTREEITEM item, bool isLeaf)>&& process) {
    //    int result = 0;
    //    HTREEITEM parent = parentItem ? parentItem : tree->GetRootItem();
    //    if (parent) {
    //        HTREEITEM child = tree->GetChildItem(parent);
    //        while (child) {
    //            HTREEITEM next = tree->GetNextSiblingItem(child);
    //            if (tree->GetChildItem(child)) { // has grandson
    //                result = RecursiveEnumerateTreeItem(tree, child, std::move(process)); // recursive
    //                if (result == 0) {
    //                    result = process(tree, child, false); // branch
    //                }
    //            } else {
    //                result = process(tree, child, true); // leaf
    //            }
    //            if (result) {
    //                break;
    //            }
    //            child = next;
    //        }
    //    }
    //    return result;
    //}

    //int countSelectTreeItem(CTreeCtrl* tree) {
    //    int count = 0;
    //    RecursiveEnumerateTreeItem(tree, nullptr, [&count](auto t, auto i, auto leaf) {
    //        if (leaf) {
    //            if (t->GetCheck(i)) {
    //                count++;
    //            }
    //        }
    //        return 0;
    //    });
    //    return count;
    //}

    //int removeSelectTreeItem(CTreeCtrl* tree) {
    //    int count = 0;
    //    RecursiveEnumerateTreeItem(tree, nullptr, [&count](auto t, auto i, auto leaf) {
    //        if (leaf) {
    //            if (t->GetCheck(i)) {
    //                DWORD_PTR data = t->GetItemData(i);
    //                // remove file of data
    //                t->DeleteItem(i);
    //            }
    //        } else {
    //            if (!t->GetChildItem(i)) {
    //                DWORD_PTR data = t->GetItemData(i);
    //                // remove directory of data
    //                t->DeleteItem(i);
    //            }
    //        }
    //        return 0;
    //    });
    //    return count;
    //}

    //int copySelectTreeItem(CTreeCtrl* left, CTreeCtrl* right) {
    //    int count = 0;
    //    RecursiveEnumerateTreeItem(left, nullptr, [&count, &right](auto t, auto i, auto leaf) {
    //        if (leaf) {
    //            if (t->GetCheck(i)) {
    //                DWORD_PTR data = t->GetItemData(i);
    //                // copy file of data
    //                CString text = t->GetItemText(i);
    //                auto parent = t->GetParentItem(i);
    //                auto previousSibling = t->GetPrevSiblingItem(i);
    //                right->InsertItem(text, parent, previousSibling);
    //            }
    //        } else {
    //            DWORD_PTR data = t->GetItemData(i);
    //            // make directory of data
    //        }
    //        return 0;
    //    });
    //    return count;
    //}

    //void SetChildrenCheck(CTreeCtrl* tree, HTREEITEM parent, BOOL checked) {
    //    //tree->Expand(parent, TVE_EXPAND);
    //    RecursiveEnumerateTreeItem(tree, nullptr, [&checked](auto t, auto i, auto leaf) {
    //        t->SetCheck(i, checked);
    //        return 0;
    //    });
    //    //HTREEITEM child = tree->GetChildItem(parent);  // 获取子项句柄
    //    //while (child) {
    //    //	tree->SetCheck(child, checked);
    //    //	SetChildrenCheck(tree, child, checked); // 递归调用
    //    //	child = tree->GetNextSiblingItem(child); // 获取兄弟的句柄
    //    //}
    //}

    //BOOL GetChildrenChecked(CTreeCtrl* tree, HTREEITEM parent) {
    //    BOOL result = FALSE;
    //    EnumerateTreeItem(tree, parent, [&result](auto tree, auto item, auto isLeaf) {
    //        int r = 0;
    //        if (tree->GetCheck(item)) {
    //            r = 1;
    //            result = TRUE;
    //        }
    //        return r;
    //    });
    //    //HTREEITEM child = tree->GetChildItem(parent);
    //    //while (child) {
    //    //	if (tree->GetCheck(child)) {
    //    //		result = TRUE;
    //    //		break;
    //    //	}
    //    //	child = tree->GetNextSiblingItem(child);
    //    //}
    //    return result;
    //}

    //BOOL GetSiblingChecked(CTreeCtrl* tree, HTREEITEM node) {
    //    BOOL siblingChecked = FALSE;
    //    HTREEITEM previousSibling = tree->GetPrevSiblingItem(node);
    //    while (previousSibling) {
    //        BOOL c = tree->GetCheck(previousSibling);
    //        if (c) {
    //            siblingChecked = TRUE;
    //            break;
    //        }
    //        previousSibling = tree->GetPrevSiblingItem(previousSibling);
    //    }
    //    if (!siblingChecked) {
    //        HTREEITEM nextSibling = tree->GetNextSiblingItem(node);
    //        while (nextSibling) {
    //            BOOL c = tree->GetCheck(nextSibling);
    //            if (c) {
    //                siblingChecked = TRUE;
    //                break;
    //            }
    //            nextSibling = tree->GetNextSiblingItem(nextSibling);
    //        }
    //    }
    //    return siblingChecked;
    //}

    // first child checked, set ancestors
    // last child unchecked, clear parent, then if parent's sibling all unchecked, uplevel
    //void CascadeCheck(CTreeCtrl* tree, HTREEITEM node, BOOL checked) {
    //    // set parent checked
    //    HTREEITEM parent = tree->GetParentItem(node);
    //    if (parent) {
    //        BOOL c = tree->GetCheck(parent);
    //        if (c != checked) {
    //            if (c) {
    //                if (!GetChildrenChecked(tree, parent)) {
    //                    CascadeCheck(tree, parent, checked);
    //                }
    //                //if (!GetSiblingChecked(tree, node)) {
    //                //	CascadeCheck(tree, parent, checked);
    //                //}
    //            } else {
    //                CascadeCheck(tree, parent, checked);
    //            }
    //        }
    //    }

    //    // self checked
    //    tree->SetCheck(node, checked);
    //}

    TV_INSERTSTRUCTW GetItemWarp(HTREEITEM parent, wchar_t* const text, int image, LPARAM lParam) {
        TV_INSERTSTRUCTW itemWarp;
        itemWarp.hParent = parent;
        itemWarp.hInsertAfter = TVI_LAST;
        itemWarp.item.mask = TVIF_TEXT;
        itemWarp.item.hItem = TVI_ROOT;
        itemWarp.item.state = 0;
        itemWarp.item.stateMask = 0;
        itemWarp.item.pszText = text;
        itemWarp.item.cchTextMax = 24;
        itemWarp.item.iImage = image;
        itemWarp.item.iSelectedImage = 1;
        itemWarp.item.cChildren = 0;
        itemWarp.item.lParam = lParam;
        return itemWarp;
    }

    bool accept(WIN32_FIND_DATAW const& file) {
        bool result = false;
        std::wstring fileName = file.cFileName;
        fileName = Texts::toLower(fileName);
        if (fileName.find(L".txt")
            || fileName.find(L".pdf")
            || fileName.find(L".doc") || fileName.find(L".docx")
            || fileName.find(L".xls") || fileName.find(L".xlsx")
            || fileName.find(L".ppt") || fileName.find(L".pptx")
            || fileName.find(L".mp3")
            || fileName.find(L".mp4")
            || fileName.find(L".wmv")
            || fileName.find(L".flv")
            || fileName.find(L".swf")) {
            result = true;
        }
        return result;
    }

    //void CreateTreeFromPath(CTreeCtrl* tree, HTREEITEM parent, std::wstring const& folder, std::function<bool(WIN32_FIND_DATA const& file)>&& filter) {
    //    TV_INSERTSTRUCT itemWarp = GetItemWarp(parent, nullptr, 0, 0);
    //    Folders::IterateFiles(folder, [&tree, &folder, &itemWarp, &filter](auto type, auto const& file) {
    //        itemWarp.item.pszText = const_cast<WCHAR*>(file.cFileName);
    //        itemWarp.item.lParam = reinterpret_cast<LPARAM>(&file);
    //        if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    //            itemWarp.item.iImage = 0;
    //            HTREEITEM node = tree->InsertItem(&itemWarp);
    //            CreateTreeFromPath(tree, node, folder + L"\\" + file.cFileName, std::move(filter));
    //        } else {
    //            if (filter(file)) {
    //                itemWarp.item.iImage = 1;
    //                tree->InsertItem(&itemWarp);
    //            }
    //        }
    //    });
    //    return;
    //}

    //void BuildTree(CTreeCtrl* tree, std::wstring const& text, std::wstring const& folder, std::function<bool(WIN32_FIND_DATA const& file)>&& filter) {
    //    tree->DeleteAllItems();
    //    TV_INSERTSTRUCT itemWarp = GetItemWarp(nullptr, const_cast<wchar_t*>(text.c_str()), 0, 0); // L"自有课件";
    //    HTREEITEM root = tree->InsertItem(&itemWarp);
    //    CreateTreeFromPath(tree, root, folder, std::move(filter));
    //    tree->Expand(root, TVE_EXPAND);
    //}
}