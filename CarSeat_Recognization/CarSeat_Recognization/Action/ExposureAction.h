/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : ExposureComp.h	                                              *
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

#include "Property.h"
#include "Observer.h"
#include "CameraEvent.h"
#include "ActionSource.h"

class CExposureAction : public CProperty, public ActionSource , public Observer  
{

public:
	CExposureAction();
	virtual ~CExposureAction();

	//observer
	virtual void update(Observable* from, CameraEvent *e);

protected:
	void OnSelChange(int index);
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);
	//DECLARE_MESSAGE_MAP()
};


