/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : AEMode.cpp                                                      *
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

#include "AEModeAction.h"
#include "CameraEvent.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2

// CAEModeAction
CAEModeAction::CAEModeAction( ):ActionSource()
{
	// set up action command
	setActionCommand("set_AEMode");
	_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	if (_propertyTable != nullptr)
	{
		// Map of value and display name
		_propertyTable->insert(std::make_pair(0, "P"));
		_propertyTable->insert(std::make_pair(1, "Tv"));
		_propertyTable->insert(std::make_pair(2, "Av"));
		_propertyTable->insert(std::make_pair(3, "M"));
		_propertyTable->insert(std::make_pair(4, "Bulb"));
		_propertyTable->insert(std::make_pair(5, "A-DEP"));
		_propertyTable->insert(std::make_pair(6, "DEP"));
		_propertyTable->insert(std::make_pair(7, "C1"));
		_propertyTable->insert(std::make_pair(16, "C2"));
		_propertyTable->insert(std::make_pair(17, "C3"));
		_propertyTable->insert(std::make_pair(8, "Lock"));
		_propertyTable->insert(std::make_pair(9, "GreenMode"));
		_propertyTable->insert(std::make_pair(10, "Night Portrait"));
		_propertyTable->insert(std::make_pair(11, "Sports"));
		_propertyTable->insert(std::make_pair(13, "LandScape"));
		_propertyTable->insert(std::make_pair(14, "Close Up"));
		_propertyTable->insert(std::make_pair(15, "No Strobo"));
		_propertyTable->insert(std::make_pair(12, "Portrait"));
		_propertyTable->insert(std::make_pair(19, "Creative Auto"));
		_propertyTable->insert(std::make_pair(20, "Movies"));
		_propertyTable->insert(std::make_pair(25, "SCN"));
		_propertyTable->insert(std::make_pair(22, "SceneIntelligentAuto"));
		_propertyTable->insert(std::make_pair(0xffffffff, "unknown"));
	}
}

CAEModeAction::~CAEModeAction()
{
}


// CAEModeAction message handler
void CAEModeAction::OnSelChange(int index) 
{
	//DWORD_PTR data = GetItemData(GetCurSel());
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CAEModeAction::update(Observable* from, CameraEvent *e)
{
	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_AEModeSelect)
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
		
		if(propertyID == kEdsPropID_AEModeSelect)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}	
	}
}

LRESULT CAEModeAction::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	//updateProperty(getCameraModel()->getAEMode());
	return 0;
}

//LRESULT CAEModeAction::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getAEModeDesc());
//	return 0;
//}





