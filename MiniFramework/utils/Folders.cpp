//#include <Windows.h>
//#include <minwinbase.h> // for WIN32_FIND_DATAW
//#include <libloaderapi.h> // for GetModuleFilePathW lib: Kernel32.lib
//#include <handleapi.h> // for CloseHandle lib: Kernel32.lib
//#include <ioapiset.h> // for DeviceIoControl lib: Kernel32.lib
//#include <fileapi.h> // for FindFirstFileW, FindNextFileW, FindClose, CreateFileW, DeleteFile, GetLogicalDriveStringsW, CreateDirectoryW, RemoveDirectoryW, GetFileAttributes, SetFileAttributes lib: Kernel32.lib
#include <WTypes.h>
#include <WinNT.h>
#include <windef.h>
#include <WinBase.h> // for CopyFile lib: Kernel32.lib
//#include <errhandlingapi.h> // for GetLastError lib: Kernel32.lib
#include <Shlwapi.h> // for PathRemoveFileSpecW lib: Shlwapi.lib
//#include <Pathcch.h> // for PathCchRemoveFileSpec lib: pathcch.lib
#include <ShellAPI.h> // for SHFILEOPSTRUCTW, SHFileOperationW, ExtractIconW, SHGetFileInfoW, ShellExecute lib: Shell32.lib
#include <WinIoCtl.h>
#include <winver.h> // for GetFileVersionInfoSizeW, VerQueryValueW lib: Version.lib
//#include <verrsrc.h> // for VS_FIXEDFILEINFO
#include <cstdio> // for std::FILE, std::fopen, std::fclose, std::fseek, std::ftell, std::fread, std::fwrite
#include <atlimage.h>

//#include <WTypesbase.h>
//#include <minwindef.h>
//#include <winapifamily.h>
//#include <sdkddkver.h>
//#include <excpt.h>
//#include <stdarg.h>
//#include <ShlObj_core.h>
//#include <ShlObj.h>
//#include <shobjidl_core.h>
//#include <corecrt_io.h>
//#include <VersionHelpers.h>
//#include <afxwin.h>

#include "Folders.h"
#include "Texts.h"
#include "Utils.h"
#include "Reg.h"

#include <utility>
#include <fstream>
#include <filesystem> // for std::filesystem::path, std::filesystem::exists
#include <cstdio>

#pragma comment(lib, "Version.lib")
//#pragma comment(lib, "Mincore.lib")
//#pragma comment(lib, "Pathcch.lib")

