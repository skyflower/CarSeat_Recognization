
#include <iostream>
#include <Windows.h>
#include "ImageClassify.h"
#include "LabelManager.h"
#include "Log.h"
#include "utils.h"
#include "ParamManager.h"

CParamManager *m_pParamManager = nullptr;
CLog *m_pLog = nullptr;
CImageClassify *m_pClassify = nullptr;
CLabelManager *m_pLabelManager = nullptr;

void initSystem()
{
	m_pParamManager = CParamManager::GetInstance();
	WriteInfo("get CParamManager success");
	m_pLog = CLog::GetInstance();

	WriteInfo("Get plog success");
	m_pClassify = new CImageClassify(m_pParamManager->GetGraphFile(), m_pParamManager->GetLabelFile());
	WriteInfo("Get m_pClassify success");
	m_pClassify->initPython("label_image_command_line_two", "seatClassify", m_pParamManager->GetCacheImagePath());
	
	WriteInfo("Get initPython success");
	
	m_pLabelManager = new CLabelManager();
	WriteInfo("Get m_pLabelManager success");
}

void deInitSystem()
{
	delete m_pClassify;
	m_pClassify = nullptr;

	delete m_pLabelManager;
	m_pLabelManager = nullptr;

	delete m_pParamManager;
	m_pParamManager = nullptr;

	delete m_pLog;
	m_pLog = nullptr;
}

void scanDirectory(char *lpPath, char *filePart, std::vector<std::string> &fileList)
{
	char szFind[MAX_PATH];
	char szFile[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy_s(szFind, lpPath);
	strcat_s(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)    return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strcpy_s(szFile, lpPath);
				strcat_s(szFile, "\\");
				strcat_s(szFile, FindFileData.cFileName);
				scanDirectory(szFile, filePart, fileList);
			}
		}

		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) && (strstr(FindFileData.cFileName, filePart) != NULL))
		{
			//std::cout << FindFileData.cFileName << std::endl;
			char tmpAbsPath[500];
			sprintf_s(tmpAbsPath, sizeof(tmpAbsPath), "%s\\%s", lpPath, FindFileData.cFileName);
			fileList.push_back(std::string(tmpAbsPath));
		}
		WriteInfo("find file: %s", FindFileData.cFileName);
		if (!FindNextFile(hFind, &FindFileData))
		{
			break;
		}
	}
	FindClose(hFind);
}


int main(int argc, char *argv[])
{
	char *filePart = ".jpg";
	char *directory = argv[1];
	std::vector<std::string> fileList;
	scanDirectory(directory, filePart, fileList);
	std::cout << "file count = " << fileList.size() << std::endl;

	initSystem();
	if (m_pClassify == nullptr)
	{
		std::cout << "init system failed, classify module init failed" << std::endl;
		return -1;
	}
	
	int reValueCount = 0;
	for (int i = 0; i < fileList.size(); ++i)
	{
		struct stat tmpStat;
		clock_t beginTime = clock();
		stat(fileList[i].c_str(), &tmpStat);
		if (tmpStat.st_size == 0)
		{
			continue;
		}
		std::string reType  = m_pClassify->compute(fileList[i].c_str());
		std::string externalType = m_pLabelManager->GetExternalTypeByClassifyType(reType);
		clock_t endTime = clock();
		//std::wstring wExternalType = utils::StrToWStr(externalType);
		std::cout << i + 1 << "  " << fileList[i].c_str() << "  " << reType.c_str()
			<< "  " << externalType.c_str() << "  " 
			<< static_cast<float>(endTime - beginTime)/CLOCKS_PER_SEC << "s" 
			<< std::endl;
		if ((reType.size() > 0) && (externalType.size() > 0))
		{
			reValueCount++;
		}
		//Sleep(1000);
	}
	deInitSystem();
	std::cout << "has value return count = " << reValueCount << std::endl;
	system("pause");
	return 0;
}