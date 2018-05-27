#pragma once
#include <iostream>
#include <unordered_map>
#include <cstring>

class CTypeIDManager
{
public:
	typedef unsigned int typeID;
	CTypeIDManager();
	~CTypeIDManager();
	typeID AddType(std::wstring desc);
	typeID GetTypeID(std::wstring desc);
	std::wstring GetTypeString(typeID type);

	static CTypeIDManager* GetInstance();

private:
	static CTypeIDManager *m_pInstance;
	
	typeID __calcTypeID(const std::wstring &desc);

	std::unordered_map<typeID, std::wstring> *m_pTypeID;
};

