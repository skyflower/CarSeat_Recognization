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
	unsigned int GetTestServerPort();
	unsigned int GetTestClientPort();

	std::vector<std::wstring> *GetFtpParameter();
	std::wstring FindCameraByLineID(std::wstring lineID);

	unsigned int GetBarcodeIp();
	unsigned int GetBarcodePort();

	const char*GetGraphFile() const;
	const char*GetLabelFile() const;

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

	// �ͷ�����������ͨ�ķ������˿ں�
	int m_nTestServerPort;

	// �ͷ�����������ͨ�Ŀͻ��˶˿ں�
	int m_nTestClientPort;

	// ��ȡ�������ip
	size_t m_nBarcodeIp;

	//��ȡ������Ķ˿ں�
	size_t m_nBarcodePort;

	// ������
	std::wstring m_strLocalName;

	//ftp�û��������Լ�ͼ��Ĵ洢·��
	std::vector<std::wstring> *m_pFtp;

	//graph
	std::string m_szGraphFile;

	std::string m_szLabelFile;

	//���ߺ�����Ķ�Ӧ��ϵ
	std::unordered_map<std::wstring, std::wstring> *m_pLineCamera;
};

