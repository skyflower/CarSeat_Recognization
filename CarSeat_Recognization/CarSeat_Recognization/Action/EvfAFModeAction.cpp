/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : EvfAFMode.cpp                                                   *
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

#include "EvfAFModeAction.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CEvfAFModeAction

CEvfAFModeAction::CEvfAFModeAction():ActionSource()
{
	// set up action command
	setActionCommand("set_EvfAFMode");

	// Map of value and display name
	if (_propertyTable == nullptr)
	{
		_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	}
	_propertyTable->insert( std::make_pair(0x00,"Quick mode"));
	_propertyTable->insert( std::make_pair(0x01,"Live mode"));
	_propertyTable->insert( std::make_pair(0x02,"Live face detection mode"));
	_propertyTable->insert( std::make_pair(0x03,"Live FlexiZone-Multi mode"));
	_propertyTable->insert( std::make_pair(0xffffffff,"unknown"));
}

CEvfAFModeAction::~CEvfAFModeAction()
{
}



// CEvfAFModeAction message handler
void CEvfAFModeAction::OnSelChange(int index)
{
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CEvfAFModeAction::update(Observable* from, CameraEvent *e)
{
	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_Evf_AFMode)
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
		
		if(propertyID == kEdsPropID_Evf_AFMode)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}	
	}
}

LRESULT CEvfAFModeAction::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	updateProperty(getCameraModel()->getEvfAFMode());
	return 0;
}

//LRESULT CEvfAFModeAction::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getEvfAFModeDesc());
//	return 0;
//}