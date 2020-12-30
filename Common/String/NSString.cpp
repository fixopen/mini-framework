// +----------------------------------------------------------------------
// |  C++ Basis Components
// +----------------------------------------------------------------------
// | std::wstring	�����ַ�����
// | std::string 	���ֽ��ַ����� 
// | NSNStr		�ַ������ò��������ռ�
// +----------------------------------------------------------------------
// +----------------------------------------------------------------------



#include "NSString.h"
#include <stdarg.h>
#include <assert.h>
#include <windows.h>


#ifdef GS_USE_CRYPT

#include "../GSNUtil.h"
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/des.h>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

#endif


#define		__1K			1024
#define		__1M			1024 * __1K
#define		__1G			1024 * __1M
#define		__1T			1024 * __1G









/*
 *	Function:		StrToWStr
 *	Explanation:	���ֽڣ�std::string ��ת�ɿ��֣�std::wstring��
 *	Input:			strBuf		���ֽ��ַ���
					nCode		����
								��CP_ACP ���ر���ANSI 
								  CP_UTF8 utf8���룩
 *	Return:			ʧ�ܷ��ؿ��ַ���
 */
std::wstring NStr::StrToWStr(std::string  strBuf, int nCode /* = CP_ACP */)
{
	std::wstring				wstrRet				= L"";
	wchar_t*				pBuf				= NULL;
	size_t					sBufSize			= 0;

	if (strBuf.empty())
	{
		return wstrRet;
	}

	sBufSize = ::MultiByteToWideChar(nCode, NULL, (char *)strBuf.c_str(), strBuf.size(), NULL, 0);

	if (0 >= sBufSize)
	{
		return wstrRet;
	}

	pBuf = new wchar_t[sBufSize + 1];

	if (sBufSize != ::MultiByteToWideChar(nCode, NULL, (char *)strBuf.c_str(), strBuf.size(), pBuf, sBufSize))
	{
		delete[]pBuf;
		
		return wstrRet;
	}

	pBuf[sBufSize] = '\0';
	wstrRet.append(pBuf, sBufSize);
	delete []pBuf;

	return wstrRet;
}

/*
 *	Function:		WStrToStr
 *	Explanation:	���֣�std::wstring��ת�ɶ��ֽڣ�std::string ��
 *	Input:			wstrBuf		�����ַ���
					nCode		����
								��CP_ACP ���ر���ANSI
								  CP_UTF8 utf8���룩
 *	Return:			ʧ�ܷ��ؿ��ַ���
 */
std::string  NStr::WStrToStr(std::wstring wstrBuf, int nCode /* = CP_ACP */)
{
	std::string 				strRet				= "";
	char*					pBuf				= NULL;
	size_t					sBufSize			= 0;

	if (wstrBuf.empty())
	{
		return strRet;
	}

	sBufSize = ::WideCharToMultiByte(nCode, NULL, (wchar_t *)wstrBuf.c_str(), wstrBuf.size(), NULL, 0, NULL, NULL);

	if (0 >= sBufSize)
	{
		return strRet;
	}

	pBuf = new char[sBufSize + 1];

	if (sBufSize != ::WideCharToMultiByte(nCode, NULL, (wchar_t *)wstrBuf.c_str(), wstrBuf.size(), pBuf, sBufSize, NULL, NULL))
	{
		delete []pBuf;

		return strRet;
	}

	pBuf[sBufSize] = '\0';
	strRet.append(pBuf, sBufSize);
	delete []pBuf;

	return strRet;
}

/*
 *	Function:		GetUtf8
 *	Explanation:	����ת�� ANSI => UTF8
 *	Input:			strBuf			ANSI�ַ���
 *	Return:			UTF8�ַ���
 */
std::string  NStr::GetUtf8(std::string  strBuf)
{
	std::wstring			wstrBuf					= L"";

	if (strBuf.empty())
	{
		return "";
	}

	wstrBuf = NStr::StrToWStr(strBuf);

	return NStr::WStrToStr(wstrBuf, CP_UTF8);
}

/*
*	Function:		GetAnsi
*	Explanation:	����ת�� UTF8 => ANSI
*	Input:			strBuf			UTF8�ַ���
*	Return:			ANSI�ַ���
*/
std::string  NStr::GetAnsi(std::string  strBuf)
{
	std::wstring			wstrBuf = L"";

	if (strBuf.empty())
	{
		return "";
	}

	wstrBuf = NStr::StrToWStr(strBuf, CP_UTF8);

	return NStr::WStrToStr(wstrBuf);
}


std::string NStr::Unicode2ANSI(LPCWSTR lpszSrc)
{
	std::string sResult;
	if (lpszSrc != NULL)
	{
		int  nANSILen = WideCharToMultiByte(CP_ACP, 0, lpszSrc, -1, NULL, 0, NULL, NULL);
		char* pANSI = new char[nANSILen + 1];
		if (pANSI != NULL)
		{
			ZeroMemory(pANSI, nANSILen + 1);
			WideCharToMultiByte(CP_ACP, 0, lpszSrc, -1, pANSI, nANSILen, NULL, NULL);
			sResult = pANSI;
			delete[] pANSI;
		}
	}
	return sResult;
}

std::string NStr::Unicode2UTF8(LPCWSTR lpszSrc)
{
	std::string sResult;
	if (lpszSrc != NULL)
	{
		int  nUTF8Len = WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, NULL, 0, NULL, NULL);
		char* pUTF8 = new char[nUTF8Len + 1];
		if (pUTF8 != NULL)
		{
			ZeroMemory(pUTF8, nUTF8Len + 1);
			WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, pUTF8, nUTF8Len, NULL, NULL);
			sResult = pUTF8;
			delete[] pUTF8;
		}
	}
	return sResult;
}

std::wstring NStr::ANSI2Unicode(LPCSTR lpszSrc)
{
	std::wstring sResult;
	if (lpszSrc != NULL)
	{
		int nUnicodeLen = MultiByteToWideChar(CP_ACP, 0, lpszSrc, -1, NULL, 0);
		LPWSTR pUnicode = new WCHAR[nUnicodeLen + 1];
		if (pUnicode != NULL)
		{
			ZeroMemory((void*)pUnicode, (nUnicodeLen + 1) * sizeof(WCHAR));
			MultiByteToWideChar(CP_ACP, 0, lpszSrc, -1, pUnicode, nUnicodeLen);
			sResult = pUnicode;
			delete[] pUnicode;
		}
	}
	return sResult;
}

_tstring NStr::ANSI2T(LPCSTR lpSrc)
{
#ifdef _UNICODE
	return ANSI2Unicode(lpSrc);
#else
	return lpSrc;
#endif
}
std::string NStr::T2ANSI(LPCTSTR lpSrc)
{
#ifdef _UNICODE
	return Unicode2ANSI(lpSrc);
#else
	return lpSrc;
#endif
}

std::string NStr::T2UTF8(LPCTSTR lpSrc)
{
#ifdef _UNICODE
	return Unicode2UTF8(lpSrc);
#else
	return lpSrc;
#endif
}

std::string NStr::utf16ToUtf8(LPCWSTR lpszSrc)
{
	std::string sResult;

	int  nUTF8Len = WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, NULL, 0, NULL, NULL);
	char* pUTF8 = new char[nUTF8Len + 1];
	ZeroMemory(pUTF8, nUTF8Len + 1);
	WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, pUTF8, nUTF8Len, NULL, NULL);
	sResult = pUTF8;
	delete[] pUTF8;

	return sResult;
}

std::wstring NStr::utf8ToUtf16(const std::string& utf8String)
{
	std::wstring sResult;
	int nUTF8Len = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, NULL);
	wchar_t* pUTF8 = new wchar_t[nUTF8Len + 1];

	ZeroMemory(pUTF8, nUTF8Len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, pUTF8, nUTF8Len);
	sResult = pUTF8;
	delete[] pUTF8;

	return sResult;
}


/*
 *	Function:		StrToInt
 *	Explanation:	�ַ���ת��int
 *	Input:			strBuf		�ַ���
 *	Return:			int��ֵ
 */
int NStr::StrToInt(std::string  strBuf)
{
	if (strBuf.empty())
	{
		return 0;
	}

	return atoi((char *)strBuf.c_str());
}

int NStr::StrToInt(std::wstring strBuf)
{
	if (strBuf.empty())
	{
		return 0;
	}

	return _wtoi((wchar_t *)strBuf.c_str());
}

/*
 *	Function:		IntToStr
 *	Explanation:	intת�ַ���
 *	Input:			nValue		��ֵ
 *	Return:			�ַ���
 */
std::string  NStr::IntToStr(int nValue)
{
	char				szBuf[1024]					= { 0 };

	_itoa_s(nValue, szBuf, 10);

	return szBuf;
}

std::wstring NStr::IntToWStr(int nValue)
{
	wchar_t				szBuf[1024]					= { 0 };

	_itow_s(nValue, szBuf, 10);

	return szBuf;
}

/*
 *	Function:		StrToFloat
 *	Explanation:	�ַ���ת��float
 *	Input:			strBuf		�ַ���
 *	Return:			float��ֵ
 */
float NStr::StrToFloat(std::string  strBuf)
{
	if (strBuf.empty())
	{
		return 0;
	}


	return (float)atof((char *)strBuf.c_str());
}

float NStr::StrToFloat(std::wstring strBuf)
{
	if (strBuf.empty())
	{
		return 0;
	}

	return (float)_wtof((wchar_t *)strBuf.c_str());
}

/*
 *	Function:		FloatToStr
 *	Explanation:	floatת���ַ���
 *	Input:			fValue		������
 *	Return:			�ַ���
 */
std::string  NStr::FloatToStr(float fValue)
{
	char				szBuf[1024]					= { 0 };

	_gcvt_s(szBuf, fValue, 8);

	return szBuf;
}

std::wstring NStr::FloatToWStr(float fValue)
{
	std::string 			strValue					= NStr::FloatToStr(fValue);

	return NStr::StrToWStr(strValue);
}

/*
 *	Function:		StrToDouble
 *	Explanation:	�ַ���ת��double
 *	Input:			strBuf		�ַ���
 *	Return:			double
 */
double NStr::StrToDouble(std::string  strBuf)
{
	return strtod((char *)strBuf.c_str(), NULL);
}

double NStr::StrToDouble(std::wstring strBuf)
{
	std::string 			strValue					= NStr::WStrToStr(strBuf);

	return NStr::StrToDouble(strValue);
}

/*
 *	Function:		doubleת�ַ���
 *	Explanation:	DoubleToStr
 *	Input:			dbValue		doubleֵ
 *	Return:			�ַ���
 */
std::string  NStr::DoubleToStr(double dbValue)
{
	char				szBuf[1024]					= { 0 };

	_gcvt_s(szBuf, dbValue, 8);

	return szBuf;
}

std::wstring NStr::DoubleToWStr(double dbValue)
{
	std::string 			strValue					= NStr::DoubleToStr(dbValue);

	return NStr::StrToWStr(strValue);
}

std::string  NStr::DoubleToStr(double dbValue, int nDigit)
{
	char				szBuf[1024]					= { 0 };
	char				szFormat[1024]				= { 0 };

	_snprintf(szFormat, 1023, "%%.%df", nDigit);
	_snprintf(szBuf, 1023, szFormat, dbValue);

	return szBuf;
}

std::wstring NStr::DoubleToWStr(double dbValue, int nDigit)
{
	std::string 			strValue					= NStr::DoubleToStr(dbValue, nDigit);

	return NStr::StrToWStr(strValue);
}

/*
 *	Function:		StringToLong
 *	Explanation:	�ַ���תlong
 *	Input:			strBuf		�ַ���
 *	Return:			long
 */
long NStr::StringToLong(std::string  strBuf)
{
	return atol((char *)strBuf.c_str());
}

long NStr::StringToLong(std::wstring strBuf)
{
	
	return _wtol((wchar_t *)strBuf.c_str());
}

/*
 *	Function:		LongToStr
 *	Explanation:	longת�ַ���
 *	Input:			lValue		long��ֵ
 *	Return:			�ַ���
 */
std::string  NStr::LongToStr(long lValue)
{
	char					szBuf[1024]				= { 0 };

	_ltoa_s(lValue, szBuf, 10);

	return szBuf;
}

std::wstring NStr::LongToWStr(long lValue)
{
	wchar_t					szBuf[1024]				= { 0 };

	_ltow_s(lValue, szBuf, 10);

	return szBuf;
}

/*
 *	Function:		StrToInt64
 *	Explanation:	�ַ���ת__int64
 *	Input:			strBuf		�ַ���
 *	Return:			__int64��ֵ
 */
__int64 NStr::StrToInt64(std::string  strBuf)
{
	return atoll((char *)strBuf.c_str());
}

__int64 NStr::StrToInt64(std::wstring strBuf)
{
	return _wtoll((wchar_t *)strBuf.c_str());
}

/*
 *	Function:		Int64ToStr
 *	Explanation:	__int64ת�ַ���
 *	Input:			nValue		��ֵ
 *	Return:			�ַ���
 */
std::string  NStr::Int64ToStr(__int64 nValue)
{
	char				szValue[1024]				= { 0 };

	_i64toa_s(nValue, szValue, 1023, 10);

	return szValue;
}

std::wstring NStr::Int64ToWStr(__int64 nValue)
{
	wchar_t				szValue[1024]				= { 0 };

	_i64tow_s(nValue, szValue, 1023, 10);

	return szValue;
}

/*
 *	Function:		SpliteStrByChar
 *	Explanation:	�ַ����ָ�(���ַ�c�ָ�)
 *	Input:			strBuf		�ַ���	
					c			�ָ���ַ�
					bSkipEmpty	�Ƿ������մ�
 *  Output:			vecList		�ָ����ַ���
 *	Return:			vecList��С
 */
int NStr::SpliteStrByChar(std::string  strBuf,  std::vector<std::string > &vecList,
								char c /* = '|' */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize			= strBuf.size();
	char*				pStart				= (char *)strBuf.c_str();
	char*				pEnd				= pStart + sBufSize;

	vecList.clear();

	if (strBuf.empty())
	{
		return 0;
	}

	for (;;Sleep(1))
	{
		std::string 	strTemp		= "";
		char*		pFind		= strchr(pStart, c);

		if (NULL == pFind)
		{
			strTemp = pStart;

			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
			}
			
			break;
		}

		strTemp.append(pStart, pFind);

		if (!bSkipEmpty || !strTemp.empty())
		{
			vecList.push_back(strTemp);
		}

		pStart = pFind + 1;

		if (pStart >= pEnd)
		{
			break;
		}
	}

	return vecList.size();
}

int NStr::SpliteStrByChar(std::wstring strBuf,  std::vector<std::wstring> &vecList,
								char c /* = '|' */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize			= strBuf.size();
	wchar_t*			pStart				= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd				= pStart + sBufSize;

	vecList.clear();

	if (strBuf.empty())
	{
		return 0;
	}

	for (;; Sleep(1))
	{
		std::wstring	strTemp			= L"";
		wchar_t*	pFind			= wcschr(pStart, c);

		if (NULL == pFind)
		{
			strTemp = pStart;

			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
			}

			break;
		}

		strTemp.append(pStart, pFind);

		if (!bSkipEmpty || !strTemp.empty())
		{
			vecList.push_back(strTemp);
		}

		pStart = pFind + 1;

		if (pStart >= pEnd)
		{
			break;
		}
	}

	return vecList.size();
}

/*
 *	Function:		SpliteStrByChars
 *	Explanation:	�ַ����ָ�(����strChars�г��ֵ��ַ��ָ�)
 *	Input:			strBuf		�ַ���
					strChars	�ָ��ַ�
					bSkipEmpty	�Ƿ������մ�	
 *  Output:			���
 *	Return:			TRUE �ɹ� FALSE ʧ��
 */
int NStr::SpliteStrByChars(std::string  strBuf,  std::vector<std::string > &vecList,
								std::string  strChars /* = "|" */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize				= strBuf.size();
	char*				pStart					= (char *)strBuf.c_str();
	char*				pEnd					= pStart + sBufSize;
	std::string 			strTemp					= "";

	vecList.clear();

	if (strBuf.empty() || strChars.empty())
	{
		return 0;
	}

	for (;;Sleep(1))
	{
		char*		pFind		= NULL;

		pFind = strchr((char *)strChars.c_str(), *pStart);

		if (NULL == pFind)
		{
			strTemp.append(1, *pStart);
		}
		else
		{
			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
				strTemp.clear();
			}
		}

		pStart++;

		if (pStart >= pEnd)
		{
			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
				strTemp.clear();
			}
			break;
		}
	}

	return vecList.size();
}

int NStr::SpliteStrByChars(std::wstring strBuf,  std::vector<std::wstring> &vecList,
								std::wstring strChars /* = L"|" */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize			= strBuf.size();
	wchar_t*			pStart				= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd				= pStart + sBufSize;
	std::wstring			strTemp				= L"";

	vecList.clear();

	if (strBuf.empty() || strChars.empty())
	{
		return 0;
	}

	for (;; Sleep(1))
	{
		wchar_t*		pFind		= NULL;

		pFind = wcschr((wchar_t *)strChars.c_str(), *pStart);

		if (NULL == pFind)
		{
			strTemp.append(1, *pStart);
		}
		else
		{
			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
				strTemp.clear();
			}
		}

		pStart++;

		if (pStart >= pEnd)
		{
			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
				strTemp.clear();
			}
			break;
		}
	}

	return vecList.size();
}

/*
 *	Function:		SpliteStrByStr
 *	Explanation:	�ַ����ָ���ַ����ָ
 *	Input:			strBuf		�ַ���
					strSplite	�ָ��ַ���
					bSkipEmpty	�Ƿ������մ�
 *  Output:			vecList		�б�
 *	Return:			vecList��С
 */
int NStr::SpliteStrByStr(std::string  strBuf,  std::vector<std::string > &vecList,
								std::string  strSplite /* = "|" */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize				= strBuf.size();
	char*				pStart					= (char *)strBuf.c_str();
	char*				pEnd					= pStart + sBufSize;

	vecList.clear();

	if (strBuf.empty())
	{
		return 0;
	}

	for (;; Sleep(1))
	{
		std::string 	strTemp			= "";
		char*		pFind			= strstr(pStart, (char *)strSplite.c_str());

		if (NULL == pFind)
		{
			strTemp = pStart;

			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
			}

			break;
		}

		strTemp.append(pStart, pFind);

		if (!bSkipEmpty || !strTemp.empty())
		{
			vecList.push_back(strTemp);
		}

		pStart = pFind + strSplite.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	return vecList.size();
}

