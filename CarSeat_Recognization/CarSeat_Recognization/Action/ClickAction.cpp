/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : ActionButton.cpp                                                *
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

#include "ClickAction.h"


/////////////////////////////////////////////////////////////////////////////
// CClickAction

CClickAction::CClickAction():ActionSource()
{
}

CClickAction::~CClickAction()
{
}


//ActionButton  message handler
void CClickAction::OnClicked() 
{
	fireEvent();
}
