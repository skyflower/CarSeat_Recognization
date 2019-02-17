#include <iostream>
#include "dll_export.h"


namespace dll_export
{
	int Add(int x, int y, int z)
	{
		return (x + y + z);
	}

	int Display(std::string str)
	{
		std::cout << str << std::endl;
		return 0;
	}

}