// +----------------------------------------------------------------------
// | C++ Basis Components
// +----------------------------------------------------------------------
// | GSLog		��־
// +----------------------------------------------------------------------
// | Author:	
// +----------------------------------------------------------------------
// | Modify:	
// +----------------------------------------------------------------------


#include "stdafx.h"
#include "NSLog.h"

#include <direct.h>
#include <io.h>


// ��־����
static GSLog	s_Logger;

/*
 *	Function:		GetLevelName
 *	Explanation:	��ȡ��־�ȼ�����
 *	Input:			nLevel		��־�ȼ�
 *	Return:			�ȼ�����
 */
inline static const char* _GetLevelName(GSLOG_LEVEL nLevel)
{
	switch (nLevel)
	{
	case LEVEL_ERROR:
		return "[ERROR]";
		break;
	case LEVEL_WARN:
		return "[WARN]";
		break;
	case LEVEL_DEBUG:
		return "[DEBUG]";
		break;
	case LEVEL_INFO:
		return "[INFO]";
		break;
	}

	return "";
}

/*
 *	Function:		GSLog
 *	Explanation:	���캯��
 */
GSLog::GSLog()
{
	_Init();
}

/*
 *	Function:		~GSLog
 *	Explanation:	��������
 */
GSLog::~GSLog()
{
	Close();
	DeleteCriticalSection(&m_Mutex);
}

/*
 *	Function:		_Init
 *	Explanation:	��ʼ��
 */
void GSLog::_Init()
{
	m_pFile = NULL;
	m_nLevel = LEVEL_DEBUG;
	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	memset(m_szDir, 0, sizeof(m_szDir));
	memset(m_szName, 0, sizeof(m_szName));
	memset(m_szCurDate, 0, sizeof(m_szCurDate));
	InitializeCriticalSection(&m_Mutex);


	char szConfig[MAX_PATH];
	GetModuleFileNameA(NULL, szConfig, 1024);
	char* p = strrchr(szConfig, '\\');
	*p = 0;
	strcat_s(szConfig, MAX_PATH, "\\config\\config.ini");
	if (_access(szConfig, 0) != 0)
	{
		return ;
	}
	else
	{
		Config(szConfig);
	}


}

void GSLog::Config(const char* pConfigFile)
{
	/*
	[set]
	#ERROR 0
	#WARN  1
	#DEBUG 2
	#INFO 3
	log_level=0
	dir_path=d:\x\y
	file_name=xxx.log
	*/

	m_nLevel = (GSLOG_LEVEL)GetPrivateProfileIntA("set", "log_level", LEVEL_ERROR, pConfigFile);

	char szSoftPath[MAX_PATH];
	char szDirPath[MAX_PATH];
	char szFileName[MAX_PATH];
	char szLogPath[MAX_PATH];

	GetModuleFileNameA(NULL, szSoftPath, 1024);

	_ParsePath(szSoftPath, szDirPath, szFileName);
	char* p = strrchr(szFileName, '.');
	*p = 0;
	
	GetPrivateProfileStringA("set", "dir_path", szDirPath, szDirPath, 1024, pConfigFile);
	GetPrivateProfileStringA("set", "file_name", szFileName, szFileName, 128, pConfigFile);
	
	sprintf_s(szLogPath, MAX_PATH, "%s\\Log\\%s.log", szDirPath, szFileName);


	s_Logger.Open((char *)szLogPath, m_nLevel);
	
}

/*
 *	Function:		_ParsePath
 *	Explanation:	����·��
 *	Input:			pPath		�ļ�·��
					pDir		Ŀ¼
					pName		����
 *	Return:			true �ɹ� false ʧ��
 */
void GSLog::_ParsePath(char *pPath, char* pDir, char *pName)
{
	char				szPath[1024]		= { 0 };
	char*				pFind				= NULL;

	strcpy_s(szPath, MAX_PATH, pPath);
	pFind = strrchr(szPath, '\\');

	if (NULL == pFind)
	{
		pFind = strrchr(szPath, '/');
	}

	if (NULL == pFind)
	{
		strcpy_s(pName, MAX_PATH, szPath);
	}
	else
	{
		*pFind = 0;
		pFind++;
		strcpy_s(pDir, MAX_PATH, szPath);
		strcpy_s(pName, MAX_PATH, pFind);
	}
}

