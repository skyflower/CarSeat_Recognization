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

	unsigned int GetKepServerIp();
	unsigned int GetKepServerPort();


	std::vector<std::wstring> *GetFtpParameter();
	std::wstring FindCameraByLineID(std::wstring lineID);

	unsigned int GetBarcodeIp();
	unsigned int GetBarcodePort();

	const char* GetGraphFile() const;
	const char* GetLabelFile() const;
	const char* GetImageDirectory() const;
	const char* GetLineName() const;
	const char* GetCameraName() const;
	const char* GetPatternImagePath() const;

	//��ʱ�洢ʶ��ģ���ͼ��·��
	const char* GetCacheImagePath() const;


	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	

	unsigned int __auxLocalIP();

	// serverip�� ������Ip
	unsigned int m_nServerIp;

	// serverport ������ͨ�Ŷ˿ں�
	int m_nServerPort;

	// �����ͷ�����ͨ�ŵ�Ip
	unsigned int m_nLocalIp;

	// �ͷ�����������ͨ�ķ������˿ں�
	int m_nTestServerPort;

	// �ͷ�����������ͨ�Ŀͻ��˶˿ں�
	int m_nTestClientPort;

	// ��ȡ�������ip
	unsigned int m_nBarcodeIp;

	//��ȡ������Ķ˿ں�
	unsigned int m_nBarcodePort;

	// kepServerģ��ͨ��ip��ַ
	unsigned int m_nKepServerIp;

	// kepServerģ��ͨ�Ŷ˿�
	unsigned int m_nKepServerPort;

	//����ͼ�񱾵ر���·��
	char m_szImagePath[256];

	//ģ��ͼ��·��
	char m_szPatternImagePath[256];

	//�������Ʊ��
	char m_szLineName[20];

	//������ֻ������кŵ�
	char m_szCameraName[100];

	//ʶ��ģ����ʱ�洢��ͼ��·��
	char m_szCacheImagePath[256];

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

