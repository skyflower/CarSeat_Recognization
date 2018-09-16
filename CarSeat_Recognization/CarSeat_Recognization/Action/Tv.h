/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : Tv.h      	                                                  *
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

// CTv

class CTv : public CProperty, public ActionSource , public Observer
{

public:
	CTv();
	virtual ~CTv();


	//observer
	virtual void update(Observable* from, CameraEvent *e);

protected:
	void OnSelChange(int index);
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);
	//DECLARE_MESSAGE_MAP()
};


