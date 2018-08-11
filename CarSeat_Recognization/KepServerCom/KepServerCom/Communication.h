#include "OPC.h"
#include <thread>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataCtrl
{
public:
	void DataControl(COPC* dOpc);
	int ParseXmlData(char* data, float* num);
	bool WriteData(COPC* dOpc, float data);
	float ReadData(COPC* dOpc);
	void exit();
	CDataCtrl();
	virtual ~CDataCtrl();
	//static void communicate(CDataCtrl* pThis, CSocket * serverSocket, COPC* dOpc);
private:
	int Dport;
	bool exitFlag;
	CSocket aSocket;
	CSocket serverSocket;
	//std::thread m_SocketThread;
};