#pragma once

#include <string>
#include <vector>
#include <functional>

#include <wtypes.h>
#include <TlHelp32.h>

namespace utils::Process {
    BOOL TerminateProcessById(DWORD dwProcessId, UINT uExitCode);
    void ExitProcess(std::string const& name);
    HANDLE RunProcess(std::wstring const& cmd, std::wstring const& param, std::wstring const& path, BOOL sync, BOOL hide);
    HINSTANCE StartProcess(std::wstring const& cmd);
    void Open(HWND parent, std::wstring const& file);
    void OpenUserFile(std::wstring const& file);
    void OpenMedia(std::wstring const& media);
    std::wstring const RunCmd(std::wstring const& cmd, std::wstring const& dir, BOOL show);
    DWORD FindProcessIdByName(std::wstring const& fileName);
    std::vector<DWORD> const FindProcessIds(std::wstring const& processName);
    std::vector<DWORD> const FindProcessIdsByName(std::wstring const& processName);

    template<unsigned int SNAPSHOT>
    struct SnapshotPolicy;

    template<typename TEntry>
    using IterateFunction = BOOL(__stdcall *)(HANDLE handler, TEntry* entry);

    //template<typename TEntry = void, IterateFunction<TEntry> First = nullptr, IterateFunction<TEntry> Next = nullptr>
    //struct SnapshotPolicy<TH32CS_SNAPALL> {
    //    using first = First;
    //    using next = Next;
    //    using entry_type = TEntry;
    //};

    template<>
    struct SnapshotPolicy<TH32CS_SNAPHEAPLIST> {
        using entry_type = HEAPLIST32;
        IterateFunction<entry_type> first = (IterateFunction<entry_type>) Heap32ListFirst;
        IterateFunction<entry_type> next = (IterateFunction<entry_type>) Heap32ListNext;
    };

    template<>
    struct SnapshotPolicy<TH32CS_SNAPTHREAD> {
        using entry_type = THREADENTRY32;
        IterateFunction<entry_type> first = (IterateFunction<entry_type>) Thread32First;
        IterateFunction<entry_type> next = (IterateFunction<entry_type>) Thread32Next;
    };

    template<>
    struct SnapshotPolicy<TH32CS_SNAPMODULE> {
        using entry_type = MODULEENTRY32W;
        IterateFunction<entry_type> first = (IterateFunction<entry_type>) Module32First;
        IterateFunction<entry_type> next = (IterateFunction<entry_type>) Module32Next;
    };

    template<>
    struct SnapshotPolicy<TH32CS_SNAPPROCESS> {
        using entry_type = PROCESSENTRY32W;
        IterateFunction<entry_type> first = (IterateFunction<entry_type>) Process32First;
        IterateFunction<entry_type> next = (IterateFunction<entry_type>) Process32Next;
    };

    enum class ProcessStatus {
        CONTINUE,
        BREAK
    };

    template<unsigned int SNAPSHOT>
    void IterateSnapshot(std::function<ProcessStatus(typename SnapshotPolicy<SNAPSHOT>::entry_type const& e)>&& process) {
        HANDLE snapshot = CreateToolhelp32Snapshot(SNAPSHOT, 0);
        if (snapshot != INVALID_HANDLE_VALUE) {
            SnapshotPolicy<SNAPSHOT> policy;
            typename SnapshotPolicy<SNAPSHOT>::entry_type entry;
            if (policy.first(snapshot, &entry)) {
                ProcessStatus ps = ProcessStatus::CONTINUE;
                do {
                    ps = process(entry);
                } while (policy.next(snapshot, &entry) && (ps == ProcessStatus::CONTINUE));
            }
            CloseHandle(snapshot);
        }
    }

    template<typename TEntry, unsigned int SNAPSHOT>
    void IterateSnapshot(TEntry entry, BOOL (__stdcall *first)(HANDLE handler, TEntry* entry), std::function<ProcessStatus(TEntry& entry)>&& process, BOOL(__stdcall *next)(HANDLE handler, TEntry* entry)) {
        HANDLE snapshot = CreateToolhelp32Snapshot(SNAPSHOT, 0);  // TH32CS_SNAPALL TH32CS_SNAPHEAPLIST(Heap32ListFirst) TH32CS_SNAPTHREAD(Thread32First) TH32CS_SNAPMODULE(Module32First) TH32CS_SNAPPROCESS(Process32First)
        if (snapshot != INVALID_HANDLE_VALUE) {
            if (first(snapshot, &entry)) {
                ProcessStatus ps = ProcessStatus::CONTINUE;
                do {
                    ps = process(entry);
                } while (next(snapshot, &entry) && (ps == ProcessStatus::CONTINUE));
            }
            CloseHandle(snapshot);
        }
    }
}
