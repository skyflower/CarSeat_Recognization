#pragma once
#include <iostream>
#include <cstring>
#include <unordered_map>
#include "./utils.h"



class CLabelManager
{
public:
	CLabelManager();
	~CLabelManager();
	

	/*
	������ת���ӿڣ���������Ч��6-8λ��ȡ����
	*/
	std::wstring GetInternalTypeByBarcode(std::wstring barcode);

	/*
	���ݴ�ʶ��ģ���õ������ͣ�ת�����ڲ�����
	*/
	std::wstring GetInternalTypeByClassifyType(std::wstring type);
	
	/*
	���ݴ�rfid�õ���������ת�����ⲿ���ͣ������û��鿴������
	*/
	std::wstring GetExternalTypeByBarcode(std::wstring barcode);
	
	/*
	����ʶ��ģ��ķ������ͣ�ת���ɷ����û��鿴���ⲿ����
	*/
	std::wstring GetExternalTypeByClassifyType(std::wstring classifyType);
	
	/*
	�����ⲿ����ץ����ʶ��ģ��ķ�������
	*/
	std::wstring GetClassifyTypeByExternal(std::wstring externalType);
	

	std::vector<std::wstring> GetBarcode();
	
	std::vector<std::wstring> GetExternalType();

	std::vector<std::wstring> GetClassifyType();

	const wchar_t *GetLoginUsrName()const;
	const wchar_t *GetLoginPasswd()const;
	bool SetLoginUsrName(const wchar_t *name);
	bool SetLoginPasswd(const wchar_t *passwd);
	bool GetLoginAutoSave();
	void SetLoginAutoSave(bool autoSave);

	int GetExposureTimeMax();
	int GetExposureTimeMin();

	/*  not implement���ӷ�������������xml��������������ձ� */
	void UpdateBarcode(const char *xmlContent, size_t len);

	

private:
	bool init();


	bool serialize();

	bool m_bSerialize;

	wchar_t m_strLoginName[MAX_USR_NAME_AND_PASSWD];
	wchar_t m_strLoginPasswd[MAX_USR_NAME_AND_PASSWD];

	int m_nExposureTimeMax;
	int m_nExposureTimeMin;
	bool m_bLoginAutoSave;
	
	/*
	�ڲ�����ת���ⲿ���͵���������ձ�,������λ��ȡ������Ч�ֶ����
	*/
	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	
	/*
	ʶ��ģ�������ת�����ڲ����͵Ķ��ձ�
	*/
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	
	static bool m_bInitFlag;
};

