// OPC.cpp: implementation of the COPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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
	bOPCConnect=false;
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
		delete []hWriteServer;
	}
}

void COPC::InitialOPC(WCHAR* SeverName,long WNum,COleVariant* WTagName)
{
	szName         = SeverName;
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
    hr = CoCreateInstance( OPCClsid, NULL, CLSCTX_LOCAL_SERVER,
		 IID_IUnknown, (void**)&pCom );

	if ( hr != 0 ) 
		return false;

	hr = pCom->QueryInterface(IID_IOPCServer,(void**)&m_pServer);
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
	OPCITEMRESULT *pItemResult;
	HRESULT *pErrors;
	HRESULT hr;
	OPCITEMDEF *ItemArray = new OPCITEMDEF[WriteNum];
	
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
	for(int i = 0; i < WriteNum; i++)
		hWriteServer[i] = pItemResult[i].hServer;

	delete []ItemArray;
	CoTaskMemFree( pItemResult );
	CoTaskMemFree( pErrors );
}

bool COPC::WriteData(long len, long startnum,VARIANT *WriteData)
{
	HRESULT *pErrors;	
	OPCHANDLE *hTagServer = new OPCHANDLE[WriteNum];
	long *TagList = new long[len];
	for(int i = 0; i < len; i++)
		*(TagList+i) = startnum+i;
	
	for( int i = 0; i < len; i++)
	{
		hTagServer[i] = hWriteServer[TagList[i]];
	}
	
	HRESULT	hr  = m_pOPCSync->Write(len, hTagServer, WriteData, &pErrors);

	delete []hTagServer;  
	delete []TagList;
	CoTaskMemFree(pErrors);
	
	if ( hr == 0 ) return true;
	else 
		return false;
} 

void COPC::AddServerName(WCHAR *SeverName)
{
	szName = SeverName;
}
