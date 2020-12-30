/**  * @copyright (C) 2016 ChinaNetCenter Inc. All rights reserved.
*
* @file NSJson.h
* @summary  NSJsonObject	Json����
*			NSJsonArray		Json����
*			NSJson			Json����
*
* @version 1.0
* @author zhengzc@wangsu.com
* @date 2018-03-05 14:25:06
*
* @others
* @MODIFY    VERSION  YYYYMMDD    TITLE
* zhengzc      1.0    20180320     title
*
*/


#ifndef NSJsonH
#define NSJsonH

#define JSON_HAS_INT64
#include "json.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


class NSJsonArray;
class NSJsonObject
{

#define NSJsonObj_NULL NSJsonObject()

public:

	// ���캯��
	NSJsonObject();

	// ��������
	~NSJsonObject();

public:

	// ��ȡJson::Value����
	Json::Value	_GetValue();

	// ����Json::Value
	void		_SetValue(Json::Value Value);

	__declspec(property (get = _GetValue, put = _SetValue)) Json::Value Value;

public:

	// �������
	void		Put(std::string Key, int Value, bool Isbool = false);

	void		Put(std::string Key, unsigned int Value);

	void		Put(std::string Key, __int64 Value);

	void		Put(std::string Key, unsigned __int64 Value);

	void		Put(std::string Key, double Value);

	void		Put(std::string Key, float Value);

	void		Put(std::string Key, std::string Value);

	void		Put(std::string Key, NSJsonObject JsonObj);

	void		Put(std::string Key, NSJsonArray JsonArr);

	// �ж�ָ������ֵ������
	bool		IsInt(std::string Key);

	bool		IsUInt(std::string Key);

	bool		IsInt64(std::string Key);

	bool		IsUInt64(std::string Key);

	bool		IsBool(std::string Key);

	bool		IsDouble(std::string Key);

	bool		IsString(std::string Key);

	bool		IsObject(std::string Key);

	bool		IsArray(std::string Key);

	//����Ĭ��ֵ
	bool				_GetDefaultBool(std::string key);
	int					_GetDefaultInt(std::string key);
	float				_GetDefaultFloat(std::string key);
	std::string			_GetDefaultString(std::string key);
	NSJsonObject		_GetDefaultObject(std::string key);
	NSJsonArray			_GetDefaultArray(std::string key);

	// ��ȡֵ��Key��Ӧ�ļ�ֵ�Ա�����ڣ�
	bool				GetBool(std::string Key);

	int					GetInt(std::string Key);

	unsigned int		GetUInt(std::string Key);

	double				GetDouble(std::string Key);

	float				GetFloat(std::string Key);

	std::string			GetString(std::string Key);

	NSJsonObject		Get(std::string Key);

	NSJsonArray			GetArr(std::string Key);


	// ��ȡֵ��Key��Ӧ�ļ�ֵ���Բ����ڣ�
	bool				OptGetBool(std::string Key, bool Default = false);

	int					OptGetInt(std::string Key, int Default = 0);

	unsigned int		OptGetUInt(std::string Key, unsigned int Default = 0);

	double				OptGetDouble(std::string Key, double Default = 0);

	float				OptGetFloat(std::string Key, float Default = 0);

	std::string			OptGetString(std::string Key, std::string Default = "");

	NSJsonObject		OptGet(std::string Key, NSJsonObject Default = NSJsonObj_NULL);


	// �ж�ָ������ֵ�Ƿ�Ϊ��(��NameΪ�����жϵ�ǰ����)
	bool				IsNull(std::string Key = "");

	// ��ȡ����Key
	std::vector<std::string>	GetKeys();

	// �Ƴ���ֵ��
	void				Remove(std::string Key);

	// ��ȡ��ֵ������
	unsigned int		Size();

	// �Ƚ�����Obj
	int					Compare(NSJsonObject JsonObj);

	// ���
	void				Clear();

	// ��ȡJson��ʽ�ַ���
	std::string			ToString(bool bStyled = true);

	// ת����NSJsonArray
	NSJsonArray			ToArray();


private:

	Json::Value		m_Obj;
};

class NSJsonArray
{

public:

	// ���캯��
	NSJsonArray();

	NSJsonArray(const std::vector<int> &vectList);

	NSJsonArray(const std::vector<std::string> &vectList);

	// ��������
	~NSJsonArray();

public:

	// ��ȡJson::Value����
	Json::Value	_GetValue();

	// ����Json::Value
	void		_SetValue(Json::Value Value);

	__declspec(property (get = _GetValue, put = _SetValue)) Json::Value Value;

public:

	// �Ƿ�Ϊ�ն���
	bool				IsNull();

	// �������
	unsigned int		Size();

	// ��ȡNSJsonObject
	NSJsonObject		Get(unsigned int unIndex);

	std::string			GetString(unsigned int unIndex);

	int					GetInt(unsigned int unIndex);

	bool				GetBool(unsigned int unIndex);

	// ���
	void				Add(NSJsonObject JsonObj);

	void				AddArray(NSJsonArray JsonObj);

	void				AddInt(int nValue);

	void				AddString(std::string strValue);

	// �Ƴ�
	void				Remove(unsigned int unIndex);

	// ���
	void				Clear();

	// ת����NSJsonObject
	NSJsonObject		ToObject();

	// ��ȡJson��ʽ�ַ���
	std::string			ToString(bool bStyled = true);

private:

	Json::Value		m_Obj;

};


class NSJson
{
public:

	// ���캯��
	NSJson();

	// ��������
	~NSJson();

public:

	// ����Json��ʽ�ַ���
	static NSJsonObject ParseStr(std::string strJson);

	// ��ȡ�ļ�������json
	static NSJsonObject ParaseFile(std::string strFile);

	// ���浽�ļ�
	static bool SaveToFile(std::string strFile, NSJsonObject JsonObj);
};

#endif