#pragma once

#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include "utils.h"


struct RecogResult
{
	char m_szBarcode[32];		//	����������
	char m_szTime[32];			//	����γɵ�ʱ����������������ں�ʱ��
	char m_szTypeByRecog[32];	//	ʶ�������ⲿ����
	char m_szTypeByBarcode[32];	//	�����������õ����ⲿ����
	bool m_bIsCorrect;			//	ʶ�����Ƿ�һ��
	char m_szRecogMethod[8];	//	ʶ�𷽷���Ŀǰֻ��"auto"
	char m_szCameraName[20];	//	�������������
	char m_szLineName[8];		//	��������
	char m_szUsrName[20];		//	���߹���Ա�û���
	char m_szImagePath[256];	//	��Ƭ·��
};



class CRecogResultManager
{
public:
	
	~CRecogResultManager();

	static CRecogResultManager *GetInstance();

	const RecogResult* findByBarcode(const char *barcode)const;
	bool add(const RecogResult &a);

private:

	CRecogResultManager();

	void init();
	bool serialize();

	bool parseLine(char *line, RecogResult &a);

	char m_szName[MAX_CHAR_LENGTH];


	std::hash<const char*> m_pHashFunc;

	static CRecogResultManager *m_pInstance;

	std::unordered_map<size_t, RecogResult> *m_pRecogResult;

};

