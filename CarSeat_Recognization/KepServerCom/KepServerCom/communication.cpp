#include "stdafx.h"
#include "Communication.h"
#include "tinyxml.h"
#include <string.h>

static char *man_do_msg = "<?xml version=\"1.0\" encoding=\"gbk\"?> \n<reply>1</reply>";
static char *man_not_do_msg = "<?xml version=\"1.0\" encoding=\"gbk\"?> \n<reply>0</reply>";

CDataCtrl::CDataCtrl()
{
	Dport = 47005;
	exitFlag = false;
}

CDataCtrl::~CDataCtrl()
{
	//exit();
}

void CDataCtrl::DataControl(COPC* dOpc)
{
	//��ʼ��Winscok
	if (!AfxSocketInit())
	{
		AfxMessageBox(TEXT("��ʼ��ʧ��"));
		return;
	}

	//socket----����2��socket--------------------------------------------
	
	//��ò�Ҫʹ��aSocket.Create��������Ϊ���׻����10048����
	if (!aSocket.Socket())
	{
		AfxMessageBox(TEXT("SOCKET����ʧ��"));
		return;
	}
	int ret = 0;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	//����Socket��ѡ��, ���10048�������Ĳ���
	aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

	DWORD block = 1; // ���÷�����ģʽ
	/*ret = aSocket.IOCtl(FIONBIO, &block);
	if (ret == 0)
	{
		TRACE2("set IOCtl FIONBIO failed, ret = %d, error = %d\n", ret, GetLastError());
	}*/


	//��
	if (!aSocket.Bind(Dport))
	{
		AfxMessageBox(TEXT("SOCKET ��ʧ��"));
		return;
	}

	//����
	if (!aSocket.Listen(1))
	{
		AfxMessageBox(TEXT("SOCKET ����ʧ��"));
		return;
	}
	int rt = 0;
	float data = 0;
	char szRecvMsg[256] = { 0 };
	char szOutMsg[256] = { 0 };
	//WSANOTINITIALISED;
	//aSocket.AsyncSelect();
	//aSocket.CancelBlockingCall();
	int acceptCount = 0;

	
	while(1)
	{
		//�����ⲿ����
		BOOL ret = aSocket.Accept(serverSocket);
		//AfxMessageBox(TEXT("���ӳɹ�"));
		if (ret != 0)
		{
			serverSocket.IOCtl(FIONBIO, &block);
		}
		//aSocket.
		TRACE1("acceptCount = %d\n", acceptCount);
		acceptCount++;
		int recycleCount = 0;
		
		while ((ret != 0) && (exitFlag == false))
		{
			memset(szRecvMsg, 0, sizeof(szRecvMsg));
			memset(szOutMsg, 0, sizeof(szOutMsg));

			rt = serverSocket.Receive(szRecvMsg, sizeof(szRecvMsg)); //���տͻ�������:����
			TRACE3("recycleCount = %d, rt = %d, szRecvMsg = %s\n", recycleCount, rt, szRecvMsg);
			recycleCount++;
			if (rt == 0)
			{
				serverSocket.Close();//�ر�
				TRACE0("serverSocket closed\n");
				break;
			}
			else if (rt == SOCKET_ERROR)
			{
				WSAENOTSOCK;//
				TRACE1("socket error, last Error = %d\n", GetLastError());
				continue;
			}
			rt = ParseXmlData(szRecvMsg, &data);
			if (rt == 0)
			{
				float result;
				result = ReadData(dOpc);
				//CString str;
				//str.Format(_T("%f"), result);
				//AfxMessageBox(str);
				if (result == 0)
				{
					serverSocket.Send(man_not_do_msg, sizeof(man_not_do_msg));//�������ݸ��ͻ���
				}
				else
				{
					serverSocket.Send(man_do_msg, sizeof(man_do_msg));//�������ݸ��ͻ���
				}
			}
			else if (rt == 1)
			{
				WriteData(dOpc, data);

				char tmpXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
				<reply>ok</reply>";
				serverSocket.Send(tmpXml, sizeof(tmpXml));
			}
			else
			{
				//AfxMessageBox(TEXT("XML����ʧ��"));
			}
			if (exitFlag == true)
			{
				break;
			}
		}
		if (exitFlag == true)
		{
			break;
		}
		
		Sleep(100);
	}
	aSocket.Close();
	serverSocket.Close();
	return;
}


int CDataCtrl::ParseXmlData(char* data, float* num)
{ 
	TiXmlDocument clientMsgXml;
	clientMsgXml.Parse(data);
	if (clientMsgXml.Error())
	{
		AfxMessageBox(TEXT("XML��ʽ����"));
		return -1;
	}
	TiXmlElement *rootElement = clientMsgXml.RootElement();
	//TiXmlElement *rootElement = doc.RootElement();
	if (rootElement != nullptr)
	{
		if (strncmp(rootElement->Value(), "read", strlen("read")) == 0) 
		{
			//AfxMessageBox(TEXT("����������"));
			return 0;
		} 
		else if ((strncmp(rootElement->Value(), "write", strlen("write")) == 0))
		{
			//AfxMessageBox(TEXT("д��������"));
			const char* value = rootElement->GetText();
			*num = atoi(value);

			return 1;
		}
		else
		{
			AfxMessageBox(TEXT("�Ƿ�����"));
			return -1;
		}
	}
	else 
	{
		AfxMessageBox(TEXT("������"));
	}
	return 0;
}

bool CDataCtrl::WriteData(COPC* dOpc, float data) 
{
	dOpc->WriteData(1, 0, &COleVariant(data));

	return true;
}

float CDataCtrl::ReadData(COPC* dOpc)
{
	dOpc->ReadData(1, 0);
	float data = dOpc->ItemData[0];
	
	return data;
}

void CDataCtrl::exit()
{
	exitFlag = true;
	aSocket.CancelBlockingCall();
	serverSocket.CancelBlockingCall();
	//if(aSocket.)
	//aSocket.Close();
}
