#pragma once


#include "CameraController.h"
#include "CameraModel.h"

#include "../Action/AEModeAction.h"
#include "../Action/AvAction.h"
#include "../Action/ClickAction.h"
#include "../Action/EvfAFModeAction.h"
#include "../Action/EvfZoomAction.h"
#include "../Action/MeteringModeAction.h"
#include "../Action/ExposureAction.h"
#include "../Action/ImageQualityAction.h"
#include "../Action/TvAction.h"
#include "../Action/IsoAction.h"
#include "ActionListener.h"

class CLineCamera
{
public:
	enum CameraStatus
	{
		CAMERA_INIT,
		CAMERA_OPEN,
		CAMERA_GRAB
	};

	CLineCamera();
	~CLineCamera();

	void setupListener(ActionListener* listener);
	void setupObserver(Observable* ob);

	std::string saveJpg();

	void startCamera();
	void stopCamera();
	void close();

	void setImageQuality(unsigned int quality);

	void setImageSaveDirectory(const char *dir);

	CLineCamera::CameraStatus getCameraStatus();

	void setCurrentImage(const char*file);
	const char* getCurrentImage();

private:

	void initCameraStatus();

	CClickAction	_btnTakePicture;
	CClickAction	_btnStartEVF;
	CClickAction	_btnEndEVF;
	//CAEModeAction	_comboAEMode;
	//CTvAction			_comboTv;
	//CAvAction			_comboAv;
	//CIsoAction		_comboIso;
	CMeteringModeAction	_comboMeteringMode;
	//CExposureAction	_comboExposureComp;
	CImageQualityAction	_comboImageQuality;

	//CClickAction	_btnPressingHalfway;
	//CClickAction	_btnPressingCompletely;
	//CClickAction	_btnPressingOff;
	//CEvfAFModeAction		_comboEvfAFMode;
	//CClickAction	_btnEvfAfON;
	//CClickAction	_btnEvfAfOFF;

	char m_szSaveDirectory[256];
	char m_szCurImagePath[256];

	CameraStatus m_nStatus;
	unsigned int m_nEdsImageQuality;
	unsigned char m_bSetupFlag[2];
};

