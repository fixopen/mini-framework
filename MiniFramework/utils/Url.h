/*!
* \file url.h
*��װurl�Ľ���
* \author zhengzc
* \date ���� 2018
*
*
*/
#pragma  once

#include <string>

using namespace std;

//Scheme :// hostname[:port] / path / [;parameters][?query]#fragment
//http://www.google.com/adsense/support/bin/request.py?contact=afv_violation&url=null&client=ca%2Dgames%2Dpub%2D9606551472994074&videoid=0
namespace common
{
	class Url
	{
	public:
		Url(const char* url);
		//http
		const string GetScheme() const
		{
			return m_scheme;
		}
		//������  www.google.com
		const string GetHost() const
		{
			return m_host;
		}
		//�˿ں�
		const string GetPort() const
		{
			return m_port;
		}
		//���������/�����Ÿ������ַ�����һ��������ʾ�����ϵ�һ��Ŀ¼���ļ���ַ��
		///adsense/support/bin/request.py
		const string GetPath() const
		{
			return m_path;
		}
		//��ѯ�����ڸ���̬��ҳ���ݲ��������ж���������á�&�����Ÿ�����ÿ������������ֵ�á� = �����Ÿ�����
		//contact=afv_violation&url=null&client=ca%2Dgames%2Dpub%2D9606551472994074&videoid=0
		const string GetQuery() const
		{
			return m_query;
		}
		//��ϢƬ�ϣ�����ָ��������Դ�е�Ƭ�ϡ�����һ����ҳ���ж�����ʽ��ͣ���ʹ��fragmentֱ�Ӷ�λ��ĳһ���ʽ��͡�
		const string GetFragment() const
		{
			return m_fragment;
		}
		const string GetUserName() const
		{
			return m_username;
		}
		const string GetPassword() const
		{
			return m_password;
		}
	private:
		bool Init(const char* url);
		bool IsSchemeChar(int c)
		{
			return (!isalpha(c) && '+' != c && '-' != c && '.' != c) ? 0 : 1;
		}
	private:
		string m_scheme;               /* mandatory */
		string m_host;                 /* mandatory */
		string m_port;                 /* optional */
		string m_path;                 /* optional */
		string m_query;                /* optional */
		string m_fragment;             /* optional */
		string m_username;             /* optional */
		string m_password;             /* optional */
	};
}

