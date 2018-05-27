#pragma once
#include <iostream>

class CBaseMatch
{
public:
	CBaseMatch();
	~CBaseMatch();
	static int GetScaleFactor(){ return 5; }
	static double GetMinInterval(){ return 0.0001; }
	static double GetMaxInterval(){ return 0.15; }
	static int GetMaxLength(){ return 800; }
	static int GetMinLength(){ return 400; }
	static int GetHistLimit() { return 50; }
	static int GetImageMinSize() { return 50; }
	static double GetAreaLimit() { return 0.2; }
};

