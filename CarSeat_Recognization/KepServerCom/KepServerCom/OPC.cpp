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
	memset(szName, 0, sizeof(szName));
	m_pServer = nullptr;//服务器接口
	m_ItemMgt = nullptr;//读写控制
	m_pOPCSync = nullptr; //同步读写接口

	//TagNameRead = nullptr;
	//TagData = nullptr; 
	//TagNameWrite = nullptr;
	hReadServer = nullptr;
	hWriteServer = nullptr;
	ReadNum = 0;
	WriteNum = 0;
	//CoInitialize(NULL);
}

COPC::~COPC()
{
	if (bOPCConnect)
	{
		if( m_pServer )
			m_pServer->Release();
		if( m_ItemMgt )
			m_ItemMgt->Release();
		
		delete []hReadServer;
		delete []hWriteServer;
		delete []TagData;
		delete []ItemData;
	}
}

void COPC::InitialOPC(WCHAR* SeverName,long RNum,long WNum,COleVariant* RTagName,COleVariant* WTagName)
{
	memset(szName, 0, sizeof(szName));
	StrCpyW(szName, SeverName);
	ReadNum		   = RNum;
	WriteNum	   = WNum;
	TagData		   = new COleVariant[ReadNum];
	ItemData	   = new float[ReadNum];
	TagNameRead    = RTagName;
	TagNameWrite   = WTagName;
	
	hReadServer = new OPCHANDLE[ReadNum*2];
	hWriteServer = new OPCHANDLE[WriteNum*2]; 
}
bool COPC::ConnectServer()
{
	HRESULT hr;
	IUnknown * pCom = NULL;
	CLSID OPCClsid;
	CoInitialize(NULL);
	//连接 OPC Server
	hr = CLSIDFromProgID( szName, &OPCClsid );
    hr = CoCreateInstance( OPCClsid, NULL, CLSCTX_LOCAL_SERVER,
		 IID_IUnknown, (void**)&pCom );

	if (hr != 0) {
		return false;
	}

	hr = pCom->QueryInterface(IID_IOPCServer,(void**)&m_pServer);
	pCom->Release();
	
	if ( hr == S_OK ) {
    	return true;	
	} else	{
		return false;
	}
}

void COPC::PreRead()
{
	int i;
	OPCITEMDEF* OPCItem = new OPCITEMDEF[ReadNum];
	HRESULT hr;
	DWORD ActualRate;
	float b = 0.0;
	HRESULT *pErrors;
	IUnknown *pGroupUnk;
	OPCHANDLE hOPCServerGroup;
	OPCITEMRESULT *pItemResult;
	
	hr = m_pServer->AddGroup(L"",TRUE,500,1235,0,&b,0,&hOPCServerGroup,
							 &ActualRate,IID_IUnknown,&pGroupUnk);
	if (pGroupUnk == nullptr)
	{
		delete[]OPCItem;
		OPCItem = nullptr;
		return;
	}
	hr = pGroupUnk->QueryInterface(IID_IOPCItemMgt, (void **)&m_ItemMgt);
	for(i=0; i<ReadNum; i++)
	{
		if (TagNameRead[i].vt!=VT_BSTR) //VT_BSTR
		{
			AfxMessageBox(TEXT("传值失败"));
		}
		//MessageBox("传值成功");
		OPCItem[i].bActive=TRUE;
		OPCItem[i].dwBlobSize=0;
		OPCItem[i].hClient=hOPCServerGroup;  //确定组
		OPCItem[i].szAccessPath=L"";
		OPCItem[i].vtRequestedDataType=VT_EMPTY;
		OPCItem[i].szItemID=TagNameRead[i].bstrVal;  //确定ItemId
		OPCItem[i].pBlob=NULL;
	}
	
	hr = m_ItemMgt->AddItems(ReadNum,OPCItem,(OPCITEMRESULT**)&pItemResult,(HRESULT **)&pErrors);
	
	for( i = 0; i < ReadNum; i++)
	{
		hReadServer[i] = pItemResult[i].hServer;
	}
	
	delete []OPCItem;
	pGroupUnk->Release();
	CoTaskMemFree( pItemResult );
	CoTaskMemFree( pErrors );
}

//num:要读取数据的总数
//startnum:在标签列表中的位置
bool COPC::ReadData(long num, long startnum)
{
	int i;

	if( num+startnum > ReadNum )
		return false;
	OPCITEMSTATE *pItemValue;	
	HRESULT hr;
	HRESULT *pErrors;
	

	hr = m_ItemMgt->QueryInterface(IID_IOPCSyncIO,(void **)&m_pOPCSync);
	
	if (hr != S_OK)
	{
		return false;
	}
	
	hr = m_pOPCSync->Read(OPC_DS_CACHE,num,hReadServer+startnum,&pItemValue,&pErrors);
	
	for(i=0; i<num; i++)
	{
		VariantCopy(&(TagData[i+startnum]),&(pItemValue[i].vDataValue));
		VariantClear(&(pItemValue[i].vDataValue));
	}
	
	//释放内存
    m_pOPCSync->Release();
	CoTaskMemFree( pErrors );
	CoTaskMemFree( pItemValue );  
	
	if( hr < 0 )
		return false;

	for( i = 0; i < num; i++)
	{
		switch( TagData[i+startnum].vt ){
			case VT_I2:
				ItemData[i+startnum] = (float)TagData[i+startnum].iVal;
				break;
			case VT_I4:
				ItemData[i+startnum] = (float)TagData[i+startnum].lVal;
				break;
			case VT_R4:
				ItemData[i+startnum] = (float)TagData[i+startnum].fltVal;
				break;
			case VT_R8:
				ItemData[i+startnum] = (float)TagData[i+startnum].dblVal;
				break;
			case VT_UI1:
				ItemData[i+startnum] = (float)TagData[i+startnum].uiVal;
				break;
			case VT_UI2:
				ItemData[i+startnum] = (float)TagData[i+startnum].ulVal;
				break;
			case VT_BOOL:
				 ItemData[i+startnum] = (VARIANT_BOOL)TagData[i+startnum].boolVal;  //true=-1 ;false=0
				 break;
			default:
				ItemData[i+startnum] = -99999;
		}
	}
	return true;
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
			//printf("WriteItem传值失败");
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
	int i;
	HRESULT *pErrors;
	HRESULT	hr;
	OPCHANDLE *hTagServer = new OPCHANDLE[WriteNum];
	long *TagList = new long[len];
	for(int i = 0; i < len; i++)
		*(TagList+i) = startnum+i;
	
	for( i = 0; i < len; i++)
	{
		hTagServer[i] = hWriteServer[TagList[i]];
	}
	hr = m_ItemMgt->QueryInterface(IID_IOPCSyncIO, (void **)&m_pOPCSync);
	if (hr != S_OK)
	{
		return false;
	}
	hr  = m_pOPCSync->Write(len, hTagServer, WriteData, &pErrors);

	delete []hTagServer;  
	delete []TagList;
	//CoTaskMemFree(pErrors);
	//释放内存
	m_pOPCSync->Release();
	CoTaskMemFree(pErrors);
	
	if ( hr == 0 ) return true;
	else 
		return false;
}

void COPC::AddServerName(WCHAR *SeverName)
{
	memset(szName, 0, sizeof(szName));
	StrCpyW(szName, SeverName);
}
