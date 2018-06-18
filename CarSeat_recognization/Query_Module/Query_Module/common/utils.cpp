#include "../stdafx.h"
#include "utils.h"



namespace utils
{

	char* WcharToChar(wchar_t* wc)
	{

		int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
		char *m_char = new char[len + 1];
		memset(m_char, 0, sizeof(char) * (len + 1));

		WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		return m_char;
	}
	wchar_t* CharToWchar(char* c)
	{

		int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
		wchar_t *m_wchar = new wchar_t[len + 1];
		memset(m_wchar, 0, sizeof(wchar_t) * (len + 1));

		MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	std::wstring StrToWStr(const std::string str)
	{
		//wchar_t* CharToWchar(char* c);
		wchar_t *pWChar = CharToWchar(const_cast<char*>(str.c_str()));
		std::wstring tmpStr(pWChar);
		delete[]pWChar;
		pWChar = nullptr;
		return tmpStr;
	}
	std::string WStrToStr(const std::wstring wstr)
	{
		char *pChar = WcharToChar(const_cast<wchar_t*>(wstr.c_str()));

		std::string tmpStr(pChar);
		delete[]pChar;
		pChar = nullptr;

		return tmpStr;
	}

	bool delBlankSpace(char * text, size_t len)
	{
		if ((text == nullptr) || (len == 0))
		{
			return true;
		}
		size_t i = 0;
		size_t j = 0;
		for (; j < len; )
		{
			if (text[j] > 0x20)
			{
				text[i] = text[j];
				++i;
				++j;
				continue;
			}
			else if (text[j] <= 0x20)
			{
				if ((j > 0) && (text[j - 1] <= 0x20))
				{
					++j;
					continue;
				}
				else if (j == 0)
				{
					++j;
					continue;
				}
				else if ((j > 0) && (text[j - 1] > 0x20))
				{
					text[i] = ' ';
					++i;
					++j;
					continue;
				}
			}
		}
		if (j < len)
		{
			memset(text + j, 0, sizeof(char) * (len - j));
		}
		return true;
	}

	bool readFile(char * file, char * &content, size_t & FileSize)
	{
		FILE *fp = nullptr;
		content = nullptr;
		FileSize = 0;
		fopen_s(&fp, file, "rb");
		if (fp == nullptr)
		{
			return false;
		}
		fseek(fp, 0, SEEK_END);

		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		content = new char[nSize + 1];
		if (content == nullptr)
		{
			return false;
		}
		memset(content, 0, sizeof(char) * (nSize + 1));
		fread(content, 1, nSize, fp);
		fclose(fp);

		FileSize = nSize;
		return true;
	}



	bool parseVector(const char *content, const char * name, std::vector<std::wstring>* pVector)
	{
		if ((content == nullptr) || (name == nullptr) || (pVector == nullptr))
		{
			return false;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p != NULL)
		{
			char *line = strchr(p, '{');
			char *end = strchr(p, '}');
			if ((line != NULL) && (end != NULL))
			{

				if (parseLineSegment(line + 1, end - line - 1, pVector) == true)
				{



				}
			}
		}
		return true;
	}

	bool parseLineSegment(const char * pContent, size_t length, std::vector<std::wstring>* pData)
	{
		if ((pContent == nullptr) || (length < 1) || (pData == nullptr))
		{
			return false;
		}

		char *p = const_cast<char*>(pContent);
		char tmpStr[100];
		char c = '\"';
		while (p < pContent + length)
		{
			char* begin = strchr(p, c);
			if ((begin == NULL) || (begin >= pContent + length))
			{
				break;
			}
			char* end = strchr(begin + 1, c);
			if ((end == NULL) || (end >= pContent + length))
			{
				break;
			}
			p = end + 1;
			if (end - begin - 1 != 0)
			{
				memset(tmpStr, 0, sizeof(tmpStr));
				memcpy(tmpStr, begin + 1, sizeof(char)*(end - begin - 1));
				wchar_t *wchar = utils::CharToWchar(tmpStr);
				if (wchar == nullptr)
				{
					continue;
				}
				std::wstring wStr(wchar);
				pData->push_back(wStr);
				if (wchar != nullptr)
				{
					delete[]wchar;
					wchar = nullptr;
				}
			}
		}
		return true;
	}

	int parseIp(const char * content, const char * name)
	{
		if ((content == nullptr) || (name == nullptr))
		{
			return 0;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p == nullptr)
		{
			return 0;
		}
		const char *quote = strchr(p, '=');
		if (quote == nullptr)
		{
			return 0;
		}
		const char *endline = strchr(quote + 1, '\n');
		char str[100];
		memset(str, 0, sizeof(str));
		memcpy(str, quote + 1, endline - quote - 1);
		TRACE1("ServerIp = %s", str);
		unsigned int one = 0;
		unsigned int two = 0;
		unsigned int three = 0;
		unsigned int four = 0;
		sscanf_s(str, "%d.%d.%d.%d", &one, &two, &three, &four);
		unsigned int tmpServerIp = (one << 24) | (two << 16) | (three << 8) | four;

		return tmpServerIp;
	}

	int parseMap(const char * content, const char * name, std::unordered_map<std::wstring, std::wstring>* pMap)
	{
		if ((content == nullptr) || (name == nullptr) || (pMap == nullptr))
		{
			return 0;
		}
		const char *p = strstr(content, name);
		if (p == nullptr)
		{
			return 0;
		}
		const char *endLine = strchr(p, '}');
		if (endLine == nullptr)
		{
			return 0;
		}
		char tmpStr[50] = { 0 };
		while (p < endLine)
		{
			const char *first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			const char *second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			size_t tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string keyChar(tmpStr);
			std::wstring keyWChar = utils::StrToWStr(keyChar);

			p = second + 1;

			first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string valueChar(tmpStr);
			std::wstring valueWChar = utils::StrToWStr(valueChar);

			pMap->insert(std::make_pair(keyWChar, valueWChar));

			p = second + 1;

		}

		return pMap->size();
	}

	bool getValueByName(const char *content, const char * name, char * value)
	{
		if ((name == nullptr) || (value == nullptr) || (content == nullptr))
		{
			return false;
		}

		char *p = strstr(const_cast<char*>(content), name);
		if (p == NULL)
		{
			return false;
		}
		char *lineEnd = strstr(p, "\n");
		if (lineEnd == NULL)
		{
			return false;
		}
		char *begin = strstr(p + 1, "=");
		if ((begin == NULL) || (begin >= lineEnd))
		{
			return false;
		}
		memcpy_s(value, MAX_CHAR_LENGTH, begin + 1, lineEnd - begin - 1);

		return true;
	}

}
