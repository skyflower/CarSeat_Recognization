/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : EvfZoomButton.h      	                                          *
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

#pragma once

#include "ClickAction.h"
#include "Observer.h"
#include "CameraEvent.h"
// CEvfZoomAction

class CEvfZoomAction : public CClickAction , public Observer
{

public:
	CEvfZoomAction();
	virtual void update(Observable* from, CameraEvent *e);

public:
	virtual ~CEvfZoomAction();

protected:
	void OnClicked();
};
