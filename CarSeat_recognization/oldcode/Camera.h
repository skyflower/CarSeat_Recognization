#pragma once

#include <iostream>

#include <unordered_map>

#ifdef OPENCV
#include<opencv2\highgui\highgui.hpp>  
#include<opencv2\imgproc\imgproc.hpp>  
#endif


class CCamera
{
public:
	CCamera();
	~CCamera();

	/*
	相机拍照的实现函数
	*/
	std::wstring takePhoto(std::wstring cameraID);


private:
	std::unordered_map<std::wstring, std::wstring> *m_pCameraLine;
	
};

