/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : PropertyComboBox.cpp                                            *
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

#include "Property.h"


// CProperty

CProperty::CProperty():m_nCurSelected(-1)
{
	_propertyTable = nullptr;
}

CProperty::~CProperty()
{
	if (_propertyTable != nullptr)
	{
		delete _propertyTable;
		_propertyTable = nullptr;
	}
}

void CProperty::OnSelChange(int index)
{
	if ((_propertyTable == nullptr) || (_propertyTable->size() >= index))
	{
		return;
	}
	m_nCurSelected = index;
}

// CProperty message handler
void CProperty::updateProperty(EdsUInt32 value)
{
	// The character string corresponding to data is acquired. 
	std::unordered_map<EdsUInt32, std::string>::iterator itr = _propertyTable->find(value);
	if (itr != _propertyTable->end())
	{
		m_nCurSelected = value;
		// Set String combo box
		//SetWindowText(itr->second);
	}
}
//
//
//void CProperty::updatePropertyDesc(const EdsPropertyDesc* desc)
//{
//	// The content of the list is deleted.
//	// Current settings values are not changed in some cases even if the list changes, so leave the selected text as it is
//	CString ss;
//	GetWindowText(ss);
//	ResetContent();
//	SetWindowText(ss);
//	
//	// It makes it to disable when there is no value list that can be set. 
//	EnableWindow( desc->numElements != 0 );
//
//
//	for(int i = 0; i < desc->numElements; i++)
//	{
//		// The character string corresponding to data is acquired.
//		std::map<EdsUInt32, const char *>::iterator itr = _propertyTable.find((EdsUInt32)desc->propDesc[i]);
//
//		// Create list of combo box
//		if (itr != _propertyTable.end())
//		{
//			// Insert string
//			int index = InsertString(-1, itr->second);
//			// Set data
//			SetItemData(index, itr->first);
//		}
//	}	
//}
