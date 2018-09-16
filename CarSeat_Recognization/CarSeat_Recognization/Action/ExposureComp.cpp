/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : ExposureComp.cpp                                                *
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

#include "ExposureComp.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CExposureComp

CExposureComp::CExposureComp():ActionSource()
{
	// set up action command
	setActionCommand("set_ExposureCompensation");

	if (_propertyTable == nullptr)
	{
		_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	}
	// Map of value and display name
	_propertyTable->insert( std::make_pair(0x28,"+5"));
	_propertyTable->insert( std::make_pair(0x25,"+4 2/3"));
	_propertyTable->insert( std::make_pair(0x24,"+4 1/2"));
	_propertyTable->insert( std::make_pair(0x23,"+4 1/3"));
	_propertyTable->insert( std::make_pair(0x20,"+4"));
	_propertyTable->insert( std::make_pair(0x1D,"+3 2/3"));
	_propertyTable->insert( std::make_pair(0x1C,"+3 1/2"));
	_propertyTable->insert( std::make_pair(0x1B,"+3 1/3"));
	_propertyTable->insert( std::make_pair(0x18,"+3"));
	_propertyTable->insert( std::make_pair(0x15,"+2 2/3"));
	_propertyTable->insert( std::make_pair(0x14,"+2 1/2"));
	_propertyTable->insert( std::make_pair(0x13,"+2 1/3"));
	_propertyTable->insert( std::make_pair(0x10,"+2"));
	_propertyTable->insert( std::make_pair(0x0d,"+1 2/3"));
	_propertyTable->insert( std::make_pair(0x0c,"+1 1/2"));
	_propertyTable->insert( std::make_pair(0x0b,"+1 1/3"));
	_propertyTable->insert( std::make_pair(0x08,"+1"));
	_propertyTable->insert( std::make_pair(0x05,"+2/3"));
	_propertyTable->insert( std::make_pair(0x04,"+1/2"));
	_propertyTable->insert( std::make_pair(0x03,"+1/3"));
	_propertyTable->insert( std::make_pair(0x00,"0"));
	_propertyTable->insert( std::make_pair(0xfd,"-1/3"));
	_propertyTable->insert( std::make_pair(0xfc,"-1/2"));
	_propertyTable->insert( std::make_pair(0xfb,"-2/3"));
	_propertyTable->insert( std::make_pair(0xf8,"-1"));
	_propertyTable->insert( std::make_pair(0xf5,"-1 1/3"));
	_propertyTable->insert( std::make_pair(0xf4,"-1 1/2"));
	_propertyTable->insert( std::make_pair(0xf3,"-1 2/3"));
	_propertyTable->insert( std::make_pair(0xf0,"-2"));
	_propertyTable->insert( std::make_pair(0xed,"-2 1/3"));
	_propertyTable->insert( std::make_pair(0xec,"-2 1/2"));
	_propertyTable->insert( std::make_pair(0xeb,"-2 2/3"));
	_propertyTable->insert( std::make_pair(0xe8,"-3"));
	_propertyTable->insert( std::make_pair(0xE5,"-3 1/3"));
	_propertyTable->insert( std::make_pair(0xE4,"-3 1/2"));
	_propertyTable->insert( std::make_pair(0xE3,"-3 2/3"));
	_propertyTable->insert( std::make_pair(0xE0,"-4"));
	_propertyTable->insert( std::make_pair(0xDD,"-4 1/3"));
	_propertyTable->insert( std::make_pair(0xDC,"-4 1/2"));
	_propertyTable->insert( std::make_pair(0xDB,"-4 2/3"));
	_propertyTable->insert( std::make_pair(0xD8,"-5"));
	_propertyTable->insert( std::make_pair(0xffffffff,"unknown"));
}

CExposureComp::~CExposureComp()
{
}


//BEGIN_MESSAGE_MAP(CExposureComp, CProperty)
//	ON_MESSAGE(WM_USER_PROPERTY_CHANGED, OnPropertyChanged)
//	ON_MESSAGE(WM_USER_PROPERTYDESC_CHANGED, OnPropertyDescChanged)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
//END_MESSAGE_MAP()



// CExposureComp message handler
void CExposureComp::OnSelChange(int index) 
{
	//DWORD_PTR data = GetItemData(GetCurSel());
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CExposureComp::update(Observable* from, CameraEvent *e)
{

	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_ExposureCompensation)
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
		
		if(propertyID == kEdsPropID_ExposureCompensation)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}	
	}
}

LRESULT CExposureComp::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	updateProperty(getCameraModel()->getExposureCompensation());
	return 0;
}

//LRESULT CExposureComp::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getExposureCompensationDesc());
//	return 0;
//}


