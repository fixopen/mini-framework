// JsonTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../Common/Json/Header/json.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Json::Value json_temp;      //��ʱ���󣬹����´���ʹ��
	json_temp["name"] = Json::Value("huchao");
	json_temp["age"] = Json::Value(26);

	Json::Value root;  //��ʾ����json����
	root["key_string"] = Json::Value("value_string");   //�½�һ��Key����Ϊ��key_string���������ַ���ֵ��"value_string"�� 
	root["key_number"] = Json::Value(12345);            //�½�һ��Key����Ϊ��key_number����������ֵ��12345�� 
	root["key_boolean"] = Json::Value(false);           //�½�һ��Key����Ϊ��key_boolean��������boolֵ��false��
	root["key_double"] = Json::Value(12.345);           //�½�һ��Key����Ϊ��key_double��������doubleֵ��12.345��
	root["key_object"] = json_temp;                     //�½�һ��Key����Ϊ��key_object��������json::Value����ֵ��
	root["key_array"].append("array_string");           //�½�һ��Key����Ϊ��key_array��������Ϊ���飬�Ե�һ��Ԫ�ظ�ֵΪ�ַ�����"array_string"��
	root["key_array"].append(1234);                     //Ϊ����key_array��ֵ���Եڶ���Ԫ�ظ�ֵΪ��1234��
	Json::ValueType type = root.type();                 //���root�����ͣ��˴�ΪobjectValue���͡�

	Json::FastWriter fast_writer;
	std::cout << fast_writer.write(root) << std::endl;

	Json::StyledWriter styled_writer;
	std::cout << styled_writer.write(root) << std::endl;

	Json::Reader reader;
	Json::Value json_object;
	const char* json_document = "{\"age\" : 26,\"name\" : \"huchao\",\"key_object\":{\"age\" : 26,\"name\" : \"huchao\"}}";
	if (!reader.parse(json_document, json_object))
		return 0;

	std::cout << json_object["name"] << std::endl;
	std::cout << json_object["age"] << std::endl;
	std::cout << json_object["key_object"]["name"] << std::endl;
	std::cout << json_object["key_object"]["age"] << std::endl;
	std::cout << json_object["key_object"] << std::endl;

	return 0;
}

