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

#include "MeteringMode.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CMeteringMode

CMeteringMode::CMeteringMode( ):ActionSource()
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

CMeteringMode::~CMeteringMode()
{
}


//BEGIN_MESSAGE_MAP(CMeteringMode, CProperty)
//	ON_MESSAGE(WM_USER_PROPERTY_CHANGED, OnPropertyChanged)
//	ON_MESSAGE(WM_USER_PROPERTYDESC_CHANGED, OnPropertyDescChanged)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
//END_MESSAGE_MAP()


// CMeteringMode message handler
void CMeteringMode::OnSelChange(int index) 
{
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CMeteringMode::update(Observable* from, CameraEvent *e)
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

LRESULT CMeteringMode::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	updateProperty(getCameraModel()->getMeteringMode());
	return 0;
}

//LRESULT CMeteringMode::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getMeteringModeDesc());
//	return 0;
//}


