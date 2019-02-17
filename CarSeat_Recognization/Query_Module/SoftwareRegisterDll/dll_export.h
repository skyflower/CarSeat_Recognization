#pragma once

#ifdef REDISTER_DLL_EXPORTS
#define REDISTER_DLL_API __declspec(dllexport)
#else
#define REDISTER_DLL_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>

namespace dll_export
{

	REDISTER_DLL_API int Add(int x, int y, int z);

	REDISTER_DLL_API int Display(std::string);

}