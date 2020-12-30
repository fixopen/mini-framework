#include <Windows.h>
#include <ShellAPI.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "Process.h"
#include "Texts.h"

#include <algorithm>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace utils::Process {
    BOOL TerminateProcessById(DWORD dwProcessId, UINT uExitCode) {
        BOOL result = FALSE;
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
        if (hProcess != NULL) {
            result = TerminateProcess(hProcess, uExitCode);
            CloseHandle(hProcess);
        }
        return result;
    }

    void ExitProcess(std::string const& name) {
        // TASKKILL /T /F /IM LitterRainProgRun.exe
        system(("TASKKILL /F /IM " + name).c_str());
    }

    // run进程
    HANDLE RunProcess(std::wstring const& cmd, std::wstring const& param, std::wstring const& path, BOOL sync, BOOL hide) {
        SHELLEXECUTEINFOW shellExecuteInfo = { 0 };
        shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
        shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shellExecuteInfo.hwnd = NULL;
        shellExecuteInfo.lpVerb = NULL;
        shellExecuteInfo.lpFile = cmd.c_str();
        shellExecuteInfo.lpParameters = param.c_str();
        shellExecuteInfo.lpDirectory = path.c_str();
        shellExecuteInfo.nShow = hide ? SW_SHOW : SW_HIDE;
        shellExecuteInfo.hInstApp = NULL;
        ShellExecuteExW(&shellExecuteInfo);
        if (sync) {
            WaitForSingleObject(shellExecuteInfo.hProcess, INFINITE);
        }
        return shellExecuteInfo.hProcess;
    }

    HINSTANCE StartProcess(std::wstring const& cmd) {
        return ShellExecuteW(NULL, NULL, cmd.c_str(), NULL, NULL, SW_SHOW);
    }

    namespace {
        inline INPUT construct(char c) {
            INPUT ip;
            ip.type = INPUT_KEYBOARD;
            ip.ki.wScan = 0;
            ip.ki.time = 0;
            ip.ki.dwExtraInfo = 0;
            ip.ki.wVk = c;
            return ip;
        }

        void PressKey(char c) {
            INPUT ip = construct(c);
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
        }

        void ReleaseKey(char c) {
            INPUT ip = construct(c);
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
        }

        void SendChar(char c) {
            INPUT ip = construct(c);
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            ip.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(1);
        }

        void SendText(char *text) {
            auto len = strlen(text);
            for (int i = 0; i < len; i++) {
                SendChar(text[i]);
            }
        }

        void SendInt(int number) {
            char buffer[32] = {};
            _itoa(number, buffer, 10);
            SendText(buffer);
        }
    }

    void Open(HWND parent, std::wstring const& file) {
        //SetWindowPos(CWnd::wndBottom.m_hWnd, nullptr, 0, 0, utils::Screens::REAL_WIDTH, utils::Screens::REAL_HEIGHT, 0);
        //utils::Utils::autoHideTaskbar(FALSE);
        ShellExecuteW(parent, L"open", file.c_str(), L"", L"", SW_SHOWMAXIMIZED);
        //utils::Utils::autoHideTaskbar(TRUE);
        //SetWindowPos(CWnd::wndTop.m_hWnd, nullptr, 0, 0, utils::Screens::REAL_WIDTH, utils::Screens::REAL_HEIGHT, 0);
    }

    void OpenUserFile(std::wstring const& file) {
        ShellExecuteW(NULL, L"open", file.c_str(), L"", L"", SW_SHOWMAXIMIZED);
    }

    void OpenMedia(std::wstring const& media) {
        //SetWindowPos(CWnd::wndBottom.m_hWnd, nullptr, 0, 0, utils::Screens::REAL_WIDTH, utils::Screens::REAL_HEIGHT, 0);
        //utils::Utils::autoHideTaskbar(FALSE);
        STARTUPINFOW StartupInfo; // This is an [in] parameter
        memset(&StartupInfo, 0, sizeof StartupInfo);
        StartupInfo.cb = sizeof StartupInfo; // Only compulsory field
        PROCESS_INFORMATION ProcessInfo;
        if (CreateProcessW(NULL, (LPWSTR)(L"C:\\Program Files\\MPlayer for Windows\\SMPlayer.exe -ontop " + media).c_str(),
                           NULL, NULL, FALSE, 0, NULL,
                           NULL, &StartupInfo, &ProcessInfo)) {
            //WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
            CloseHandle(ProcessInfo.hThread);
            CloseHandle(ProcessInfo.hProcess);
        } else {
            //AfxMessageBox(L"The process could not be started...");
            ::MessageBoxW(nullptr, L"The process could not be started...", L"", 0);
        }
        //utils::Utils::autoHideTaskbar(TRUE);
        //SetWindowPos(CWnd::wndTop.m_hWnd, nullptr, 0, 0, utils::Screens::REAL_WIDTH, utils::Screens::REAL_HEIGHT, 0);
    }

    std::wstring const RunCmd(std::wstring const& cmd, std::wstring const& dir, BOOL bshow) {
        std::wstring result;
        std::wstring command = L"Cmd.exe /C " + cmd;
        STARTUPINFOW si = {};
        si.cb = sizeof(STARTUPINFOW);
        GetStartupInfoW(&si);
        HANDLE hRead, hWrite;
        SECURITY_ATTRIBUTES sa = {};
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = nullptr;
        sa.bInheritHandle = TRUE;
        if (CreatePipe(&hRead, &hWrite, &sa, 0)) {
            si.hStdError = hWrite; // 把创建进程的标准错误输出重定向到管道输入
            si.hStdOutput = hWrite; // 把创建进程的标准输出重定向到管道输入
            si.wShowWindow = bshow ? SW_SHOW : SW_HIDE;
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            PROCESS_INFORMATION pi;
            CloseHandle(hWrite);
            if (CreateProcessW(NULL, const_cast<wchar_t*>(command.c_str()), NULL, NULL, TRUE, NULL, NULL, dir.c_str(), &si, &pi)) {
                std::string r;
                char buffer[4096] = { 0 }; // 用4K的空间来存储输出的内容，只要不是显示文件内容，一般情况下是够用了。
                while (true) {
                    DWORD bytesRead = 0;
                    BOOL bret = ReadFile(hRead, buffer, 4095, &bytesRead, NULL);
                    buffer[bytesRead] = 0;
                    if (bret == FALSE)
                        break;
                    r += buffer;
                }
                result = Texts::fromNative(r);
            }
            CloseHandle(hRead);
        }
        return result;
    }

    // find进程from名字
    DWORD FindProcessIdByName(std::wstring const& fileName) {
        DWORD result = 0;
        DWORD processIds[1024], needed;
        if (EnumProcesses(processIds, sizeof(processIds), &needed)) {
            DWORD processCount = needed / sizeof(DWORD);
            for (unsigned int i = 0; i < processCount; i++) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_ALL_ACCESS, FALSE, processIds[i]);
                if (hProcess) {
                    HMODULE hMod;
                    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &needed)) {
                        wchar_t szProcessName[MAX_PATH] = {};
                        GetModuleBaseNameW(hProcess, hMod, szProcessName, MAX_PATH);
                        if (fileName == szProcessName) {
                            result = processIds[i];
                            TerminateProcess(hProcess, 0);
                            break;
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        }
        return result;
    }

    std::vector<DWORD> const FindProcessIds(std::wstring const& processName) {
        std::vector<DWORD> result;
        //typename SnapshotPolicy<TH32CS_SNAPPROCESS>::entry_type entry;
        //entry.dwSize = sizeof(entry);
        IterateSnapshot<TH32CS_SNAPPROCESS>(std::move([&result, &processName](typename SnapshotPolicy<TH32CS_SNAPPROCESS>::entry_type const& entry) {
            ProcessStatus r = ProcessStatus::CONTINUE;
            if (Texts::toLower(processName) == Texts::toLower(entry.szExeFile)) {
                result.push_back(entry.th32ProcessID);
                // r = ProcessStatus::BREAK;
            }
            return r;
        }));
        return result;
    }

    std::vector<DWORD> const FindProcessIdsByName(std::wstring const& processName) {
        std::vector<DWORD> result;
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof(PROCESSENTRY32W);
        IterateSnapshot<PROCESSENTRY32W, TH32CS_SNAPPROCESS>(entry, Process32FirstW, [&result, &processName](PROCESSENTRY32W& entry) {
            ProcessStatus r = ProcessStatus::CONTINUE;
            if (Texts::toLower(processName) == Texts::toLower(entry.szExeFile)) {
                result.push_back(entry.th32ProcessID);
                // r = ProcessStatus::BREAK;
            }
            return r;
        }, Process32NextW);
        return result;
    }
}