int NStr::SpliteStrByStr(std::wstring strBuf,  std::vector<std::wstring> &vecList,
								std::wstring strSplite /* = L"|" */, bool bSkipEmpty /* = false */)
{
	size_t				sBufSize				= strBuf.size();
	wchar_t*			pStart					= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd					= pStart + sBufSize;

	vecList.clear();

	if (strBuf.empty())
	{
		return 0;
	}

	for (;; Sleep(1))
	{
		std::wstring	strTemp			= L"";
		wchar_t*	pFind			= wcsstr(pStart, (wchar_t *)strSplite.c_str());

		if (NULL == pFind)
		{
			strTemp = pStart;

			if (!bSkipEmpty || !strTemp.empty())
			{
				vecList.push_back(strTemp);
			}

			break;
		}

		strTemp.append(pStart, pFind);

		if (!bSkipEmpty || !strTemp.empty())
		{
			vecList.push_back(strTemp);
		}

		pStart = pFind + strSplite.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	return vecList.size();
}

/*
 *	Function:		MutilStrToArrays
 *	Explanation:	����ַ����ָ��\0�������ָ
 *	Input:			pData			����ַ���
 *  Output:			vecList			�ַ����б�
 *	Return:			�ַ�������(vecList.size())
 */
int NStr::MutilStrToArrays(char *pData,  std::vector<std::string > &vecList)
{
	std::string 			strBuf;

	for (;;)
	{
		strBuf = pData;
		vecList.push_back(strBuf);
		pData += strlen(pData) + 1;

		if (0 == pData[0])
		{
			break;
		}
	}

	return vecList.size();
}

int NStr::MutilStrToArrays(wchar_t *pData,  std::vector<std::wstring> &vecList)
{
	std::wstring			strBuf;

	for (;;)
	{
		strBuf = pData;
		vecList.push_back(strBuf);
		pData += wcslen(pData) + 1;

		if (0 == pData[0])
		{
			break;
		}
	}

	return vecList.size();
}

/*
 *	Function:		DelStrByChars
 *	Explanation:	ɾ���ַ�����ָ�����ַ�(��strChars�еĶ�ɾ��)
 *	Input:			strBuf		�ַ���
					strDel		��Ҫɾ�����ַ�
 *  Output:			strBuf		ɾ������ַ���
 *	Return:			ɾ�����ַ����ĳ���
 */
int NStr::DelStrByChars(std::string  &strBuf, std::string  strDel)
{
	size_t				sBufSize				= strBuf.size();
	char*				pStart					= (char *)strBuf.c_str();
	char*				pEnd					= pStart + sBufSize;
	std::string 			strReturn				= "";


	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;; Sleep(1))
	{
		char*		pFind		= NULL;

		pFind = strchr((char *)strDel.c_str(), *pStart);

		if (NULL == pFind)
		{
			strReturn.append(1, *pStart);
		}
		
		pStart++;

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return strBuf.size();
}

int NStr::DelStrByChars(std::wstring &strBuf, std::wstring strDel)
{
	size_t				sBufSize				= strBuf.size();
	wchar_t*			pStart					= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd					= pStart + sBufSize;
	std::wstring			strReturn				= L"";

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;; Sleep(1))
	{
		wchar_t*		pFind = NULL;

		pFind = wcschr((wchar_t *)strDel.c_str(), *pStart);

		if (NULL == pFind)
		{
			strReturn.append(1, *pStart);
		}

		pStart++;

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return strBuf.size();
}

/*
 *	Function:		DelStrByChar
 *	Explanation:	ɾ���ַ�����ָ�����ַ�
 *	Input:			strBuf		�ַ���
					c			��Ҫɾ�����ַ�
 *  Output:			strBuf		ɾ������ַ���
 *	Return:			ɾ�����ַ�������
 */
int NStr::DelStrByStr(std::string  &strBuf, std::string  strDel)
{
	size_t				sBufSize				= strBuf.size();
	char*				pStart					= (char *)strBuf.c_str();
	char*				pEnd					= pStart + sBufSize;
	std::string 			strReturn;

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;; Sleep(1))
	{
		char*		pFind			= strstr(pStart, (char *)strDel.c_str());

		if (NULL == pFind)
		{
			strReturn.append(pStart);
			break;
		}

		strReturn.append(pStart, pFind);
		pStart = pFind + strDel.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return strBuf.size();
}

int NStr::DelStrByStr(std::wstring &strBuf, std::wstring strDel)
{
	size_t				sBufSize				= strBuf.size();
	wchar_t*			pStart					= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd					= pStart + sBufSize;
	std::wstring			strReturn;

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;; Sleep(1))
	{
		wchar_t*		pFind = wcsstr(pStart, (wchar_t *)strDel.c_str());

		if (NULL == pFind)
		{
			strReturn.append(pStart);
			break;
		}

		strReturn.append(pStart, pFind);
		pStart = pFind + strDel.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return strBuf.size();
}

/*
 *	Function:		ReplaceStr
 *	Explanation:	�滻�ַ�����ָ�����ַ���
 *	Input:			strBuf		�ַ���
					strSrc		��Ҫ�滻���ַ���
					strDes		�滻���ַ���
 *  Output:			strBuf		�滻����ַ���
 *	Return:			�滻����
 */
int NStr::ReplaceStr(std::string  &strBuf, std::string  strSrc, std::string  strDes)
{
	size_t				sBufSize				= strBuf.size();
	char*				pStart					= (char *)strBuf.c_str();
	char*				pEnd					= pStart + sBufSize;
	std::string 			strReturn;
	int					nCount					= 0;

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;;)
	{
		char*		pFind = strstr(pStart, (char *)strSrc.c_str());

		if (NULL == pFind)
		{
			strReturn.append(pStart);
			break;
		}

		nCount++;
		strReturn.append(pStart, pFind);
		strReturn.append(strDes);
		pStart = pFind + strSrc.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return nCount;
}

int NStr::ReplaceStr(std::wstring &strBuf, std::wstring strSrc, std::wstring strDes)
{
	size_t				sBufSize				= strBuf.size();
	wchar_t*			pStart					= (wchar_t *)strBuf.c_str();
	wchar_t*			pEnd					= pStart + sBufSize;
	std::wstring			strReturn;
	int					nCount					= 0;

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (;; Sleep(1))
	{
		wchar_t*		pFind = wcsstr(pStart, (wchar_t *)strSrc.c_str());

		if (NULL == pFind)
		{
			strReturn.append(pStart);
			break;
		}

		nCount++;
		strReturn.append(pStart, pFind);
		strReturn.append(strDes);
		pStart = pFind + strSrc.size();

		if (pStart >= pEnd)
		{
			break;
		}
	}

	strBuf = strReturn;

	return nCount;
}

/*
 *	Function:		StrHeadCmp
 *	Explanation:	�ַ���ͷ���Ƚ�
 *	Input:			strBuf		�ַ���(���ַ���)
					strHead		ͷ���ַ���
					bNoCase		�Ƿ����ִ�Сд
 *	Return:			0 ���  >0 �� <0 С
 */
int NStr::StrHeadCmp(std::string  strBuf, std::string  strHead, bool bNoCase /* = true */)
{
	if (strBuf.size() < strHead.size())
	{
		return -1;
	}

	if (bNoCase)
	{
		return _strnicmp((char *)strBuf.c_str(), (char *)strHead.c_str(), strHead.size());
	}

	return strncmp((char *)strBuf.c_str(), (char *)strHead.c_str(), strHead.size());
}

int NStr::StrHeadCmp(std::wstring strBuf, std::wstring strHead, bool bNoCase /* = true */)
{
	if (strBuf.size() < strHead.size())
	{
		return -1;
	}

	if (bNoCase)
	{
		return _wcsnicmp((wchar_t *)strBuf.c_str(), (wchar_t *)strHead.c_str(), strHead.size());
	}

	return wcsncmp((wchar_t *)strBuf.c_str(), (wchar_t *)strHead.c_str(), strHead.size());
}

/*
 *	Function:		StrTailCmp
 *	Explanation:	�ַ���β���Ƚ�
 *	Input:			strBuf		�ַ��������ַ�����
					strTail		β���ַ���
					bNoCase		�Ƿ����ִ�Сд
 *	Return:			0 ���  >0 �� <0 С
 */
int NStr::StrTailCmp(std::string  strBuf, std::string  strTail, bool bNoCase /* = true */)
{
	int					nPos					= 0;
	char*				pStart					= NULL;

	nPos = strBuf.size() - strTail.size();

	if (0 > nPos)
	{
		return -1;
	}

	pStart = (char *)strBuf.c_str() + nPos;

	if (bNoCase)
	{
		return _strnicmp(pStart, (char *)strTail.c_str(), strTail.size());
	}

	return strncmp(pStart, (char *)strTail.c_str(), strTail.size());
}

int NStr::StrTailCmp(std::wstring strBuf, std::wstring strTail, bool bNoCase /* = true */)
{
	int					nPos				= 0;
	wchar_t*			pStart				= NULL;

	nPos = strBuf.size() - strTail.size();

	if (0 > nPos)
	{
		return -1;
	}

	pStart = (wchar_t *)strBuf.c_str() + nPos;

	if (bNoCase)
	{
		return _wcsnicmp(pStart, (wchar_t *)strTail.c_str(), strTail.size());
	}

	return wcsncmp(pStart, (wchar_t *)strTail.c_str(), strTail.size());
}

/*
 *	Function:		StrTrim
 *	Explanation:	ȥ���ַ������߳��ֵ��ַ�(��strChars�г��ֵĶ�ɾ��)
 *	Input:			strBuf		�ַ���
					strChars	��Ҫȥ�����ַ�
 *	Output:			strBuf		ȥ������ַ���
 *	Return:			ȥ�����ַ����Ĵ�С
 */
int NStr::StrTrim(std::string  &strBuf, std::string  strChars /* = " " */)
{
	std::string ::iterator				Iter;		// ���������
	std::string ::reverse_iterator		reIter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		Iter = strBuf.begin();

		if (NULL != strchr((char *)strChars.c_str(), *Iter))
		{
			strBuf.erase(Iter);
			continue;
		}

		break;
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		reIter = strBuf.rbegin();

		if (NULL != strchr((char *)strChars.c_str(), *reIter))
		{
			strBuf.erase(strBuf.begin() + strBuf.size() - 1);
			continue;
		}

		break;
	}

	return strBuf.size();
}

int NStr::StrTrim(std::wstring &strBuf, std::wstring strChars /* = L" " */)
{
	std::wstring::iterator				Iter;		// ���������
	std::wstring::reverse_iterator		reIter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		Iter = strBuf.begin();

		if (NULL != wcschr((wchar_t *)strChars.c_str(), *Iter))
		{
			strBuf.erase(Iter);
			continue;
		}

		break;
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		reIter = strBuf.rbegin();

		if (NULL != wcschr((wchar_t *)strChars.c_str(), *reIter))
		{
			strBuf.erase(strBuf.begin() + strBuf.size() - 1);
			continue;
		}

		break;
	}

	return strBuf.size();
}

/*
 *	Function:		StrTrimLeft
 *	Explanation:	ȥ���ַ�����߳��ֵ�ָ���ַ�(��strChars�г��ֵĶ�ɾ��)
 *	Input:			strBuf		�ַ���
					strChars	��Ҫȥ�����ַ�
 *	Output:			strBuf		ȥ������ַ���
 *	Return:			ȥ�����ַ����Ĵ�С
 */
int NStr::StrTrimLeft(std::string  &strBuf, std::string  strChars /* = " " */)
{
	std::string ::iterator				Iter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		Iter = strBuf.begin();

		if (NULL != strchr((char *)strChars.c_str(), *Iter))
		{
			strBuf.erase(Iter);
			continue;
		}

		break;
	}

	return strBuf.size();
}

int NStr::StrTrimLeft(std::wstring &strBuf, std::wstring strChars /* = L" " */)
{
	std::wstring::iterator				Iter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		Iter = strBuf.begin();

		if (NULL != wcschr((wchar_t *)strChars.c_str(), *Iter))
		{
			strBuf.erase(Iter);
			continue;
		}

		break;
	}

	return strBuf.size();
}

/*
 *	Function:		StrTrimRight
 *	Explanation:	ȥ���ַ����ұ߳��ֵ�ָ���ַ�(��strChars�г��ֵĶ�ɾ��)
 *	Input:			strBuf		�ַ���
					strChars	��Ҫȥ�����ַ�
 *	Output:			strBuf		ȥ������ַ���
 *	Return:			ȥ�����ַ����Ĵ�С
 */
int NStr::StrTrimRight(std::string  &strBuf, std::string  strChars /* = " " */)
{
	std::string ::reverse_iterator		reIter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		reIter = strBuf.rbegin();

		if (NULL != strchr((char *)strChars.c_str(), *reIter))
		{
			strBuf.erase(strBuf.begin() + strBuf.size() - 1);
			continue;
		}

		break;
	}

	return strBuf.size();
}

int NStr::StrTrimRight(std::wstring &strBuf, std::wstring strChars /* = L" " */)
{
	std::wstring::reverse_iterator		reIter;		// ���������

	if (strBuf.empty())
	{
		return strBuf.size();
	}

	for (size_t sIndex = 0; sIndex < strBuf.size(); sIndex++)
	{
		reIter = strBuf.rbegin();

		if (NULL != wcschr((wchar_t *)strChars.c_str(), *reIter))
		{
			strBuf.erase(strBuf.begin() + strBuf.size() - 1);
			continue;
		}

		break;
	}

	return strBuf.size();
}

/*
 *	Function:		CreateRandomStr
 *	Explanation:	��������ַ���
 *	Input:			nSize		�ַ�������	
					strSeed		�������
 *	Return:			����ַ���
 */
std::string  NStr::CreateRandomStr(int nSize, std::string  strSeed /* = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" */)
{
	std::string 				strReturn;
	char*					pStr					= (char *)strSeed.c_str();
	size_t					sLength					= strSeed.size();
	static unsigned long	lThreadId				= 0;

	if (strSeed.empty())
	{
		return "";
	}

	if (GetCurrentThreadId() != lThreadId)
	{
		srand(GetTickCount());
		lThreadId = GetCurrentThreadId();
	}

	for (int i = 0; i < nSize; i++)
	{
		strReturn.append(1, pStr[rand() % sLength]);
	}

	return strReturn;
}

std::wstring NStr::CreateRandomWStr(int nSize, std::wstring strSeed /* = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" */)
{
	std::wstring				strReturn;
	wchar_t*				pStr				= (wchar_t *)strSeed.c_str();
	size_t					sLength				= strSeed.size();
	static unsigned long	lThreadId			= 0;

	if (strSeed.empty())
	{
		return L"";
	}

	if (GetCurrentThreadId() != lThreadId)
	{
		srand(GetTickCount());
		lThreadId = GetCurrentThreadId();
	}

	for (int i = 0; i < nSize; i++)
	{
		strReturn.append(1, pStr[rand() % sLength]);
	}

	return strReturn;
}

/*
 *	Function:		FormatStr
 *	Explanation:	��ʽ���ַ���
 *	Input:			pFmt		��ʽ����
 *	Return:			��ʽ������ַ���
 */
std::string  NStr::FormatStr(const char* pFmt, ...)
{
	std::string 			strBuf				= "";
	va_list				varList;
	char*				pData				= NULL;
	int					nRet				= 0;
	int					nSize				= 2048;
	bool				bSuccess			= false;

	pData = (char *)malloc(nSize);

	if (NULL == pData)
	{
		return strBuf;
	}

	va_start(varList, pFmt);

	for (; nSize < (10 * __1M); Sleep(1))
	{
		nRet = _vsnprintf(pData, nSize - 1, pFmt, varList);

		if (0 <= nRet)
		{
			bSuccess = true;
			break;
		}

		free(pData);
		nSize *= 2;
		pData = (char *)malloc(nSize);

		if (NULL == pData)
		{
			return strBuf;
		}
	}

	va_end(varList);

	if (bSuccess)
	{
		strBuf = pData;
	}

	free(pData);

	return strBuf;
}

/*
*	Function:		FormatStr
*	Explanation:	��ʽ���ַ���
*	Input:			pFmt		��ʽ����
*	Return:			��ʽ������ַ���
*/
std::wstring NStr::FormatWStr(const wchar_t* pFmt, ...)
{
	std::wstring			strBuf				= L"";
	va_list				varList;
	wchar_t*			pData				= NULL;
	int					nRet				= 0;
	int					nSize				= 2048;
	bool				bSuccess			= false;

	pData = (wchar_t *)malloc(nSize);

	if (NULL == pData)
	{
		return strBuf;
	}

	va_start(varList, pFmt);

	for (; nSize < (10 * __1M); Sleep(1))
	{
		nRet = _vsnwprintf(pData, nSize - 2, pFmt, varList);

		if (0 <= nRet)
		{
			bSuccess = true;
			break;
		}

		free(pData);
		nSize *= 2;
		pData = (wchar_t *)malloc(nSize);

		if (NULL == pData)
		{
			return strBuf;
		}
	}

	va_end(varList);

	if (bSuccess)
	{
		strBuf = pData;
	}

	free(pData);

	return strBuf;
}

/*
 *	Function:		StrToLwr
 *	Explanation:	�ַ���תСд
 *	Input:			strBuf		�ַ���
 *	Return:			Сд�ַ���
 */
std::string  NStr::StrToLwr(std::string  strBuf)
{
	if (strBuf.empty())
	{
		return "";
	}

	return _strlwr((char *)strBuf.c_str());
}

std::wstring NStr::StrToLwr(std::wstring strBuf)
{
	if (strBuf.empty())
	{
		return L"";
	}

	return _wcslwr((wchar_t *)strBuf.c_str());
}

/*
*	Function:		StrToLwr
*	Explanation:	�ַ���ת��д
*	Input:			strBuf		�ַ���
*	Return:			��д�ַ���
*/
std::string  NStr::StrToUpr(std::string  strBuf)
{
	if (strBuf.empty())
	{
		return "";
	}

	return _strupr((char *)strBuf.c_str());
}

std::wstring NStr::StrToUpr(std::wstring strBuf)
{
	if (strBuf.empty())
	{
		return L"";
	}

	return _wcsupr((wchar_t *)strBuf.c_str());
}

#define Ansi2Type_Number	0	// ����
#define Ansi2Type_AlphUpr	1	// ��д��ĸ
#define Ansi2Type_AlphLwr	2	// Сд��ĸ
#define Ansi2Type_Operate	3	// ������

static byte s_Ansi_Table[256] =
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x03, 0xFF, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0x03, 0x03, 0x03,
	//        !     "     #     $     %     &     '      (     )     *     +     ,     -     .     /

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0x03, 0x03, 0x03, 0x03,
	//  0     1     2     3     4     5     6     7      8     9     :     ;     <     =     >     ?

	0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	//  @     A     B     C     D     E     F     G      H     I     J     K     L     M     N     O

	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03,
	//  P     Q     R     S     T     U     V     W      X     Y     Z     [     \     ]     ^     _

	0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	//  `     a     b     c     d     e     f     g      h     i     j     k     l     m     n     o

	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0xFF,
	//  p     q     r     s     t     u     v     w      x     y     z     {     |     }     ~

	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

/*
 *	Function:		IsNumber
 *	Explanation:	�ж��ַ��Ƿ�������
 *	Input:			c		�ַ�
 *	Return:			true ������ false ��������
 */
bool NStr::IsNumber(char c)
{
	if (Ansi2Type_Number == s_Ansi_Table[(byte)c])
	{
		return true;
	}

	return false;
}

/*
*	Function:		IsAlphUpr
*	Explanation:	�ж��ַ��Ƿ��Ǵ�д��ĸ
*	Input:			c		�ַ�
*	Return:			true �� false ����
*/
bool NStr::IsAlphUpr(char c)
{
	if (Ansi2Type_AlphUpr == s_Ansi_Table[(byte)c])
	{
		return true;
	}

	return false;
}

/*
*	Function:		IsAplhLwr
*	Explanation:	�ж��ַ��Ƿ���Сд��ĸ
*	Input:			c		�ַ�
*	Return:			true �� false ����
*/
bool NStr::IsAplhLwr(char c)
{
	if (Ansi2Type_AlphLwr == s_Ansi_Table[(byte)c])
	{
		return true;
	}

	return false;
}

/*
*	Function:		IsAplh
*	Explanation:	�ж��ַ��Ƿ�����ĸ
*	Input:			c		�ַ�
*	Return:			true �� false ����
*/
bool NStr::IsAplh(char c)
{
	if (Ansi2Type_AlphUpr == s_Ansi_Table[(byte)c]
		|| Ansi2Type_AlphLwr == s_Ansi_Table[(byte)c])
	{
		return true;
	}

	return false;
}

/*
*	Function:		IsOperate
*	Explanation:	�ж��ַ��Ƿ��ǲ�����
*	Input:			c		�ַ�
*	Return:			true �� false ����
*/
bool NStr::IsOperate(char c)
{
	if (Ansi2Type_Operate == s_Ansi_Table[(byte)c])
	{
		return true;
	}

	return false;
}

/**
 * ��������˵�����ж��ַ����Ƿ��������
 * @param [in]  strBuf			�ַ���
 * @return TRUE ���� FALSE ������
 * by Maga 2016-9-22
 */
bool NStr::HasChinese(std::string  strBuf)
{
	for (int i = 0; i < strBuf.size(); i++)
	{
		char		c1;
		char		c2;

		c1 = strBuf[i];

		if ('\0' == c1)
		{
			break;
		}

		c2 = strBuf[i + 1];

		if ('\0' == c2)
		{
			break;
		}

		if ( (c1 & 0x80) && (c2 & 0x80) )
		{
			return true;
		}
	}

	return false;
}

/**
 * ��������˵������ȡGUID
 * @return GUID
 * by Maga 2016-7-22
 */
std::string  NStr::GetGUIDStr()
{
	static char				buf[64]					= { 0 };
	GUID					guid;

	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, sizeof(buf)
			, "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}

	return (char*)buf;
}

static const char s_Base64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
*	Function:		_GetBase64TableIndex
*	Explanation:	��ȡ��Ӧ�ַ���s_Base64Table�е�����
*	Input:			c		�ַ�
*	Return:			������ ʧ�ܷ���-1
*/
inline int _GetBase64TableIndex(char c)
{
	for (int i = 0; s_Base64Table[i]; i++)
	{
		if (c == s_Base64Table[i])
		{
			return i;
		}
	}

	return -1;
}

/*
 *	Function:		Base64Encode
 *	Explanation:	Base64����
 *	Input:			strSrc		��Ҫ���ܵ��ַ���
 *	Return:			���ܺ���ַ�����ʧ�ܷ���""
 */
