// +----------------------------------------------------------------------
// | Green C++ Basis Components
// +----------------------------------------------------------------------
// | std::wstring	�����ַ�����
// | std::string 	���ֽ��ַ����� 
// | NSNStr		�ַ������ò��������ռ�
// +----------------------------------------------------------------------
// +----------------------------------------------------------------------

#pragma  once

#include <string>
#include <vector>
#include <windows.h>



#ifdef _UNICODE
#define _tstring std::wstring
#else
#define _tstring std::string
#endif



namespace NStr
{
	// ���ֽڣ�std::string ��ת�ɿ��֣�std::wstring��
	std::wstring	StrToWStr(std::string  strBuf, int nCode = CP_ACP);

	// ���֣�std::wstring��ת�ɶ��ֽڣ�std::string ��
	std::string 	WStrToStr(std::wstring wstrBuf, int nCode = CP_ACP);

	// ����ת�� ANSI => UTF8
	std::string 	GetUtf8(std::string  strBuf);

	// ����ת�� UTF8 => ANSI
	std::string 	GetAnsi(std::string  strBuf);

	_tstring ANSI2T(LPCSTR lpSrc);

	std::string T2ANSI(LPCTSTR lpSrc);

	std::string T2UTF8(LPCTSTR lpSrc);

	std::wstring ANSI2Unicode(LPCSTR lpszSrc);

	std::string Unicode2UTF8(LPCWSTR lpszSrc);

	std::string Unicode2ANSI(LPCWSTR lpszSrc);

	std::wstring utf8ToUtf16(const std::string& utf8String);

	std::string utf16ToUtf8(LPCWSTR lpszSrc);
	// �ַ���ת��int
	int			StrToInt(std::string  strBuf);
	int			StrToInt(std::wstring strBuf);

	// intת�ַ���
	std::string 	IntToStr(int nValue);
	std::wstring	IntToWStr(int nValue);

	// �ַ���ת��float
	float		StrToFloat(std::string  strBuf);
	float		StrToFloat(std::wstring strBuf);

	// floatת���ַ���
	std::string 	FloatToStr(float fValue);
	std::wstring	FloatToWStr(float fValue);

	// �ַ���ת��double
	double		StrToDouble(std::string  strBuf);
	double		StrToDouble(std::wstring strBuf);

	// doubleת�ַ���
	std::string 	DoubleToStr(double dbValue);
	std::wstring	DoubleToWStr(double dbValue);

	std::string 	DoubleToStr(double dbValue, int nDigit);
	std::wstring	DoubleToWStr(double dbValue, int nDigit);

	// �ַ���תlong
	long		StringToLong(std::string  strBuf);
	long		StringToLong(std::wstring strBuf);

	// longת�ַ���
	std::string 	LongToStr(long lValue);
	std::wstring	LongToWStr(long lValue);

	// �ַ���ת__int64
	__int64		StrToInt64(std::string  strBuf);
	__int64		StrToInt64(std::wstring strBuf);

	// __int64ת�ַ���
	std::string 	Int64ToStr(__int64 nValue);
	std::wstring	Int64ToWStr(__int64 nValue);

	// �ַ����ָ�(���ַ�c�ָ�)
	int			SpliteStrByChar(std::string  strBuf, std::vector<std::string > &vecList, char c = '|', bool bSkipEmpty = false);
	int			SpliteStrByChar(std::wstring strBuf, std::vector<std::wstring> &vecList, char c = '|', bool bSkipEmpty = false);

	// �ַ����ָ�(����strChars�г��ֵ��ַ��ָ�)
	int			SpliteStrByChars(std::string  strBuf, std::vector<std::string > &vecList, std::string  strChars = "|", bool bSkipEmpty = false);
	int			SpliteStrByChars(std::wstring strBuf, std::vector<std::wstring> &vecList, std::wstring strChars = L"|", bool bSkipEmpty = false);

	// �ַ����ָ���ַ����ָ
	int			SpliteStrByStr(std::string  strBuf, std::vector<std::string > &vecList, std::string  strSplite = "|", bool bSkipEmpty = false);
	int			SpliteStrByStr(std::wstring strBuf, std::vector<std::wstring> &vecList, std::wstring strSplite = L"|", bool bSkipEmpty = false);

	// ����ַ����ָ��\0�������ָ
	int			MutilStrToArrays(char *pData, std::vector<std::string > &vecList);
	int			MutilStrToArrays(wchar_t *pData, std::vector<std::wstring> &vecList);

	// ɾ���ַ�����ָ�����ַ�(��strChars�еĶ�ɾ��)
	int			DelStrByChars(std::string  &strBuf, std::string  strDel);
	int			DelStrByChars(std::wstring &strBuf, std::wstring strDel);

	// ɾ���ַ�����ָ�����ַ���
	int			DelStrByStr(std::string  &strBuf, std::string  strDel);
	int			DelStrByStr(std::wstring &strBuf, std::wstring strDel);

	// �滻�ַ�����ָ�����ַ���
	int			ReplaceStr(std::string  &strBuf, std::string  strSrc, std::string  strDes);
	int			ReplaceStr(std::wstring &strBuf, std::wstring strSrc, std::wstring strDes);

	// �ַ���ͷ���Ƚ�
	int			StrHeadCmp(std::string  strBuf, std::string  strHead, bool bNoCase = true);
	int			StrHeadCmp(std::wstring strBuf, std::wstring strHead, bool bNoCase = true);

