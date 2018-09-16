/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : MeteringMode.cpp                                                *
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

#include "MeteringModeAction.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CMeteringModeAction

CMeteringModeAction::CMeteringModeAction( ):ActionSource()
{
	// set up action command
	setActionCommand("set_MeteringMode");
	if (_propertyTable == nullptr)
	{
		_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	}

	// Map of value and display name
	_propertyTable->insert( std::make_pair(1,"Spot"));
	_propertyTable->insert( std::make_pair(3,"Evaluative"));
	_propertyTable->insert( std::make_pair(4,"Partial"));
	_propertyTable->insert( std::make_pair(5,"Center-Weighted Average"));
	_propertyTable->insert( std::make_pair(0xffffffff,"unknown"));
}

CMeteringModeAction::~CMeteringModeAction()
{
}


//BEGIN_MESSAGE_MAP(CMeteringModeAction, CProperty)
//	ON_MESSAGE(WM_USER_PROPERTY_CHANGED, OnPropertyChanged)
//	ON_MESSAGE(WM_USER_PROPERTYDESC_CHANGED, OnPropertyDescChanged)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
//END_MESSAGE_MAP()


// CMeteringModeAction message handler
void CMeteringModeAction::OnSelChange(int index) 
{
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CMeteringModeAction::update(Observable* from, CameraEvent *e)
{

	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_MeteringMode)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTY_CHANGED, NULL, NULL);
			OnPropertyChanged(0, 0);
		}
	}

	//Update of list that can set property
	if(event == "PropertyDescChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_MeteringMode)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}
	}
}

LRESULT CMeteringModeAction::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	//updateProperty(getCameraModel()->getMeteringMode());
	return 0;
}

//LRESULT CMeteringModeAction::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getMeteringModeDesc());
//	return 0;
//}


