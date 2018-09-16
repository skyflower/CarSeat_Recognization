/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : MeteringMode.h	                                              *
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


#include "MeteringModeAction.h"
#include "Property.h"
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
// CMeteringModeAction

class CMeteringModeAction : public CProperty, public ActionSource , public Observer  
{

public:
	CMeteringModeAction();
	virtual ~CMeteringModeAction();

	//observer
	virtual void update(Observable* from, CameraEvent *e);

protected:
	void OnSelChange(int index);
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);

	//DECLARE_MESSAGE_MAP()
};


