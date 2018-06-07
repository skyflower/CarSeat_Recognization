// OPC.cpp: implementation of the COPC class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "OPC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COPC::COPC()
{
	bOPCConnect = false;
	hWriteServer = nullptr;

	// 构造函数，统一初始化成员变量,xusiyuan 2018.06.06
	m_pServer = nullptr;//服务器接口
	m_ItemMgt = nullptr;//读写控制
	m_pOPCSync = nullptr; //同步读写接口
	hWriteServer = nullptr;
	memset(szName, 0, sizeof(szName));
	WriteNum = 0;


	CoInitialize(NULL);
}

COPC::~COPC()
{
	if (bOPCConnect)
	{
		if( m_pServer )
			m_pServer->Release();
		if( m_ItemMgt )
			m_ItemMgt->Release();
		
	}
	// 将内存释放提取到外面来，xusiyuan
	if (hWriteServer != nullptr)
	{
		delete[]hWriteServer;
		hWriteServer = nullptr;
	}
}

void COPC::InitialOPC(WCHAR* SeverName,long WNum,COleVariant* WTagName)
{
	//szName = SeverName;
	memset(szName, 0, sizeof(szName));
	StrCpyW(szName, SeverName);

	WriteNum	   = WNum;
	TagNameWrite   = WTagName;

	hWriteServer = new OPCHANDLE[WriteNum*2]; 
}
bool COPC::ConnectServer()
{
	HRESULT hr;
	IUnknown * pCom = NULL;
	CLSID OPCClsid;
	//连接 OPC Server
	hr = CLSIDFromProgID( szName, &OPCClsid );
    hr = CoCreateInstance( OPCClsid, NULL, CLSCTX_LOCAL_SERVER | CLSCTX_ACTIVATE_32_BIT_SERVER,
		 IID_IUnknown, (void**)&pCom );

	if ( hr != 0 ) 
		return false;

	int tmpPointer = 0;
	hr = pCom->QueryInterface(IID_IOPCServer, (void**)&m_pServer);
	pCom->Release();
	
	if ( hr == S_OK ) 
	{
    	return true;	
	}
	else
	{
		return false;
	}
}

void COPC::PreWrite()
{
	OPCITEMRESULT *pItemResult = nullptr;
	HRESULT *pErrors = nullptr;
	HRESULT hr;
	DWORD ActualRate;
	float b = 0.0;
	IUnknown *pGroupUnk = nullptr;
	OPCHANDLE hOPCServerGroup;
	OPCITEMDEF *ItemArray = new OPCITEMDEF[WriteNum];
	//  
	hr = m_pServer->AddGroup(L"", TRUE, 500, 1235, 0, &b, 0, &hOPCServerGroup,
		&ActualRate, IID_IUnknown, &pGroupUnk);
	if (pGroupUnk == nullptr)
	{
		delete []ItemArray;
		ItemArray = nullptr;
		return;
	}
	hr = pGroupUnk->QueryInterface(IID_IOPCItemMgt, (void **)&m_ItemMgt);

	for(int ItemNumber = 0; ItemNumber < WriteNum; ItemNumber++)
	{
		if( TagNameWrite[ItemNumber].vt != VT_BSTR )
			//	MessageBox("传值失败");
		ItemArray[ItemNumber].bActive = TRUE;
		ItemArray[ItemNumber].dwBlobSize = 0;
		ItemArray[ItemNumber].pBlob = NULL;
		ItemArray[ItemNumber].hClient = ItemNumber;
		ItemArray[ItemNumber].szAccessPath = L"";
		ItemArray[ItemNumber].szItemID = TagNameWrite[ItemNumber].bstrVal;
		ItemArray[ItemNumber].vtRequestedDataType = VT_EMPTY;	
	}
	
	hr = m_ItemMgt->AddItems(WriteNum, ItemArray, (OPCITEMRESULT**)&pItemResult,
		                         (HRESULT **)&pErrors);
	for (int i = 0; i < WriteNum; i++)
	{
		hWriteServer[i] = pItemResult[i].hServer;
	}

	delete []ItemArray;
	CoTaskMemFree( pItemResult );
	CoTaskMemFree( pErrors );
}

bool COPC::WriteData(long len, long startnum,VARIANT *WriteData)
{
	HRESULT hr;
	HRESULT *pErrors = nullptr; // 指针都要初始化空值	xusiyuan
	OPCHANDLE *hTagServer = new OPCHANDLE[WriteNum];
	long *TagList = new long[len];
	for (int i = 0; i < len; i++)
	{
		*(TagList + i) = startnum + i;
	}
	
	for( int i = 0; i < len; i++)
	{
		hTagServer[i] = hWriteServer[TagList[i]];
	}
	hr = m_ItemMgt->QueryInterface(IID_IOPCSyncIO, (void **)&m_pOPCSync);
	hr  = m_pOPCSync->Write(len, hTagServer, WriteData, &pErrors);

	delete []hTagServer;  
	delete []TagList;
	CoTaskMemFree(pErrors);
	
	if (hr == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
} 

void COPC::AddServerName(WCHAR *SeverName)
{
	// 作为内部变量，分配内存感觉比较合理，xusiyuan
	memset(szName, 0, sizeof(szName));
	StrCpyW(szName, SeverName);
}
