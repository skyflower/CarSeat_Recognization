#pragma once
#include <iostream>
#include <Windows.h>


#define MAX_CHAR_LENGTH 256


namespace utils
{

	char* WcharToChar(wchar_t* wc);
	wchar_t* CharToWchar(char* c);
	std::wstring StrToWStr(const std::string str);
	std::string WStrToStr(const std::wstring wstr);
};

