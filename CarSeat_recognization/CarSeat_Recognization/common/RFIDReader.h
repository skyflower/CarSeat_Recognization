#pragma once
#include <iostream>
#include <Windows.h>

class CRFIDReader
{
public:
	/*enum class Status
	{
		RFID_INIT,
		RFID_CONNECT
		
	};*/

	enum class ErrorType
	{
		ERROR_OK,
		ERROR_SOCKET_INIT,
		ERROR_SOCKET_INVALID,
		ERROR_SOCKET_CREATE,
		ERROR_SOCKET_CONNECT,
		ERROR_SOCKET_CLOSED,
		ERROR_SOCKET_SEND,
		ERROR_SOCKET_RECV,
		ERROR_BUFFER_NULL,
		ERROR_XML_PARSE,
		ERROR_XML_LABEL
		//ERROR_
	};


	CRFIDReader();
	~CRFIDReader();


	/*
	读取条形码接口，其他暂时没有实现
	*/
	std::string readBarcode();

	ErrorType initRFID(unsigned int rfidIp, int port);

	ErrorType reset(const char* param);



	// not implement
	ErrorType isConnect();


private:
	bool init();

	/*
	连接rfid通讯
	*/
	

	ErrorType hostGreetings(SOCKET fd);

	ErrorType hostGoodbye(SOCKET fd);

	ErrorType startRead(SOCKET fd);

	ErrorType stopRead(SOCKET fd);

	ErrorType heartBeat(SOCKET fd);


	/*
	此操作会造成和rfid断开连接
	*/
	ErrorType setIPConfig(SOCKET fd, char *Ip, char* Netmask, char *Gateway, bool enableDHCP);

	/*
	
	*/
	ErrorType resetReader(SOCKET fd, char *resetType);

	ErrorType parseReplyPackage(char *buffer, size_t length, int &id, int &resultCode);

	ErrorType __communicate(SOCKET fd, char *buffer, int length);

	/*
	提取ascii码形式的条形码
	*/
	bool parseBarcode(const char* text, char *barcode);


	char m_szDefaultIp[16];			// 192.168.0.254
	char m_szDefaultNetmask[16];	// 255.255.255.0
	unsigned short m_nDefaultPort;	// 10001
	char m_szDefaultGatewap[16];	// 192.168.0.1
	bool m_bDefaultDHCP;			////关

	SOCKET m_nSocket;

	int m_nSessionID;
	char m_szVersion[16];
	char m_szReadType[16];
	char m_szReadMode[16];

	/*
	当前条形码
	*/
	char m_szCurrentValue[50];

};

