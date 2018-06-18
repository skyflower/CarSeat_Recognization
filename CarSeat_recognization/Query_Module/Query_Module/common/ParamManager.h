#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

/*

���������ļ�config.txt
���ļ��еĲ������ᶯ̬�ı�

*/

class CParamManager
{
public:
	CParamManager();
	~CParamManager();

	static CParamManager* GetInstance();

	unsigned int GetLocalIP();

	unsigned int GetServerIP();
	unsigned int GetServerPort();
	
	const std::vector<std::wstring> *GetFtpParameter()const;
	const std::vector<std::wstring> *GetMethodType()const;
	const std::vector<std::wstring> *GetSeatType()const;
	const std::vector<std::wstring> *GetLineNo()const;


	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	

	unsigned int __auxLocalIP();

	// serverip�� ������Ip
	size_t m_nServerIp;

	// serverport ������ͨ�Ŷ˿ں�
	int m_nServerPort;

	// �����ͷ�����ͨ�ŵ�Ip
	size_t m_nLocalIp;

	// ������
	std::wstring m_strLocalName;

	//ftp�û��������Լ�ͼ��Ĵ洢·��
	std::vector<std::wstring> *m_pFtp;

	//���߱��
	std::vector<std::wstring> *m_pLineVec;

	//methodType
	std::vector<std::wstring> *m_pMethodType;

	//seatType
	std::vector<std::wstring> *m_pSeatType;
};

