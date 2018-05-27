#include "FileManager.h"


CFileManager* CFileManager::m_pInstance = nullptr;

CFileManager* CFileManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CFileManager;
	}
	return m_pInstance;
}

CFileManager::CFileManager() :m_pType(nullptr)
{
}


CFileManager::~CFileManager()
{
	if (m_pType != nullptr)
	{
		m_pType->clear();
		delete m_pType;
		m_pType = nullptr;
	}
}


#ifdef UNICODE_DEFINE
void CFileManager::initRoot(const wchar_t *fileRoot)
{
	m_szRootDir = std::wstring(fileRoot);
	m_pType = m_pInstance->EnumerateFileInDirectory((LPWSTR)fileRoot, FILE_ATTRIBUTE_DIRECTORY);
}
#else
void CFileManager::initRoot(const char *fileRoot)
{

}
#endif



std::vector<std::wstring> * CFileManager::EnumerateFileInDirectory(LPWSTR szPath, DWORD fileType, LPWSTR extName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	WCHAR szFilePath[MAX_PATH];

	if (extName == nullptr)
	{
		swprintf_s(szFilePath, L"%s\\*", szPath);
	}
	else
	{
		swprintf_s(szFilePath, L"%s\\*.%s", szPath, extName);
	}
	
	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// �жϾ��
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}
	else
	{
		std::vector<std::wstring> *result = new std::vector<std::wstring>;
		do
		{
			/* ���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����
			����ʹ��ע�Ͳ��ֵĴ������*/
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

			// ��ӡ�ļ�����Ŀ¼��
			//printf("%ws\t\t", FindFileData.cFileName);
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ������ļ���
			
			if ((FindFileData.dwFileAttributes & fileType) != 0)
			{
				std::wstring tmpStr(FindFileData.cFileName);
				result->emplace_back(tmpStr);

				//printf("<����> ");
			}
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ����ļ���
			//if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			//{
			//	printf("<����> ");
			//}
			//// �ж��ļ����ԣ��Ƿ�ΪĿ¼
			//if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			//{
			//	printf("<DIR> ");
			//}
			// ���߿ɸ����ļ����Ա��е�����������ӡ��ж��ļ�����
			//printf("\n");
		} while (FindNextFile(hListFile, &FindFileData));
		return result;
	}
	return nullptr;
}

std::wstring CFileManager::GetFileNameByType(const wchar_t *fileType)
{
	std::wstring tmpStr(fileType);
	size_t nSize = m_pType->size();
	for (size_t i = 0; i < nSize; ++i)
	{
		std::wstring & p = *(m_pType->data() + i);
		if (p == tmpStr)
		{
			return m_szRootDir + L"\\" + p;
		}
	}
	return nullptr;
}

const std::vector<std::wstring> *CFileManager::GetFileType()
{
	return m_pType;
}