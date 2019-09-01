
#include <iostream>
#include <Windows.h>
#include "LabelManager.h"
#include "Log.h"
#include "utils.h"
#include "ParamManager.h"
#include "RFIDReader.h"


CParamManager *m_pParamManager = nullptr;
CLog *m_pLog = nullptr;
CLabelManager *m_pLabelManager = nullptr;
CRFIDReader *pRFIDReader = nullptr;

void initSystem()
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		std::cout << "init wsa start up failed" << std::endl;
	}

	m_pParamManager = CParamManager::GetInstance();
	WriteInfo("get CParamManager success");
	m_pLog = CLog::GetInstance();

	WriteInfo("Get plog success");

	pRFIDReader = new CRFIDReader();
	WriteInfo("new rfid reader success");
	pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam());

	m_pLabelManager = new CLabelManager();
	WriteInfo("Get m_pLabelManager success");
}

void deInitSystem()
{
	WSACleanup();

	delete pRFIDReader;
	pRFIDReader = nullptr;

	delete m_pLabelManager;
	m_pLabelManager = nullptr;

	delete m_pParamManager;
	m_pParamManager = nullptr;

	delete m_pLog;
	m_pLog = nullptr;
}

void scanDirectory(char *lpPath, char *filePart, std::vector<std::string> &fileList)
{
	char szFind[MAX_PATH];
	char szFile[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy_s(szFind, lpPath);
	strcat_s(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)    return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strcpy_s(szFile, lpPath);
				strcat_s(szFile, "\\");
				strcat_s(szFile, FindFileData.cFileName);
				scanDirectory(szFile, filePart, fileList);
			}
		}

		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) && (strstr(FindFileData.cFileName, filePart) != NULL))
		{
			//std::cout << FindFileData.cFileName << std::endl;
			char tmpAbsPath[500];
			sprintf_s(tmpAbsPath, sizeof(tmpAbsPath), "%s\\%s", lpPath, FindFileData.cFileName);
			fileList.push_back(std::string(tmpAbsPath));
		}
		WriteInfo("find file: %s", FindFileData.cFileName);
		if (!FindNextFile(hFind, &FindFileData))
		{
			break;
		}
	}
	FindClose(hFind);
}

std::string ipToStr(unsigned int ipInt)
{
	char buffer[20];
	memset(buffer, 0, sizeof(buffer));
	ULONG ipAddr = htonl(ipInt);
	struct in_addr addr1;
	memcpy(&addr1, &ipAddr, 4);
	return inet_ntoa(addr1);
}

void readBarcode()
{
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	if ((m_pLabelManager->GetObtainBarcodeFunction() == true)	\
		&& (pRFIDReader->isConnect() != CRFIDReader::ErrorType::ERROR_OK))
	{
		std::cout << "m_pLabelManager->GetObtainBarcodeFunction() = " << m_pLabelManager->GetObtainBarcodeFunction() << std::endl;
		unsigned int rfidIP = m_pParamManager->GetBarcodeIp();
		unsigned int rfidPort = m_pParamManager->GetBarcodePort();
		pRFIDReader->initRFID(rfidIP, rfidPort);
		pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam());
		//if (CRFIDReader::ErrorType::ERROR_SOCKET_INVALID == pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam()))
		{
			//pRFIDReader->initRFID(rfidIP, rfidPort);
			//pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam());
		}
		std::cout << ipToStr(rfidIP).c_str() << ":" << rfidPort << "\t disconnect, re init and connect" << std::endl;
		
		std::chrono::duration<int, std::milli> b = std::chrono::milliseconds(500);
		std::this_thread::sleep_for(b);

		return;
	}
	std::string tmpBarcode;
	int ret = 0;
	pRFIDReader->readBarcode(m_pLabelManager->GetObtainBarcodeFunction(), tmpBarcode);
	if (ret == 1)
	{
		std::cout << "barcode = " << tmpBarcode.c_str() << std::endl;
	}
	else
	{
		std::cout << "get no barcode" << std::endl;
	}
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}



int main(int argc, char *argv[])
{
	initSystem();
	int reValueCount = 0;



	while (true)
	{
		readBarcode();
		std::chrono::duration<int, std::milli> b = std::chrono::milliseconds(500);
		std::this_thread::sleep_for(b);
	}

	deInitSystem();
	std::cout << "has value return count = " << reValueCount << std::endl;
	system("pause");
	return 0;
}