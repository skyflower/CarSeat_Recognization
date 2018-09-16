/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : ImageQuality.cpp                                                *
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

#include "ImageQuality.h"

#define WM_USER_PROPERTY_CHANGED		WM_APP+1
#define WM_USER_PROPERTYDESC_CHANGED	WM_APP+2
// CImageQuality

CImageQuality::CImageQuality():ActionSource()
{
	// set up action command
	setActionCommand("set_ImageQuality");

	if (_propertyTable == nullptr)
	{
		_propertyTable = new std::unordered_map<EdsUInt32, std::string>;
	}
	// PTP Camera
	_propertyTable->insert( std::make_pair(EdsImageQuality_LR,     "RAW"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRLJF,  "RAW + Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRMJF,  "RAW + Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRSJF,  "RAW + Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRLJN,  "RAW + Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRMJN,  "RAW + Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRSJN,  "RAW + Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRS1JF, "RAW + Small1 Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRS1JN, "RAW + Small1 Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRS2JF, "RAW + Small2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRS3JF, "RAW + Small3 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRLJ,   "RAW + Large Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRM1J,  "RAW + Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRM2J,  "RAW + Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LRSJ,   "RAW + Small Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MR,     "Middle Raw(Small RAW1)"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRLJF,  "Middle Raw(Small RAW1) + Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRMJF,  "Middle Raw(Small RAW1) + Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRSJF,  "Middle Raw(Small RAW1) + Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRLJN,  "Middle Raw(Small RAW1) + Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRMJN,  "Middle Raw(Small RAW1) + Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRSJN,  "Middle Raw(Small RAW1) + Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRS1JF, "Middle RAW + Small1 Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRS1JN, "Middle RAW + Small1 Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRS2JF, "Middle RAW + Small2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRS3JF, "Middle RAW + Small3 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRLJ,   "Middle Raw + Large Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRM1J,  "Middle Raw + Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRM2J,  "Middle Raw + Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MRSJ,   "Middle Raw + Small Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SR,     "Small RAW(Small RAW2)"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRLJF,  "Small RAW(Small RAW2) + Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRMJF,  "Small RAW(Small RAW2) + Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRSJF,  "Small RAW(Small RAW2) + Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRLJN,  "Small RAW(Small RAW2) + Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRMJN,  "Small RAW(Small RAW2) + Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRSJN,  "Small RAW(Small RAW2) + Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRS1JF, "Small RAW + Small1 Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRS1JN, "Small RAW + Small1 Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRS2JF, "Small RAW + Small2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRS3JF, "Small RAW + Small3 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRLJ,   "Small RAW + Large Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRM1J,  "Small RAW + Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRM2J,  "Small RAW + Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SRSJ,   "Small RAW + Small Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LJF,    "Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LJN,    "Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MJF,    "Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_MJN,    "Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SJF,    "Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SJN,    "Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_S1JF,   "Small1 Fine Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_S1JN,   "Small1 Normal Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_S2JF,   "Small2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_S3JF,   "Small3 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_LJ,     "Large Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_M1J,    "Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_M2J,    "Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(EdsImageQuality_SJ,     "Small Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR,    "RAW"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRLJF, "RAW + Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRMJF, "RAW + Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRSJF, "RAW + Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRLJN, "RAW + Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRMJN, "RAW + Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LRSJN, "RAW + Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LJF,   "Large Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_MJF,   "Middle Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_SJF,   "Small Fine Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LJN,   "Large Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_MJN,   "Middle Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_SJN,   "Small Normal Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR2,   "RAW"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR2LJ, "RAW + Large Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR2M1J,"RAW + Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR2M2J,"RAW + Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LR2SJ, "RAW + Small Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_LJ,    "Large Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_M1J,   "Middle1 Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_M2J,   "Middle2 Jpeg"));
	_propertyTable->insert( std::make_pair(kEdsImageQualityForLegacy_SJ,    "Small Jpeg"));

}

CImageQuality::~CImageQuality()
{
}


//BEGIN_MESSAGE_MAP(CImageQuality, CProperty)
//	ON_MESSAGE(WM_USER_PROPERTY_CHANGED, OnPropertyChanged)
//	ON_MESSAGE(WM_USER_PROPERTYDESC_CHANGED, OnPropertyDescChanged)
//	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
//END_MESSAGE_MAP()


// CImageQuality message handler
void CImageQuality::OnSelChange(int index) 
{
	//DWORD_PTR data = GetItemData(GetCurSel());
	CProperty::OnSelChange(index);
	fireEvent(&index);
}

void CImageQuality::update(Observable* from, CameraEvent *e)
{

	std::string event = e->getEvent();

	//Update property
	if(event == "PropertyChanged")
	{
		EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());
		
		if(propertyID == kEdsPropID_ImageQuality)
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
		
		if(propertyID == kEdsPropID_ImageQuality)
		{
			//The update processing can be executed from another thread. 
			//::PostMessage(this->m_hWnd, WM_USER_PROPERTYDESC_CHANGED, NULL, NULL);
		}	
	}
}

LRESULT CImageQuality::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	updateProperty(getCameraModel()->getImageQuality());
	return 0;
}

//LRESULT CImageQuality::OnPropertyDescChanged(WPARAM wParam, LPARAM lParam)
//{
//	updatePropertyDesc(&getCameraModel()->getImageQualityDesc());
//	return 0;
//}


