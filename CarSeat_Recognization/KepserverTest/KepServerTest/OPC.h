// OPC.h: interface for the COPC class.
//
//////////////////////////////////////////////////////////////////////
#include "opccomn.h"
#include "opcda.h"

#if !defined(AFX_OPC_H__D5838E9D_B5AA_43B5_862D_64EE78AC2F41__INCLUDED_)
#define AFX_OPC_H__D5838E9D_B5AA_43B5_862D_64EE78AC2F41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COPC  
{
public:
	COPC();
	virtual ~COPC();
    
	void AddServerName(WCHAR* SeverName);
	void InitialOPC(WCHAR* SeverName,long WNum,COleVariant* WTagName);
	bool ConnectServer();
	void PreWrite();
	bool WriteData(long len, long startnum, VARIANT *WriteData);
	// ˵��:�����WriteData��Ҫ�� COleVariant���Ͷ������飬����:
	// float plc_flag = 1
	// WriteData(1,0,&COleVariant(plc_flag));
	bool bOPCConnect;//�жϷ������Ƿ�����
	
private:
	WCHAR  szName[100];
    long WriteNum;
	
	IOPCServer *m_pServer;//�������ӿ�
	IOPCItemMgt *m_ItemMgt;//��д����
	IOPCSyncIO *m_pOPCSync; //ͬ����д�ӿ�
	
	COleVariant TagNameWrite;
    OPCHANDLE *hWriteServer;
};


#endif // !defined(AFX_OPC_H__D5838E9D_B5AA_43B5_862D_64EE78AC2F41__INCLUDED_)