#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"
#include "../ConditionFilter.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"

class CNetworkTask
{
public:

	CNetworkTask(unsigned int serverIp, unsigned short port);
	~CNetworkTask();
	
	static bool IsReachable(unsigned int clientIp, unsigned int serverIp);

	bool heartBlood(unsigned int serverIp, unsigned int port);

	std::vector<RecogResultW> *QueryRecogInfo(CConditionFilterA & filter, unsigned int ip, const std::string &userName, const std::string &passwd);


	bool UpdateBarcodeCheckList(std::unordered_map<std::string, std::string> *pBarcode, \
		std::unordered_map<std::string, std::string> *pClassify);
	
private:
	

	bool __sendToServer(SOCKET & socketFD, const char *sendMsg, \
		size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen);

	std::vector<RecogResultW>* ParseQueryResult(char *info, int length);

	bool ParseRecogResult(TiXmlElement * elment, RecogResultA &recog);

	bool replyMessage(SOCKET & socketFD, char *rootName, char *packageName, char* status);

	bool replayFunc(char *xml, char* packageName);

	bool _aux_connect();

	SOCKET m_nSocket;

	unsigned int m_nIp;
	unsigned short m_nPort;

};

