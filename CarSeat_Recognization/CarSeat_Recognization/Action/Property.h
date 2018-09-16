/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : PropertyComboBox.h	                                          *
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

#include <unordered_map>
#include <EDSDK.h>
// CProperty

class CProperty
{

public:
	CProperty();
	virtual ~CProperty();

protected:
	virtual void OnSelChange(int index) = 0;

	int m_nCurSelected;


	std::unordered_map<EdsUInt32, std::string> *_propertyTable;

	void updateProperty(EdsUInt32 value);
	//void updatePropertyDesc(const EdsPropertyDesc* desc);
};