namespace utils::Folders {
    void IterateFiles(std::wstring const& folder, std::function<void(FileItemType type, WIN32_FIND_DATAW const& findFileData)> const& fileproc) {
        WIN32_FIND_DATAW findFileData;
        auto findHandle = ::FindFirstFileW((folder + L"\\*.*").c_str(), &findFileData);
        if (findHandle != INVALID_HANDLE_VALUE) {
            do {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if ((wcscmp(findFileData.cFileName, L".") != 0) && (wcscmp(findFileData.cFileName, L"..") != 0)) {
                        fileproc(FileItemType::ENTER_DIRECTORY, findFileData);
                        fileproc(FileItemType::LEAVE_DIRECTORY, findFileData);
                    }
                } else {
                    fileproc(FileItemType::FILE, findFileData);
                }
            } while (::FindNextFileW(findHandle, &findFileData));
            ::FindClose(findHandle);
        }
    }

    void RecursiveIterateFiles(std::wstring const& folder, std::function<void(FileItemType type, std::wstring const& innerPath, WIN32_FIND_DATAW const& findFileData)> const& fileproc) {
        WIN32_FIND_DATAW findFileData;
        auto findHandle = ::FindFirstFileW((folder + L"\\*.*").c_str(), &findFileData);
        if (findHandle != INVALID_HANDLE_VALUE) {
            do {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if ((wcscmp(findFileData.cFileName, L".") != 0) && (wcscmp(findFileData.cFileName, L"..") != 0)) {
                        fileproc(FileItemType::ENTER_DIRECTORY, folder, findFileData);
                        std::wstring innerPath = folder + L"\\" + findFileData.cFileName;
                        RecursiveIterateFiles(innerPath, fileproc);
                        fileproc(FileItemType::LEAVE_DIRECTORY, innerPath, findFileData);
                    }
                } else {
                    fileproc(FileItemType::FILE, folder, findFileData);
                }
            } while (::FindNextFileW(findHandle, &findFileData));
            ::FindClose(findHandle);
        }
    }

    std::vector<std::wstring> splitPath(std::wstring const& path) {
        std::wstring temp;
        std::transform(path.begin(), path.end(), std::back_inserter(temp), [] (auto const& it) {
            auto r = it;
            if (r == L'/') {
                r = L'\\';
            }
            return r;
        });
        if (temp[temp.length() - 1] == L'\\') {
            temp = temp.substr(0, temp.length() - 1);
        }
        std::vector<std::wstring> result;
        if (!temp.empty()) {
            size_t offset = 0;
            size_t pos = temp.find(L'\\', offset);
            if (pos == std::wstring::npos) {
                result.push_back(temp);
            } else {
                do {
                    result.push_back(temp.substr(offset, pos - offset));
                    offset = pos + 1;
                    pos = temp.find(L'\\', offset);
                } while (pos != std::wstring::npos);
                result.push_back(temp.substr(offset));
            }
        }
        return result;
    }

    bool IsFileExist(std::wstring const& path) {
        std::filesystem::path p(path);
        return std::filesystem::exists(p);

        //bool result = false;
        //WIN32_FIND_DATA wfd;
        //HANDLE hFind = FindFirstFile(path.c_str(), &wfd);
        //if (hFind != INVALID_HANDLE_VALUE) {
        //    FindClose(hFind);
        //    result = true;
        //}
        //return result;

        //bool r = false;
        //if (_access(Texts::toNative(path).c_str(), 0) != -1) {
        //    r = true;
        //}
        //return r;
    }

    bool IsDirectoryExist(std::wstring const& path) {
        bool result = false;
        if (IsFileExist(path)) {
            result = true;
        }
        return result;
    }

    bool IsUsbDisk(std::wstring const& path) {
        bool result = false;
        // ���豸
        HANDLE device = ::CreateFileW(path.c_str(), // �ļ���
            GENERIC_READ,                           // ������Ҫ����
            FILE_SHARE_READ | FILE_SHARE_WRITE,     // ������ʽ
            NULL,                                   // Ĭ�ϵİ�ȫ������
            OPEN_EXISTING,                          // ������ʽ
            0,                                      // ���������ļ�����
            NULL);

        if (device != INVALID_HANDLE_VALUE) {
            // �� IOCTL_STORAGE_QUERY_PROPERTY
            STORAGE_PROPERTY_QUERY queryParams; // input param for query
            queryParams.PropertyId = StorageDeviceProperty;
            queryParams.QueryType = PropertyStandardQuery;

            STORAGE_DEVICE_DESCRIPTOR deviceDescriptor;
            deviceDescriptor.Size = sizeof(STORAGE_DEVICE_DESCRIPTOR);

            DWORD outBytes; // IOCTL output length

            BOOL bResult = ::DeviceIoControl(device, // device handle
                IOCTL_STORAGE_QUERY_PROPERTY, // info of device property
                &queryParams, sizeof(STORAGE_PROPERTY_QUERY), // input data buffer
                &deviceDescriptor, deviceDescriptor.Size, // output data buffer
                &outBytes, // out's length
                (LPOVERLAPPED)NULL);
            // 0x0 Unknown
            // 0x1 SCSI
            // 0x2 ATAPI
            // 0x3 ATA
            // 0x4 IEEE1394
            // 0x5 SSA(Serial storage architecture)
            // 0x6 Fibre Channel
            // 0x7 USB
            // 0x8 RAID
            if (deviceDescriptor.BusType == 0x07) {
                result = true;
            }
        }
        return result;
    }

    std::wstring const GetUsbDisk() {
        std::wstring result;
        std::map<std::wstring, unsigned int> const disks = GetDisks();
        for (auto item : disks) {
            switch (item.second) {
            case DRIVE_NO_ROOT_DIR:
                break;
            case DRIVE_REMOVABLE:
                result = item.first;
                break;
            case DRIVE_FIXED:
                if (IsUsbDisk(item.first)) {
                    result = item.first;
                }
                break;
            case DRIVE_REMOTE:
                break;
            case DRIVE_CDROM:
                break;
            }
            if (result != L"") {
                break;
            }
        }
        return result;
    }

    std::map<std::wstring, unsigned int> const GetDisks() {
        std::map<std::wstring, unsigned int> result;
        DWORD charCount = GetLogicalDriveStringsW(0, NULL);
        wchar_t* driveString = new wchar_t[charCount + 1];
        GetLogicalDriveStringsW(charCount, driveString);
        wchar_t* currentDrive = driveString;
        for (size_t currentDriveLength = wcslen(currentDrive); currentDriveLength; currentDriveLength = wcslen(currentDrive)) {
            result.insert(std::make_pair(std::wstring(currentDrive), GetDriveTypeW(currentDrive)));
            currentDrive += currentDriveLength + 1;
        }
        delete[] driveString;
        return result;
    }

    std::pair<std::wstring, std::wstring> const GetPhyDriveSerial() {
        std::pair<std::wstring, std::wstring> result;
        HANDLE drive = CreateFileW(L"\\\\.\\PHYSICALDRIVE0", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (drive == INVALID_HANDLE_VALUE) {
            DWORD returnedBytes;
            GETVERSIONINPARAMS versionParams;
            // get version
            DeviceIoControl(drive, SMART_GET_VERSION, NULL, 0,
                &versionParams, sizeof(versionParams), &returnedBytes, NULL);

            // identify device
            SENDCMDINPARAMS requestParams = { 0 };
            requestParams.irDriveRegs.bCommandReg = (versionParams.bIDEDeviceMap & 0x10) ? ATAPI_ID_CMD : ID_CMD;
            // -1����Ϊ SENDCMDOUTPARAMS �Ľ�β�� BYTE bBuffer[1];
            BYTE driveIdentify[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
            DeviceIoControl(drive, SMART_RCV_DRIVE_DATA, &requestParams, sizeof(SENDCMDINPARAMS) - 1,
                driveIdentify, sizeof(driveIdentify), &returnedBytes, NULL);
            CloseHandle(drive);

            // adjust the byte order
            PUSHORT words = (USHORT*)(((SENDCMDOUTPARAMS*)driveIdentify)->bBuffer);
            auto modelNo = Utils::ToLittleEndian(words, 27, 46);
            auto serialNo = Utils::ToLittleEndian(words, 10, 19);

            modelNo = Utils::TrimSpace(modelNo);
            serialNo = Utils::TrimSpace(serialNo);

            result.first = Utils::ToWString(modelNo);
            result.second = Utils::ToWString(serialNo);
        }
        return result;
    }

    std::wstring const GetProfilePathName() {
        std::wstring result;
        TCHAR szSpecialPath[MAX_PATH] = { 0 };
        //if (SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_PROFILE, FALSE) != FALSE) {
        //    result = szSpecialPath;
        //    // L"\\AppData\\Local\\Temp\\TTkvod"
        //}
        return result;
    }

    std::vector<std::wstring> ListFiles(std::wstring const& folder) {
        std::vector<std::wstring> result;
        RecursiveIterateFiles(folder, [&result](FileItemType type, auto const& innerPath, auto const& fileData) {
            if (type == FileItemType::FILE) {
                result.push_back(fileData.cFileName);
            }
        });
        return result;

        //std::vector<std::wstring> result;
        //std::filesystem::path p(folder);
        //std::filesystem::recursive_directory_iterator endIt;
        //for (std::filesystem::recursive_directory_iterator it(p); it != endIt; ++it) {
        //	if (it->is_regular_file()) {
        //		result.push_back(it->path().generic_wstring());
        //	}
        //}
    }

    std::vector<std::wstring> ListFileNames(std::wstring const& folder) {
        std::vector<std::wstring> result;
        std::filesystem::path p(folder);
        std::filesystem::directory_iterator endIt;
        for (std::filesystem::directory_iterator it(p); it != endIt; ++it) {
            if (it->is_regular_file()) {
                result.push_back(it->path().generic_wstring());
            }
        }
        //CFileFind finder; // �ļ�������
        //BOOL find = finder.FindFile((folder + L"\\*").c_str());
        //while (find) {
        //    find = finder.FindNextFile();
        //    if (finder.IsDots()) {
        //        continue; // �ҵ����ǵ�ǰĿ¼���ϼ�Ŀ¼������
        //    }
        //    auto fileName = finder.GetFileName(); // ��ȡ�ҵ����ļ���
        //    result.push_back(Texts::fromCString(fileName));
        //}
        //finder.Close();
        return result;
    }

    std::vector<WIN32_FIND_DATAW> ListOneLevelFiles(std::wstring const& folder) {
        std::vector<WIN32_FIND_DATAW> result;
        auto findFilename = folder + L"/*.*";
        WIN32_FIND_DATAW findFileData;
        BOOL hasNext = TRUE;
        auto checkAndExit = [&hasNext](auto hFind) {
            bool result = (hFind != INVALID_HANDLE_VALUE) && hasNext;
            if (!result) {
                ::FindClose(hFind);
            }
            return result;
        };
        for (auto hFind = ::FindFirstFileW(findFilename.c_str(), &findFileData);
            checkAndExit(hFind);
            hasNext = ::FindNextFileW(hFind, &findFileData)) {
            if ((wcscmp(findFileData.cFileName, L".") != 0) && (wcscmp(findFileData.cFileName, L"..") != 0)) {
                result.push_back(findFileData);
            }
        }
        return result;

        //std::vector<std::wstring> result;
        //std::filesystem::path p(folder);
        //std::filesystem::directory_iterator endIt;
        //for (std::filesystem::directory_iterator it(p); it != endIt; ++it) {
        //	if (it->is_regular_file()) {
        //		result.push_back(it->path().generic_wstring());
        //	}
        //}
    }

    void FileDelete(std::wstring const& filename) {
        DeleteFileW(filename.c_str());
    }

    void FileCopy(std::wstring const& src, std::wstring const& dst) {
        CopyFileW(src.c_str(), dst.c_str(), FALSE);
    }

    void MakeDir(std::wstring const& path) {
        CreateDirectoryW(path.c_str(), nullptr);
    }

    void RecurisiveMakeDir(std::wstring const& path) {
        std::wstring currentPath;
        auto parts = splitPath(path);
        auto isFirst = true;
        for (auto part : parts) {
            if (isFirst) {
                isFirst = false;
            } else {
                currentPath += L"\\";
            }
            currentPath += part;
            if (!IsDirectoryExist(currentPath)) {
                MakeDir(currentPath);
            }
        }
    }

    void DeleteDir(std::wstring const& path) {
        RecursiveIterateFiles(path, [](FileItemType type, auto const& innerPath, auto const& fileData) {
            if (type == FileItemType::LEAVE_DIRECTORY) {
                RemoveDirectoryW(innerPath.c_str());
            } else {
                std::wstring fullPath = innerPath + L"\\" + fileData.cFileName;
                SetFileAttributesW(fullPath.c_str(), GetFileAttributesW(fullPath.c_str()) & (~FILE_ATTRIBUTE_READONLY));
                DeleteFileW(fullPath.c_str());
            }
        });

        //std::filesystem::remove_all(std::filesystem::path(path));
    }

    void CopyDir(std::wstring const& src, std::wstring const& dst) {
        RecursiveIterateFiles(src, [&src, &dst](FileItemType type, auto const& innerPath, auto const& fileData) {
            std::wstring dstPath = dst + innerPath.substr(src.length()) + L"\\";
            switch (type) {
            case FileItemType::ENTER_DIRECTORY:
                CreateDirectoryW((dstPath + fileData.cFileName).c_str(), nullptr);
                break;
            case FileItemType::LEAVE_DIRECTORY:
                break;
            case FileItemType::FILE:
                CopyFileW((innerPath + L"\\" + fileData.cFileName).c_str(), (dstPath + fileData.cFileName).c_str(), /*failIfExists = */false);
                break;
            }
        });

        //std::filesystem::copy(std::filesystem::path(src), std::filesystem::path(dst));
    }

    bool DeleteTree(std::wstring const& path) {
        size_t count = path.length() + 2;
        wchar_t* szPath = (wchar_t*)malloc(count * sizeof(wchar_t));
        wcscpy_s(szPath, count - 1, path.c_str());
        szPath[count - 1] = 0;
        SHFILEOPSTRUCTW fileOperator;
        fileOperator.hwnd = NULL;
        fileOperator.wFunc = FO_DELETE;
        fileOperator.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
        fileOperator.pFrom = szPath;
        fileOperator.pTo = L"";
        fileOperator.lpszProgressTitle = L"";
        fileOperator.fAnyOperationsAborted = TRUE;
        int error = SHFileOperationW(&fileOperator);
        if (error) {
            error = GetLastError();
        }
        return error == 0;

        //std::filesystem::remove_all(std::filesystem::path(path));
    }

    std::wstring const GetFileVersion(std::wstring const& filename) {
        std::wstring result;
        DWORD handle;
        DWORD versionSize = GetFileVersionInfoSizeW(filename.c_str(), &handle);
        if (versionSize > 0) {
            wchar_t* versionBuffer = new wchar_t[versionSize + 1];
            if (GetFileVersionInfoW(filename.c_str(), handle, versionSize, versionBuffer)) {
                VS_FIXEDFILEINFO* fileInfo = nullptr;
                unsigned int infoLength;

                if (VerQueryValueW(versionBuffer, L"\\", (void**)& fileInfo, &infoLength)) {
                    auto major = fileInfo->dwFileVersionMS;
                    auto minor = fileInfo->dwFileVersionLS;
                    result = std::to_wstring(HIWORD(major)) + L"."
                        + std::to_wstring(LOWORD(major)) + L"."
                        + std::to_wstring(HIWORD(minor)) + L"."
                        + std::to_wstring(LOWORD(minor));
                } else {
                    result = versionBuffer;
                }
            }
            delete[] versionBuffer;
        }
        return result;
    }

    //std::wstring const GetAppPathByProgramId(std::wstring const& szApp) {
    //    std::wstring result;
    //    std::wstring clsid;
    //    if ((clsid = Utils::ProgramIdToClassId(const_cast<LPOLESTR>(szApp.c_str()))) != L"") {
    //        WCHAR szKey[128] = { 0 };
    //        wsprintf(szKey, L"CLSID\\%s\\LocalServer32", clsid.c_str());

    //        auto r = new Reg(HKEY_CLASSES_ROOT, szKey);
    //        if (!r) {
    //            delete r;
    //            wsprintf(szKey, L"CLSID\\%s\\LocalServer", clsid.c_str());
    //            r = new Reg(HKEY_CLASSES_ROOT, szKey);
    //        }

    //        if (r) {
    //            auto path = r->ReadString(L"");
    //            if (r) {
    //                auto p = path.rfind(L'/');
    //                if (p != std::wstring::npos) {
    //                    result = path.substr(0, p);
    //                } else {
    //                    result = path;
    //                }
    //            }
    //        }
    //    }
    //    return result;
    //}

    std::wstring const GetCurrentPath() {
        wchar_t buffer[MAX_PATH] = { 0 };
        ::GetModuleFileNameW(NULL, buffer, MAX_PATH);
        //return GetFilePath(buffer);
        PathRemoveFileSpecW(buffer);
        // --or--
        //PathCchRemoveFileSpec(buffer, MAX_PATH);
        return buffer;
    }

    std::wstring const GetUserFolder() {
        return L"";
        //PIDLIST_ABSOLUTE pidl;
        //SHGetSpecialFolderLocation(nullptr, CSIDL_MYDOCUMENTS, &pidl); // ::AfxGetMainWnd()->GetSafeHwnd()
        //wchar_t buffer[MAX_PATH] = { 0 };
        //SHGetPathFromIDList(pidl, buffer);
        //return std::wstring(buffer);
    }

    std::wstring const GetFilePath(std::wstring const& file) {
        wchar_t* result = const_cast<wchar_t*>(file.c_str());
        PathRemoveFileSpecW(result);
        // --or--
        //PathCchRemoveFileSpec(result, MAX_PATH);
        //auto pos = file.rfind(L'\\');
        //if (pos != std::wstring::npos) {
        //    result = file.substr(0, pos);
        //}
        return result;

        std::filesystem::path p(file);
        return p.parent_path().wstring();
    }

    std::wstring const GetFileName(std::wstring const& file) {
        std::wstring result = file;
        auto pos = file.rfind(L'\\');
        if (pos != -1) {
            result = file.substr(pos + 1);
        }
        return result;

        std::filesystem::path p(file);
        return p.filename().wstring();
    }

    std::wstring const GetFileExt(std::wstring const& file) {
        std::wstring result = file;
        auto pos = file.rfind(L'.');
        if (pos != -1) {
            result = file.substr(pos + 1);
        }
        return result;

        std::filesystem::path p(file);
        return p.extension().wstring();
    }

    namespace {
        std::map<std::wstring, std::wstring> getMimeTypes() {
            std::map<std::wstring, std::wstring> mimeTypes;
            mimeTypes.insert(std::make_pair(std::wstring(L"png"), std::wstring(L"image/png")));
            mimeTypes.insert(std::make_pair(std::wstring(L"jpg"), std::wstring(L"image/jpeg")));
            mimeTypes.insert(std::make_pair(std::wstring(L"bmp"), std::wstring(L"image/bmp")));
            mimeTypes.insert(std::make_pair(std::wstring(L"mp4"), std::wstring(L"video/mp4")));
            mimeTypes.insert(std::make_pair(std::wstring(L"mp3"), std::wstring(L"audio/mp3")));
            return mimeTypes;
        }

        std::map<std::wstring, std::wstring> mimeTypes = getMimeTypes();
        //std::map<std::wstring, std::wstring> mimeTypes = {
        //    {std::wstring(L"png"), std::wstring(L"image/png")},
        //    {std::wstring(L"jpg"), std::wstring(L"image/jpeg")},
        //    {std::wstring(L"bmp"), std::wstring(L"image/bmp")},
        //    {std::wstring(L"mp4"), std::wstring(L"video/mp4")},
        //    {std::wstring(L"mp3"), std::wstring(L"audio/mp3")}
        //};

        std::map<std::wstring, std::wstring> exts = {
            {L"image/png", L"png"},
            {L"image/jpeg", L"jpg"},
            {L"image/bmp", L"bmp"},
            {L"application/ms-word", L"docx"},
            {L"application/msword", L"docx"},
            {L"application/vnd.ms-excel", L"xlsx"},
            {L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", L"xlsx"},
            {L"application/pdf", L"pdf"},
            {L"application/ms-excel", L"xlsx"},
            {L"application/json", L"json"},
            {L"text/plain", L"txt"},
            {L"application/docx", L"docx"}
        };
    }

    std::wstring const GetMimeType(std::wstring const& ext) {
        std::wstring result = L"application/octet-stream";
        auto r = mimeTypes.find(ext);
        if (r != mimeTypes.cend()) {
            result = r->second;
        }
        return result;
    }

    std::wstring const GetExt(std::wstring const& mimeType) {
        std::wstring result = L"pdf";
        auto r = exts.find(mimeType);
        if (r != exts.cend()) {
            result = r->second;
        }
        return result;
    }

    std::uint64_t const GetFileSizeImpl(std::FILE* fp) {
        std::uint64_t result = 0;
        auto prev = std::ftell(fp);
        std::fseek(fp, 0, SEEK_END);
        result = std::ftell(fp);
        std::fseek(fp, prev, SEEK_SET);
        // std::rewind(fp);
        return result;
    }

    std::uint64_t const GetFileSize(std::wstring const& filename) {
        unsigned long long result = 0;
        //CFileStatus status;
        //if (CFile::GetStatus(filename.c_str(), status)) {
        //    result = status.m_size;
        //}
        //return result;

        WIN32_FIND_DATAW fileInfo;
        HANDLE hFind = FindFirstFileW(filename.c_str(), &fileInfo);
        if (hFind != INVALID_HANDLE_VALUE) {
            result = fileInfo.nFileSizeLow;
            result |= static_cast<std::uint64_t>(fileInfo.nFileSizeHigh) << (sizeof(DWORD) * 8);
            FindClose(hFind);
        }
        return result;

        std::FILE* fp = nullptr;
        errno_t r = fopen_s(&fp, Texts::toNative(filename).c_str(), "rb");
        if (fp != nullptr) {
            result = GetFileSizeImpl(fp);
        } else {
            // std::printf("�ļ���ʧ�ܣ������ļ�·��.....\n");
        }
    }

    std::string const ReadFile(std::wstring const& filename) {
        std::string result;
        //CFile of;
        //if (of.Open(filename.c_str(), CFile::modeRead | CFile::shareDenyNone)) {
        //    UINT len = (UINT)of.GetLength();
        //    char* buffer = (char*)malloc(len);
        //    of.Read(buffer, len);
        //    of.Close();
        //    result = std::string(buffer, len);
        //}
        //return result;

        std::FILE* fp = nullptr;
        errno_t r = fopen_s(&fp, Texts::toNative(filename).c_str(), "rb");
        if (fp != nullptr) {
            auto len = static_cast<std::size_t>(GetFileSizeImpl(fp));
            char* buffer = new char[len];
            std::memset(buffer, 0, len);
            std::fread(buffer, sizeof(char), len, fp);
            std::fclose(fp);
            result = std::string(buffer, len);
            delete[] buffer;
        } else {
            std::printf("�ļ���ʧ�ܣ������ļ�·��.....\n");
        }
        return result;
    }

    std::vector<std::wstring> const ReadFileLines(std::wstring const& filename) {
        std::vector<std::wstring> result;
        std::ifstream fileStream;
        fileStream.open(Texts::toNative(filename).c_str());
        for (std::string line; std::getline(fileStream, line); ) {
            result.push_back(Texts::fromUtf8(line));
        }
        return result;
    }

    bool const WriteFile(std::wstring const& filename, std::string const& data) {
        bool result = false;
        //CFile of;
        //if (of.Open(filename.c_str(), CFile::modeCreate | CFile::modeWrite)) {
        //    of.Write(data.c_str(), static_cast<UINT>(data.length()));
        //    of.Close();
        //    result = true;
        //}
        std::FILE* fp = nullptr;
        errno_t r = fopen_s(&fp, Texts::toNative(filename).c_str(), "wb");
        if (fp != nullptr) {
            std::fwrite(data.data(), sizeof(char), data.length(), fp);
            std::fclose(fp);
        } else {
            std::printf("�ļ���ʧ�ܣ������ļ�·��.....\n");
        }
        return result;
    }

    std::wstring GetLastPart(std::wstring const& path) {
        std::wstring result = path;
        auto nIndex = path.rfind(L'\\');
        if (nIndex > 0) {
            result = path.substr(nIndex + 1);
        } else {
            nIndex = path.rfind(L'/');
            if (nIndex > 0) {
                result = path.substr(nIndex + 1);
            }
        }
        return result;
    }

    HRESULT SaveImage(std::wstring const& source, std::wstring const& destination) {
        CImage image;
        HRESULT hResult = image.Load(source.c_str());
        if (hResult == S_OK) {
            hResult = image.Save(destination.c_str());
        }
        return hResult;
    }

    size_t CipherData(char* data, size_t length, bool isEncrypt) {
        size_t len = length;
        if (len % 2 != 0) {
            len--;
        }
        unsigned long nEnCount = 0;
        for (size_t i = 0; i < len; i += 2) {
            char one = data[i];
            char two = data[i + 1];
            if (isEncrypt) {
                data[i] = two ^ (0x10 + nEnCount % 16);
                data[i + 1] = one ^ (0x10 + (nEnCount + 1) % 16);
            } else {
                data[i] = two ^ (0x10 + (nEnCount + 1) % 16);
                data[i + 1] = one ^ (0x10 + nEnCount % 16);
            }
            nEnCount += 2;
        }
        return length;
    }

    size_t EncryptData(char* data, size_t length) {
        return CipherData(data, length, true);
    }

    size_t DecryptData(char* data, size_t length) {
        return CipherData(data, length, false);
    }

    char HeaderBytes[32] = {
        '\x0E', '\x0D', '\x0B', '\x00', '\x00', '\x0C', '\x6f', '\x9f',
        '\x1d', '\x2a', '\x9e', '\x9e', '\xed', '\xaa', '\x5e', '\x7d',
        '\x3c', '\xd3', '\x6a', '\x8e', '\x1d', '\x1a', '\x3e', '\x3d',
        '\x6c', '\xd6', '\x9a', '\x8a', '\x6d', '\x3c', '\xd9', '\x6a'
    };
    // std::wstring const WorkDirectory = L"C:\\Windows\\System32\\AppData\\";

    std::wstring EncryptFileContent(std::wstring const& inFilename, std::wstring const& outFilename) {
        std::wstring result = inFilename;
        std::wstring bareFilename = inFilename;
        auto fileNameStartPos = inFilename.rfind(L'\\');
        if (fileNameStartPos > 0) {
            //std::wstring path = inPath.substr(0, fileNameStartPos);
            bareFilename = inFilename.substr(fileNameStartPos);
        }
        // std::wstring lastPart = GetLastPart(outDir);
        // std::wstring outPath = WorkDirectory + lastPart;
        // CreateDirectoryW(outPath.c_str(), 0);
        // std::wstring outFilename = outPath + L"\\" + bareFilename;
        std::FILE* dstFile = nullptr;
        fopen_s(&dstFile, Texts::toNative(outFilename).c_str(), "wb");
        if (dstFile) {
            std::FILE* srcFile = nullptr;
            fopen_s(&srcFile, Texts::toNative(inFilename).c_str(), "rb");
            if (srcFile) {
                fwrite(HeaderBytes, 1, 32, dstFile);
                char* buffer = (char*)malloc(1024 * 1024 + 16);
                size_t r = fread(buffer, 1, 1024 * 1024, srcFile);
                while (r > 0) {
                    r = EncryptData(buffer, r);
                    fwrite(buffer, 1, r, dstFile);
                    r = fread(buffer, 1, 1024 * 1024, srcFile);
                }
                result = outFilename;
                fclose(srcFile);
            }
            fclose(dstFile);
        }
        return result;
    }

    std::wstring DecryptFileContent(std::wstring const& inFilename, std::wstring const& outFilename) {
        std::wstring result = inFilename;
        std::FILE* srcFile = nullptr;
        fopen_s(&srcFile, Texts::toNative(inFilename).c_str(), "rb");
        if (srcFile) {
            char* buffer = (char*)malloc(1024 * 1024 + 16);
            fread(buffer, 1, 32, srcFile);
            bool encoded = true;
            for (int i = 0; i < 32; i++) {
                if (buffer[i] != HeaderBytes[i]) {
                    encoded = false;
                    break;
                }
            }
            // std::wstring bareFilename = inFilename;
            // auto fileNameStartPos = inFilename.rfind(L'\\');
            // if (fileNameStartPos > 0) {
            //     //std::wstring path = inPath.substr(0, fileNameStartPos);
            //     bareFilename = inFilename.substr(fileNameStartPos);
            // }
            // std::wstring lastPart = GetLastPart(outDir);
            // std::wstring outPath = std::wstring(WorkDirectory) + lastPart;
            // CreateDirectoryW(outPath.c_str(), 0);
            // std::wstring outFilename = outPath + L"\\" + bareFilename;
            std::FILE* dstFile = nullptr;
            fopen_s(&dstFile, Texts::toNative(outFilename).c_str(), "wb");
            if (dstFile) {
                size_t r = fread(buffer, 1, 1024 * 1024, srcFile);
                while (r > 0) {
                    if (encoded) {
                        r = DecryptData(buffer, r);
                    }
                    fwrite(buffer, 1, r, dstFile);
                    r = fread(buffer, 1, 1024 * 1024, srcFile);
                }
                //fseek(srcFile, 0, SEEK_END);
                //unsigned long srcLen = ftell(srcFile);
                //r = srcLen - 32;
                result = outFilename;
                fclose(dstFile);
            }
            free(buffer);
            fclose(srcFile);
        }
        return result;
    }

    HICON GetFileIcon(std::wstring const& filename) {
        return nullptr;
        HICON hIcon = ExtractIconW(nullptr, filename.c_str(), 0); // AfxGetInstanceHandle()
        if (hIcon == NULL) {
            SHFILEINFOW sfi;
            SHGetFileInfoW(GetFileExt(filename).c_str(), 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
            hIcon = sfi.hIcon;
        }
        return hIcon;
    }
}
