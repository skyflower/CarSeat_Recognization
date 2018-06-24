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
	//namedWindow("����Ƶ��", 1);
	//namedWindow("��ͼƬ��", 1);
	while (1)
	{
		key = cv::waitKey(50);
		capture >> frame;
		//cv::imshow("����Ƶ��", frame);

		if (key == 27)
			break;//��ESC���˳�����  
		if (key == 32)//���ո����������  
		{
			sprintf(filename, "Picture %d.jpg", ++count);
			cv::imwrite(filename, frame);//ͼƬ���浽������Ŀ¼��  
			//cv::imshow("��ͼƬ��", frame);
		}
	}
#endif //  OPENCV

	return std::wstring();
}
