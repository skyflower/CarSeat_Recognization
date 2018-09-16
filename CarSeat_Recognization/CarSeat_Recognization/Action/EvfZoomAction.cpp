/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : EvfZoomButton.cpp                                                *
*                                                                             *
*   Description: This is the Sample code to show the usage of EDSDK.          *
*                                                                             *
*                                                                             *
*******************************************************************************
*                                                                             *
*   Written and developed by Camera Design Dept.53                            *
*   Copyright Canon Inc. 2006-2008 All Rights Reserved                        *
*                                                                             *
*******************************************************************************/

#include "EvfZoomAction.h"


/////////////////////////////////////////////////////////////////////////////
// CEvfZoomAction

CEvfZoomAction::CEvfZoomAction():CClickAction()
{
}

CEvfZoomAction::~CEvfZoomAction()
{
}
void CEvfZoomAction::update(Observable* from, CameraEvent *e)
{
	std::string event = e->getEvent();
	//PropertyChanged event
	if (event == "PropertyChanged")
	{
		EdsInt32 proeprtyID = *static_cast<EdsInt32 *>(e->getArg());
		if(proeprtyID==kEdsPropID_Evf_AFMode)
		{
			CameraModel* model = (CameraModel *)from;
			//this->EnableWindow(model->getEvfAFMode()!=2);
		}
	}
}


//EvfZoomButton  message handler
void CEvfZoomAction::OnClicked() 
{
	CClickAction::OnClicked();
}