std::string  NStr::Base64Encode(std::string  strSrc)
{
	size_t				stSrcSize				= strSrc.size();
	unsigned char		cIn[3]					= { 0 };
	unsigned char		cOut;
	std::string 			strDes;

	for (size_t stIndex = 0; stIndex < stSrcSize;)
	{
		// 0
		cIn[0] = (unsigned char)strSrc[stIndex++];
		cOut = (cIn[0] >> 2) & 0x3F;
		strDes.append(1, s_Base64Table[cOut]);

		// 1
		if (stIndex < stSrcSize)
		{
			cIn[1] = (unsigned char)strSrc[stIndex++];
			cOut = ((cIn[0] << 4) & 0x30) + ((cIn[1] >> 4) & 0xF);
			strDes.append(1, s_Base64Table[cOut]);
		}
		else
		{
			cOut = (cIn[0] << 4) & 0x30;
			strDes.append(1, s_Base64Table[cOut]);
			strDes.append("==");
			break;
		}

		// 2
		if (stIndex < stSrcSize)
		{
			cIn[2] = (unsigned char)strSrc[stIndex++];
			cOut = ((cIn[1] << 2) & 0x3C) + ((cIn[2] >> 6) & 0x3);
			strDes.append(1, s_Base64Table[cOut]);
		}
		else
		{
			cOut = (cIn[1] << 2) & 0x3C;
			strDes.append(1, s_Base64Table[cOut]);
			strDes.append("=");
			break;
		}

		// 3
		cOut = cIn[2] & 0x3F;
		strDes.append(1, s_Base64Table[cOut]);
	}

	return strDes;
}

/*
 *	Function:		Base64Decode
 *	Explanation:	Base64����
 *	Input:			strSrc		��Ҫ���ܵ��ַ���
 *	Return:			���ܺ���ַ���
 */
std::string  NStr::Base64Decode(std::string  strSrc)
{
	size_t				stTempSize				= strSrc.size();
	std::string 			strDes;
	std::string 			strTemp;
	unsigned char		cIn[4];
	unsigned char		cOut;

	// ȥ��=
	strTemp = strSrc.substr(0, strSrc.find_first_of("=", strSrc.size() - 2));
	stTempSize = strTemp.size();

	for (size_t stIndex = 0; stIndex < stTempSize;)
	{
		// 0
		cIn[0] = _GetBase64TableIndex(strTemp[stIndex++]);

		// 1
		if (stIndex < stTempSize)
		{
			cIn[1] = _GetBase64TableIndex(strTemp[stIndex++]);
			cOut = (unsigned char)((cIn[0] << 2) & 0xFC) + (unsigned char)((cIn[1] >> 4) & 0x3);
			strDes.append(1, cOut);
		}
		else
		{
			break;
		}

		// 2
		if (stIndex < stTempSize)
		{
			cIn[2] = _GetBase64TableIndex(strTemp[stIndex++]);
			cOut = (unsigned char)((cIn[1] << 4) & 0xF0) + (unsigned char)((cIn[2] >> 2) & 0x0F);
			strDes.append(1, cOut);
		}
		else
		{
			break;
		}

		// 3
		if (stIndex < stTempSize)
		{
			cIn[3] = _GetBase64TableIndex(strTemp[stIndex++]);
			cOut = (unsigned char)((cIn[2] << 6) & 0xC0) + (unsigned char)(cIn[3] & 0x3F);
			strDes.append(1, cOut);
		}
		else
		{
			break;
		}
	}

	return strDes;
}


inline unsigned char _ToHex(unsigned char c)
{
	return c > 9 ? c + 55 : c + 48;
}

/*
 *	Function:		UrlEncode
 *	Explanation:	UrlEncode����
 *	Input:			strBuf		��Ҫ���ܵ��ַ���
 *	Return:			���ܺ��ַ���
 */
std::string  NStr::UrlEncode(std::string  strBuf)
{
	std::string 			strReturn;

	for (size_t stIndex = 0; stIndex < strBuf.size(); stIndex++)
	{
		if (isalnum((unsigned char)strBuf[stIndex])
			|| '-' == strBuf[stIndex]
			|| '_' == strBuf[stIndex]
			|| '.' == strBuf[stIndex]
			|| '~' == strBuf[stIndex])
		{
			strReturn += strBuf[stIndex];
		}
		else if (isspace((unsigned char)strBuf[stIndex]))
		{
			strReturn += "+";
		}
		else
		{
			strReturn += '%';
			strReturn += _ToHex((unsigned char)strBuf[stIndex] >> 4);
			strReturn += _ToHex((unsigned char)strBuf[stIndex] % 16);
		}
	}

	return strReturn;
}

inline unsigned char _FromHex(unsigned char c)
{
	unsigned char			cRet;

	if (c >= 'A' && c <= 'Z')
	{
		cRet = c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'z')
	{
		cRet = c - 'a' + 10;
	}
	else if (c >= '0' && c <= '9')
	{
		cRet = c - '0';
	}
	else
	{
		assert(0);
	}

	return cRet;
}

/*
*	Function:		UrlDecode
*	Explanation:	UrlDecode����
*	Input:			strBuf		��Ҫ���ܵ��ַ���
*	Return:			���ܺ��ַ���
*/
std::string  NStr::UrlDecode(std::string  strBuf)
{
	std::string 			strReturn;

	for (size_t stIndex = 0; stIndex < strBuf.size(); stIndex++)
	{
		if ('+' == strBuf[stIndex])
		{
			strReturn += ' ';
		}
		else if ('%' == strBuf[stIndex])
		{
			unsigned char		cHigh;
			unsigned char		cLow;

			assert(stIndex + 2 < strBuf.size());
			cHigh = _FromHex((unsigned char)strBuf[++stIndex]);
			cLow = _FromHex((unsigned char)strBuf[++stIndex]);
			strReturn += cHigh * 16 + cLow;
		}
		else
		{
			strReturn += strBuf[stIndex];
		}
	}

	return strReturn;
}










#ifdef GS_USE_CRYPT

/*
 *	Function:		GetSha1
 *	Explanation:	Sha1����
 *	Input:			strBuf		��Ҫ�����ַ���
 *	Return:			���ܺ��ַ���
 */
std::string  NStr::GetSha1(std::string  strBuf)
{
	SHA_CTX				Ctx;
	unsigned char		szMemory[21]				= { 0 };
	char				szSha1[41]					= { 0 };

	SHA1_Init(&Ctx);
	SHA1_Update(&Ctx, (void *)strBuf.c_str(), strBuf.size());
	SHA1_Final(szMemory, &Ctx);
	GSNUtil::DataToHex(szMemory, 20, szSha1, 41);

	return szSha1;
}

/*
 *	Function:		GetMD5
 *	Explanation:	MD5����
 *	Input:			strBuf		��Ҫ�����ַ���
 *	Return:			���ܺ��ַ���
 */
std::string  NStr::GetMD5(std::string  strBuf)
{
	MD5_CTX				Ctx;
	unsigned char		szMemory[17]				= { 0 };
	char				szMD5[33]					= { 0 };

	MD5_Init(&Ctx);
	MD5_Update(&Ctx, (void *)strBuf.c_str(), strBuf.size());
	MD5_Final(szMemory, &Ctx);
	GSNUtil::DataToHex(szMemory, 16, szMD5, 33);

	return szMD5;
}

/*
 *	Function:		Aes128Encode
 *	Explanation:	Aes����(���ص����ڴ�����)
 *	Input:			strPwd		����
					strBuf		��������
 *	Return:			���ܺ�����
 */