	// �ַ���β���Ƚ�
	int			StrTailCmp(std::string  strBuf, std::string  strTail, bool bNoCase = true);
	int			StrTailCmp(std::wstring strBuf, std::wstring strTail, bool bNoCase = true);

	// ȥ���ַ������߳��ֵ�ָ���ַ�(��strChars�г��ֵĶ�ɾ��)
	int			StrTrim(std::string  &strBuf, std::string  strChars = " ");
	int			StrTrim(std::wstring &strBuf, std::wstring strChars = L" ");

	// ȥ���ַ�����߳��ֵ�ָ���ַ�(��strChars�г��ֵĶ�ɾ��)
	int			StrTrimLeft(std::string  &strBuf, std::string  strChars = " ");
	int			StrTrimLeft(std::wstring &strBuf, std::wstring strChars = L" ");

	// ȥ���ַ����ұ߳��ֵ�ָ���ַ�(��strChars�г��ֵĶ�ɾ��)
	int			StrTrimRight(std::string  &strBuf, std::string  strChars = " ");
	int			StrTrimRight(std::wstring &strBuf, std::wstring strChars = L" ");

	// ��������ַ���
	std::string 	CreateRandomStr(int nSize, std::string  strSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	std::wstring	CreateRandomWStr(int nSize, std::wstring strSeed = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	// ��ʽ���ַ���
	std::string 	FormatStr(const char* pFmt, ...);
	std::wstring	FormatWStr(const wchar_t* pFmt, ...);

	// �ַ���תСд
	std::string 	StrToLwr(std::string  strBuf);
	std::wstring	StrToLwr(std::wstring strBuf);

	// �ַ���ת��д
	std::string 	StrToUpr(std::string  strBuf);
	std::wstring	StrToUpr(std::wstring strBuf);

	// �ж��ַ��Ƿ�������
	bool		IsNumber(char c);

	// �ж��ַ��Ƿ��Ǵ�д��ĸ
	bool		IsAlphUpr(char c);

	// �ж��ַ��Ƿ���Сд��ĸ
	bool		IsAplhLwr(char c);

	// �ж��ַ��Ƿ�����ĸ
	bool		IsAplh(char c);

	// �ж��ַ��Ƿ��ǲ�����
	bool		IsOperate(char c);

	// �ж��ַ����Ƿ��������
	bool		HasChinese(std::string  strBuf);

	// ��ȡGUID
	std::string 	GetGUIDStr();

	// Base64����
	std::string 	Base64Encode(std::string  strSrc);

	// Base64����
	std::string 	Base64Decode(std::string  strSrc);

	// UrlEncode
	std::string 	UrlEncode(std::string  strBuf);

	// UrlDecode
	std::string 	UrlDecode(std::string  strBuf);


#ifdef GS_USE_CRYPT
	// Sha1����
	std::string 	GetSha1(std::string  strBuf);

	// MD5����
	std::string 	GetMD5(std::string  strBuf);

	// Aes����(���ص����ڴ�����)
	std::string 	Aes128Encode(std::string  strPwd, std::string  strBuf);

	// Aes����
	std::string 	Aes128Decode(std::string  strPwd, std::string  strBuf);

	// Rsa��Կ����(���ص����ڴ�����)RSA_PKCS1_PADDING
	std::string 	RsaPubEncode(std::string  strKey, std::string  strBuf);

	// Rsa��Կ����
	std::string 	RsaPubDecode(std::string  strKey, std::string  strBuf);

	// Rsa˽Կ���ܣ����ص����ڴ����ݣ�RSA_PKCS1_PADDING
	std::string 	RsaPriEncode(std::string  strKey, std::string  strBuf);

	// Rsa˽Կ����
	std::string 	RsaPriDecode(std::string  strKey, std::string  strBuf);

	// ��ȡRsa��Կ��˽Կ��
	bool		GetRsaKey(std::string  &strPubKey, std::string  &strPriKey);


	// Des���� PKCS7
	std::string 	DesEncode(std::string  strKey, std::string  strBuf, bool bEcb = true);

	// Des���� PKCS7
	std::string 	DesDecode(std::string  strKey, std::string  strBuf, bool bEcb = true);

	// 3Des���� PKCS7 ECB
	std::string 	Des3Encode(std::string  strKey, std::string  strBuf);

	// 3Des���� PKCS7 ECB
	std::string 	Des3Decode(std::string  strKey, std::string  strBuf);
	
#endif

	//////////////////////////////////////////////////////////////////
	//	����ִ��Ƿ�������֡����֡��ַ�	add by cherish 2016-08-20
	BOOL		CheckUserPass(const char * iName);

	// ����Ƿ��������
	BOOL		CheckHasChina(std::string  strData);

	// ����תƴ��
	std::string 	ChineseToPinyin(std::string  strData);

	// �ļ���Сת��Ϊ�ַ���
	std::string 	FileSizeToStr(__int64 ulSize);

	// �ļ���Сת��Ϊ�ַ���
	std::wstring	FileSizeToWStr(__int64 ulSize);

	// ����ת�ַ������Դ�дת��1��
	std::string 	IntToStrForCapital(int nValue);

	// ����ת�ַ������Դ�дת��1��
	std::wstring	IntToWStrForCapital(int nValue);

	// �汾�Ƚϣ������Ƿ����°汾�� a.b.c
	bool		VersionCompare(std::string  strOldVer, std::string  strNewVer, std::string  strSplite = ".");
}



