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
	std::string GetInternalTypeByBarcode(std::string barcode);

	/*
	���ݴ�ʶ��ģ���õ������ͣ�ת�����ڲ�����
	*/
	std::string GetInternalTypeByClassifyType(std::string type);
	
	/*
	���ݴ�rfid�õ���������ת�����ⲿ���ͣ������û��鿴������
	*/
	std::string GetExternalTypeByBarcode(std::string barcode);
	
	/*
	����ʶ��ģ��ķ������ͣ�ת���ɷ����û��鿴���ⲿ����
	*/
	std::string GetExternalTypeByClassifyType(std::string classifyType);
	
	/*
	�����ⲿ����ץ����ʶ��ģ��ķ�������
	*/
	std::string GetClassifyTypeByExternal(std::string externalType);
	

	std::vector<std::string> GetBarcode();
	
	std::vector<std::string> GetExternalType();

	std::vector<std::string> GetClassifyType();

	const char *GetLoginUsrName()const;
	const char *GetLoginPasswd()const;
	bool SetLoginUsrName(const char *name);
	bool SetLoginPasswd(const char *passwd);
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

	char m_strLoginName[MAX_USR_NAME_AND_PASSWD];
	char m_strLoginPasswd[MAX_USR_NAME_AND_PASSWD];

	int m_nExposureTimeMax;
	int m_nExposureTimeMin;
	bool m_bLoginAutoSave;
	
	/*
	�ڲ�����ת���ⲿ���͵���������ձ�,������λ��ȡ������Ч�ֶ����
	*/
	std::unordered_map<std::string, std::string> *m_pBarcode;
	
	/*
	ʶ��ģ�������ת�����ڲ����͵Ķ��ձ�
	*/
	std::unordered_map<std::string, std::string> *m_pClassifyType;

	char m_configFile[MAX_CHAR_LENGTH];
	
	static bool m_bInitFlag;
};