std::string  NStr::Aes128Encode(std::string  strPwd, std::string  strBuf)
{
	AES_KEY				AesKey;
	unsigned char		szOut[AES_BLOCK_SIZE]				= { 0 };
	int					nIndex								= 0;
	std::string 			strEncode;
	std::string 			strSub;

	// 128-bit
	if (0 > AES_set_encrypt_key((const unsigned char *)strPwd.c_str(), AES_BLOCK_SIZE * 8, &AesKey))
	{
		return "";
	}

	for (; nIndex < strBuf.size() / AES_BLOCK_SIZE; nIndex++)
	{
		memset(szOut, 0, sizeof(szOut));
		strSub = strBuf.substr(nIndex * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		AES_encrypt((const unsigned char *)strSub.c_str(), szOut, &AesKey);
		strEncode += std::string ((char *)szOut, AES_BLOCK_SIZE);
	}

	if (0 < (strBuf.size() - nIndex * AES_BLOCK_SIZE))
	{
		memset(szOut, 0, sizeof(szOut));
		strSub = strBuf.substr(nIndex * AES_BLOCK_SIZE, strBuf.size() - nIndex * AES_BLOCK_SIZE);
		AES_encrypt((const unsigned char *)strSub.c_str(), szOut, &AesKey);
		strEncode += std::string ((char *)szOut, AES_BLOCK_SIZE);
	}

	return strEncode;
}

/*
 *	Function:		Aes128Decode
 *	Explanation:	Aes����
 *	Input:			strPwd		����
					strBuf		��������
 *	Return:			���ܺ�����
 */
std::string  NStr::Aes128Decode(std::string  strPwd, std::string  strBuf)
{
	AES_KEY				AesKey;
	unsigned char		szOut[AES_BLOCK_SIZE]				= { 0 };
	int					nIndex								= 0;
	std::string 			strDecode;
	std::string 			strSub;

	if (0 > AES_set_decrypt_key((const unsigned char *)strPwd.c_str(), AES_BLOCK_SIZE * 8, &AesKey))
	{
		return "";
	}

	for (; nIndex < strBuf.size() / AES_BLOCK_SIZE; nIndex++)
	{
		memset(szOut, 0, sizeof(szOut));
		strSub = strBuf.substr(nIndex * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		AES_decrypt((const unsigned char *)strSub.c_str(), szOut, &AesKey);
		strDecode += std::string ((char *)szOut, AES_BLOCK_SIZE);
	}

	if (0 < (strBuf.size() - nIndex * AES_BLOCK_SIZE))
	{
		memset(szOut, 0, sizeof(szOut));
		strSub = strBuf.substr(nIndex * AES_BLOCK_SIZE, strBuf.size() - nIndex * AES_BLOCK_SIZE);
		AES_decrypt((const unsigned char *)strSub.c_str(), szOut, &AesKey);
		strDecode += std::string ((char *)szOut, AES_BLOCK_SIZE);
	}

	return strDecode;
}

/*
 *	Function:		RsaPubEncode
 *	Explanation:	Rsa��Կ����(���ص����ڴ�����)
 *	Input:			strKey		��Կ
					strBuf		��Ҫ���ܵ�����
 *	Return:			���ܺ������
 */
std::string  NStr::RsaPubEncode(std::string  strKey, std::string  strBuf)
{
	BIO*				pBio;
	RSA*				pRsa;
	int					nBlock;
	int					nBlockReal;
	char*				pData					= NULL;
	int					nDataSize;
	int					nCount;
	int					nLen					= 0;
	std::string 			strReturn;

	pBio = BIO_new_mem_buf((void *)strKey.c_str(), -1);

	if (NULL == pBio)
	{
		return "";
	}

	pRsa = PEM_read_bio_RSA_PUBKEY(pBio, NULL, NULL, NULL);

	if (NULL == pRsa)
	{
		BIO_free(pBio);
		return "";
	}

	nBlock = RSA_size(pRsa);
	nBlockReal = nBlock - 11;
	nCount = (strBuf.size() + nBlockReal - 1) / nBlockReal;
	nDataSize = nCount * nBlock;
	pData = (char *)malloc(nDataSize);

	if (NULL == pData)
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	memset(pData, 0, nDataSize);

	for (int i = 0; i < nCount; i++)
	{
		int			nRet			= 0;
		int			nSize			= min(strBuf.size() - i * nBlockReal, nBlockReal);

		nRet = RSA_public_encrypt(nSize,
								(const unsigned char *)strBuf.c_str() + i * nBlockReal,
								(unsigned char *)pData + nLen,
								pRsa, RSA_PKCS1_PADDING);

		if (0 > nRet)
		{
			nLen = -1;
			break;
		}

		nLen += nRet;
	}

	if (-1 != nLen)
	{
		strReturn = std::string (pData, nLen);
	}

	free(pData);
	RSA_free(pRsa);
	BIO_free(pBio);

	return strReturn;
}

/*
 *	Function:		RsaPubDecode
 *	Explanation:	Rsa��Կ����
 *	Input:			strKey		��Կ
					strBuf		��Ҫ��������
 *	Return:			��������
 */
std::string  NStr::RsaPubDecode(std::string  strKey, std::string  strBuf)
{
	BIO*				pBio;
	RSA*				pRsa;
	int					nBlock;
	int					nBlockReal;
	char*				pData					= NULL;
	int					nDataSize;
	int					nCount;
	int					nLen					= 0;
	std::string 			strReturn;

	pBio = BIO_new_mem_buf((void *)strKey.c_str(), -1);

	if (NULL == pBio)
	{
		return "";
	}

	pRsa = PEM_read_bio_RSA_PUBKEY(pBio, NULL, NULL, NULL);

	if (NULL == pRsa)
	{
		BIO_free(pBio);
		return "";
	}

	nBlock = RSA_size(pRsa);
	nBlockReal = nBlock - 11;

	if (0 != (strBuf.size() % nBlock))
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	nCount = strBuf.size() / nBlock;
	nDataSize = (strBuf.size() + nBlock - 1) / nBlock * nBlockReal + 1;
	pData = (char *)malloc(nDataSize);

	if (NULL == pData)
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	memset(pData, 0, nDataSize);

	for (int i = 0; i < nCount; i++)
	{
		int			nRet			= 0;

		nRet = RSA_public_decrypt(nBlock,
									(unsigned char *)strBuf.c_str() + i * nBlock,
									(unsigned char *)pData + nLen,
									pRsa,
									RSA_PKCS1_PADDING);

		if (0 > nRet)
		{
			nLen = -1;
			break;
		}

		nLen += nRet;
	}

	if (-1 != nLen)
	{
		strReturn = std::string (pData, nLen);
	}

	free(pData);
	RSA_free(pRsa);
	BIO_free(pBio);

	return strReturn;
}

/*
*	Function:		RsaPriEncode
*	Explanation:	Rsa˽Կ����(���ص����ڴ�����)
*	Input:			strKey		��Կ
					strBuf		��Ҫ���ܵ�����
*	Return:			���ܺ������
*/
std::string  NStr::RsaPriEncode(std::string  strKey, std::string  strBuf)
{
	BIO*				pBio;
	RSA*				pRsa;
	int					nBlock;
	int					nBlockReal;
	char*				pData				= NULL;
	int					nDataSize;
	int					nCount;
	int					nLen				= 0;
	std::string 			strReturn;

	pBio = BIO_new_mem_buf((void *)strKey.c_str(), -1);

	if (NULL == pBio)
	{
		return "";
	}

	pRsa = PEM_read_bio_RSAPrivateKey(pBio, NULL, NULL, NULL);

	if (NULL == pRsa)
	{
		BIO_free(pBio);
		return "";
	}

	nBlock = RSA_size(pRsa);
	nBlockReal = nBlock - 11;
	nCount = (strBuf.size() + nBlockReal - 1) / nBlockReal;
	nDataSize = nCount * nBlock;
	pData = (char *)malloc(nDataSize);

	if (NULL == pData)
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	memset(pData, 0, nDataSize);

	for (int i = 0; i < nCount; i++)
	{
		int			nRet = 0;
		int			nSize = min(strBuf.size() - i * nBlockReal, nBlockReal);

		nRet = RSA_private_encrypt(nSize,
									(const unsigned char *)strBuf.c_str() + i * nBlockReal,
									(unsigned char *)pData + nLen,
									pRsa, RSA_PKCS1_PADDING);

		if (0 > nRet)
		{
			nLen = -1;
			break;
		}

		nLen += nRet;
	}

	if (-1 != nLen)
	{
		strReturn = std::string (pData, nLen);
	}

	free(pData);
	RSA_free(pRsa);
	BIO_free(pBio);

	return strReturn;
}

/*
*	Function:		RsaPriDecode
*	Explanation:	Rsa˽Կ����
*	Input:			strKey		��Կ
					strBuf		��Ҫ��������
*	Return:			��������
*/
std::string  NStr::RsaPriDecode(std::string  strKey, std::string  strBuf)
{
	BIO*				pBio;
	RSA*				pRsa;
	int					nBlock;
	int					nBlockReal;
	char*				pData					= NULL;
	int					nDataSize;
	int					nCount;
	int					nLen					= 0;
	std::string 			strReturn;

	pBio = BIO_new_mem_buf((void *)strKey.c_str(), -1);

	if (NULL == pBio)
	{
		return "";
	}

	pRsa = PEM_read_bio_RSAPrivateKey(pBio, NULL, NULL, NULL);

	if (NULL == pRsa)
	{
		BIO_free(pBio);
		return "";
	}

	nBlock = RSA_size(pRsa);
	nBlockReal = nBlock - 11;

	if (0 != (strBuf.size() % nBlock))
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	nCount = strBuf.size() / nBlock;
	nDataSize = (strBuf.size() + nBlock - 1) / nBlock * nBlockReal + 1;
	pData = (char *)malloc(nDataSize);

	if (NULL == pData)
	{
		RSA_free(pRsa);
		BIO_free(pBio);
		return "";
	}

	memset(pData, 0, nDataSize);

	for (int i = 0; i < nCount; i++)
	{
		int			nRet = 0;

		nRet = RSA_private_decrypt(nBlock,
			(unsigned char *)strBuf.c_str() + i * nBlock,
			(unsigned char *)pData + nLen,
			pRsa,
			RSA_PKCS1_PADDING);

		if (0 > nRet)
		{
			nLen = -1;
			break;
		}

		nLen += nRet;
	}

	if (-1 != nLen)
	{
		strReturn = std::string (pData, nLen);
	}

	free(pData);
	RSA_free(pRsa);
	BIO_free(pBio);

	return strReturn;
}

/*
 *	Function:		GetRsaKey
 *	Explanation:	��ȡRsa��Կ��˽Կ��
 *	Input:			����
 *  Output:			strPubKey		��Կ
					strPriKey		˽Կ
 *	Return:			true �ɹ� false ʧ��
 */
bool NStr::GetRsaKey(std::string  &strPubKey, std::string  &strPriKey)
{
	RSA*				pRsa;
	int					nLen;
	char				szPubKey[1024]				= { 0 };
	char				szPriKey[1024]				= { 0 };

	pRsa = RSA_new();

	if (NULL == pRsa)
	{
		return false;
	}

	pRsa = RSA_generate_key(1024, 0x10001, NULL, NULL);
	nLen = i2d_RSAPublicKey(pRsa, (unsigned char **)&szPubKey);

	if (0 > nLen)
	{
		RSA_free(pRsa);
		return false;
	}

	strPubKey = std::string (szPubKey, nLen);
	nLen = i2d_RSAPrivateKey(pRsa, (unsigned char **)&szPriKey);

	if (0 > nLen)
	{
		RSA_free(pRsa);
		return false;
	}

	strPriKey = std::string (szPriKey, nLen);
	RSA_free(pRsa);

	return true;
}

/*
 *	Function:		DesEncode
 *	Explanation:	Des���� PKCS7
 *	Input:			strKey		��Կ
					strBuf		��������
					bEcb		�Ƿ���ECBģʽ
 *	Return:			���ܺ�����
 */
std::string  NStr::DesEncode(std::string  strKey, std::string  strBuf, bool bEcb /* = true */)
{
	int					nPkLen				= 0;
	DES_key_schedule	DesKs;
	std::string 			strReturn;
	unsigned char		szOut[9]			= { 0 };
	DES_cblock			DesCB;

	// ��Կ���/�ض�
	if (8 < strKey.size())
	{
		strKey = strKey.substr(0, 8);
	}
	else
	{
		strKey.append(8 - strKey.size(), 0);
	}

	// �������
	nPkLen = strBuf.size() % 8;
	strBuf.append(8 - nPkLen, 8 - nPkLen);
	

	DES_set_key_unchecked((const_DES_cblock *)strKey.c_str(), &DesKs);

	for (int i = 0; i < (strBuf.size() / 8); i++)
	{
		memset(szOut, 0, sizeof(szOut));

		if (bEcb)
		{
			DES_ecb_encrypt((DES_cblock *)(strBuf.c_str() + i * 8), (DES_cblock *)&szOut, &DesKs, DES_ENCRYPT);
		}
		else
		{
			DES_ncbc_encrypt((const unsigned char *)(strBuf.c_str() + i * 8), szOut, 8, &DesKs, &DesCB, DES_ENCRYPT);
		}

		strReturn += std::string ((char *)szOut, 8);
	}

	return strReturn;
}

/*
*	Function:		DesDecode
*	Explanation:	Des���� PKCS7
*	Input:			strKey		��Կ
					strBuf		��������
					bEcb		�Ƿ���ECBģʽ
*	Return:			���ܺ�����
*/
std::string  NStr::DesDecode(std::string  strKey, std::string  strBuf, bool bEcb /* = true */)
{
	int					nPkLen					= 0;
	DES_key_schedule	DesKs;
	std::string 			strReturn;
	unsigned char		szOut[9]				= { 0 };
	DES_cblock			DesCB;
	int					nDel;

	// ��Կ���/�ض�
	if (8 < strKey.size())
	{
		strKey = strKey.substr(0, 8);
	}
	else
	{
		strKey.append(8 - strKey.size(), 0);
	}

	if (0 != (strBuf.size() % 8))
	{
		return "";
	}

	DES_set_key_unchecked((const_DES_cblock *)strKey.c_str(), &DesKs);

	for (int i = 0; i < (strBuf.size() / 8); i++)
	{
		memset(szOut, 0, sizeof(szOut));

		if (bEcb)
		{
			DES_ecb_encrypt((DES_cblock *)(strBuf.c_str() + i * 8), (DES_cblock *)&szOut, &DesKs, DES_DECRYPT);
		}
		else
		{
			DES_ncbc_encrypt((const unsigned char *)(strBuf.c_str() + i * 8), szOut, 8, &DesKs, &DesCB, DES_DECRYPT);
		}

		strReturn += std::string ((char *)szOut, 8);
	}

	// ȥ�����
	nDel = strReturn[strReturn.size() - 1];
	strReturn = strReturn.substr(0, strReturn.size() - nDel);

	return strReturn;
}

/*
 *	Function:		Des3Encode
 *	Explanation:	3Des���� PKCS7 ECB
 *	Input:			strKey		��Կ
					strBuf		��������
 *	Return:			���ܺ�����
 */
std::string  NStr::Des3Encode(std::string  strKey, std::string  strBuf)
{
	int					nPkLen				= 0;
	DES_key_schedule	DesKs;
	DES_key_schedule	DesKs2;
	DES_key_schedule	DesKs3;
	std::string 			strReturn;
	unsigned char		szOut[9]			= { 0 };
	DES_cblock			BlockKey;

	// ��Կ���/�ض�
	if (24 < strKey.size())
	{
		strKey = strKey.substr(0, 24);
	}
	else
	{
		strKey.append(24 - strKey.size(), 0);
	}

	// �������
	nPkLen = strBuf.size() % 8;
	strBuf.append(8 - nPkLen, 8 - nPkLen);

	memcpy(&BlockKey, (void *)(strKey.c_str()), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs);
	memcpy(&BlockKey, (void *)(strKey.c_str() + 8), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs2);
	memcpy(&BlockKey, (void *)(strKey.c_str() + 16), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs3);

	for (int i = 0; i < (strBuf.size() / 8); i++)
	{
		memset(szOut, 0, sizeof(szOut));
		DES_ecb3_encrypt((DES_cblock *)(strBuf.c_str() + i * 8), (DES_cblock *)&szOut, &DesKs, &DesKs2, &DesKs3, DES_ENCRYPT);
		strReturn += std::string ((char *)szOut, 8);
	}

	return strReturn;
}

/*
*	Function:		Des3Decode
*	Explanation:	3Des���� PKCS7 ECB
*	Input:			strKey		��Կ
					strBuf		��������
*	Return:			���ܺ�����
*/
std::string  NStr::Des3Decode(std::string  strKey, std::string  strBuf)
{
	int					nPkLen				= 0;
	DES_key_schedule	DesKs;
	DES_key_schedule	DesKs2;
	DES_key_schedule	DesKs3;
	std::string 			strReturn;
	unsigned char		szOut[9]			= { 0 };
	DES_cblock			BlockKey;
	int					nDel;

	// ��Կ���/�ض�
	if (24 < strKey.size())
	{
		strKey = strKey.substr(0, 24);
	}
	else
	{
		strKey.append(24 - strKey.size(), 0);
	}

	if (0 != (strBuf.size() % 8))
	{
		return "";
	}

	memcpy(&BlockKey, (void *)(strKey.c_str()), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs);
	memcpy(&BlockKey, (void *)(strKey.c_str() + 8), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs2);
	memcpy(&BlockKey, (void *)(strKey.c_str() + 16), 8);
	DES_set_key_unchecked((const_DES_cblock *)BlockKey, &DesKs3);

	for (int i = 0; i < (strBuf.size() / 8); i++)
	{
		memset(szOut, 0, sizeof(szOut));
		DES_ecb3_encrypt((DES_cblock *)(strBuf.c_str() + i * 8), (DES_cblock *)&szOut, &DesKs, &DesKs2, &DesKs3, DES_DECRYPT);
		strReturn += std::string ((char *)szOut, 8);
	}

	// ȥ�����
	nDel = strReturn[strReturn.size() - 1];
	strReturn = strReturn.substr(0, strReturn.size() - nDel);

	return strReturn;
}

BOOL NStr::CheckUserPass(const char * szcontent)
{
	BOOL result = false;
	BOOL bfindsz = FALSE;	//�Ƿ�����ַ�
	BOOL bfindnum = FALSE;	//�Ƿ��������
	BOOL bfinfch = FALSE;	//�Ƿ��������

	if ((strlen(szcontent) > 16) || strlen(szcontent) < 6)
		return result;//���Ȳ��ܳ���16���ַ�

	while (*szcontent)
	{
		if ((*szcontent) & 0x80){
			//�Ǻ���
			bfinfch = TRUE;
			szcontent++;//֪���Ǻ��ֵĻ�����һ���ֽڼ��
			break;
		}
		else if ((*szcontent >= 'a' && *szcontent <= 'z' || *szcontent >= 'A' && *szcontent <= 'Z'))
		{
			bfindsz = true;
		}
		else if (((*szcontent) >= '0' && (*szcontent) <= '9'))
		{
			bfindnum = true;
		}
		else{
			return false;
		}
		szcontent++;
	}

	if (bfinfch)
		return result;

	if (bfindsz && bfindnum)			// ��Ҫͬʱ�������ֺ���ĸ
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * ��������˵��������Ƿ��������
 * @param [in]  strData		�ַ���
 * @return TRUE ���� FALSE ������
 * by Maga 2016-12-2
 */
BOOL NStr::CheckHasChina(std::string  strData)
{
	for (int i = 0; i < strData.size(); i++)
	{
		if (0x80 & strData[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}

// �ļ���Сת��Ϊ�ַ���
std::string 	NStr::FileSizeToStr(__int64 ulSize)
{
	if (ulSize > 1024 * 1024 * 1024)
	{
		int nGb = (ulSize / (1024 * 1024 * 1024));
		int nMb = ((ulSize % (1024 * 1024 * 1024)) / (1000 * 1000 * 100));
		return NStr::FormatStr("%d.%01dG", nGb, nMb >= 9 ? 9 : nMb);
	}
	else if (ulSize > 1024 * 1024)
	{
		int nMb = ulSize / (1024 * 1024);
		int nKb = (ulSize % (1024 * 1024)) / (1000 * 100);
		return NStr::FormatStr("%d.%01dM", nMb, nKb >= 9 ? 9 : nKb);
	}
	else
	{
		int nKb = ulSize / 1024;
		int nb = ulSize % 1024 / 100;
		return NStr::FormatStr("%d.%01dKB", nKb, nb >= 9 ? 9 : nb);
	}
}

// �ļ���Сת��Ϊ�ַ���
std::wstring	NStr::FileSizeToWStr(__int64 ulSize)
{
	if (ulSize > 1024 * 1024 * 1024)
	{
		int nGb = (ulSize / (1024 * 1024 * 1024));
		int nMb = ((ulSize % (1024 * 1024 * 1024)) / (1000 * 1000 * 100));
		return NStr::FormatWStr(L"%d.%01dG", nGb, nMb >= 9 ? 9 : nMb);
	}
	else if(ulSize > 1024 * 1024)
	{
		int nMb = ulSize / (1024 * 1024);
		int nKb = (ulSize % (1024 * 1024)) / (1000 * 100);
		return NStr::FormatWStr(L"%d.%01dM", nMb, nKb >= 9 ? 9 : nKb);
	}
	else
	{
		int nKb = ulSize / 1024;
		int nb = ulSize % 1024 / 100;
		return NStr::FormatWStr(L"%d.%01dKB", nKb, nb >= 9 ? 9 : nb);
	}
}

// ����ת�ַ������Դ�дת��1��
std::string  NStr::IntToStrForCapital(int nValue)
{
	if (nValue > 999)
	{
		nValue = 999;
	}
	const char* sNum[] = { "��", "һ", "��", "��", "��", "��", "��", "��", "��", "��" };
	const char* sBit[] = { "", "ʮ", "��", "ǧ" };
	const char* sTep[] = { "", "��", "��", "��", "��", "��", "��" };

	int nHun = nValue / 10 / 10;
	int nTen = nValue / 10 % 10;
	int nUnit = nValue % 10;

	std::string  strOut = "";
	if (nHun != 0)
	{
		strOut = sNum[nHun];
		strOut += sBit[2];
	}

	if (nTen == 0)
	{
		strOut += nHun == 0 ? "" : sNum[0];
		strOut += sNum[nUnit];
	}
	else if (nTen == 1)
	{
		strOut += nHun == 0 ? "" : sNum[nTen];
		strOut += sBit[1];
		strOut += nUnit == 0 ? "" : sNum[nUnit];
	}
	else
	{
		strOut += sNum[nTen];
		strOut += sBit[1];
		strOut += nUnit == 0 ? "" : sNum[nUnit];
	}

	return strOut;
}

// ����ת�ַ������Դ�дת��1��
std::wstring NStr::IntToWStrForCapital(int nValue)
{
	if (nValue > 999)
	{
		nValue = 999;
	}
	const wchar_t* sNum[] = { L"��", L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��" };
	const wchar_t* sBit[] = { L"", L"ʮ", L"��", L"ǧ" };
	const wchar_t* sTep[] = { L"", L"��", L"��", L"��", L"��", L"��", L"��" };

	int nHun = nValue / 10 / 10;
	int nTen = nValue / 10 % 10;
	int nUnit = nValue % 10;

	std::wstring strOut = L"";
	if (nHun != 0)
	{
		strOut = sNum[nHun];
		strOut += sBit[2];
	}

	if (nTen == 0)
	{
		strOut += nHun == 0 ? L"" : sNum[0];
		strOut += sNum[nUnit];
	}
	else if (nTen == 1)
	{
		strOut += nHun == 0 ? L"" : sNum[nTen];
		strOut += sBit[1];
		strOut += nUnit == 0 ? L"" : sNum[nUnit];
	}
	else
	{
		strOut += sNum[nTen];
		strOut += sBit[1];
		strOut += nUnit == 0 ? L"" : sNum[nUnit];
	}

	return strOut;
}


//************************************
// Method:      VersionCompare
// Description: �汾�Ƚϣ������Ƿ����°汾�� a.b.c
// Returns:     bool
// Parameter:   std::string  strOldVer		�ɰ汾��
// Parameter:   std::string  strNewVer		�°汾��
// Parameter:   std::string  strSplite		�汾�ż������Ĭ��ʹ�õ�"."
// Author:	    JJHUANG
// Date:        2017-6-23
//************************************
bool NStr::VersionCompare(std::string  strOldVer, std::string  strNewVer, std::string  strSplite /* = "." */)
{
	if (strOldVer.empty() || strNewVer.empty())
		return false;

	bool bRet = false;
	 std::vector<std::string > vectOldVer;
	 std::vector<std::string > vectNewVer;
	NStr::SpliteStrByChar(strOldVer, vectOldVer, '.', true);
	NStr::SpliteStrByChar(strNewVer, vectNewVer, '.', true);

	if (vectOldVer.empty() || vectNewVer.empty() || vectOldVer.size() != vectNewVer.size())
		return false;

	int nOldVer = 0;
	int nNewVer = 0;
	//a.b.c -> a*100*100 + b*100 + c
	for (int i=0; i<vectOldVer.size(); ++i)
	{
		int nVer = NStr::StrToInt(vectOldVer[i]);
		nOldVer += nVer * pow(100, 2 - i);
	}
	for (int i=0; i<vectNewVer.size(); ++i)
	{
		int nVer = NStr::StrToInt(vectNewVer[i]);
		nNewVer += nVer * pow(100, 2 - i);
	}

	return nOldVer < nNewVer;
}

//////////////////////////////////////////////////////////////////////////
typedef struct h2p {
	char *py;
	char *py_shengdiao;
	unsigned shengdiao;
	char *hz;
} pyhz_tab;

pyhz_tab p2h[] = {
	{ "a", "a1", 1, "��������߹�" },
	{ "a", "a2", 2, "������" },
	{ "a", "a3", 3, "����" },
	{ "a", "a4", 4, "����" },
	{ "a", "a5", 5, "������" },
	{ "ai", "ai1", 1, "��������������" },
	{ "ai", "ai2", 2, "����������" },
	{ "ai", "ai3", 3, "����������" },
	{ "ai", "ai4", 4, "�����������������������" },
	{ "an", "an1", 1, "���ϰ��������ƹ㳧" },
	{ "an", "an3", 3, "�������" },
	{ "an", "an4", 4, "��������������" },
	{ "ang", "ang1", 1, "��" },
	{ "ang", "ang2", 2, "��" },
	{ "ang", "ang4", 4, "��" },
	{ "ao", "ao1", 1, "����" },
	{ "ao", "ao2", 2, "�����������������������" },
	{ "ao", "ao3", 3, "������" },
	{ "ao", "ao4", 4, "�°İ������������" },
	{ "ba", "ba1", 1, "�˰ɰͰȰŰǰ̰���ᱰ�" },
	{ "ba", "ba2", 2, "�˰ΰ�������" },
	{ "ba", "ba3", 3, "�Ѱ���" },
	{ "ba", "ba4", 4, "�Ѱְհ԰Ӱ����" },
	{ "ba", "ba5", 5, "�ɰ�" },
	{ "bai", "bai1", 1, "��" },
	{ "bai", "bai2", 2, "��" },
	{ "bai", "bai3", 3, "�ٰڲ��ذ���" },
	{ "bai", "bai4", 4, "�ܰ��°�" },
	{ "ban", "ban1", 1, "����߰����" },
	{ "ban", "ban3", 3, "�����������" },
	{ "ban", "ban4", 4, "��������" },
	{ "bang", "bang1", 1, "���亰�" },
	{ "bang", "bang3", 3, "����" },
	{ "bang", "bang4", 4, "���������������" },
	{ "bao", "bao1", 1, "�����ڰ�����������" },
	{ "bao", "bao2", 2, "����" },
	{ "bao", "bao3", 3, "�������������" },
	{ "bao", "bao4", 4, "���������������ٱ���" },
	{ "bei", "bei1", 1, "������������������" },
	{ "bei", "bei3", 3, "��" },
	{ "bei", "bei4", 4, "��������������㣱�����������������" },
	{ "bei", "bei5", 5, "����" },
	{ "ben", "ben1", 1, "�����" },
	{ "ben", "ben3", 3, "������" },
	{ "ben", "ben4", 4, "��������" },
	{ "beng", "beng1", 1, "������" },
	{ "beng", "beng2", 2, "��" },
	{ "beng", "beng3", 3, "��" },
	{ "beng", "beng4", 4, "���ı�괱ð�" },
	{ "bi", "bi1", 1, "��" },
	{ "bi", "bi2", 2, "��ݩ" },
	{ "bi", "bi3", 3, "�ȱʱ˱�ذ����������" },
	{ "bi", "bi4", 4, "�رϱ��رܱձڱ۱ױٱ̷��бα�赱��ڱ������㹱���������ر�����������������޵������������" },
	{ "bian", "bian1", 1, "��߱����������" },
	{ "bian", "bian3", 3, "����������" },
	{ "bian", "bian4", 4, "������������������" },
	{ "bian", "bian5", 5, "��" },
	{ "biao", "biao1", 1, "������ڱ������������" },
	{ "biao", "biao3", 3, "�����" },
	{ "biao", "biao4", 4, "��" },
	{ "bie", "bie1", 1, "����" },
	{ "bie", "bie2", 2, "����" },
	{ "bie", "bie3", 3, "��" },
	{ "bie", "bie4", 4, "��" },
	{ "bin", "bin1", 1, "��������ͱ�����������" },
	{ "bin", "bin4", 4, "���������" },
	{ "bing", "bing1", 1, "��������" },
	{ "bing", "bing3", 3, "����������������" },
	{ "bing", "bing4", 4, "������" },
	{ "bo", "bo1", 1, "�㲨���������Ĳ�����" },
	{ "bo", "bo2", 2, "�ٲ����������ز��ǲ��������粯���������������" },
	{ "bo", "bo3", 3, "����" },
	{ "bo", "bo4", 4, "�����������" },
	{ "bo", "bo5", 5, "���" },
	{ "bu", "bu1", 1, "������" },
	{ "bu", "bu2", 2, "����" },
	{ "bu", "bu3", 3, "����������߲" },
	{ "bu", "bu4", 4, "�������������Ҳ����" },
	{ "ca", "ca1", 1, "������" },
	{ "ca", "ca3", 3, "��" },
	{ "cai", "cai1", 1, "��" },
	{ "cai", "cai2", 2, "�ŲƲĲ�" },
	{ "cai", "cai3", 3, "�ɲʲȲ�" },
	{ "cai", "cai4", 4, "�ɲ˲�" },
	{ "can", "can1", 1, "�β���" },
	{ "can", "can2", 2, "�вѲ�" },
	{ "can", "can3", 3, "����" },
	{ "can", "can4", 4, "�ҲӲ������" },
	{ "cang", "cang1", 1, "�Բֲײ���" },
	{ "cang", "cang2", 2, "��" },
	{ "cao", "cao1", 1, "�ٲ�" },
	{ "cao", "cao2", 2, "�ܲ��������" },
	{ "cao", "cao3", 3, "��" },
	{ "ce", "ce4", 4, "�߲��޲���" },
	{ "cen", "cen1", 1, "��" },
	{ "cen", "cen2", 2, "��" },
	{ "ceng", "ceng1", 1, "��" },
	{ "ceng", "ceng2", 2, "����" },
	{ "ceng", "ceng4", 4, "��" },
	{ "cha", "cha1", 1, "��������������" },
	{ "cha", "cha2", 2, "��������⪲����" },
	{ "cha", "cha3", 3, "������" },
	{ "cha", "cha4", 4, "��ɲ����������" },
	{ "chai", "chai1", 1, "�����" },
	{ "chai", "chai2", 2, "���٭" },
	{ "chai", "chai4", 4, "���" },
	{ "chan", "chan1", 1, "�����" },
	{ "chan", "chan2", 2, "�������������濲����������" },
	{ "chan", "chan3", 3, "��������������" },
	{ "chan", "chan4", 4, "������" },
	{ "chang", "chang1", 1, "��潲���������" },
	{ "chang", "chang2", 2, "����������������������" },
	{ "chang", "chang3", 3, "����������" },
	{ "chang", "chang4", 4, "����������" },
	{ "chao", "chao1", 1, "���������½�����" },
	{ "chao", "chao2", 2, "����������" },
	{ "chao", "chao3", 3, "����" },
	{ "chao", "chao4", 4, "��" },
	{ "che", "che1", 1, "���" },
	{ "che", "che3", 3, "�߳�" },
	{ "che", "che4", 4, "����������" },
	{ "chen", "chen1", 1, "�������" },
	{ "chen", "chen2", 2, "�³����򳾳����ȳ����" },
	{ "chen", "chen3", 3, "��" },
	{ "chen", "chen4", 4, "�Ƴóĳ������" },
	{ "chen", "chen5", 5, "��" },
	{ "cheng", "cheng1", 1, "�Ƴų����������" },
	{ "cheng", "cheng2", 2, "�ɳǳ̳г�ʢ�˳ʳͳγ�ة������������" },
	{ "cheng", "cheng3", 3, "�ѳ���" },
	{ "cheng", "cheng4", 4, "�Ƴ�" },
	{ "chi", "chi1", 1, "�Գ����������������" },
	{ "chi", "chi2", 2, "�ֳٳس۳׳���ܯ������" },
	{ "chi", "chi3", 3, "�߳ݳܳ�����" },
	{ "chi", "chi4", 4, "����ള��߳�������" },
	{ "chong", "chong1", 1, "���ӿ���������" },
	{ "chong", "chong2", 2, "���س��" },
	{ "chong", "chong3", 3, "��" },
	{ "chong", "chong4", 4, "���" },
	{ "chou", "chou1", 1, "���" },
	{ "chou", "chou2", 2, "�������㰳����ٱ��" },
	{ "chou", "chou3", 3, "���" },
	{ "chou", "chou4", 4, "��" },
	{ "chu", "chu1", 1, "������" },
	{ "chu", "chu2", 2, "��������������ۻ����" },
	{ "chu", "chu3", 3, "��������������" },
	{ "chu", "chu4", 4, "�����������ء��" },
	{ "chuai", "chuai1", 1, "����" },
	{ "chuai", "chuai3", 3, "��" },
	{ "chuai", "chuai4", 4, "���������" },
	{ "chuan", "chuan1", 1, "�������" },
	{ "chuan", "chuan2", 2, "�����״���" },
	{ "chuan", "chuan3", 3, "���" },
	{ "chuan", "chuan4", 4, "����" },
	{ "chuang", "chuang1", 1, "������" },
	{ "chuang", "chuang2", 2, "����" },
	{ "chuang", "chuang3", 3, "��" },
	{ "chuang", "chuang4", 4, "����" },
	{ "chui", "chui1", 1, "����" },
	{ "chui", "chui2", 2, "��������׵��" },
	{ "chun", "chun1", 1, "������" },
	{ "chun", "chun2", 2, "����������ݻ" },
	{ "chun", "chun3", 3, "��" },
	{ "chuo", "chuo1", 1, "����" },
	{ "chuo", "chuo4", 4, "������" },
	{ "ci", "ci1", 1, "��̴���" },
	{ "ci", "ci2", 2, "�ʴǴȴŴ��ȴĴ���������" },
	{ "ci", "ci3", 3, "��" },
	{ "ci", "ci4", 4, "�δ̴���" },
	{ "cong", "cong1", 1, "�ӴҴϴд��������" },
	{ "cong", "cong2", 2, "�Ӵ�����" },
	{ "cou", "cou4", 4, "������" },
	{ "cu", "cu1", 1, "��" },
	{ "cu", "cu2", 2, "����" },
	{ "cu", "cu4", 4, "�ٴش������������" },
	{ "cuan", "cuan1", 1, "��ߥ����" },
	{ "cuan", "cuan2", 2, "��" },
	{ "cuan", "cuan4", 4, "�ܴ���" },
	{ "cui", "cui1", 1, "˥�ߴݴ�����" },
	{ "cui", "cui3", 3, "�" },
	{ "cui", "cui4", 4, "����ʹ��㲴����" },
	{ "cun", "cun1", 1, "����" },
	{ "cun", "cun2", 2, "���" },
	{ "cun", "cun3", 3, "��" },
	{ "cun", "cun4", 4, "��" },
	{ "cuo", "cuo1", 1, "������" },
	{ "cuo", "cuo2", 2, "����������" },
	{ "cuo", "cuo3", 3, "����" },
	{ "cuo", "cuo4", 4, "�������" },
	{ "da", "da1", 1, "����������" },
	{ "da", "da2", 2, "���������������" },
	{ "da", "da3", 3, "��" },
	{ "da", "da4", 4, "��" },
	{ "da", "da5", 5, "����" },
	{ "dai", "dai1", 1, "����߾" },
	{ "dai", "dai3", 3, "������" },
	{ "dai", "dai4", 4, "������������������쵡��������ܤ߰" },
	{ "dan", "dan1", 1, "����������������������" },
	{ "dan", "dan3", 3, "������������" },
	{ "dan", "dan4", 4, "����ʯ�������������壵�����" },
	{ "dang", "dang1", 1, "������" },
	{ "dang", "dang3", 3, "������" },
	{ "dang", "dang4", 4, "��������������" },
	{ "dao", "dao1", 1, "��߶���" },
	{ "dao", "dao2", 2, "߶" },
	{ "dao", "dao3", 3, "������������" },
	{ "dao", "dao4", 4, "������������������" },
	{ "de", "de2", 2, "�õ��" },
	{ "de", "de5", 5, "�ĵصõ�" },
	{ "dei", "dei3", 3, "��" },
	{ "deng", "deng1", 1, "�ǵƵ�����" },
	{ "deng", "deng3", 3, "���" },
	{ "deng", "deng4", 4, "�˵ʵɳε�������" },
	{ "di", "di1", 1, "��͵ε���ص����" },
	{ "di", "di2", 2, "�ĵеϵѵ��ֵҵյ�ݶ������" },
	{ "di", "di3", 3, "�׵�ڮۡ������ص��" },
	{ "di", "di4", 4, "�ĵصڵ۵ܵݵٵ����������" },
	{ "dia", "dia3", 3, "��" },
	{ "dian", "dian1", 1, "�ߵ�����" },
	{ "dian", "dian3", 3, "������ؼ" },
	{ "dian", "dian4", 4, "����������������������" },
	{ "diao", "diao1", 1, "�����������" },
	{ "diao", "diao3", 3, "��" },
	{ "diao", "diao4", 4, "�����������" },
	{ "die", "die1", 1, "������" },
	{ "die", "die2", 2, "������������������ܦ�������" },
	{ "ding", "ding1", 1, "�������������������" },
	{ "ding", "ding3", 3, "������" },
	{ "ding", "ding4", 4, "���������붧���" },
	{ "diu", "diu1", 1, "����" },
	{ "dong", "dong1", 1, "���������" },
	{ "dong", "dong3", 3, "������" },
	{ "dong", "dong4", 4, "���������������������" },
	{ "dou", "dou1", 1, "��������" },
	{ "dou", "dou3", 3, "�������" },
	{ "dou", "dou4", 4, "��������񼶻" },
	{ "du", "du1", 1, "�����" },
	{ "du", "du2", 2, "�����ٶ���빶�������" },
	{ "du", "du3", 3, "�Ƕö¶���" },
	{ "du", "du4", 4, "�ȶɶǶŶʶ�ܶ�" },
	{ "duan", "duan1", 1, "��" },
	{ "duan", "duan3", 3, "��" },
	{ "duan", "duan4", 4, "�϶ζͶ������" },
	{ "dui", "dui1", 1, "��" },
	{ "dui", "dui4", 4, "�ԶӶҶ�������" },
	{ "dun", "dun1", 1, "�ֶض׶�����" },
	{ "dun", "dun3", 3, "����" },
	{ "dun", "dun4", 4, "�ٶܶ���������" },
	{ "duo", "duo1", 1, "���Ͷ߶���" },
	{ "duo", "duo2", 2, "�ȶ�����" },
	{ "duo", "duo3", 3, "�������" },
	{ "duo", "duo4", 4, "�������������" },
	{ "e", "e1", 1, "�����" },
	{ "e", "e2", 2, "���Ŷ������ݭ�" },
	{ "e", "e3", 3, "��" },
	{ "e", "e4", 4, "�����㵶�ج��������������������������" },
	{ "e", "e5", 5, "��" },
	{ "ei", "ei2", 2, "��" },
	{ "ei", "ei3", 3, "��" },
	{ "ei", "ei4", 4, "��" },
	{ "en", "en1", 1, "����" },
	{ "en", "en4", 4, "��" },
	{ "er", "er2", 2, "�������" },
	{ "er", "er3", 3, "�����Ƕ�������" },
	{ "er", "er4", 4, "����٦" },
	{ "fa", "fa1", 1, "��" },
	{ "fa", "fa2", 2, "������������" },
	{ "fa", "fa3", 3, "����" },
	{ "fa", "fa4", 4, "����" },
	{ "fan", "fan1", 1, "���������ެ" },
	{ "fan", "fan2", 2, "����������ެ�ܷ�������" },
	{ "fan", "fan3", 3, "����" },
	{ "fan", "fan4", 4, "�������������" },
	{ "fang", "fang1", 1, "��������������" },
	{ "fang", "fang2", 2, "������������" },
	{ "fang", "fang3", 3, "�÷·�����" },
	{ "fang", "fang4", 4, "��" },
	{ "fei", "fei1", 1, "�Ƿɷȷ������������" },
	{ "fei", "fei2", 2, "������" },
	{ "fei", "fei3", 3, "�Ʒ˷����������" },
	{ "fei", "fei4", 4, "�ѷϷзη���������" },
	{ "fen", "fen1", 1, "�ַ׷շҷԷ���" },
	{ "fen", "fen2", 2, "�طٷ�����" },
	{ "fen", "fen3", 3, "��" },
	{ "fen", "fen4", 4, "�ַݷܷ߷�������" },
	{ "feng", "feng1", 1, "����������ۺ�����" },
	{ "feng", "feng2", 2, "����" },
	{ "feng", "feng3", 3, "����" },
	{ "feng", "feng4", 4, "����ٺ��" },
	{ "fo", "fo2", 2, "��" },
	{ "fou", "fou3", 3, "���" },
	{ "fu", "fu1", 1, "������߻��������" },
	{ "fu", "fu2", 2, "������������������������ܽ�����������������ݳ�������ۮ������������������" },
	{ "fu", "fu3", 3, "����������������������������" },
	{ "fu", "fu4", 4, "�����������������������������������������������" },
	{ "fu", "fu5", 5, "��" },
	{ "ga", "ga1", 1, "�п��¸�٤�" },
	{ "ga", "ga2", 2, "�¸�������" },
	{ "ga", "ga3", 3, "����" },
	{ "ga", "ga4", 4, "��" },
	{ "gai", "gai1", 1, "��������" },
	{ "gai", "gai3", 3, "��" },
	{ "gai", "gai4", 4, "�Ÿ�ؤ�ƽ���" },
	{ "gan", "gan1", 1, "�ɸʸθ���Ǭ�����ո��������" },
	{ "gan", "gan3", 3, "�иҸϸ��ϸ�ߦ��" },
	{ "gan", "gan4", 4, "�ɸ�����" },
	{ "gang", "gang1", 1, "�ոָٸ׿��ܸԸ��" },
	{ "gang", "gang3", 3, "�۸�" },
	{ "gang", "gang4", 4, "�ָ����" },
	{ "gao", "gao1", 1, "�߸��޸�غ����" },
	{ "gao", "gao3", 3, "���޻���ɸ��" },
	{ "gao", "gao4", 4, "���ھ۬�" },
	{ "ge", "ge1", 1, "���������������������" },
	{ "ge", "ge2", 2, "��������������������ܪت" },
	{ "ge", "ge3", 3, "�����ϸǸ�����" },
	{ "ge", "ge4", 4, "���������" },
	{ "gei", "gei3", 3, "��" },
	{ "gen", "gen1", 1, "����" },
	{ "gen", "gen2", 2, "��" },
	{ "gen", "gen3", 3, "��" },
	{ "gen", "gen4", 4, "ب��ݢ" },
	{ "geng", "geng1", 1, "����������" },
	{ "geng", "geng3", 3, "���������ṡ�" },
	{ "geng", "geng4", 4, "��" },
	{ "gong", "gong1", 1, "�������칩�������������������" },
	{ "gong", "gong3", 3, "��������" },
	{ "gong", "gong4", 4, "������" },
	{ "gou", "gou1", 1, "�乵��������������" },
	{ "gou", "gou3", 3, "���������" },
	{ "gou", "gou4", 4, "����������ڸ�����" },
	{ "gu", "gu1", 1, "�ùǹ¹������ɹ�����������������" },
	{ "gu", "gu2", 2, "��" },
	{ "gu", "gu3", 3, "�ŹɹĹǹȼ�����������ڬ����������" },
	{ "gu", "gu4", 4, "�ʹ˹̹���������������" },
	{ "gua", "gua1", 1, "���Ϲ��������" },
	{ "gua", "gua3", 3, "���ɹ�" },
	{ "gua", "gua4", 4, "�ҹ���ڴ" },
	{ "guai", "guai1", 1, "����" },
	{ "guai", "guai3", 3, "��" },
	{ "guai", "guai4", 4, "��" },
	{ "guan", "guan1", 1, "�ع۹ٹڹ���ݸ������" },
	{ "guan", "guan3", 3, "�ܹ�ݸ" },
	{ "guan", "guan4", 4, "�۹߹ڹ�޹���������" },
	{ "guang", "guang1", 1, "��������" },
	{ "guang", "guang3", 3, "����" },
	{ "guang", "guang4", 4, "����" },
	{ "gui", "gui1", 1, "��������������" },
	{ "gui", "gui3", 3, "��������������" },
	{ "gui", "gui4", 4, "������Ȳ��������" },
	{ "gun", "gun3", 3, "��������޹�" },
	{ "gun", "gun4", 4, "��" },
	{ "guo", "guo1", 1, "������������������" },
	{ "guo", "guo2", 2, "���������" },
	{ "guo", "guo3", 3, "��������" },
	{ "guo", "guo4", 4, "��" },
	{ "ha", "ha1", 1, "����" },
	{ "ha", "ha2", 2, "Ϻ��" },
	{ "ha", "ha3", 3, "��" },
	{ "ha", "ha4", 4, "��" },
	{ "hai", "hai1", 1, "�ٿ���" },
	{ "hai", "hai2", 2, "������" },
	{ "hai", "hai3", 3, "������" },
	{ "hai", "hai4", 4, "��������" },
	{ "han", "han1", 1, "����������" },
	{ "han", "han2", 2, "���������������ʺ���" },
	{ "han", "han3", 3, "������" },
	{ "han", "han4", 4, "����������������嫺������" },
	{ "hang", "hang1", 1, "��" },
	{ "hang", "hang2", 2, "�к��Ժ������" },
	{ "hang", "hang4", 4, "������" },
	{ "hao", "hao1", 1, "��޶��" },
	{ "hao", "hao2", 2, "�ź�������������" },
	{ "hao", "hao3", 3, "�ú�" },
	{ "hao", "hao4", 4, "�úźƺ���尸��" },
	{ "he", "he1", 1, "�Ⱥ�ڭ��" },
	{ "he", "he2", 2, "�ͺκϺӺ˺к̺ɺҺ��غ����������������" },
	{ "he", "he4", 4, "�ͺκȺ��źغɺ��ֺ�" },
	{ "hei", "hei1", 1, "�ں���" },
	{ "hen", "hen2", 2, "��" },
	{ "hen", "hen3", 3, "�ܺ�" },
	{ "hen", "hen4", 4, "��" },
	{ "heng", "heng1", 1, "�ߺ�" },
	{ "heng", "heng2", 2, "�к���޿����" },
	{ "heng", "heng4", 4, "��" },
	{ "hng", "hng5", 5, "��" },
	{ "hong", "hong1", 1, "����ް��" },
	{ "hong", "hong2", 2, "�����������ޮ��ݦ" },
	{ "hong", "hong3", 3, "��" },
	{ "hong", "hong4", 4, "��ڧޮ" },
	{ "hou", "hou2", 2, "������������" },
	{ "hou", "hou3", 3, "��" },
	{ "hou", "hou4", 4, "���������ܩ��" },
	{ "hu", "hu1", 1, "����Ϸ�������������" },
	{ "hu", "hu2", 2, "�ͺ������˺����������������������������" },
	{ "hu", "hu3", 3, "��䰻���" },
	{ "hu", "hu4", 4, "����������������������������" },
	{ "hua", "hua1", 1, "���������" },
	{ "hua", "hua2", 2, "��������������" },
	{ "hua", "hua4", 4, "������������" },
	{ "huai", "huai2", 2, "����������" },
	{ "huai", "huai4", 4, "��" },
	{ "huai", "huai5", 5, "��" },
	{ "huan", "huan1", 1, "���" },
	{ "huan", "huan2", 2, "������߻����ۨ������" },
	{ "huan", "huan3", 3, "��" },
	{ "huan", "huan4", 4, "�����û��»������ۼߧ��������" },
	{ "huang", "huang1", 1, "�Ļ���" },
	{ "huang", "huang2", 2, "�ƻʻͻ���諻ɻ�����������������" },
	{ "huang", "huang3", 3, "�λлѻ�" },
	{ "huang", "huang4", 4, "��" },
	{ "hui", "hui1", 1, "�ӻԻһֻն�ڶ�����������" },
	{ "hui", "hui2", 2, "�ػ������" },
	{ "hui", "hui3", 3, "�ٻ��" },
	{ "hui", "hui4", 4, "���ݻ������߻޻������������������ޥ�" },
	{ "hun", "hun1", 1, "������" },
	{ "hun", "hun2", 2, "��������" },
	{ "hun", "hun4", 4, "��ڻ��" },
	{ "huo", "huo1", 1, "����߫����" },
	{ "huo", "huo2", 2, "�ͻ�" },
	{ "huo", "huo3", 3, "������" },
	{ "huo", "huo4", 4, "�ͻ����������޽�����" },
	{ "ji", "ji1", 1, "�伸�ڻ������漤��������������������ߴ�������������촻�������������ܸآ������" },
	{ "ji", "ji2", 2, "�Ｐ���������������������弹��������ڵؽ�����٥���ު" },
	{ "ji", "ji3", 3, "��������üͼ���������" },
	{ "ji", "ji4", 4, "��ϵ�Ƽüļʼ��ͼ̼��뼾�ż��ɼ����������¼�������������������������" },
	{ "jia", "jia1", 1, "�ҼӼѼм���Ю������٤�������������" },
	{ "jia", "jia2", 2, "�м�ꩼ�ۣ�������" },
	{ "jia", "jia3", 3, "�ٽǽż׽��ֽɽʽȽ�ٮ�ƽ˽���ν��������븼�����" },
	{ "jia", "jia4", 4, "�ۼټܼݼ޼�" },
	{ "jia", "jia5", 5, "��" },
	{ "jian", "jian1", 1, "���ཥ����ǳ��齦��߼���������������������" },
	{ "jian", "jian3", 3, "������������������������������������" },
	{ "jian", "jian4", 4, "��������ཥ�������������������������Խ������������" },
	{ "jiang", "jiang1", 1, "��������������������" },
	{ "jiang", "jiang3", 3, "����������" },
	{ "jiang", "jiang4", 4, "��ǿ�������罳������" },
	{ "jiao", "jiao1", 1, "�̽�����������������ӽ�����������ܴ��" },
	{ "jiao", "jiao2", 2, "�����" },
	{ "jiao", "jiao3", 3, "�ǽŽ��ɽʽȽ�ٮ�ƽ˽��������븽�" },
	{ "jiao", "jiao4", 4, "�̾�У�нϽν��ѽ��������" },
	{ "jie", "jie1", 1, "�ӽ�ڽֽ׽Խҿ�ൽ����" },
	{ "jie", "jie2", 2, "��ڽܽݽؽ�ٽ߽޽�����ڵ�������ڦ�����" },
	{ "jie", "jie3", 3, "���" },
	{ "jie", "jie4", 4, "���۽�����������" },
	{ "jie", "jie5", 5, "�Ҽ�" },
	{ "jin", "jin1", 1, "��������������" },
	{ "jin", "jin3", 3, "���������������������" },
	{ "jin", "jin4", 4, "�������������������ƽ�����ݣ���" },
	{ "jing", "jing1", 1, "��������������������캾���ݼ��" },
	{ "jing", "jing3", 3, "�����������������" },
	{ "jing", "jing4", 4, "����������������������������������" },
	{ "jiong", "jiong1", 1, "��" },
	{ "jiong", "jiong3", 3, "��������" },
	{ "jiu", "jiu1", 1, "��������������" },
	{ "jiu", "jiu3", 3, "�žƾþ¾ľ�" },
	{ "jiu", "jiu4", 4, "�;ɾȾξ˾̾����־�����" },
	{ "ju", "ju1", 1, "�����ҾӾ�оԾϾ�����������︾������" },
	{ "ju", "ju2", 2, "�־ս����" },
	{ "ju", "ju3", 3, "�ٹ�ؾ׾����������" },
	{ "ju", "ju4", 4, "�ݾ�߾�޾۾ܾ�����ľ����쫾��������������ڪ����" },
	{ "juan", "juan1", 1, "��Ȧ��������" },
	{ "juan", "juan3", 3, "����" },
	{ "juan", "juan4", 4, "Ȧ������������۲" },
	{ "jue", "jue1", 1, "ൾ���" },
	{ "jue", "jue2", 2, "�������ǽŽ�����Ⱦ������������������������������ާ��" },
	{ "jue", "jue3", 3, "��" },
	{ "jue", "jue4", 4, "��" },
	{ "jun", "jun1", 1, "���������޹������" },
	{ "jun", "jun4", 4, "���������������ܿ�" },
	{ "ka", "ka1", 1, "������" },
	{ "ka", "ka3", 3, "����������" },
	{ "kai", "kai1", 1, "�����" },
	{ "kai", "kai3", 3, "������������������" },
	{ "kai", "kai4", 4, "��" },
	{ "kan", "kan1", 1, "�����������" },
	{ "kan", "kan3", 3, "٩��������ݨ" },
	{ "kan", "kan4", 4, "��Ƕ���" },
	{ "kang", "kang1", 1, "��������" },
	{ "kang", "kang2", 2, "��" },
	{ "kang", "kang4", 4, "������������" },
	{ "kao", "kao1", 1, "��" },
	{ "kao", "kao3", 3, "��������" },
	{ "kao", "kao4", 4, "������" },
	{ "ke", "ke1", 1, "�ƿſºǿÿ��Ŀ�������������������" },
	{ "ke", "ke2", 2, "�ȿ��" },
	{ "ke", "ke3", 3, "�ɿʿ����" },
	{ "ke", "ke4", 4, "�ɿ˿Ϳ̿������������" },
	{ "ken", "ken3", 3, "�Ͽҿп���" },
	{ "ken", "ken4", 4, "��" },
	{ "keng", "keng1", 1, "�ӿ��" },
	{ "kong", "kong1", 1, "��������" },
	{ "kong", "kong3", 3, "�ֿ���" },
	{ "kong", "kong4", 4, "�տ�" },
	{ "kou", "kou1", 1, "������" },
	{ "kou", "kou3", 3, "��" },
	{ "kou", "kou4", 4, "�ۿ�ߵޢ��" },
	{ "ku", "ku1", 1, "�޿ݿ�����ܥ" },
	{ "ku", "ku3", 3, "��" },
	{ "ku", "ku4", 4, "������" },
	{ "kua", "kua1", 1, "��" },
	{ "kua", "kua3", 3, "��٨" },
	{ "kua", "kua4", 4, "����" },
	{ "kuai", "kuai3", 3, "��" },
	{ "kuai", "kuai4", 4, "�������������ۦ" },
	{ "kuan", "kuan1", 1, "����" },
	{ "kuan", "kuan3", 3, "��" },
	{ "kuang", "kuang1", 1, "������ڲ" },
	{ "kuang", "kuang2", 2, "��ڿ" },
	{ "kuang", "kuang3", 3, "��" },
	{ "kuang", "kuang4", 4, "����������������" },
	{ "kui", "kui1", 1, "���������" },
	{ "kui", "kui2", 2, "����ӿ���ظ�����������" },
	{ "kui", "kui3", 3, "����" },
	{ "kui", "kui4", 4, "����������������" },
	{ "kun", "kun1", 1, "�������������" },
	{ "kun", "kun3", 3, "�����" },
	{ "kun", "kun4", 4, "��" },
	{ "kuo", "kuo4", 4, "��������������" },
	{ "la", "la1", 1, "����������" },
	{ "la", "la2", 2, "�������" },
	{ "la", "la3", 3, "����" },
	{ "la", "la4", 4, "��������������" },
	{ "la", "la5", 5, "����" },
	{ "lai", "lai2", 2, "����������" },
	{ "lai", "lai4", 4, "�����������" },
	{ "lan", "lan2", 2, "������������������������" },
	{ "lan", "lan3", 3, "������������" },
	{ "lan", "lan4", 4, "����" },
	{ "lang", "lang1", 1, "�" },
	{ "lang", "lang2", 2, "�����������������" },
	{ "lang", "lang3", 3, "��" },
	{ "lang", "lang4", 4, "����ݹ����" },
	{ "lao", "lao1", 1, "��" },
	{ "lao", "lao2", 2, "�������������" },
	{ "lao", "lao3", 3, "������������" },
	{ "lao", "lao4", 4, "��������������" },
	{ "le", "le1", 1, "��" },
	{ "le", "le4", 4, "������߷����" },
	{ "le", "le5", 5, "��" },
	{ "lei", "lei1", 1, "����" },
	{ "lei", "lei2", 2, "����������������" },
	{ "lei", "lei3", 3, "����������ڳ��" },
	{ "lei", "lei4", 4, "������������" },
	{ "lei", "lei5", 5, "��" },
	{ "leng", "leng1", 1, "��" },
	{ "leng", "leng2", 2, "����ܨ" },
	{ "leng", "leng3", 3, "��" },
	{ "leng", "leng4", 4, "�" },
	{ "li", "li1", 1, "��" },
	{ "li", "li2", 2, "���������������������޼�����������������" },
	{ "li", "li3", 3, "������������ٵ�����������" },
	{ "li", "li4", 4, "����������������������������������ݰ��ٳ����۪�����߿�����������������������ت������޺" },
	{ "li", "li5", 5, "����" },
	{ "lia", "lia3", 3, "��" },
	{ "lian", "lian2", 2, "�������������������������" },
	{ "lian", "lian3", 3, "����������" },
	{ "lian", "lian4", 4, "�������������" },
	{ "liang", "liang2", 2, "��������������ݹ�ܮ" },
	{ "liang", "liang3", 3, "������" },
	{ "liang", "liang4", 4, "����������������" },
	{ "liao", "liao1", 1, "����" },
	{ "liao", "liao2", 2, "������������������������" },
	{ "liao", "liao3", 3, "������ޤ��" },
	{ "liao", "liao4", 4, "����������������" },
	{ "lie", "lie1", 1, "��" },
	{ "lie", "lie3", 3, "����" },
	{ "lie", "lie4", 4, "�����������������������" },
	{ "lie", "lie5", 5, "��" },
	{ "lin", "lin2", 2, "������������������������������" },
	{ "lin", "lin3", 3, "��������" },
	{ "lin", "lin4", 4, "�����������" },
	{ "ling", "ling1", 1, "��" },
	{ "ling", "ling2", 2, "����������������������������������۹����������" },
	{ "ling", "ling3", 3, "������" },
	{ "ling", "ling4", 4, "������" },
	{ "liu", "liu1", 1, "����" },
	{ "liu", "liu2", 2, "��������������������������" },
	{ "liu", "liu3", 3, "����" },
	{ "liu", "liu4", 4, "��½��µ��������" },
	{ "lo", "lo5", 5, "��" },
	{ "long", "long1", 1, "¡" },
	{ "long", "long2", 2, "��¡���������������������" },
	{ "long", "long3", 3, "��£¢¤��" },
	{ "long", "long4", 4, "Ū" },
	{ "lou", "lou1", 1, "§" },
	{ "lou", "lou2", 2, "¥���¦��������" },
	{ "lou", "lou3", 3, "§¨��" },
	{ "lou", "lou4", 4, "¶ª©����" },
	{ "lou", "lou5", 5, "�" },
	{ "lu", "lu1", 1, "��ߣ" },
	{ "lu", "lu2", 2, "¬¯®«­���������������" },
	{ "lu", "lu3", 3, "³«±²°����" },
	{ "lu", "lu4", 4, "��·½¼¶��¹µ»�´¸��¾����º�����ޤ��" },
	{ "lu", "lu5", 5, "���" },
	{ "lv", "lv3", 3, "������������������������" },
	{ "lv", "lv4", 4, "������������" },
	{ "lv", "lv5", 5, "¿���" },
	{ "luan", "luan2", 2, "����������������" },
	{ "luan", "luan3", 3, "��" },
	{ "luan", "luan4", 4, "��" },
	{ "lue", "lue3", 3, "��" },
	{ "lue", "lue4", 4, "�����" },
	{ "lun", "lun1", 1, "��" },
	{ "lun", "lun2", 2, "����������������" },
	{ "lun", "lun4", 4, "��" },
	{ "luo", "luo1", 1, "������" },
	{ "luo", "luo2", 2, "��������������������" },
	{ "luo", "luo3", 3, "�������" },
	{ "luo", "luo4", 4, "�������濩����������������" },
	{ "luo", "luo5", 5, "��" },
	{ "m", "m2", 2, "߼" },
	{ "ma", "ma1", 1, "����ĦĨ����" },
	{ "ma", "ma2", 2, "�����" },
	{ "ma", "ma3", 3, "������������" },
	{ "ma", "ma4", 4, "�������" },
	{ "ma", "ma5", 5, "ô����" },
	{ "mai", "mai2", 2, "����" },
	{ "mai", "mai3", 3, "��ݤ" },
	{ "mai", "mai4", 4, "��������۽" },
	{ "man", "man1", 1, "�" },
	{ "man", "man2", 2, "����������á����" },
	{ "man", "man3", 3, "����" },
	{ "man", "man4", 4, "��������áܬ�������" },
	{ "mang", "mang2", 2, "æãäâå����" },
	{ "mang", "mang3", 3, "ç����" },
	{ "mao", "mao1", 1, "è" },
	{ "mao", "mao2", 2, "ëèìé��ê��������" },
	{ "mao", "mao3", 3, "îí�����" },
	{ "mao", "mao4", 4, "ðóñòï������" },
	{ "me", "me5", 5, "ô��" },
	{ "mei", "mei2", 2, "ûü÷ýöúùõ��øݮ����������" },
	{ "mei", "mei3", 3, "��ÿþ�" },
	{ "mei", "mei4", 4, "��������������" },
	{ "men", "men1", 1, "��" },
	{ "men", "men2", 2, "������" },
	{ "men", "men4", 4, "�����" },
	{ "men", "men5", 5, "��" },
	{ "meng", "meng1", 1, "��" },
	{ "meng", "meng2", 2, "������å����ޫ�������" },
	{ "meng", "meng3", 3, "���������������" },
	{ "meng", "meng4", 4, "����" },
	{ "mi", "mi1", 1, "����" },
	{ "mi", "mi2", 2, "���������������������" },
	{ "mi", "mi3", 3, "��������������" },
	{ "mi", "mi4", 4, "���������������������" },
	{ "mian", "mian2", 2, "������" },
	{ "mian", "mian3", 3, "����������������������" },
	{ "mian", "mian4", 4, "��" },
	{ "miao", "miao1", 1, "��" },
	{ "miao", "miao2", 2, "��������" },
	{ "miao", "miao3", 3, "��������������" },
	{ "miao", "miao4", 4, "������" },
	{ "mie", "mie1", 1, "ؿ��" },
	{ "mie", "mie4", 4, "�������" },
	{ "min", "min2", 2, "�����������" },
	{ "min", "min3", 3, "�������������������" },
	{ "ming", "ming2", 2, "����������ڤ���������" },
	{ "ming", "ming3", 3, "��" },
	{ "ming", "ming4", 4, "��" },
	{ "miu", "miu4", 4, "����" },
	{ "mo", "mo1", 1, "��" },
	{ "mo", "mo2", 2, "��ģ��ĥ��ĦħĤĢ��ġ����" },
	{ "mo", "mo3", 3, "Ĩ" },
	{ "mo", "mo4", 4, "û��ĬĪĩðĥįĮīĨİ����ĭ���Ժ��������������" },
	{ "mou", "mou1", 1, "��" },
	{ "mou", "mou2", 2, "ıĲ��������ٰ" },
	{ "mou", "mou3", 3, "ĳ" },
	{ "mu", "mu2", 2, "ģ�" },
	{ "mu", "mu3", 3, "ĸķ��ĶĴĵ" },
	{ "mu", "mu4", 4, "ĿľĻĽ��ĹļĺĲ��������������" },
	{ "n", "n2", 2, "����" },
	{ "n", "n3", 3, "����" },
	{ "n", "n4", 4, "��" },
	{ "na", "na1", 1, "����" },
	{ "na", "na2", 2, "����" },
	{ "na", "na3", 3, "����" },
	{ "na", "na4", 4, "������������������" },
	{ "na", "na5", 5, "����" },
	{ "nai", "nai3", 3, "��������ܵ" },
	{ "nai", "nai4", 4, "����ؾ٦����" },
	{ "nan", "nan1", 1, "����" },
	{ "nan", "nan2", 2, "��������" },
	{ "nan", "nan3", 3, "������" },
	{ "nan", "nan4", 4, "��" },
	{ "nang", "nang1", 1, "����" },
	{ "nang", "nang2", 2, "����" },
	{ "nang", "nang3", 3, "��߭��" },
	{ "nao", "nao1", 1, "ث" },
	{ "nao", "nao2", 2, "Ŭ�����������" },
	{ "nao", "nao3", 3, "�������" },
	{ "nao", "nao4", 4, "����" },
	{ "ne", "ne2", 2, "��" },
	{ "ne", "ne4", 4, "����ګ" },
	{ "ne", "ne5", 5, "����" },
	{ "nei", "nei3", 3, "����" },
	{ "nei", "nei4", 4, "����" },
	{ "nen", "nen4", 4, "���" },
	{ "neng", "neng2", 2, "��" },
	{ "ng", "ng2", 2, "����" },
	{ "ng", "ng3", 3, "����" },
	{ "ng", "ng4", 4, "��" },
	{ "ni", "ni1", 1, "��" },
	{ "ni", "ni2", 2, "�������������������" },
	{ "ni", "ni3", 3, "�������" },
	{ "ni", "ni4", 4, "�����������������٣" },
	{ "nian", "nian1", 1, "����" },
	{ "nian", "nian2", 2, "��ճ�����" },
	{ "nian", "nian3", 3, "��������" },
	{ "nian", "nian4", 4, "��إ����" },
	{ "niang", "niang2", 2, "����" },
	{ "niang", "niang4", 4, "��" },
	{ "niao", "niao3", 3, "��������" },
	{ "niao", "niao4", 4, "������" },
	{ "nie", "nie1", 1, "��" },
	{ "nie", "nie4", 4, "���������������ؿ�������" },
	{ "nin", "nin2", 2, "���" },
	{ "ning", "ning2", 2, "����š�����������" },
	{ "ning", "ning3", 3, "š" },
	{ "ning", "ning4", 4, "��šŢ��" },
	{ "niu", "niu1", 1, "�" },
	{ "niu", "niu2", 2, "ţ" },
	{ "niu", "niu3", 3, "ŦŤť����" },
	{ "niu", "niu4", 4, "��" },
	{ "nong", "nong2", 2, "ũŨٯ��ŧ" },
	{ "nong", "nong4", 4, "Ū" },
	{ "nou", "nou4", 4, "��" },
	{ "nu", "nu2", 2, "ū����" },
	{ "nu", "nu3", 3, "Ŭ����" },
	{ "nu", "nu4", 4, "ŭ" },
	{ "nv", "nv3", 3, "Ů��" },
	{ "nv", "nv4", 4, "���" },
	{ "nuan", "nuan3", 3, "ů" },
	{ "nue", "nue4", 4, "Űű" },
	{ "nuo", "nuo2", 2, "��Ų��" },
	{ "nuo", "nuo4", 4, "ŵųŴ�����" },
	{ "O", "o1", 1, "���" },
	{ "O", "o2", 2, "Ŷ" },
	{ "O", "o4", 4, "Ŷ" },
	{ "Ou", "ou1", 1, "��ŷŹŸک�Ž" },
	{ "Ou", "ou3", 3, "żŻź��" },
	{ "Ou", "ou4", 4, "ŻŽ��" },
	{ "pa", "pa1", 1, "�ɰ�ſž��" },
	{ "pa", "pa2", 2, "���ǰ�������" },
	{ "pa", "pa4", 4, "����" },
	{ "pa", "pa5", 5, "��" },
	{ "pai", "pai1", 1, "��" },
	{ "pai", "pai2", 2, "������ٽ" },
	{ "pai", "pai3", 3, "����" },
	{ "pai", "pai4", 4, "��������" },
	{ "pan", "pan1", 1, "�����˰�" },
	{ "pan", "pan2", 2, "�������������" },
	{ "pan", "pan4", 4, "����������������" },
	{ "pang", "pang1", 1, "�Ұ���" },
	{ "pang", "pang2", 2, "���Ӱ�������" },
	{ "pang", "pang3", 3, "��" },
	{ "pang", "pang4", 4, "��" },
	{ "pao", "pao1", 1, "��������" },
	{ "pao", "pao2", 2, "����������������" },
	{ "pao", "pao3", 3, "��" },
	{ "pao", "pao4", 4, "������" },
	{ "pei", "pei1", 1, "������" },
	{ "pei", "pei2", 2, "����������" },
	{ "pei", "pei4", 4, "�������������" },
	{ "pen", "pen1", 1, "��" },
	{ "pen", "pen2", 2, "����" },
	{ "pen", "pen4", 4, "��" },
	{ "peng", "peng1", 1, "������������" },
	{ "peng", "peng2", 2, "������������������ܡ�" },
	{ "peng", "peng3", 3, "��" },
	{ "peng", "peng4", 4, "��" },
	{ "pi", "pi1", 1, "����������������ا�������" },
	{ "pi", "pi2", 2, "ƤƣơƢ����ۯܱ��������������������" },
	{ "pi", "pi3", 3, "��ƥ��Ʀ���ߨ��������" },
	{ "pi", "pi4", 4, "ƨ��ƧƩ������" },
	{ "pian", "pian1", 1, "Ƭƪƫ�����" },
	{ "pian", "pian2", 2, "����������" },
	{ "pian", "pian3", 3, "��" },
	{ "pian", "pian4", 4, "Ƭƭ" },
	{ "piao", "piao1", 1, "ƯƮ�����" },
	{ "piao", "piao2", 2, "��ư��" },
	{ "piao", "piao3", 3, "Ư�����ݳ" },
	{ "piao", "piao4", 4, "ƱƯ����" },
	{ "pie", "pie1", 1, "ƲƳ�" },
	{ "pie", "pie3", 3, "Ʋد��" },
	{ "pin", "pin1", 1, "ƴ���" },
	{ "pin", "pin2", 2, "ƶƵ���" },
	{ "pin", "pin3", 3, "Ʒ�" },
	{ "pin", "pin4", 4, "Ƹ��" },
	{ "ping", "ping1", 1, "ƹ�ٷ" },
	{ "ping", "ping2", 2, "ƽ��ƿƾƼ����ƻƺ����" },
	{ "po", "po1", 1, "���²������������" },
	{ "po", "po2", 2, "����۶�" },
	{ "po", "po3", 3, "������" },
	{ "po", "po4", 4, "������������" },
	{ "pou", "pou1", 1, "��" },
	{ "pou", "pou2", 2, "����" },
	{ "pou", "pou3", 3, "��" },
	{ "pu", "pu1", 1, "��������" },
	{ "pu", "pu2", 2, "�����͸�����������" },
	{ "pu", "pu3", 3, "�ձ������������������" },
	{ "pu", "pu4", 4, "���̱�����" },
	{ "qi", "qi1", 1, "�������ۼ������������������������" },
	{ "qi", "qi2", 2, "��������������������������������ٹ��������������ݽޭ������������" },
	{ "qi", "qi3", 3, "�������������ܻ���" },
	{ "qi", "qi4", 4, "�����������������������ؽ������" },
	{ "qia", "qia1", 1, "��٤����" },
	{ "qia", "qia3", 3, "��" },
	{ "qia", "qia4", 4, "ǡǢ��" },
	{ "qian", "qian1", 1, "ǧǩǣǨǫǦ��ܷ���Ǫ�Ǥ����ǥ" },
	{ "qian", "qian2", 2, "ǰǮǱǬ�ǯ��ǭݡ��������" },
	{ "qian", "qian3", 3, "ǳǲǴ����" },
	{ "qian", "qian4", 4, "ǷǸ��Ƕٻǵ�������" },
	{ "qiang", "qiang1", 1, "��ǹ��ǻǺ����Ǽ��������" },
	{ "qiang", "qiang2", 2, "ǿǽǾ����" },
	{ "qiang", "qiang3", 3, "ǿ��������" },
	{ "qiang", "qiang4", 4, "Ǻ�����" },
	{ "qiao", "qiao1", 1, "����ȸ������������" },
	{ "qiao", "qiao2", 2, "���������̽�����������" },
	{ "qiao", "qiao3", 3, "����ȸ�" },
	{ "qiao", "qiao4", 4, "�����Ͽ�������ڽ��" },
	{ "qie", "qie1", 1, "��" },
	{ "qie", "qie2", 2, "��٤" },
	{ "qie", "qie3", 3, "��" },
	{ "qie", "qie4", 4, "����������������ۧ���" },
	{ "qin", "qin1", 1, "��������" },
	{ "qin", "qin2", 2, "������������������������������" },
	{ "qin", "qin3", 3, "��" },
	{ "qin", "qin4", 4, "������" },
	{ "qing", "qing1", 1, "������������������" },
	{ "qing", "qing2", 2, "������������" },
	{ "qing", "qing3", 3, "��������" },
	{ "qing", "qing4", 4, "������������" },
	{ "qiong", "qiong2", 2, "�����������������" },
	{ "qiu", "qiu1", 1, "��������������" },
	{ "qiu", "qiu2", 2, "������������ٴ����������������" },
	{ "qiu", "qiu3", 3, "��" },
	{ "qu", "qu1", 1, "�����������������������ڰ��" },
	{ "qu", "qu2", 2, "�������۾�������ޡ�޾" },
	{ "qu", "qu3", 3, "ȡ��Ȣȣ��" },
	{ "qu", "qu4", 4, "ȥȤ����" },
	{ "qu", "qu5", 5, "��" },
	{ "quan", "quan1", 1, "Ȧ�" },
	{ "quan", "quan2", 2, "ȫȨȪȭڹȧ������Ȭȩ������" },
	{ "quan", "quan3", 3, "Ȯ��" },
	{ "quan", "quan4", 4, "Ȱȯ" },
	{ "que", "que1", 1, "ȱ��Ȳ" },
	{ "que", "que2", 2, "ȳ" },
	{ "que", "que4", 4, "ȴȷȸȶȵ�����" },
	{ "qun", "qun1", 1, "��" },
	{ "qun", "qun2", 2, "Ⱥȹ��" },
	{ "ran", "ran2", 2, "Ȼȼ����" },
	{ "ran", "ran3", 3, "ȾȽ��" },
	{ "rang", "rang1", 1, "��" },
	{ "rang", "rang2", 2, "ȿ���" },
	{ "rang", "rang3", 3, "��������" },
	{ "rang", "rang4", 4, "��" },
	{ "rao", "rao2", 2, "�������" },
	{ "rao", "rao3", 3, "�����" },
	{ "rao", "rao4", 4, "��" },
	{ "re", "re3", 3, "������" },
	{ "re", "re4", 4, "��" },
	{ "ren", "ren2", 2, "��������" },
	{ "ren", "ren3", 3, "������" },
	{ "ren", "ren4", 4, "���������������������" },
	{ "reng", "reng1", 1, "��" },
	{ "reng", "reng2", 2, "��" },
	{ "ri", "ri4", 4, "��" },
	{ "rong", "rong2", 2, "����������������������������" },
	{ "rong", "rong3", 3, "��" },
	{ "rou", "rou2", 2, "����������" },
	{ "rou", "rou4", 4, "��" },
	{ "ru", "ru2", 2, "�������������޸����" },
	{ "ru", "ru3", 3, "������" },
	{ "ru", "ru4", 4, "�����������" },
	{ "ruan", "ruan3", 3, "������" },
	{ "rui", "rui2", 2, "ި" },
	{ "rui", "rui3", 3, "��" },
	{ "rui", "rui4", 4, "����������" },
	{ "run", "run4", 4, "����" },
	{ "ruo", "ruo4", 4, "����ټ��" },
	{ "sa", "sa1", 1, "������" },
	{ "sa", "sa3", 3, "����" },
	{ "sa", "sa4", 4, "��ئ���" },
	{ "sai", "sai1", 1, "˼��������" },
	{ "sai", "sai4", 4, "����" },
	{ "san", "san1", 1, "�����" },
	{ "san", "san3", 3, "ɢɡ������" },
	{ "san", "san4", 4, "ɢ" },
	{ "sang", "sang1", 1, "ɥɣ" },
	{ "sang", "sang3", 3, "ɤ�����" },
	{ "sang", "sang4", 4, "ɥ" },
	{ "sao", "sao1", 1, "ɧɦ��������" },
	{ "sao", "sao3", 3, "ɨɩ" },
	{ "sao", "sao4", 4, "ɨ����ܣ��" },
	{ "se", "se4", 4, "ɫ��ɬɪ����" },
	{ "sen", "sen1", 1, "ɭ" },
	{ "seng", "seng1", 1, "ɮ" },
	{ "sha", "sha1", 1, "ɱɳɲɴɼɯɷɰ���������" },
	{ "sha", "sha3", 3, "ɵ" },
	{ "sha", "sha4", 4, "ɳɶ��ɷ�������" },
	{ "shai", "shai1", 1, "ɸ��" },
	{ "shai", "shai3", 3, "ɫ" },
	{ "shai", "shai4", 4, "ɹ" },
	{ "shan", "shan1", 1, "ɽ��ɾɿ��ɺɼդ���������������ɻ��" },
	{ "shan", "shan3", 3, "���²���" },
	{ "shan", "shan4", 4, "������������ڨ�������ӵ�����ɻ۷�������" },
	{ "shang", "shang1", 1, "��������������" },
	{ "shang", "shang3", 3, "��������" },
	{ "shang", "shang4", 4, "�����" },
	{ "shang", "shang5", 5, "��" },
	{ "shao", "shao1", 1, "����������������" },
	{ "shao", "shao2", 2, "���������" },
	{ "shao", "shao3", 3, "��" },
	{ "shao", "shao4", 4, "����������������ۿ" },
	{ "she", "she1", 1, "������" },
	{ "she", "she2", 2, "��������" },
	{ "she", "she3", 3, "��" },
	{ "she", "she4", 4, "�����������������������" },
	{ "shei", "shei2", 2, "˭" },
	{ "shen", "shen1", 1, "�������������ݷ��ڷ����" },
	{ "shen", "shen2", 2, "ʲ����" },
	{ "shen", "shen3", 3, "��������������" },
	{ "shen", "shen4", 4, "���������������" },
	{ "sheng", "sheng1", 1, "����ʤ��������" },
	{ "sheng", "sheng2", 2, "����" },
	{ "sheng", "sheng3", 3, "ʡ��" },
	{ "sheng", "sheng4", 4, "ʤʥʢ��ʣ����" },
	{ "shi", "shi1", 1, "ʦʫʧʩʬʪʨ��ʭ������" },
	{ "shi", "shi2", 2, "ʱʮʵʲʶʳʯʰʴ��ݪ����" },
	{ "shi", "shi3", 3, "ʹʼʷʻʺʸ��" },
	{ "shi", "shi4", 4, "��������ʿʽ����ʾ������������������������������������߱��ݪ���������" },
	{ "shi", "shi5", 5, "ֳ��" },
	{ "shou", "shou1", 1, "��" },
	{ "shou", "shou2", 2, "��" },
	{ "shou", "shou3", 3, "��������" },
	{ "shou", "shou4", 4, "���������������" },
	{ "shu", "shu1", 1, "����������������������ٿ����������" },
	{ "shu", "shu2", 2, "����������" },
	{ "shu", "shu3", 3, "������������������" },
	{ "shu", "shu4", 4, "������������ˡ��������������ح��" },
	{ "shua", "shua1", 1, "ˢ�" },
	{ "shua", "shua3", 3, "ˣ" },
	{ "shua", "shua4", 4, "ˢ" },
	{ "shuai", "shuai1", 1, "˥ˤ" },
	{ "shuai", "shuai3", 3, "˦" },
	{ "shuai", "shuai4", 4, "��˧�" },
	{ "shuan", "shuan1", 1, "˨˩��" },
	{ "shuan", "shuan4", 4, "��" },
	{ "shuang", "shuang1", 1, "˫˪����" },
	{ "shuang", "shuang3", 3, "ˬ" },
	{ "shui", "shui2", 2, "˭" },
	{ "shui", "shui3", 3, "ˮ" },
	{ "shui", "shui4", 4, "˵˰˯" },
	{ "shun", "shun3", 3, "˱" },
	{ "shun", "shun4", 4, "˳˴˲" },
	{ "shuo", "shuo1", 1, "˵" },
	{ "shuo", "shuo4", 4, "��˷˶˸����������" },
	{ "si", "si1", 1, "˼˹˾˽˿˺��˻�������������" },
	{ "si", "si3", 3, "��" },
	{ "si", "si4", 4, "����ʳ������������������ٹ���������" },
	{ "si", "si5", 5, "��" },
	{ "song", "song1", 1, "����������ڡݿ" },
	{ "song", "song3", 3, "������" },
	{ "song", "song4", 4, "����������" },
	{ "sou", "sou1", 1, "��������������" },
	{ "sou", "sou3", 3, "����޴���" },
	{ "sou", "sou4", 4, "����" },
	{ "su", "su1", 1, "������" },
	{ "su", "su2", 2, "��" },
	{ "su", "su4", 4, "���������������������������������" },
	{ "suan", "suan1", 1, "���" },
	{ "suan", "suan4", 4, "����" },
	{ "sui", "sui1", 1, "����ݴ����" },
	{ "sui", "sui2", 2, "��������" },
	{ "sui", "sui3", 3, "��" },
	{ "sui", "sui4", 4, "������������������" },
	{ "sun", "sun1", 1, "��ݥ���" },
	{ "sun", "sun3", 3, "�������" },
	{ "suo", "suo1", 1, "��ɯ�������������������" },
	{ "suo", "suo3", 3, "����������" },
	{ "ta", "ta1", 1, "������̤����������" },
	{ "ta", "ta3", 3, "����̡" },
	{ "ta", "ta4", 4, "̤����̣�̢����" },
	{ "tai", "tai1", 1, "̨̥̦" },
	{ "tai", "tai2", 2, "̨̧̦ۢ޷��������" },
	{ "tai", "tai3", 3, "߾" },
	{ "tai", "tai4", 4, "̫̬̩̭̪����" },
	{ "tan", "tan1", 1, "̯̰̲̱̮" },
	{ "tan", "tan2", 2, "̸��̷̶̳��̵�̴�����۰" },
	{ "tan", "tan3", 3, "̹̺��̻��" },
	{ "tan", "tan4", 4, "̼̽̾̿" },
	{ "tang", "tang1", 1, "�����������" },
	{ "tang", "tang2", 2, "��������������������������" },
	{ "tang", "tang3", 3, "����������" },
	{ "tang", "tang4", 4, "����" },
	{ "tao", "tao1", 1, "������߶�������" },
	{ "tao", "tao2", 2, "�������������ػ" },
	{ "tao", "tao3", 3, "��" },
	{ "tao", "tao4", 4, "��" },
	{ "te", "te4", 4, "����߯��" },
	{ "tei", "tei1", 1, "߯" },
	{ "teng", "teng2", 2, "����������" },
	{ "ti", "ti1", 1, "����������" },
	{ "ti", "ti2", 2, "����������������" },
	{ "ti", "ti3", 3, "��" },
	{ "ti", "ti4", 4, "��������������������" },
	{ "tian", "tian1", 1, "����" },
	{ "tian", "tian2", 2, "��������������" },
	{ "tian", "tian3", 3, "��������" },
	{ "tian", "tian4", 4, "��" },
	{ "tiao", "tiao1", 1, "��٬��" },
	{ "tiao", "tiao2", 2, "������������������" },
	{ "tiao", "tiao3", 3, "���" },
	{ "tiao", "tiao4", 4, "������" },
	{ "tie", "tie1", 1, "������" },
	{ "tie", "tie3", 3, "����" },
	{ "tie", "tie4", 4, "����" },
	{ "ting", "ting1", 1, "����͡��" },
	{ "ting", "ting2", 2, "ͣͥͤ��͢��������" },
	{ "ting", "ting3", 3, "ͦͧ�����" },
	{ "ting", "ting4", 4, "��" },
	{ "tong", "tong1", 1, "ͨ����" },
	{ "tong", "tong2", 2, "ͬͯͮͭͩͫ١ͪ�������������" },
	{ "tong", "tong3", 3, "ͳͲͰͱ��" },
	{ "tong", "tong4", 4, "ͬͨʹ��" },
	{ "tou", "tou1", 1, "͵" },
	{ "tou", "tou2", 2, "ͷͶ��" },
	{ "tou", "tou3", 3, "��" },
	{ "tou", "tou4", 4, "͸" },
	{ "tu", "tu1", 1, "ͻͺ͹" },
	{ "tu", "tu2", 2, "ͼ;ͽ��Ϳݱ����" },
	{ "tu", "tu3", 3, "������" },
	{ "tu", "tu4", 4, "����ܢ��" },
	{ "tuan", "tuan1", 1, "��" },
	{ "tuan", "tuan2", 2, "����" },
	{ "tuan", "tuan3", 3, "�" },
	{ "tuan", "tuan4", 4, "��" },
	{ "tui", "tui1", 1, "��߯" },
	{ "tui", "tui2", 2, "��" },
	{ "tui", "tui3", 3, "��" },
	{ "tui", "tui4", 4, "��������" },
	{ "tun", "tun1", 1, "����" },
	{ "tun", "tun2", 2, "����ζ���" },
	{ "tun", "tun3", 3, "��" },
	{ "tun", "tun4", 4, "��" },
	{ "tuo", "tuo1", 1, "������ر" },
	{ "tuo", "tuo2", 2, "�Ӷ�����������������٢��������" },
	{ "tuo", "tuo3", 3, "������" },
	{ "tuo", "tuo4", 4, "����������" },
	{ "wa", "wa1", 1, "���۰������" },
	{ "wa", "wa2", 2, "��" },
	{ "wa", "wa3", 3, "����" },
	{ "wa", "wa4", 4, "������" },
	{ "wa", "wa5", 5, "��" },
	{ "wai", "wai1", 1, "��" },
	{ "wai", "wai3", 3, "��" },
	{ "wai", "wai4", 4, "��" },
	{ "wan", "wan1", 1, "����������" },
	{ "wan", "wan2", 2, "����������ܹ��" },
	{ "wan", "wan3", 3, "������������ݸ������������" },
	{ "wan", "wan4", 4, "������" },
	{ "wang", "wang1", 1, "����" },
	{ "wang", "wang2", 2, "������â" },
	{ "wang", "wang3", 3, "�������������" },
	{ "wang", "wang4", 4, "������������" },
	{ "wei", "wei1", 1, "ί��΢ΣΡή��ޱ����������" },
	{ "wei", "wei2", 2, "ΪάΧΨΥΤΩ�������ΫΦ�������" },
	{ "wei", "wei3", 3, "ίΰΨβ��α�γή�έ�������������" },
	{ "wei", "wei4", 4, "Ϊλδζ��ν��οκεηθιξμ��" },
	{ "wen", "wen1", 1, "����" },
	{ "wen", "wen2", 2, "�������������" },
	{ "wen", "wen3", 3, "��������" },
	{ "wen", "wen4", 4, "�������" },
	{ "weng", "weng1", 1, "����" },
	{ "weng", "weng3", 3, "��" },
	{ "weng", "weng4", 4, "��޳" },
	{ "wo", "wo1", 1, "�����������ݫ" },
	{ "wo", "wo2", 2, "Ŷ" },
	{ "wo", "wo3", 3, "��" },
	{ "wo", "wo4", 4, "����Ŷ������������" },
	{ "wu", "wu1", 1, "춶�������������أ������" },
	{ "wu", "wu2", 2, "�����������������������" },
	{ "wu", "wu3", 3, "������������������������������" },
	{ "wu", "wu4", 4, "�������������������أ��������������������" },
	{ "xi", "xi1", 1, "��Ϣϣ��ϧϡϤ��Ϧ����������Ϫ��������Ϩϥ��ۭϬ�������������ϫ������ϩ�����������ݾ�������������" },
	{ "xi", "xi2", 2, "ϯϰϮϱϭ����" },
	{ "xi", "xi3", 3, "ϲϴ������������ϳ" },
	{ "xi", "xi4", 4, "ϵϸϷ϶�������" },
	{ "xia", "xia1", 1, "ϹϺ��" },
	{ "xia", "xia2", 2, "Ͽ����ϼϾϽ��ϻ�����������" },
	{ "xia", "xia4", 4, "�������û���" },
	{ "xian", "xian1", 1, "������������ݲ�����������" },
	{ "xian", "xian2", 2, "���������������������" },
	{ "xian", "xian3", 3, "������ϴ����޺ϳ�������" },
	{ "xian", "xian4", 4, "�ּ������������������������" },
	{ "xiang", "xiang1", 1, "��������������������ܼ�" },
	{ "xiang", "xiang2", 2, "����������" },
	{ "xiang", "xiang3", 3, "������������" },
	{ "xiang", "xiang4", 4, "���������������" },
	{ "xiao", "xiao1", 1, "������Ф�������������������������������" },
	{ "xiao", "xiao2", 2, "����" },
	{ "xiao", "xiao3", 3, "С����" },
	{ "xiao", "xiao4", 4, "ЦУЧФТХ" },
	{ "xie", "xie1", 1, "ЩЪШЫ" },
	{ "xie", "xie2", 2, "ҶЭЬЯбвгаЮ��ߢ�����" },
	{ "xie", "xie3", 3, "дѪ" },
	{ "xie", "xie4", 4, "д��лй��емжик��з����ޯ������������" },
	{ "xin", "xin1", 1, "��������нܰ��о����п" },
	{ "xin", "xin2", 2, "Ѱ��" },
	{ "xin", "xin4", 4, "��о��ض" },
	{ "xing", "xing1", 1, "����������" },
	{ "xing", "xing2", 2, "�����������������" },
	{ "xing", "xing3", 3, "ʡ��ߩ" },
	{ "xing", "xing4", 4, "�������������" },
	{ "xiong", "xiong1", 1, "����������ܺ" },
	{ "xiong", "xiong2", 2, "����" },
	{ "xiu", "xiu1", 1, "�����������������" },
	{ "xiu", "xiu3", 3, "����" },
	{ "xiu", "xiu4", 4, "�����޳����������" },
	{ "xu", "xu1", 1, "�����������������������" },
	{ "xu", "xu2", 2, "��" },
	{ "xu", "xu3", 3, "�����ڼ����" },
	{ "xu", "xu4", 4, "�����������������������������" },
	{ "xu", "xu5", 5, "ޣ" },
	{ "xuan", "xuan1", 1, "������������������" },
	{ "xuan", "xuan2", 2, "�����������" },
	{ "xuan", "xuan3", 3, "ѡѢ" },
	{ "xuan", "xuan4", 4, "��ȯ����Ѥѣ���������" },
	{ "xue", "xue1", 1, "��ѥѦ" },
	{ "xue", "xue2", 2, "ѧѨ�����" },
	{ "xue", "xue3", 3, "ѩ��" },
	{ "xue", "xue4", 4, "Ѫ��" },
	{ "xun", "xun1", 1, "Ѭѫ����޹������" },
	{ "xun", "xun2", 2, "ѰѯѲѭѮѱ������ۨ���" },
	{ "xun", "xun4", 4, "ѵѸѶѷѬѳ����Ѵަ��" },
	{ "ya", "ya1", 1, "ѹ��ѽѺѻ��ѼѾ����" },
	{ "ya", "ya2", 2, "������ѿ�����������" },
	{ "ya", "ya3", 3, "����ƥ����" },
	{ "ya", "ya4", 4, "��ѹ�����������" },
	{ "ya", "ya5", 5, "ѽ" },
	{ "yan", "yan1", 1, "������������������������۳������" },
	{ "yan", "yan2", 2, "��������������������Ǧ��������ܾ����" },
	{ "yan", "yan3", 3, "����������ٲ��������۱���������" },
	{ "yan", "yan4", 4, "��������������������������������������������" },
	{ "yang", "yang1", 1, "������������" },
	{ "yang", "yang2", 2, "���������������������" },
	{ "yang", "yang3", 3, "��������" },
	{ "yang", "yang4", 4, "�����������" },
	{ "yao", "yao1", 1, "ҪԼ����ز��ߺ��" },
	{ "yao", "yao2", 2, "ҡңҦ��Ңҥ��Ҥ�Ƚ������س������" },
	{ "yao", "yao3", 3, "ҧ���Ҩ��" },
	{ "yao", "yao4", 4, "ҪҩҫԿ����ű" },
	{ "ye", "ye1", 1, "ҮҭҬҴ" },
	{ "ye", "ye2", 2, "үҮа����" },
	{ "ye", "ye3", 3, "ҲҰұ" },
	{ "ye", "ye4", 4, "ҵҹҶҳҺ�ʻ�ҷק��ҴҸ��������" },
	{ "yi", "yi1", 1, "һҽ������������Ҿ���Ҽҿ���" },
	{ "yi", "yi2", 2, "����������������������������������������ڱ����" },
	{ "yi", "yi3", 3, "������β�����������ƶ����������" },
	{ "yi", "yi4", 4, "�������������������������������۰���������������������������߮��߽ܲ��޲����������������������٫��������������������" },
	{ "yin", "yin1", 1, "��������������������ܧ���" },
	{ "yin", "yin2", 2, "����������۴���������" },
	{ "yin", "yin3", 3, "��������������" },
	{ "yin", "yin4", 4, "ӡ����ط���" },
	{ "ying", "ying1", 1, "ӦӢӥӤӣ��ݺ���ӧ�������" },
	{ "ying", "ying2", 2, "ӪӭӮӯӬӨӫө����������������" },
	{ "ying", "ying3", 3, "Ӱӱ��۫" },
	{ "ying", "ying4", 4, "ӦӲӳ��" },
	{ "yo", "yo1", 1, "��Ӵ�" },
	{ "yo", "yo5", 5, "Ӵ" },
	{ "yong", "yong1", 1, "ӵӹӶӺӷ����ܭ�Ӹ������" },
	{ "yong", "yong2", 2, "�" },
	{ "yong", "yong3", 3, "����ӿӻӾӽٸ���Ӽ" },
	{ "yong", "yong4", 4, "��Ӷ" },
	{ "you", "you1", 1, "������������" },
	{ "you", "you2", 2, "����������������ݵ����������������ݯ" },
	{ "you", "you3", 3, "��������ݬ�����" },
	{ "you", "you4", 4, "�������������������٧����" },
	{ "yu", "yu1", 1, "�����������" },
	{ "yu", "yu2", 2, "���������������������������������������خ�������������������������" },
	{ "yu", "yu3", 3, "������������������������������ٶ" },
	{ "yu", "yu4", 4, "��������������Ԥ������������εԢԥ������ԣԡ��Ԧξ��ع���������������������������������" },
	{ "yuan", "yuan1", 1, "ԩԨԧ�����" },
	{ "yuan", "yuan2", 2, "ԱԪԭ԰ԴԲԵԮԬԳԫԯ����ܾ����ܫ����" },
	{ "yuan", "yuan3", 3, "Զ" },
	{ "yuan", "yuan4", 4, "ԺԸԹԷ�������" },
	{ "yue", "yue1", 1, "ԼԻ" },
	{ "yue", "yue4", 4, "˵����Խ��Ծ������Կ����������" },
	{ "yun", "yun1", 1, "���" },
	{ "yun", "yun2", 2, "Ա������ܿ�������" },
	{ "yun", "yun3", 3, "��������" },
	{ "yun", "yun4", 4, "Ա�˾��������������۩��" },
	{ "za", "za1", 1, "��������" },
	{ "za", "za2", 2, "������" },
	{ "za", "za3", 3, "զ" },
	{ "zai", "zai1", 1, "��������" },
	{ "zai", "zai3", 3, "��������" },
	{ "zai", "zai4", 4, "������" },
	{ "zan", "zan1", 1, "����" },
	{ "zan", "zan2", 2, "��" },
	{ "zan", "zan3", 3, "��������" },
	{ "zan", "zan4", 4, "�������" },
	{ "zan", "zan5", 5, "��" },
	{ "zang", "zang1", 1, "�����" },
	{ "zang", "zang3", 3, "��" },
	{ "zang", "zang4", 4, "��������" },
	{ "zao", "zao1", 1, "����" },
	{ "zao", "zao2", 2, "��" },
	{ "zao", "zao3", 3, "������������" },
	{ "zao", "zao4", 4, "��������������" },
	{ "ze", "ze2", 2, "��������զ��������������" },
	{ "ze", "ze4", 4, "�����" },
	{ "zei", "zei2", 2, "��" },
	{ "zen", "zen3", 3, "��" },
	{ "zen", "zen4", 4, "��" },
	{ "zeng", "zeng1", 1, "����������" },
	{ "zeng", "zeng4", 4, "��������" },
	{ "zha", "zha1", 1, "����զ���������߸��" },
	{ "zha", "zha2", 2, "ը�������բա" },
	{ "zha", "zha3", 3, "գ��" },
	{ "zha", "zha4", 4, "ըզթէ��դե��߸������" },
	{ "zhai", "zhai1", 1, "ժ��ի" },
	{ "zhai", "zhai2", 2, "��լ��" },
	{ "zhai", "zhai3", 3, "խ" },
	{ "zhai", "zhai4", 4, "ծ��կ���" },
	{ "zhan", "zhan1", 1, "ռմճհղձ���" },
	{ "zhan", "zhan3", 3, "չնշյո��" },
	{ "zhan", "zhan4", 4, "սվռ����տպջ" },
	{ "zhang", "zhang1", 1, "��������������۵�" },
	{ "zhang", "zhang3", 3, "��������" },
	{ "zhang", "zhang4", 4, "���������������������" },
	{ "zhao", "zhao1", 1, "���г���������" },
	{ "zhao", "zhao2", 2, "��" },
	{ "zhao", "zhao3", 3, "��צ��" },
	{ "zhao", "zhao4", 4, "������������گ����" },
	{ "zhe", "zhe1", 1, "������" },
	{ "zhe", "zhe2", 2, "������������ߡ����" },
	{ "zhe", "zhe3", 3, "��������" },
	{ "zhe", "zhe4", 4, "����������" },
	{ "zhe", "zhe5", 5, "��" },
	{ "zhei", "zhei4", 4, "��" },
	{ "zhen", "zhen1", 1, "���������������������������������" },
	{ "zhen", "zhen3", 3, "������������" },
	{ "zhen", "zhen4", 4, "���������������" },
	{ "zheng", "zheng1", 1, "����������֢������������������" },
	{ "zheng", "zheng3", 3, "����" },
	{ "zheng", "zheng4", 4, "����֤��֣֢���ں֡" },
	{ "zhi", "zhi1", 1, "ָֻ֪֧֮֯��֦֭��֥֨֫֬֩��ش����" },
	{ "zhi", "zhi2", 2, "ְֱֲֳִֵֶָ����������" },
	{ "zhi", "zhi3", 3, "ַָֹֻּֽ��ֺ��������������" },
	{ "zhi", "zhi4", 4, "֪����ʶ��־����������������ֿ�����������������������������������������ۤ��" },
	{ "zhong", "zhong1", 1, "�������������������" },
	{ "zhong", "zhong3", 3, "������ڣ" },
	{ "zhong", "zhong4", 4, "����������" },
	{ "zhou", "zhou1", 1, "��������������" },
	{ "zhou", "zhou2", 2, "�����" },
	{ "zhou", "zhou3", 3, "����" },
	{ "zhou", "zhou4", 4, "�����������������ݧ������" },
	{ "zhu", "zhu1", 1, "�������������������ۥ������٪" },
	{ "zhu", "zhu2", 2, "������������������" },
	{ "zhu", "zhu3", 3, "�����������������" },
	{ "zhu", "zhu4", 4, "סע����פף������������������������" },
	{ "zhua", "zhua1", 1, "ץ��" },
	{ "zhua", "zhua3", 3, "צ" },
	{ "zhuai", "zhuai1", 1, "ק" },
	{ "zhuai", "zhuai3", 3, "ת" },
	{ "zhuai", "zhuai4", 4, "ҷק��" },
	{ "zhuan", "zhuan1", 1, "רש�" },
	{ "zhuan", "zhuan3", 3, "ת" },
	{ "zhuan", "zhuan4", 4, "��ת׬׫��׭����" },
	{ "zhuang", "zhuang1", 1, "װׯױ׮" },
	{ "zhuang", "zhuang3", 3, "��" },
	{ "zhuang", "zhuang4", 4, "״׳ײ�����" },
	{ "zhui", "zhui1", 1, "׷׶��׵��" },
	{ "zhui", "zhui4", 4, "׹׺׸���" },
	{ "zhun", "zhun1", 1, "��׻���" },
	{ "zhun", "zhun3", 3, "׼" },
	{ "zhuo", "zhuo1", 1, "��׽׿׾����پ" },
	{ "zhuo", "zhuo2", 2, "��������������������������ߪ�" },
	{ "zi", "zi1", 1, "����������֨����������������������������������" },
	{ "zi", "zi3", 3, "�����������������������" },
	{ "zi", "zi4", 4, "���������" },
	{ "zong", "zong1", 1, "��������������" },
	{ "zong", "zong3", 3, "����" },
	{ "zong", "zong4", 4, "����" },
	{ "zou", "zou1", 1, "������۸����" },
	{ "zou", "zou3", 3, "��" },
	{ "zou", "zou4", 4, "����" },
	{ "zu", "zu1", 1, "����" },
	{ "zu", "zu2", 2, "��������" },
	{ "zu", "zu3", 3, "����������" },
	{ "zuan", "zuan1", 1, "����" },
	{ "zuan", "zuan3", 3, "����" },
	{ "zuan", "zuan4", 4, "׬��߬" },
	{ "zui", "zui1", 1, "��" },
	{ "zui", "zui3", 3, "�����" },
	{ "zui", "zui4", 4, "������ީ" },
	{ "zun", "zun1", 1, "��������" },
	{ "zun", "zun3", 3, "ߤ" },
	{ "zuo", "zuo1", 1, "����" },
	{ "zuo", "zuo2", 2, "��������" },
	{ "zuo", "zuo3", 3, "������" },
	{ "zuo", "zuo4", 4, "������������������������" }
};

int  PY_CNT = sizeof(p2h) / sizeof(p2h[0]);
char *getcnchar(char *cnChar)
{
	char query[] = "��";
	char *curHzString;
	char curHz[4] = "";
	int i, j;
	int found = 0;
	char *gotChar = NULL;
	query[0] = cnChar[0];
	query[1] = cnChar[1];

	for (i = 0; i < PY_CNT && !found; i++) {
		curHzString = p2h[i].hz;
		for (j = 0; j < (int)strlen((const char *)curHzString); j += 2) {
			memcpy(curHz, curHzString + j, 2);
			if (strcmp(query, curHz))
				continue;
			else {
				gotChar = p2h[i].py;
				found = 1;
				break;
			}
		}
	}
	if (!found)
		return NULL;
	else
	{
		return gotChar;
	}

}





//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/* ���ֶ����ֽ�㴦����ֵ����*/
const unsigned short code_pin[] = {
	0xb0a1, 0xb0a3, 0xb0b0, 0xb0b9, 0xb0bc, 0xb0c5, 0xb0d7, 0xb0df, 0xb0ee, 0xb0fa, 0xb1ad, 0xb1bc, 0xb1c0, 0xb1c6,
	0xb1de, 0xb1ea, 0xb1ee, 0xb1f2, 0xb1f8, 0xb2a3, 0xb2b8, 0xb2c1, 0xb2c2, 0xb2cd, 0xb2d4, 0xb2d9, 0xb2de, 0xb2e3,
	0xb2e5, 0xb2f0, 0xb2f3, 0xb2fd, 0xb3ac, 0xb3b5, 0xb3bb, 0xb3c5, 0xb3d4, 0xb3e4, 0xb3e9, 0xb3f5, 0xb4a7, 0xb4a8,
	0xb4af, 0xb4b5, 0xb4ba, 0xb4c1, 0xb4c3, 0xb4cf, 0xb4d5, 0xb4d6, 0xb4da, 0xb4dd, 0xb4e5, 0xb4e8, 0xb4ee, 0xb4f4,
	0xb5a2, 0xb5b1, 0xb5b6, 0xb5c2, 0xb5c5, 0xb5cc, 0xb5df, 0xb5ef, 0xb5f8, 0xb6a1, 0xb6aa, 0xb6ab, 0xb6b5, 0xb6bc,
	0xb6cb, 0xb6d1, 0xb6d5, 0xb6de, 0xb6ea, 0xb6f7, 0xb6f8, 0xb7a2, 0xb7aa, 0xb7bb, 0xb7c6, 0xb7d2, 0xb7e1, 0xb7f0,
	0xb7f1, 0xb7f2, 0xb8c1, 0xb8c3, 0xb8c9, 0xb8d4, 0xb8dd, 0xb8e7, 0xb8f8, 0xb8f9, 0xb8fb, 0xb9a4, 0xb9b3, 0xb9bc,
	0xb9ce, 0xb9d4, 0xb9d7, 0xb9e2, 0xb9e5, 0xb9f5, 0xb9f8, 0xb9fe, 0xbaa1, 0xbaa8, 0xbabb, 0xbabe, 0xbac7, 0xbad9,
	0xbadb, 0xbadf, 0xbae4, 0xbaed, 0xbaf4, 0xbba8, 0xbbb1, 0xbbb6, 0xbbc4, 0xbbd2, 0xbbe7, 0xbbed, 0xbbf7, 0xbcce,
	0xbcdf, 0xbda9, 0xbdb6, 0xbdd2, 0xbded, 0xbea3, 0xbebc, 0xbebe, 0xbecf, 0xbee8, 0xbeef, 0xbef9, 0xbfa6, 0xbfaa,
	0xbfaf, 0xbfb5, 0xbfbc, 0xbfc0, 0xbfcf, 0xbfd3, 0xbfd5, 0xbfd9, 0xbfdd, 0xbfe4, 0xbfe9, 0xbfed, 0xbfef, 0xbff7,
	0xc0a4, 0xc0a8, 0xc0ac, 0xc0b3, 0xc0b6, 0xc0c5, 0xc0cc, 0xc0d5, 0xc0d7, 0xc0e2, 0xc0e5, 0xc1a9, 0xc1aa, 0xc1b8,
	0xc1c3, 0xc1d0, 0xc1d5, 0xc1e1, 0xc1ef, 0xc1fa, 0xc2a5, 0xc2ab, 0xc2bf, 0xc2cd, 0xc2d3, 0xc2d5, 0xc2dc, 0xc2e8,
	0xc2f1, 0xc2f7, 0xc3a2, 0xc3a8, 0xc3b4, 0xc3b5, 0xc3c5, 0xc3c8, 0xc3d0, 0xc3de, 0xc3e7, 0xc3ef, 0xc3f1, 0xc3f7,
	0xc3fd, 0xc3fe, 0xc4b1, 0xc4b4, 0xc4c3, 0xc4ca, 0xc4cf, 0xc4d2, 0xc4d3, 0xc4d8, 0xc4d9, 0xc4db, 0xc4dc, 0xc4dd,
	0xc4e8, 0xc4ef, 0xc4f1, 0xc4f3, 0xc4fa, 0xc4fb, 0xc5a3, 0xc5a7, 0xc5ab, 0xc5ae, 0xc5af, 0xc5b0, 0xc5b2, 0xc5b6,
	0xc5b7, 0xc5be, 0xc5c4, 0xc5ca, 0xc5d2, 0xc5d7, 0xc5de, 0xc5e7, 0xc5e9, 0xc5f7, 0xc6aa, 0xc6ae, 0xc6b2, 0xc6b4,
	0xc6b9, 0xc6c2, 0xc6cb, 0xc6da, 0xc6fe, 0xc7a3, 0xc7b9, 0xc7c1, 0xc7d0, 0xc7d5, 0xc7e0, 0xc7ed, 0xc7ef, 0xc7f7,
	0xc8a6, 0xc8b1, 0xc8b9, 0xc8bb, 0xc8bf, 0xc8c4, 0xc8c7, 0xc8c9, 0xc8d3, 0xc8d5, 0xc8d6, 0xc8e0, 0xc8e3, 0xc8ed,
	0xc8ef, 0xc8f2, 0xc8f4, 0xc8f6, 0xc8f9, 0xc8fd, 0xc9a3, 0xc9a6, 0xc9aa, 0xc9ad, 0xc9ae, 0xc9af, 0xc9b8, 0xc9ba,
	0xc9ca, 0xc9d2, 0xc9dd, 0xc9e9, 0xc9f9, 0xcaa6, 0xcad5, 0xcadf, 0xcba2, 0xcba4, 0xcba8, 0xcbaa, 0xcbad, 0xcbb1,
	0xcbb5, 0xcbb9, 0xcbc9, 0xcbd1, 0xcbd4, 0xcbe1, 0xcbe4, 0xcbef, 0xcbf2, 0xcbfa, 0xcca5, 0xccae, 0xccc0, 0xcccd,
	0xccd8, 0xccd9, 0xccdd, 0xccec, 0xccf4, 0xccf9, 0xccfc, 0xcda8, 0xcdb5, 0xcdb9, 0xcdc4, 0xcdc6, 0xcdcc, 0xcdcf,
	0xcdda, 0xcde1, 0xcde3, 0xcdf4, 0xcdfe, 0xcec1, 0xcecb, 0xcece, 0xced7, 0xcef4, 0xcfb9, 0xcfc6, 0xcfe0, 0xcff4,
	0xd0a8, 0xd0bd, 0xd0c7, 0xd0d6, 0xd0dd, 0xd0e6, 0xd0f9, 0xd1a5, 0xd1ab, 0xd1b9, 0xd1c9, 0xd1ea, 0xd1fb, 0xd2ac,
	0xd2bb, 0xd2f0, 0xd3a2, 0xd3b4, 0xd3b5, 0xd3c4, 0xd3d9, 0xd4a7, 0xd4bb, 0xd4c5, 0xd4d1, 0xd4d4, 0xd4db, 0xd4df,
	0xd4e2, 0xd4f0, 0xd4f4, 0xd4f5, 0xd4f6, 0xd4fa, 0xd5aa, 0xd5b0, 0xd5c1, 0xd5d0, 0xd5da, 0xd5e4, 0xd5f4, 0xd6a5,
	0xd6d0, 0xd6db, 0xd6e9, 0xd7a5, 0xd7a7, 0xd7a8, 0xd7ae, 0xd7b5, 0xd7bb, 0xd7bd, 0xd7c8, 0xd7d7, 0xd7de, 0xd7e2,
	0xd7ea, 0xd7ec, 0xd7f0, 0xd7f2 };

/* ���ֶ�������*/
const char *str_pin[] = {
	"a", "ai", "an", "ang", "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao",
	"bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "ceng", "cha", "chai", "chan",
	"chang", "chao", "che", "chen", "cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui",
	"chun", "chuo", "ci", "cong", "cou", "cu", "cuan", "cui", "cun", "cuo", "da", "dai", "dan", "dang", "dao",
	"de", "deng", "di", "dian", "diao", "die", "ding", "diu", "dong", "dou", "du", "duan", "dui", "dun", "duo",
	"e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu", "ga", "gai", "gan", "gang",
	"gao", "ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun",
	"guo", "ha", "hai", "han", "hang", "hao", "he", "hei", "hen", "heng", "hong", "hou", "hu", "hua", "huai",
	"huan", "huang", "hui", "hun", "huo", "ji", "jia", "jian", "jiang", "jiao", "jie", "jin", "jing",
	"jiong", "jiu", "ju", "juan", "jue", "jun", "ka", "kai", "kan", "kang", "kao", "ke", "ken", "keng",
	"kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la", "lai", "lan", "lang",
	"lao", "le", "lei", "leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long", "lou",
	"lu", "lv", "luan", "lue", "lun", "luo", "ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng",
	"mi", "mian", "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu", "na", "nai", "nan", "nang", "nao", "ne",
	"nei", "nen", "neng", "ni", "nian", "niang", "niao", "nie", "nin", "ning", "niu", "nong", "nu", "nv", "nuan",
	"nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei", "pen", "peng", "pi", "pian", "piao", "pie",
	"pin", "ping", "po", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing", "qiong", "qiu", "qu",
	"quan", "que", "qun", "ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui",
	"run", "ruo", "sa", "sai", "san", "sang", "sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao",
	"she", "shen", "sheng", "shi", "shou", "shu", "shua", "shuai", "shuan", "shuang", "shui", "shun", "shuo",
	"si", "song", "sou", "su", "suan", "sui", "sun", "suo", "ta", "tai", "tan", "tang", "tao", "te", "teng",
	"ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo", "wa", "wai", "wan",
	"wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing",
	"xiong", "xiu", "xu", "xuan", "xue", "xun", "ya", "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo",
	"yong", "you", "yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen", "zeng",
	"zha", "zhai", "zhan", "zhang", "zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua",
	"zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo", "zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo" };



/* ���鳤��*/
static const size_t SIZE_ARRAY = sizeof(code_pin) / sizeof(short);


/* ��ȡһ�����ֵ�ƴ��*/
const char* get_pin(unsigned short char_zh, const char* pszTemp)
{
	size_t low = 0, high = SIZE_ARRAY - 1;
	size_t index;

	if (char_zh > code_pin[high])
	{
		char* pszChar = getcnchar((char*)pszTemp);
		if (pszChar != NULL)
		{
			return pszChar;
		}
	}

	while (high - low != 1)
	{
		index = (low + high) / 2;
		if (code_pin[index] == char_zh)
			return str_pin[index];
		if (code_pin[index] < char_zh)
			low = index;
		else
			high = index;
	}
	return str_pin[code_pin[high] <= char_zh ? high : low];
}

/*
����Ӣ�����ֻ�ϴ�ת����ƴ������ÿһ��������֮�����һ���ո�

�����������ֻ�����������ַ����������ȷ���ɵ��ö˱�֤��
*/
std::string  NStr::ChineseToPinyin(std::string  strData)
{
	std::string  result;
	unsigned short char_zh;
	int inputLength = strData.length();
	unsigned char high, low;
	for (int i = 0; i < inputLength; ++i)
	{
		high = strData[i];
		if (high < 123)
		{
			if (i > 0 && (strData[i - 1] < 0 || strData[i - 1] > 57))
			{
				//result.append(1, ' ');
			}
			result.append(1, (high >= 'A' && high <= 'Z') ? (high + ('a' - 'A')) : high);
		}
// 		else if (high > 64 && high < 123)
// 		{
// 			if (i > 0 && (strData[i - 1] < 0 || strData[i - 1] < 58))
// 			{
// 				//result.append(1, ' ');
// 			}
// 			result.append(1, (high >= 'A' && high <= 'Z') ? (high + ('a' - 'A')) : high);
// 		}
		else
		{
			//if (i > 0) result.append(1, ' ');//�ӿո���������ĸ
			low = strData[++i];
			char_zh = (high << 8) + low;
			result.append(get_pin(char_zh, strData.c_str() + i - 1));
		}
	}
	return result;
}


#endif
