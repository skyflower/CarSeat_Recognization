#include "LineCamera.h"
#include "../../common/Log.h"


CLineCamera::CLineCamera():m_nStatus(CLineCamera::CameraStatus::CAMERA_INIT)
{
	memset(m_szSaveDirectory, 0, sizeof(m_szSaveDirectory));
	m_bSetupFlag[0] = m_bSetupFlag[1] = 0;

}


CLineCamera::~CLineCamera()
{

}

void CLineCamera::setupListener(ActionListener * listener)
{
	_comboAEMode.setActionCommand("set_AEMode");
	_comboAEMode.addActionListener(listener);
	_comboTv.setActionCommand("set_Tv");
	_comboTv.addActionListener(listener);
	_comboAv.setActionCommand("set_Av");
	_comboAv.addActionListener(listener);
	_comboIso.setActionCommand("set_ISOSpeed");
	_comboIso.addActionListener(listener);
	_comboMeteringMode.addActionListener(listener);
	_comboExposureComp.setActionCommand("set_ExposureCompensation");
	_comboExposureComp.addActionListener(listener);
	_comboImageQuality.setActionCommand("set_ImageQuality");
	_comboImageQuality.addActionListener(listener);
	_comboEvfAFMode.setActionCommand("set_EvfAFMode");
	_comboEvfAFMode.addActionListener(listener);

	_btnTakePicture.setActionCommand("TakePicture");
	_btnTakePicture.addActionListener(listener);
	_btnStartEVF.setActionCommand("startEVF");
	_btnStartEVF.addActionListener(listener);
	_btnEndEVF.setActionCommand("endEVF");
	_btnEndEVF.addActionListener(listener);

	_btnPressingHalfway.setActionCommand("pressingHalfway");
	_btnPressingHalfway.addActionListener(listener);
	_btnPressingCompletely.setActionCommand("pressingCompletely");
	_btnPressingCompletely.addActionListener(listener);
	_btnPressingOff.setActionCommand("pressingOff");
	_btnPressingOff.addActionListener(listener);
	_btnEvfAfON.setActionCommand("evfAFOn");
	_btnEvfAfON.addActionListener(listener);
	_btnEvfAfOFF.setActionCommand("evfAFOff");
	_btnEvfAfOFF.addActionListener(listener);
	m_bSetupFlag[0] = 1;
	if ((m_bSetupFlag[0] & m_bSetupFlag[1]) == 1)
	{
		m_nStatus = CLineCamera::CameraStatus::CAMERA_OPEN;
	}
}

void CLineCamera::setupObserver(Observable * ob)
{
	ob->addObserver(static_cast<Observer*>(&_comboAEMode));
	ob->addObserver(static_cast<Observer*>(&_comboTv));
	ob->addObserver(static_cast<Observer*>(&_comboAv));
	ob->addObserver(static_cast<Observer*>(&_comboIso));
	ob->addObserver(static_cast<Observer*>(&_comboMeteringMode));
	ob->addObserver(static_cast<Observer*>(&_comboExposureComp));
	ob->addObserver(static_cast<Observer*>(&_comboImageQuality));
	ob->addObserver(static_cast<Observer*>(&_comboEvfAFMode));
	m_bSetupFlag[1] = 1;
	if ((m_bSetupFlag[0] & m_bSetupFlag[1]) == 1)
	{
		m_nStatus = CLineCamera::CameraStatus::CAMERA_OPEN;
		
	}
}

std::string CLineCamera::saveJpg()
{
	_btnTakePicture.OnClicked();
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
	m_nStatus = CameraStatus::CAMERA_GRAB;
}

void CLineCamera::stopCamera()
{
	if (m_nStatus != CameraStatus::CAMERA_GRAB)
	{
		WriteInfo("start Camera status not match, m_nStatus = %d not CAMERA_GRAB", m_nStatus);
	}
	_btnEndEVF.OnClicked();
	m_nStatus = CameraStatus::CAMERA_OPEN;
}

void CLineCamera::setImageSaveDirectory(const char * dir)
{
	memcpy(m_szSaveDirectory, dir, strlen(dir) * sizeof(char));
}

CLineCamera::CameraStatus CLineCamera::getCameraStatus()
{
	return m_nStatus;
}

void CLineCamera::initCameraStatus()
{
	_comboImageQuality.OnSelChange(EdsImageQuality_MJF);
}
