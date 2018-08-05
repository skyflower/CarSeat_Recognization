#pragma once

#include <winsock.h>

class CKepServerSocket
{
public:
	CKepServerSocket(unsigned int ip, unsigned int port);

	~CKepServerSocket();

	bool SetError();

	bool SetWarning();
	
	bool SetCorrect();

	void HeartBlood();

	void resetConnect();

	bool GetSocketStatus();


private:

	//void resetConnect();

	bool SetValue(int value);

	bool SendMessageToServer(char *msg, int len, char *recv, int &recvLen);


	SOCKET initSocket(unsigned int ip, unsigned int port);

	SOCKET m_nSocket;
	unsigned int m_nIp;
	unsigned int m_nPort;
};

