#if !defined(_AFX_COMMON_NFILE_H)
#define _AFX_COMMON_NFILE_H

#include <vector>
#include <string>

#ifdef NString
#undef NString
#endif

#ifdef UNICODE
#define NString std::wstring
#else
#define NString std::string
#endif

//using namespace std;

namespace NFile {
    // �ڹ�����Դ�ļ��г�ȡ�ļ�
    // nID ��ʾ��Դ�ļ�ID  lpType�ļ�����  lpPaht ��ʾ�ļ��ı���·��
    BOOL ExtrFileFromRes(UINT nID, LPCTSTR lpType, LPCTSTR lpPath);

    // ɾ��Ŀ¼�� lpDirΪָ��Ŀ¼��ָ��
    BOOL DeleteDirTree(LPCTSTR lpDir, LPCTSTR exclusionFile = NULL);

    BOOL DeleteDirFilesByPrefix(LPCTSTR lpDir, LPCTSTR lpPrefix);

    // ���Ŀ¼�µ������ļ�·��
    BOOL GetFilePathInDir(LPCTSTR lpDir, std::vector <NString>& vFilePaths);

    // ��·����ȡĿ¼���ļ���
    BOOL SplitPathBySlash(LPCTSTR lpPath, LPTSTR lpDirTree, LPTSTR lpFileName = NULL);

    // ����Ŀ¼��
    BOOL CreateDirTree(LPCTSTR lpDir);

    // ����Ŀ¼��
    BOOL CreateDirTree(LPCSTR lpDir);

    // ��ȡ����Ŀ¼·��
    std::string GetDir(std::string strPath);

    // ��ȡ�ļ���С
    double GetFileSize(std::string strFilePath, unsigned long* pHighSize = NULL);

    // ����ļ���ģ���е�ȫ·��
    BOOL GetFilePathInModule(TCHAR* pFilePath, TCHAR* pFileName = NULL, HMODULE hMod = NULL);

    // Ŀ¼���ļ�������
    int GetFileCountInDir(LPCTSTR lpDir);

    // ��ȡĿ¼���ļ���С�ܺ�
    __int64 GetFolderSize(const std::wstring& strDir);

    // �ж�ָ��Ŀ¼�Ƿ����
    BOOL IsDirectory(LPCTSTR lpDir);

    // �ж�ָ��Ŀ¼�Ƿ����
    BOOL IsDirectory(LPCSTR pDir);

    // �ж��ļ��Ƿ����
    BOOL IsFileExist(LPCTSTR lpPath);

    // ��ȡ�ļ�����
    BYTE* GetFileData(LPCTSTR pFileName, DWORD* pdwSize);

    // ���ļ���
    BOOL OpenFolder(LPCTSTR lpPath);

    // ��ȡȫ·��
    BOOL GetFullPath(LPCTSTR lpShortPath, LPTSTR lpFullPath);

    // ����Ŀ¼�� lpDirΪָ��Ŀ¼��ָ��
    BOOL CopyDirTree(LPCTSTR lpSrc, LPCTSTR lpDest);

    // ���IE����
    BOOL DeleteInternetCache();

    // ɾ��IE������ָ��ģ���ļ������ļ�
    BOOL DeleteInternetTempFile(LPCTSTR lpName = NULL);

    // ���IE Cookie
    BOOL DeleteInternetCookie();

    // ����IE�����С
    BOOL SetInternetCacheLimit(int size);

    // ���flash����
    BOOL DeleteFlashCache();

    int ReplaceStr(std::string& strBuf, std::string strSrc, std::string strDes);

    NString GetSpecificDocumentDir(LPCTSTR dirName);

    NString Get4399Dir();

    NString Get4399TempPath();

    NString Get4399DownPath();

    NString GetBinDir();

    NString GetBinFileName();

    NString GetUserProfileDir();

    std::vector <NString> GetVolumes();
}

#endif
