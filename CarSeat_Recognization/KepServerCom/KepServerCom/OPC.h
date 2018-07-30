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
	void AddServerName(WCHAR* SeverName);
	COPC();
	virtual ~COPC();
    
	void InitialOPC(WCHAR* SeverName,long RNum,long WNum,COleVariant* RTagName,COleVariant* WTagName);
	bool ConnectServer();
	void PreRead();
	bool ReadData(long num, long startnum);
	void PreWrite();
	bool WriteData(long len, long startnum, VARIANT *WriteData);
	
	float *ItemData;        //����������
	bool bOPCConnect;//�жϷ������Ƿ�����
	
private:
	WCHAR  szName[100];
    long ReadNum,WriteNum;
	
	IOPCServer *m_pServer;//�������ӿ�
	IOPCItemMgt *m_ItemMgt;//��д����
	IOPCSyncIO *m_pOPCSync; //ͬ����д�ӿ�
	
	COleVariant *TagNameRead,*TagData,*TagNameWrite;
    OPCHANDLE *hReadServer, *hWriteServer;
};


#endif // !defined(AFX_OPC_H__D5838E9D_B5AA_43B5_862D_64EE78AC2F41__INCLUDED_)
