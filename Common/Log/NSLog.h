// +----------------------------------------------------------------------
// | C++ Basis Components
// +----------------------------------------------------------------------
// | GSLog		��־
// +----------------------------------------------------------------------
// | Author:	
// +----------------------------------------------------------------------
// | Modify:	
// +----------------------------------------------------------------------

#pragma  once

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>


// ��־�ȼ�
typedef enum _GSLOG_LEVEL_
{
	LEVEL_ERROR		= 0,
	LEVEL_WARN		= 1,
	LEVEL_DEBUG		= 2,
	LEVEL_INFO		= 3,
	LEVEL_ALL		= 4,
	LEVEL_CLOSE		= 5
}GSLOG_LEVEL;

class GSLog
{

public:

	// ���캯��
	GSLog();

	// ��������
	virtual ~GSLog();

protected:

	// ��ʼ��
	void _Init();

	// ����·��
	void _ParsePath(char *pPath, char* pDir, char *pName);

	// ��ȡ��ǰ����
	void _GetCurTime(char *pDate = NULL, char *pTime = NULL);

	// �����־�ļ�ʱ��
	bool _CheckFileTime();

	// ���´����ļ�
	void _Rotate();

	// �����ļ���
	int CreateDir(const char* pDir);
	// �����ļ�
	void Config(const char* pConfigFile);
public:

	// ����־�ļ�
	bool Open(char *pFilePath, GSLOG_LEVEL nLevel = LEVEL_DEBUG);

	// д��־
	int Logv(GSLOG_LEVEL nLevel, const char *pFmt, va_list vaList);

	// д������־
	int LogError(const char *pFmt, ...);

	// д������־
	int LogWarn(const char *pFmt, ...);

	// д������־
	int LogDebug(const char *pFmt, ...);

	// д��Ϣ��־
	int LogInfo(const char *pFmt, ...);

	// �ر�
	void Close();


private:

	GSLOG_LEVEL			m_nLevel;
	FILE*				m_pFile;
	CRITICAL_SECTION	m_Mutex;
	char				m_szLogPath[1024];
	char				m_szName[1024];
	char				m_szDir[1024];
	char				m_szCurDate[128];
};

void _log_init(const char *pFilePath, GSLOG_LEVEL nLevel = LEVEL_DEBUG);
int _log_write(GSLOG_LEVEL nLevel, const char *pFmt, ...);

#define GSLOG_INIT(pFilePath, nLevel) \
	_log_init(pFilePath, nLevel)


#define GSLOG_INFO(fmt, ...) _log_write(LEVEL_INFO, "%s(%d): " fmt, __FILE__, __LINE__, __VA_ARGS__)

#define GSLOG_ERROR(fmt, ...) _log_write(LEVEL_ERROR, "%s(%d): " fmt, __FILE__, __LINE__, __VA_ARGS__)

#define GSLOG_WARN(fmt, ...) _log_write(LEVEL_WARN, "%s(%d): " fmt, __FILE__, __LINE__, __VA_ARGS__)

#define GSLOG_DEBUG(fmt, ...) _log_write(LEVEL_DEBUG, "%s(%d): " fmt, __FILE__, __LINE__, __VA_ARGS__)





