#include "Camera.h"
#include "../common/Log.h"
#include <cstdlib>


CCamera::CCamera()
{

}


CCamera::~CCamera()
{

}

std::wstring CCamera::takePhoto(std::wstring cameraID)
{
#ifdef  OPENCV
	cv::VideoCapture  capture(0);
	cv::Mat frame;
	if (!capture.isOpened())
	{
		WriteError("Open Camera Failed cameraID = %u", cameraID);
		return std::wstring();
	}
	char key;
	char filename[200];
	int count = 0;
	//namedWindow("【视频】", 1);
	//namedWindow("【图片】", 1);
	while (1)
	{
		key = cv::waitKey(50);
		capture >> frame;
		//cv::imshow("【视频】", frame);

		if (key == 27)
			break;//按ESC键退出程序  
		if (key == 32)//按空格键进行拍照  
		{
			sprintf(filename, "Picture %d.jpg", ++count);
			cv::imwrite(filename, frame);//图片保存到本工程目录中  
			//cv::imshow("【图片】", frame);
		}
	}
#endif //  OPENCV

	return std::wstring();
}
