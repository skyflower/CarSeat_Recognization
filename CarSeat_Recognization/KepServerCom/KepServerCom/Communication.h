#include "OPC.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataCtrl
{
public:
	void DataControl(COPC* dOpc);
	int ParseXmlData(char* data, float* num);
	bool WriteData(COPC* dOpc, float data);
	float ReadData(COPC* dOpc);
	CDataCtrl();
	virtual ~CDataCtrl();
private:
	int Dport;
};