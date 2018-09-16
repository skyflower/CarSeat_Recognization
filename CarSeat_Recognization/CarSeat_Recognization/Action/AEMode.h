/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : AEMode.h    	                                                  *
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

#include "Observer.h"
#include "ActionSource.h"
#include "Property.h"
#include "EDSDK.h"
// CAEModeAction

class CAEModeAction : public CProperty, public ActionSource , public Observer
{

public:
	CAEModeAction( );
	virtual ~CAEModeAction();

	//observer
	virtual void update(Observable* from, CameraEvent *e);

protected:
	void OnSelChange(int index);

	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);
	
	//std::unordered_map<EdsUInt32, std::string> *_propertyTable;
};


