#pragma once

#include <iostream>
#include <vector>
#include "./common/Log.h"
#include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

template<typename type>
struct CConditionFilter
{
	type mDateTimeBeign[32];
	type mDateTimeEnd[32];
	type mLineBegin[20];
	type mLineEnd[20];
	type mBarcodeBegin[50];
	type mBarcodeEnd[50];
	type mSeatType[50];
	type mMethodType[20];
};


struct imageHeader
{
	char path[128];
	UINT64 size;
};

typedef CConditionFilter<char> CConditionFilterA;
typedef CConditionFilter<wchar_t> CConditionFilterW;

static char* generateSearchXml(CConditionFilterA & filter, unsigned int ip, const std::string &userName, const std::string &passwd)
{
	WriteInfo("date = [begin = %s],[end = %s]", filter.mDateTimeBeign, filter.mDateTimeEnd);
	WriteInfo("line = [begin = %s],[end = %s]", filter.mLineBegin, filter.mLineEnd);
	WriteInfo("barcode = [begin = %s],[end = %s]", filter.mBarcodeBegin, filter.mBarcodeEnd);
	WriteInfo("seatType = [%s], methodType = [%s]", filter.mSeatType, filter.mMethodType);

	char tmpTime[20] = { 0 };
	memset(tmpTime, 0, sizeof(tmpTime));

	time_t now_time = time(NULL);
	tm tmp_now_tm;
	localtime_s(&tmp_now_tm, &now_time);

	/*
	将查询条件以及用户名和密码发送到服务器端
	*/
	char queryXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><search><version>0.1</version><info ip=\"%s\" usr=\"%s\" passwd=\"%s\" time=\"%s\"/>%s</search>";

	const size_t length = 1000;
	char *tmpXml = new char[length];
	if (tmpXml == nullptr)
	{
		WriteError("malloc %d memory failed", length);
		//AfxMessageBox(L"malloc 1000 memory faield");
		return nullptr;
	}
	memset(tmpXml, 0, sizeof(char) * length);

	char *tmpChar = new char[length];
	if (tmpChar == nullptr)
	{
		WriteError("malloc %d memory failed", length);
		//AfxMessageBox(L"malloc 1000 memory faield");
		delete tmpXml;
		tmpXml = nullptr;
		return nullptr;
	}
	memset(tmpChar, 0, sizeof(char) * length);

	time_t tmpTime_t = time(NULL);
	struct tm nowTime;
	localtime_s(&nowTime, &tmpTime_t);

	sprintf_s(tmpTime, "%04d%02d%02d.%02d%02d%02d", \
		nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, \
		nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

	char ipStr[20] = { 0 };
	memset(ipStr, 0, sizeof(ipStr));
	//unsigned int ip = m_pParamManager->GetLocalIP();

	in_addr in;
	in.S_un.S_addr = ip;
	inet_ntop(AF_INET, (void *)&in, ipStr, sizeof(ipStr));

	// 时间选择策略
	const int policyLength = 200;
	char *timePolicy = new char[policyLength];

	memset(timePolicy, 0, sizeof(timePolicy));
	int timePolicyControl = 1;
	if (strcmp(filter.mDateTimeBeign, filter.mDateTimeEnd) == 0)
	{
		timePolicyControl = 0;
	}
	sprintf_s(timePolicy, policyLength, "<timePolicy control=\"%d\" startTime=\"%s\" endTime=\"%s\"/>",
		timePolicyControl, filter.mDateTimeBeign, filter.mDateTimeEnd);

	//产线选择策略
	char *linePolicy = new char[policyLength];
	memset(linePolicy, 0, sizeof(linePolicy));
	int linePolicyControl = 1;
	if ((strlen(filter.mLineBegin) == 0) && (strlen(filter.mLineEnd) == 0))
	{
		linePolicyControl = 0;
	}
	sprintf_s(linePolicy, policyLength, "<linePolicy control=\"%d\" startLine=\"%s\" endLine=\"%s\"/>",
		linePolicyControl, filter.mLineBegin, filter.mLineEnd);

	// 格式化条形码策略
	char *barcodePolicy = new char[policyLength];
	memset(barcodePolicy, 0, sizeof(barcodePolicy));
	int barcodePolicyControl = 1;
	if ((strlen(filter.mBarcodeBegin) == 0) && (strlen(filter.mBarcodeEnd) == 0))
	{
		barcodePolicyControl = 0;
	}
	sprintf_s(barcodePolicy, policyLength, "<barcodePolicy control=\"%d\" startBarcode=\"%s\" endBarcode=\"%s\"/>",
		barcodePolicyControl, filter.mBarcodeBegin, filter.mBarcodeEnd);

	// 座椅类型策略
	char *typePolicy = new char[policyLength];
	memset(typePolicy, 0, sizeof(typePolicy));
	int typePolicyControl = 1;
	if ((strlen(filter.mSeatType) == 0))
	{
		typePolicyControl = 0;
	}
	sprintf_s(typePolicy, policyLength, "<typePolicy control=\"%d\" type=\"%s\"/>",
		typePolicyControl, filter.mSeatType);

	// 识别类型策略，目前只支持自动识别
	char *methodPolicy = new char[policyLength];
	memset(methodPolicy, 0, sizeof(methodPolicy));
	int methodPolicyControl = 1;

	sprintf_s(methodPolicy, policyLength, "<methodPolicy control=\"%d\" method=\"auto\"/>", methodPolicyControl);

	WriteInfo("timePolicy = %s", timePolicy);
	WriteInfo("barcodePolicy = %s", barcodePolicy);
	WriteInfo("linePolicy = %s", linePolicy);
	WriteInfo("typePolicy = %s", typePolicy);
	WriteInfo("methodPolicy = %s", methodPolicy);


	sprintf_s(tmpChar, sizeof(char) * length, "%s\n%s\n%s\n%s\n%s\n", timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy);

	if (timePolicy != nullptr)
	{
		delete[]timePolicy;
		timePolicy = nullptr;
	}
	if (typePolicy != nullptr)
	{
		delete[]typePolicy;
		typePolicy = nullptr;
	}
	if (barcodePolicy != nullptr)
	{
		delete[]barcodePolicy;
		barcodePolicy = nullptr;
	}
	if (linePolicy != nullptr)
	{
		delete[]linePolicy;
		linePolicy = nullptr;
	}
	if (methodPolicy != nullptr)
	{
		delete[]methodPolicy;
		methodPolicy = nullptr;
	}

	sprintf_s(tmpXml, sizeof(char) * length, queryXml, ipStr, userName.c_str(), passwd.c_str(), tmpTime, tmpChar);

	/*
	将过滤xml发送到服务器端
	*/
	delete[]tmpChar;
	tmpChar = nullptr;

	return tmpXml;
}