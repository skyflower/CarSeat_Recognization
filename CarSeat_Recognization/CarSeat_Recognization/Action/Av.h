/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : Av.h      	                                                  *
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
#include "CameraEvent.h"
#include "Property.h"

#include <iostream>
#include <unordered_map>
// CAvAction

class CAvAction : public CProperty, public ActionSource , public Observer  
{

public:
	CAvAction();
	virtual ~CAvAction();

	//observer
	virtual void update(Observable* from, CameraEvent *e);


	void OnSelChange(int index);
protected:
	LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	//LRESULT OnPropertyDescChanged(WPARAM wParam, LPARAM lParam);

//private:
//	std::unordered_map<int, std::string> *_propertyTable;
//
//	int m_nCurSelected;

};


