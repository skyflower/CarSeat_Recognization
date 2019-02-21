#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <unordered_map>


#define MAX_CHAR_LENGTH 256



namespace utils
{
	char* WCharToChar(wchar_t* wc);
	//char* WcharToChar(wchar_t* wc);
	//wchar_t* CharToWchar(char* c);
	wchar_t* CharToWChar(char* c);

	bool WCharToChar(wchar_t* wc, char *bc, int *length);

	bool CharToWChar(char* bc, wchar_t *wc, int *length);

	std::wstring StrToWStr(const std::string str);
	std::string WStrToStr(const std::wstring wstr);

	template<typename type>
	bool delBlankSpace(type *text, size_t textCount, type specialChar)
	{
		if ((text == nullptr) || (textCount == 0))
		{
			return false;
		}
		//type *buffer = new type[textCount + 1];
		//memset(buffer, 0, sizeof(type) * (textCount + 1));
		size_t j = 0;
		size_t i = 0;
		for (i = 0; i < textCount; )
		{
			if (text[i] == specialChar)
			{
				++i;
			}
			else
			{
				text[j] = text[i];
				++i;
				++j;
			}
		}
		memset(&text[j], 0, sizeof(type) * (textCount - j));
		return true;
	}

	bool readFile(char *file, char *&content, size_t &FileSize);


	// XXX={"AAA", "BBBB", "CCC"}
	bool parseVector(const char *content, const char*name, std::vector<std::string> *pVector);
	
	//
	bool parseLineSegment(const char *name, size_t, std::vector<std::string> *pVec);
	
	//XXX=192.168.1.153
	int parseIp(const char *content, const char *name);
	
	// XXX={"AAA":"MMM", "BBB":"NNN", "CCC":"LLL"}
	int parseMap(const char*content, const char*name, std::unordered_map<std::string, std::string>*pMap);

	// XXX=AAA
	bool getValueByName(const char *content, const char*name, char*value);

	int SystemTimeCmp(SYSTEMTIME a, SYSTEMTIME b);

	bool saveFile(char *buffer, size_t bufferLength, char *filePath);

	std::string randomStr(int length);

};

