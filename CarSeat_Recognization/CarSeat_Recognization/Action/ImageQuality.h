/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : ImageQuality.h	                                              *
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
#include "ActionSource.h"
#include "CameraEvent.h"
// CImageQuality

class CImageQuality : public CProperty, public ActionSource , public Observer  
{

public:
	CImageQuality();
	virtual ~CImageQuality();

	//observer
	virtual void update(Observable* from, CameraEvent *e);

protected:
	void OnSelChange(int index);
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);

	//DECLARE_MESSAGE_MAP()
};


