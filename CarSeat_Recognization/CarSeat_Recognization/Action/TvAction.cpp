/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : Tv.cpp                                                          *
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

#include "TvAction.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CTvAction

CTvAction::CTvAction():ActionSource()
{
	setActionCommand("set_Tv");
	if (_propertyTable == nullptr)
	{
		_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	}

	_propertyTable->insert( std::make_pair(0x0c,"Bulb"));
	_propertyTable->insert( std::make_pair(0x10,"30h"));
	_propertyTable->insert( std::make_pair(0x13,"25h"));
	_propertyTable->insert( std::make_pair(0x14,"20h"));
	_propertyTable->insert( std::make_pair(0x15,"20h"));
	_propertyTable->insert( std::make_pair(0x18,"15h"));
	_propertyTable->insert( std::make_pair(0x1B,"13h"));
	_propertyTable->insert( std::make_pair(0x1C,"10h"));
	_propertyTable->insert( std::make_pair(0x1D,"10h"));
	_propertyTable->insert( std::make_pair(0x20,"8h"));
	_propertyTable->insert( std::make_pair(0x23,"6h"));
	_propertyTable->insert( std::make_pair(0x24,"6h"));
	_propertyTable->insert( std::make_pair(0x25,"5h"));
	_propertyTable->insert( std::make_pair(0x28,"4h"));
	_propertyTable->insert( std::make_pair(0x2B,"3h2"));
	_propertyTable->insert( std::make_pair(0x2C,"3h"));
	_propertyTable->insert( std::make_pair(0x2D,"2h5"));
	_propertyTable->insert( std::make_pair(0x30,"2h"));
	_propertyTable->insert( std::make_pair(0x33,"1h6"));
	_propertyTable->insert( std::make_pair(0x34,"1h5"));
	_propertyTable->insert( std::make_pair(0x35,"1h3"));
	_propertyTable->insert( std::make_pair(0x38,"1h"));
	_propertyTable->insert( std::make_pair(0x3B,"0h8"));
	_propertyTable->insert( std::make_pair(0x3C,"0h7"));
	_propertyTable->insert( std::make_pair(0x3D,"0h6"));
	_propertyTable->insert( std::make_pair(0x40,"0h5"));
	_propertyTable->insert( std::make_pair(0x43,"0h4"));
	_propertyTable->insert( std::make_pair(0x44,"0h3"));
	_propertyTable->insert( std::make_pair(0x45,"0h3"));
	_propertyTable->insert( std::make_pair(0x48,"4"));
	_propertyTable->insert( std::make_pair(0x4B,"5"));
	_propertyTable->insert( std::make_pair(0x4C,"6"));
	_propertyTable->insert( std::make_pair(0x4D,"6"));
	_propertyTable->insert( std::make_pair(0x50,"8"));
	_propertyTable->insert( std::make_pair(0x53,"10"));
	_propertyTable->insert( std::make_pair(0x54,"10"));
	_propertyTable->insert( std::make_pair(0x55,"13"));
	_propertyTable->insert( std::make_pair(0x58,"15"));
	_propertyTable->insert( std::make_pair(0x5B,"20"));
	_propertyTable->insert( std::make_pair(0x5C,"20"));
	_propertyTable->insert( std::make_pair(0x5D,"25"));
	_propertyTable->insert( std::make_pair(0x60,"30"));
	_propertyTable->insert( std::make_pair(0x63,"40"));
	_propertyTable->insert( std::make_pair(0x64,"45"));
	_propertyTable->insert( std::make_pair(0x65,"50"));
	_propertyTable->insert( std::make_pair(0x68,"60"));
	_propertyTable->insert( std::make_pair(0x6B,"80"));
	_propertyTable->insert( std::make_pair(0x6C,"90"));
	_propertyTable->insert( std::make_pair(0x6D,"100"));
	_propertyTable->insert( std::make_pair(0x70,"125"));
	_propertyTable->insert( std::make_pair(0x73,"160"));
	_propertyTable->insert( std::make_pair(0x74,"180"));
	_propertyTable->insert( std::make_pair(0x75,"200"));
	_propertyTable->insert( std::make_pair(0x78,"250"));
	_propertyTable->insert( std::make_pair(0x7B,"320"));
	_propertyTable->insert( std::make_pair(0x7C,"350"));
	_propertyTable->insert( std::make_pair(0x7D,"400"));
	_propertyTable->insert( std::make_pair(0x80,"500"));
	_propertyTable->insert( std::make_pair(0x83,"640"));
	_propertyTable->insert( std::make_pair(0x84,"750"));
	_propertyTable->insert( std::make_pair(0x85,"800"));
	_propertyTable->insert( std::make_pair(0x88,"1000"));
	_propertyTable->insert( std::make_pair(0x8B,"1250"));
	_propertyTable->insert( std::make_pair(0x8C,"1500"));
	_propertyTable->insert( std::make_pair(0x8D,"1600"));
	_propertyTable->insert( std::make_pair(0x90,"2000"));
	_propertyTable->insert( std::make_pair(0x93,"2500"));
	_propertyTable->insert( std::make_pair(0x94,"3000"));
	_propertyTable->insert( std::make_pair(0x95,"3200"));
	_propertyTable->insert( std::make_pair(0x98,"4000"));
	_propertyTable->insert( std::make_pair(0x9B,"5000"));
	_propertyTable->insert( std::make_pair(0x9C,"6000"));
	_propertyTable->insert( std::make_pair(0x9D,"6400"));
	_propertyTable->insert( std::make_pair(0xA0,"8000"));
	_propertyTable->insert( std::make_pair(0xffffffff,"unknown"));
}

CTvAction::~CTvAction()
{
}

//
//BEGIN_MESSAGE_MAP(CTvAction, CProperty)
//	ON_MESSAGE(WM_USER_PROPERTY_CHANGED, OnPropertyChanged)
//	ON_MESSAGE(WM_USER_PROPERTYDESC_CHANGED, OnPropertyDescChanged)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
//END_MESSAGE_MAP()


// CTvAction message handler
void CTvAction::OnSelChange(int index) 
{
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CTvAction::update(Observable* from, CameraEvent *e)
{

	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_Tv)
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
		
		if(propertyID == kEdsPropID_Tv)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}	
	}
}

LRESULT CTvAction::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	updateProperty(getCameraModel()->getTv());
	return 0;
}

//LRESULT CTvAction::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getTvDesc());
//	return 0;
//}


