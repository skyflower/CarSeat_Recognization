#include "LineCamera.h"
#include "../../common/Log.h"


CLineCamera::CLineCamera():m_nStatus(CLineCamera::CameraStatus::CAMERA_INIT),
m_nEdsImageQuality(0x0013ff0f)
{
	memset(m_szSaveDirectory, 0, sizeof(m_szSaveDirectory));
	memset(m_szCurImagePath, 0, sizeof(m_szCurImagePath));
	m_bSetupFlag[0] = m_bSetupFlag[1] = 0;
}


CLineCamera::~CLineCamera()
{

}

void CLineCamera::setupListener(ActionListener * listener)
{
	WriteInfo("set up listener");
	//_comboAEMode.setActionCommand("set_AEMode");
	//_comboAEMode.addActionListener(listener);
	//_comboTv.setActionCommand("set_Tv");
	//_comboTv.addActionListener(listener);
	////_comboAv.setActionCommand("set_Av");
	//_comboAv.addActionListener(listener);
	//_comboIso.setActionCommand("set_ISOSpeed");
	//_comboIso.addActionListener(listener);
	_comboMeteringMode.addActionListener(listener);
	/*_comboExposureComp.setActionCommand("set_ExposureCompensation");
	_comboExposureComp.addActionListener(listener);*/
	_comboImageQuality.setActionCommand("set_ImageQuality");
	_comboImageQuality.addActionListener(listener);
	/*_comboEvfAFMode.setActionCommand("set_EvfAFMode");
	_comboEvfAFMode.addActionListener(listener);*/

	_btnTakePicture.setActionCommand("TakePicture");
	_btnTakePicture.addActionListener(listener);
	_btnStartEVF.setActionCommand("startEVF");
	_btnStartEVF.addActionListener(listener);
	_btnEndEVF.setActionCommand("endEVF");
	_btnEndEVF.addActionListener(listener);

	/*_btnPressingHalfway.setActionCommand("pressingHalfway");
	_btnPressingHalfway.addActionListener(listener);
	_btnPressingCompletely.setActionCommand("pressingCompletely");
	_btnPressingCompletely.addActionListener(listener);
	_btnPressingOff.setActionCommand("pressingOff");
	_btnPressingOff.addActionListener(listener);
	_btnEvfAfON.setActionCommand("evfAFOn");
	_btnEvfAfON.addActionListener(listener);
	_btnEvfAfOFF.setActionCommand("evfAFOff");
	_btnEvfAfOFF.addActionListener(listener);*/
	m_bSetupFlag[0] = 1;
	if ((m_bSetupFlag[0] & m_bSetupFlag[1]) == 1)
	{
		WriteInfo("set listener, m_nStatus = %d", m_nStatus);
		m_nStatus = CLineCamera::CameraStatus::CAMERA_OPEN;
	}
}

void CLineCamera::setupObserver(Observable * ob)
{
	unsigned long long tmp = (unsigned long long)ob;
	//unsigned int tmpValue
	//WriteInfo("set up Observable 0x%X, 0x%X", (tmp & 0xFFFFFFFF), ((tmp >> 32) & 0xFFFFFFFF));
	//ob->addObserver(static_cast<Observer*>(&_comboAEMode));
	//ob->addObserver(static_cast<Observer*>(&_comboTv));
	//ob->addObserver(static_cast<Observer*>(&_comboAv));
	//ob->addObserver(static_cast<Observer*>(&_comboIso));
	ob->addObserver(static_cast<Observer*>(&_comboMeteringMode));
	//ob->addObserver(static_cast<Observer*>(&_comboExposureComp));
	ob->addObserver(static_cast<Observer*>(&_comboImageQuality));
	//ob->addObserver(static_cast<Observer*>(&_comboEvfAFMode));
	//_comboImageQuality.OnSelChange(EdsImageQuality_MJF);
	//_comboMeteringMode.OnSelChange(3);
	//_comboEvfAFMode.OnSelChange(0);
	m_bSetupFlag[1] = 1;
	if ((m_bSetupFlag[0] & m_bSetupFlag[1]) == 1)
	{
		WriteInfo("set, m_nStatus = %d", m_nStatus);
		m_nStatus = CLineCamera::CameraStatus::CAMERA_OPEN;
	}
}

std::string CLineCamera::saveJpg()
{
	_btnTakePicture.OnClicked();
	WriteInfo("saveJpg Camera, m_nStatus = %d", m_nStatus);
	memset(m_szCurImagePath, 0, sizeof(m_szCurImagePath));
	return std::string();
}

void CLineCamera::startCamera()
{
	if (m_nStatus != CameraStatus::CAMERA_OPEN)
	{
		WriteInfo("start Camera status not match, m_nStatus = %d not CAMERA_OPEN", m_nStatus);
		return;
	}
	initCameraStatus();
	_btnStartEVF.OnClicked();
	
	WriteInfo("startCamera Camera, m_nStatus = %d", m_nStatus);
	m_nStatus = CameraStatus::CAMERA_GRAB;
}

void CLineCamera::stopCamera()
{
	if (m_nStatus != CameraStatus::CAMERA_GRAB)
	{
		WriteInfo("start Camera status not match, m_nStatus = %d not CAMERA_GRAB", m_nStatus);
		return;
	}
	

	_comboMeteringMode.OnSelChange(5);
	_btnEndEVF.OnClicked();
	
	
	WriteInfo("stop Camera, m_nStatus = %d", m_nStatus);
	m_nStatus = CameraStatus::CAMERA_OPEN;
}

void CLineCamera::close()
{
	stopCamera();
	//_btnEvfAfOFF.OnClicked();
}

void CLineCamera::setImageQuality(unsigned int quality)
{
	m_nEdsImageQuality = quality;
	_comboImageQuality.OnSelChange(m_nEdsImageQuality);
}

void CLineCamera::setImageSaveDirectory(const char * dir)
{
	memcpy(m_szSaveDirectory, dir, strlen(dir) * sizeof(char));
}

CLineCamera::CameraStatus CLineCamera::getCameraStatus()
{
	return m_nStatus;
}

void CLineCamera::setCurrentImage(const char * file)
{
	if (file != nullptr)
	{
		strcpy_s(m_szCurImagePath, file);
		
	}
}

const char * CLineCamera::getCurrentImage()
{
	return m_szCurImagePath;
}

//unsigned int CLineCamera::getImageWidth()
//{
//	return m_nImageWidth;
//}
//
//unsigned int CLineCamera::getImageHeight()
//{
//	return m_nImageHeight;
//}

//unsigned int CLineCamera::setImageWidth(unsigned int width)
//{
//	return m_nImageWidth = width;
//}
//
//unsigned int CLineCamera::setImageHeight(unsigned int height)
//{
//	return m_nImageHeight = height;
//}


void CLineCamera::initCameraStatus()
{
	_comboImageQuality.OnSelChange(m_nEdsImageQuality);
	_comboMeteringMode.OnSelChange(3);
	//_comboEvfAFMode.OnSelChange(0);
	WriteInfo("InitCamera Status, set image quality 0x%X, m_nStatus = %d", m_nEdsImageQuality, m_nStatus);
}
