#pragma once
#include <iostream>
#include <Windows.h>

class CRFIDReader
{
public:
	enum class Status
	{
		RFID_INIT,
		RFID_CONNECT
		
	};


	CRFIDReader();
	~CRFIDReader();

private:
	bool init();

	/*
	连接rfid通讯
	*/
	bool initRFID(unsigned int rfidIp, int port);

	bool isConnect(SOCKET fd);

	bool hostGreetings(SOCKET fd);



	char m_szDefaultIp[16];			// 192.168.0.254
	char m_szDefaultNetmask[16];	// 255.255.255.0
	unsigned short m_nDefaultPort;	// 10001
	char m_szDefaultGatewap[16];	// 192.168.0.1
	bool m_bDefaultDHCP;			////关

	SOCKET m_nSocket;



};

