#include <string>
#include <map>
#include <vector>
#include <functional>
#include <cstdint>
#include <shtypes.h> // for WIN32_FIND_DATAW

namespace utils::Folders {
    enum class FileItemType {
        ENTER_DIRECTORY,
        LEAVE_DIRECTORY,
        FILE
    };

    void IterateFiles(std::wstring const &folder,
                      std::function<void(FileItemType type, WIN32_FIND_DATAW const &findFileData)> const &fileproc);

    void RecursiveIterateFiles(std::wstring const &folder,
            std::function<void(FileItemType type, std::wstring const& innerPath, WIN32_FIND_DATAW const &findFileData)> const &fileproc);

    std::vector<std::wstring> splitPath(std::wstring const& path);

    bool IsFileExist(std::wstring const &path);

    bool IsDirectoryExist(std::wstring const &path);

    bool IsUsbDisk(std::wstring const &path);

    std::wstring const GetUsbDisk();

    std::map<std::wstring, unsigned int> const GetDisks();

    std::pair<std::wstring, std::wstring> const GetPhyDriveSerial();

    std::wstring const GetProfilePathName();

    std::vector<std::wstring> ListFiles(std::wstring const &folder);

    std::vector<std::wstring> ListFileNames(std::wstring const &folder);

    std::vector<WIN32_FIND_DATAW> ListOneLevelFiles(std::wstring const &folder);

    void FileDelete(std::wstring const& filename);

    void FileCopy(std::wstring const& src, std::wstring const& dst);

    void MakeDir(std::wstring const& path);

    void RecurisiveMakeDir(std::wstring const& path);

    void DeleteDir(std::wstring const &str);

    void CopyDir(std::wstring const &src, std::wstring const &dst);

    bool DeleteTree(std::wstring const &path);

    std::wstring const GetFileVersion(std::wstring const &filename);

    //std::wstring const GetAppPathByProgramId(std::wstring const& szApp);
    std::wstring const GetCurrentPath();

    std::wstring const GetUserFolder();

    std::wstring const GetFilePath(std::wstring const &file);

    std::wstring const GetFileName(std::wstring const &file);

    std::wstring const GetFileExt(std::wstring const &file);

    std::wstring const GetMimeType(std::wstring const &ext);

    std::wstring const GetExt(std::wstring const& mimeType);

    std::uint64_t const GetFileSize(std::wstring const &filename);

    std::string const ReadFile(std::wstring const &filename);

    std::vector<std::wstring> const ReadFileLines(std::wstring const &filename);

    HRESULT SaveImage(std::wstring const& source, std::wstring const& destinaion);

    size_t EncryptData(char *data, size_t length);

    size_t DecryptData(char *data, size_t length);

    std::wstring EncryptFileContent(std::wstring const &inFilename, std::wstring const &outDir);

    std::wstring DecryptFileContent(std::wstring const &inFilename, std::wstring const &outDir);

    bool const WriteFile(std::wstring const &filename, std::string const &data);
}
