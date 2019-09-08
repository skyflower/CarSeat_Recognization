/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : Command.h	                                                      *
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

#include  "CameraModel.h"

class Command {

protected:
	// Camera Model
	CameraModel* _model;
	int failedCount;

	void incFail() { ++failedCount; };

public:
	Command(CameraModel *model) : _model(model), failedCount(0){}

	CameraModel* getCameraModel(){return _model;}

	int getFailedCount() { return failedCount; }

	virtual std::string getCommandName() { return "baseCommand"; }

	// Execute command	
	virtual bool execute() = 0;
};

