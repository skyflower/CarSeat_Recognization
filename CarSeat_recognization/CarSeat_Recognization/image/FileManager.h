#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>

class CFileManager
{
public:
	~CFileManager();
	std::wstring GetFileNameByType(const wchar_t *fileType);
	const std::vector<std::wstring> *GetFileType();
	static CFileManager* GetInstance();
	static std::vector<std::wstring> * EnumerateFileInDirectory(LPWSTR szPath, DWORD fileType, LPWSTR extName = nullptr);

	void initRoot(const char *fileRoot);

private:
	CFileManager();

	static CFileManager* m_pInstance;

	std::wstring m_szRootDir;
	std::vector<std::wstring> *m_pType;
};

