

#include "../Header/NSJson.h"

#include <fstream>
#include <stdexcept>
#include <memory>
#include <iostream>




/*
*	Function:		StrTrim
*	Explanation:	ȥ���ַ������߳��ֵ��ַ�(��strChars�г��ֵĶ�ɾ��)
*	Input:			strBuf		�ַ���
strChars	��Ҫȥ�����ַ�
*	Output:			strBuf		ȥ������ַ���
*	Return:			ȥ�����ַ����Ĵ�С
*/
int StrTrim(std::string &strBuf, std::string strChars /* = " " */)
{
	std::string::iterator				Iter;		// ���������
	std::string::reverse_iterator		reIter;		// ���������

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









/*
 *	Function:		NSJsonObject
 *	Explanation:	���캯��
 */
NSJsonObject::NSJsonObject()
{

}

/*
 *	Function:		~NSJsonObject
 *	Explanation:	��������
 */
NSJsonObject::~NSJsonObject()
{

}

/*
 *	Function:		_GetValue
 *	Explanation:	��ȡJson::Value����
 *	Return:			Json::Value����
 */
Json::Value NSJsonObject::_GetValue()
{
	return m_Obj;
}

/*
 *	Function:		_SetValue
 *	Explanation:	����Json::Value
 *	Input:			Value
 *  Output:			���
 *	Return:			TRUE �ɹ� FALSE ʧ��
 */
void NSJsonObject::_SetValue(Json::Value Value)
{
	m_Obj = Value;
}

/*
 *	Function:		Put
 *	Explanation:	�������
 *	Input:			Key			��
					Value		ֵ
 */
void NSJsonObject::Put(std::string Key, int Value, bool Isbool /* = false */)
{
	if (!Isbool)
	{
		m_Obj[Key] = Value;
	}
	else
	{
		if (0 == Value)
		{
			m_Obj[Key] = false;
		}
		else
		{
			m_Obj[Key] = true;
		}
	}
	
}

void NSJsonObject::Put(std::string Key, unsigned int Value)
{
	m_Obj[Key] = Value;
}
void NSJsonObject::Put(std::string Key, double Value)
{
	m_Obj[Key] = Value;
}

void NSJsonObject::Put(std::string Key, float Value)
{
	m_Obj[Key] = Value;
}

void NSJsonObject::Put(std::string Key, std::string Value)
{
	m_Obj[Key] = Value;
}

void NSJsonObject::Put(std::string Key, NSJsonObject JsonObj)
{
	m_Obj[Key] = JsonObj.Value;
}

void NSJsonObject::Put(std::string Key, NSJsonArray JsonArr)
{
	m_Obj[Key] = JsonArr.Value;
}

/*
 *	Function:		IsInt
 *	Explanation:	�ж�ָ������ֵ������
 *	Input:			Key			��
 *	Return:			true �� false ����
 */
bool NSJsonObject::IsInt(std::string Key)
{
	return m_Obj[Key].isInt();
}

bool NSJsonObject::IsUInt(std::string Key)
{
	return m_Obj[Key].isUInt();
}



bool NSJsonObject::IsBool(std::string Key)
{
	return m_Obj[Key].isBool();
}

bool NSJsonObject::IsDouble(std::string Key)
{
	return m_Obj[Key].isDouble();
}

bool NSJsonObject::IsString(std::string Key)
{
	return m_Obj[Key].isString();
}

bool NSJsonObject::IsObject(std::string Key)
{
	return m_Obj[Key].isObject();
}

bool NSJsonObject::IsArray(std::string Key)
{
	return m_Obj[Key].isArray();
}

/*
 *	Function:		GetBool
 *	Explanation:	��ȡֵ
 *	Input:			Key			��
 *	Return:			��Ӧ��ֵ����
 */
bool NSJsonObject::GetBool(std::string Key)
{
	if (IsNull(Key) || m_Obj[Key].isBool() == false)
	{
		throw std::invalid_argument("key not exist");
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultBool(Key);
	}

	return m_Obj[Key].asBool();

}

int NSJsonObject::GetInt(std::string Key)
{
	if (IsNull(Key) || m_Obj[Key].isInt() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultInt(Key);
	}

	return m_Obj[Key].asInt();
}

unsigned int NSJsonObject::GetUInt(std::string Key)
{
	if (IsNull(Key) ||  m_Obj[Key].isUInt() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultInt(Key);
	}

	return m_Obj[Key].asUInt();
}


double NSJsonObject::GetDouble(std::string Key)
{
	if (IsNull(Key) || m_Obj[Key].isDouble() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultFloat(Key);
	}

	return m_Obj[Key].asDouble();
}


std::string NSJsonObject::GetString(std::string Key)
{
	if (IsNull(Key) || m_Obj[Key].isString() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultString(Key);
	}

	return m_Obj[Key].asString();
}

NSJsonObject NSJsonObject::Get(std::string Key)
{
	NSJsonObject			Obj;

	if (IsNull(Key) || m_Obj[Key].isObject() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultObject(Key);
	}

	Obj.Value = m_Obj[Key];

	return Obj;
}

NSJsonArray NSJsonObject::GetArr(std::string Key)
{
	NSJsonArray			Arr;

	if (IsNull(Key) || m_Obj[Key].isArray() == false)
	{
		//_THROW(std::invalid_argument, "key not exist");
		return _GetDefaultArray(Key);
	}

	Arr.Value = m_Obj[Key];

	return Arr;
}

/*
 *	Function:		OptGetBool
 *	Explanation:	��ȡֵ��Key��Ӧ�ļ�ֵ���Բ����ڣ�
 *	Input:			Key			��
					Default		Ĭ��ֵ
 *	Return:			ֵ
 */
bool NSJsonObject::OptGetBool(std::string Key, bool Default /* = false */)
{
	if (IsNull(Key) || m_Obj[Key].isBool() == false)
	{
		return Default;
	}

	return m_Obj[Key].asBool();
}

int NSJsonObject::OptGetInt(std::string Key, int Default /* = 0 */)
{
	if (IsNull(Key) || m_Obj[Key].isInt() == false)
	{
		return Default;
	}

	return m_Obj[Key].asInt();
}

unsigned int NSJsonObject::OptGetUInt(std::string Key, unsigned int Default /* = 0 */)
{
	if (IsNull(Key) || m_Obj[Key].isUInt() == false)
	{
		return Default;
	}

	return m_Obj[Key].asUInt();
}


double NSJsonObject::OptGetDouble(std::string Key, double Default /* = 0 */)
{
	if (IsNull(Key) || m_Obj[Key].isDouble() == false )
	{
		return Default;
	}

	return m_Obj[Key].asDouble();
}



std::string NSJsonObject::OptGetString(std::string Key, std::string Default /* = "" */)
{
	if (IsNull(Key) || m_Obj[Key].isString() == false)
	{
		return Default;
	}

	return m_Obj[Key].asString();
}

NSJsonObject NSJsonObject::OptGet(std::string Key, NSJsonObject Default /* = NSJsonObj_NULL */)
{
	NSJsonObject			Obj;

	if (IsNull(Key) || m_Obj[Key].isObject() == false)
	{
		return Default;
	}

	Obj.Value = m_Obj[Key];

	return Obj;
}

/*
*	Function:		IsNull
*	Explanation:	�ж�ָ������ֵ�Ƿ�Ϊ��(��NameΪ�����жϵ�ǰ����)
*	Input:			Key			��
*	Return:			true �� false �ǿ�
*/
bool NSJsonObject::IsNull(std::string Key /* = "" */)
{
	if (Key.empty())
	{
		return m_Obj.isNull();
	}
	else
	{
		return !m_Obj.isMember(Key);
	}
}

/*
 *	Function:		GetKeys
 *	Explanation:	��ȡ����Key
 *	Return:			Key�б�
 */
std::vector<std::string> NSJsonObject::GetKeys()
{
	std::vector<std::string>		vecList;
	Json::Value::Members	memList;

	memList = m_Obj.getMemberNames();

	for (size_t stIndex = 0; stIndex < memList.size(); stIndex ++)
	{
		vecList.push_back(memList[stIndex]);
	}

	return vecList;
}

/*
 *	Function:		Remove
 *	Explanation:	�Ƴ���ֵ��
 *	Input:			Key			��
 */
void NSJsonObject::Remove(std::string Key)
{
	if (IsNull(Key))
	{
		return;
	}

	m_Obj.removeMember(Key);
}

/*
 *	Function:		Size
 *	Explanation:	��ȡ��ֵ������
 *	Return:			��ֵ������
 */
unsigned int NSJsonObject::Size()
{
	return m_Obj.size();
}

/*
 *	Function:		Compare
 *	Explanation:	�Ƚ�����Obj
 *	Input:			����
 *  Output:			���
 *	Return:			TRUE �ɹ� FALSE ʧ��
 */
int NSJsonObject::Compare(NSJsonObject JsonObj)
{
	return m_Obj.compare(JsonObj.Value);
}

/*
 *	Function:		Clear
 *	Explanation:	���
 */
void NSJsonObject::Clear()
{
	m_Obj.clear();
}

/*
 *	Function:		ToString
 *	Explanation:	��ȡJson��ʽ�ַ���
 *	Return:			Json��ʽ�ַ���
 */
std::string NSJsonObject::ToString(bool bStyled /* = true */)
{
	if (bStyled)
	{
		return m_Obj.toStyledString();
	}

	Json::FastWriter	writer;
	std::string jsonStr = writer.write(m_Obj);
	StrTrim(jsonStr, "\n");
	return jsonStr;
}

/*
 *	Function:		ToArray
 *	Explanation:	ת����NSJsonArray
 *	Return:			NSJsonArray
 */
NSJsonArray NSJsonObject::ToArray()
{
	NSJsonArray			JsonArr;

	JsonArr.Value = m_Obj;

	return JsonArr;
}

bool NSJsonObject::_GetDefaultBool(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return false;
}

int NSJsonObject::_GetDefaultInt(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return 0;
}

float NSJsonObject::_GetDefaultFloat(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return 0.0;
}

std::string NSJsonObject::_GetDefaultString(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return "";
}

NSJsonObject NSJsonObject::_GetDefaultObject(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return NSJsonObject();
}

NSJsonArray NSJsonObject::_GetDefaultArray(std::string key)
{
	//GSLOG_WARN("key %s not exist", key.c_str());
	return NSJsonArray();
}





/*
 *	Function:		NSJsonArray
 *	Explanation:	���캯��
 */
NSJsonArray::NSJsonArray() : m_Obj(Json::arrayValue)
{
}

NSJsonArray::NSJsonArray(const std::vector<int> &vectList)
{
	for (auto item : vectList)
	{
		m_Obj.append(item);
	}
}

NSJsonArray::NSJsonArray(const std::vector<std::string> &vectList)
{
	for (auto item : vectList)
	{
		m_Obj.append(item);
	}
}

/*
 *	Function:		~NSJsonArray
 *	Explanation:	��������
 */
NSJsonArray::~NSJsonArray()
{

}

/*
*	Function:		_GetValue
*	Explanation:	��ȡJson::Value����
*	Return:			Json::Value����
*/
Json::Value NSJsonArray::_GetValue()
{
	return m_Obj;
}

/*
*	Function:		_SetValue
*	Explanation:	����Json::Value
*	Input:			Value
*  Output:			���
*	Return:			TRUE �ɹ� FALSE ʧ��
*/
void NSJsonArray::_SetValue(Json::Value Value)
{
	m_Obj = Value;
}

/*
 *	Function:		IsNull
 *	Explanation:	�Ƿ�Ϊ�ն���
 *	Return:			true �� false ����
 */
bool NSJsonArray::IsNull()
{
	return m_Obj.isNull();
}

/*
 *	Function:		Size
 *	Explanation:	�������
 *	Return:			�������
 */
unsigned int NSJsonArray::Size()
{
	return m_Obj.size();
}

/*
 *	Function:		Get
 *	Explanation:	��ȡNSJsonObject
 *	Input:			unIndex		����
 *	Return:			NSJsonObject
 */
NSJsonObject NSJsonArray::Get(unsigned int unIndex)
{
	NSJsonObject			JsonObj;

	if (!m_Obj.isValidIndex(unIndex))
	{
		throw std::invalid_argument("beyond the index");
		//_THROW(std::invalid_argument, "beyond the index");
	}
	else
	{
		JsonObj.Value = m_Obj[unIndex];
	}
	
	return JsonObj;
}

std::string NSJsonArray::GetString(unsigned int unIndex)
{
	std::string				strValue;

	if (!m_Obj.isValidIndex(unIndex))
	{
		throw std::invalid_argument("beyond the index");
		//_THROW(std::invalid_argument, "beyond the index");
	}
	else
	{
		strValue = m_Obj[unIndex].asString();
	}

	return strValue;
}

int NSJsonArray::GetInt(unsigned int unIndex)
{
	int						nValue;

	if (!m_Obj.isValidIndex(unIndex))
	{
		throw std::invalid_argument("beyond the index");
		//_THROW(std::invalid_argument, "beyond the index");
	}
	else
	{
		nValue = m_Obj[unIndex].asInt();
	}

	return nValue;
}

bool NSJsonArray::GetBool(unsigned int unIndex)
{
	bool					bValue;

	if (!m_Obj.isValidIndex(unIndex))
	{
		throw std::invalid_argument("beyond the index");
		//_THROW(std::invalid_argument, "beyond the index");
	}
	else
	{
		bValue = m_Obj[unIndex].asBool();
	}

	return bValue;
}

/*
 *	Function:		Add
 *	Explanation:	���
 *	Input:			JsonObj		json����
 */
void NSJsonArray::Add(NSJsonObject JsonObj)
{
	if (!m_Obj.isArray())
	{
		return;
	}

	m_Obj.append(JsonObj.Value);
}

void NSJsonArray::AddArray(NSJsonArray JsonObj)
{
	if (!m_Obj.isArray())
	{
		return;
	}

	m_Obj.append(JsonObj.Value);
}

void NSJsonArray::AddInt(int nValue)
{
	m_Obj.append(nValue);
}

void NSJsonArray::AddString(std::string strValue)
{
	m_Obj.append(strValue);
}


/*
 *	Function:		Clear
 *	Explanation:	���
 */
void NSJsonArray::Clear()
{
	m_Obj.clear();
}

/*
 *	Function:		ToObject
 *	Explanation:	ת����NSJsonObject
 *	Return:			NSJsonObject
 */
NSJsonObject NSJsonArray::ToObject()
{
	NSJsonObject			JsonObj;

	JsonObj.Value = m_Obj;

	return JsonObj;
}

/*
 *	Function:		ToString
 *	Explanation:	��ȡJson��ʽ�ַ���
 *	Return:			Json��ʽ�ַ���
 */
std::string NSJsonArray::ToString(bool bStyled /* = true */)
{
	if (bStyled)
	{
		return m_Obj.toStyledString();
	}

	Json::FastWriter	writer;
	std::string  jsonStr = writer.write(m_Obj);
	StrTrim(jsonStr, "\n");
	return jsonStr;
}



/*
 *	Function:		NSJson
 *	Explanation:	���캯��
 */
NSJson::NSJson()
{

}

/*
 *	Function:		~NSJson
 *	Explanation:	��������
 */
NSJson::~NSJson()
{

}

/*
 *	Function:		ParseStr
 *	Explanation:	����Json��ʽ�ַ���
 *	Input:			strJson		
 *	Return:			NSJsonObject
 */
NSJsonObject NSJson::ParseStr(std::string strJson)
{
	NSJsonObject			JsonObj;
	Json::Reader			Reader;
	Json::Value				Value;

	if (strJson.empty())
	{
		return JsonObj;
	}

	if (false == Reader.parse(strJson, Value))
	{
		return JsonObj;
	}

	JsonObj.Value = Value;
	return JsonObj;
}

/*
 *	Function:		ParaseFile
 *	Explanation:	��ȡ�ļ�������json
 *	Input:			strFile		�ļ�·��
 *	Return:			NSJsonObject
 */
NSJsonObject NSJson::ParaseFile(std::string strFile)
{
	NSJsonObject			JsonObj;
	Json::Reader			Reader;
	Json::Value				Value;
	std::ifstream			Stream;

	Stream.open((char *)strFile.c_str(), std::ios::binary);

	if (false == Stream.is_open())
	{
		return JsonObj;
	}

	if (false == Reader.parse(Stream, Value, false))
	{
		Stream.close();

		return JsonObj;
	}

	JsonObj.Value = Value;
	Stream.close();

	return JsonObj;
}



/*
 *	Function:		SaveToFile
 *	Explanation:	���浽�ļ�
 *	Input:			strFile		�ļ�·��
					JsonObj		Json����
 *	Return:			true �ɹ� false ʧ��
 */
bool NSJson::SaveToFile(std::string strFile, NSJsonObject JsonObj)
{
	std::string				strJson;
	std::ofstream			Stream;

	strJson = JsonObj.ToString();
	Stream.open((char *)strFile.c_str(), std::ios::binary | std::ios::trunc);

	if (false == Stream.is_open())
	{
		return false;
	}

	Stream.write((char *)strJson.c_str(), strJson.size());
	Stream.close();
	return true;
}