/*
 *	Function:		_GetCurDate
 *	Explanation:	��ȡ��ǰ����
 *  Output:			pDate		����
					pTime		ʱ��
 */
void GSLog::_GetCurTime(char *pDate /* = NULL */, char *pTime /* = NULL */)
{
	SYSTEMTIME			SysTime					= { 0 };

	GetLocalTime(&SysTime);

	if (NULL != pDate)
	{
		sprintf_s(pDate,1024, "%04d-%02d-%02d", SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	}

	if (NULL != pTime)
	{
		sprintf_s(pTime,1024, "%02d:%02d:%02d.%03d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	}
}

/*
 *	Function:		_CheckFileTime
 *	Explanation:	�����־�ļ�ʱ��
 */
bool GSLog::_CheckFileTime()
{
	char				szCurDate[128]		= { 0 };

	_GetCurTime(szCurDate);

	if (0 != _stricmp(szCurDate, m_szCurDate))
	{
		strcpy_s(m_szCurDate, szCurDate);
		return false;
	}
	
	return true;
}

/*
 *	Function:		_Rotate
 *	Explanation:	���´����ļ�
 */
void GSLog::_Rotate()
{
	char				szReName[1024]				= { 0 };

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	if (0 == m_szDir[0])
	{
		_snprintf_s(m_szLogPath, sizeof(m_szLogPath) - 1, "%s_%s", m_szCurDate, m_szName);
	}
	else
	{
		_snprintf_s(m_szLogPath, sizeof(m_szLogPath) - 1, "%s\\%s_%s", m_szDir, m_szCurDate, m_szName);
	}

	fopen_s(&m_pFile, m_szLogPath, "ab+");
	//m_pFile = fopen_s(m_szLogPath, "ab+");
}

/*
 *	Function:		Open
 *	Explanation:	����־�ļ�
 *	Input:			pFilePath	�ļ�·��
					nLevel		��־�ȼ�
 *	Return:			true �ɹ� false ʧ��
 */
bool GSLog::Open(char *pFilePath, GSLOG_LEVEL nLevel /* = LEVEL_DEBUG */)
{
	if (NULL == pFilePath || 0 == pFilePath[0])
	{
		return false;
	}

	_GetCurTime(m_szCurDate);
	m_nLevel = nLevel;
	_ParsePath(pFilePath, m_szDir, m_szName);

	//�����ռ������ļ��� //edit by zzc 2017/11/22
	CreateDir(m_szDir);


	if (0 == m_szDir[0])
	{
		_snprintf_s(m_szLogPath, sizeof(m_szLogPath) - 1, "%s_%s", m_szCurDate, m_szName);
	}
	else
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		_snprintf_s(m_szLogPath, sizeof(m_szLogPath) - 1, "%s\\%s_%s", m_szDir, m_szCurDate, m_szName);
	}
	fopen_s(&m_pFile, m_szLogPath, "ab+");
	//m_pFile = fopen(m_szLogPath, "ab+");

	if (NULL == m_pFile)
	{
		return false;
	}

	return true;
}

/*
 *	Function:		Logv
 *	Explanation:	д��־
 *	Input:			nLevel		��־�ȼ�
					pFmt		��ʽ���ַ���
					vaList		�����б�
 *	Return:			ʵ��д�볤��
 */
int GSLog::Logv(GSLOG_LEVEL nLevel, const char *pFmt, va_list vaList)
{
	char				szDate[64]				= { 0 };
	char				szTime[64]				= { 0 };
	char				szDateTime[128]			= { 0 };
	char				szContent[40960]		= { 0 };
	int					nWrite					= 0;
	std::string			strLog					= "";
	std::string			strLevel;

	if (LEVEL_CLOSE == nLevel || 
		m_nLevel < nLevel)
	{
		return 0;
	}

	if (!m_pFile)
	{
		return 0;
	}

	if (false == _CheckFileTime())
	{
		_Rotate();

		if (NULL == m_pFile)
		{
			return false;
		}
	}

	if (NULL == m_pFile)
	{
		return 0;
	}

	_GetCurTime(szDate, szTime);
	sprintf_s(szDateTime,128, "%s %s  ", szDate, szTime);
	strLog.append(szDateTime);
	strLevel = _GetLevelName(nLevel);

	if (!strLevel.empty())
	{
		strLog.append(strLevel.c_str());
		strLog.append("  ");
	}

	_vsnprintf_s(szContent, 40960, pFmt, vaList);
	
	if (0 != szContent[0])
	{
		strLog.append(szContent);
	}

	strLog.append("\r\n");
	EnterCriticalSection(&m_Mutex);
	nWrite = fwrite(strLog.c_str(), 1, strLog.size(), m_pFile);
	fflush(m_pFile);
	LeaveCriticalSection(&m_Mutex);
	OutputDebugStringA(strLog.c_str());
	return nWrite;
}

/*
 *	Function:		LogError
 *	Explanation:	д������־
 *	Input:			pFmt		��ʽ���ַ���
 *	Return:			ʵ��д�볤��
 */
int GSLog::LogError(const char *pFmt, ...)
{
	va_list				vaList;
	int					nRet;

	va_start(vaList, pFmt);
	nRet = Logv(LEVEL_ERROR, pFmt, vaList);
	va_end(vaList);

	return nRet;
}

/*
*	Function:		LogWarn
*	Explanation:	д������־
*	Input:			pFmt		��ʽ���ַ���
*	Return:			ʵ��д�볤��
*/
int GSLog::LogWarn(const char *pFmt, ...)
{
	va_list				vaList;
	int					nRet;

	va_start(vaList, pFmt);
	nRet = Logv(LEVEL_WARN, pFmt, vaList);
	va_end(vaList);

	return nRet;
}

/*
*	Function:		LogDebug
*	Explanation:	д������־
*	Input:			pFmt		��ʽ���ַ���
*	Return:			ʵ��д�볤��
*/
int GSLog::LogDebug(const char *pFmt, ...)
{
	va_list				vaList;
	int					nRet;

	va_start(vaList, pFmt);
	nRet = Logv(LEVEL_DEBUG, pFmt, vaList);
	va_end(vaList);

	return nRet;
}

/*
 *	Function:		LogInfo
 *	Explanation:	д��Ϣ��־
 *	Input:			pFmt		��ʽ���ַ���
 *	Return:			ʵ��д�볤��
 */
int GSLog::LogInfo(const char *pFmt, ...)
{
	va_list				vaList;
	int					nRet;

	va_start(vaList, pFmt);
	nRet = Logv(LEVEL_INFO, pFmt, vaList);
	va_end(vaList);

	return nRet;
}

/*
 *	Function:		Close
 *	Explanation:	�ر�
 */
void GSLog::Close()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}


/*
 *	Function:		_log_init
 *	Explanation:	��ʼ����־
 *	Input:			pFilePath		��־�ļ�·��
					nLevel			��־�ȼ�
 */
void _log_init(const char *pFilePath, GSLOG_LEVEL nLevel)
{
	s_Logger.Open((char *)pFilePath, nLevel);
}

/*
 *	Function:		_log_write
 *	Explanation:	д��־
 *	Input:			nLevel			��־�ȼ�
					pFmt			��ʽ���ַ���
 *	Return:			ʵ��д����
 */
int _log_write(GSLOG_LEVEL nLevel, const char *pFmt, ...)
{
	va_list				vaList;
	int					nRet;

	va_start(vaList, pFmt);
	nRet = s_Logger.Logv(nLevel, pFmt, vaList);
	va_end(vaList);

	return nRet;
}


/*
*	Function:		CreateDir
*	Explanation:	�����ļ���
*	pDir:					�ļ�·��
*/
int GSLog::CreateDir(const char* pDir)
{
	int nLen = strlen(pDir);
	std::string strPath;

	for (int i = 0; i < nLen; i++)
	{
		if (pDir[i] == '\\' || pDir[i] == '/')
		{
			strPath = std::string(pDir, &pDir[i] - pDir);
			if (_access(strPath.c_str(), 0 != 0))
			{
				_mkdir(strPath.c_str());
			}
		}
	}
	if (pDir[nLen - 1] != '\\' && pDir[nLen - 1] != '/'){
		strPath = pDir;
		if (_access(strPath.c_str(), 0) != 0){
			_mkdir(strPath.c_str());
		}
	}
	return 0;
}

