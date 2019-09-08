/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : DownloadCommand.h	                                              *
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

#include "Command.h"
#include "CameraEvent.h"
#include "EDSDK.h"

class DownloadCommand : public Command
{
private:
	EdsDirectoryItemRef _directoryItem;

public:
	DownloadCommand(CameraModel *model, EdsDirectoryItemRef dirItem) 
			: _directoryItem(dirItem), Command(model){}


	virtual std::string getCommandName() { return "DownloadCommand"; }


	virtual ~DownloadCommand()
	{
		//Release item
		if(_directoryItem != NULL)
		{
			EdsRelease( _directoryItem);
			_directoryItem = NULL;
		}
	}


	// Execute command 	
	virtual bool execute()
	{
		EdsError				err = EDS_ERR_OK;
		EdsStreamRef			stream = NULL;

		//Acquisition of the downloaded image information
		EdsDirectoryItemInfo	dirItemInfo;
		err = EdsGetDirectoryItemInfo( _directoryItem, &dirItemInfo);

		//WriteInfo("EdsGetDirectoryItemInfo, err = %d", err);
	
		// Forwarding beginning notification	
		if(err == EDS_ERR_OK)
		{
			CameraEvent e("DownloadStart");
			_model->notifyObservers(&e);
		}

		//WriteInfo("notifyObservers, err = %d", err);

		//Make the file stream at the forwarding destination
		if(err == EDS_ERR_OK)
		{	
			err = EdsCreateFileStream(dirItemInfo.szFileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
		}	

		//WriteInfo("EdsCreateFileStream, err = %d", err);

		//Set Progress
		if(err == EDS_ERR_OK)
		{
			err = EdsSetProgressCallback(stream, ProgressFunc, kEdsProgressOption_Periodically, this);
		}

		//WriteInfo("EdsSetProgressCallback, err = %d", err);

		//Download image
		if(err == EDS_ERR_OK)
		{
			err = EdsDownload( _directoryItem, dirItemInfo.size, stream);
		}

		//WriteInfo("EdsDownload, err = %d", err);

		//Forwarding completion
		if(err == EDS_ERR_OK)
		{
			err = EdsDownloadComplete( _directoryItem);
		}

		//WriteInfo("EdsDownloadComplete, err = %d", err);

		//Release Item
		if(_directoryItem != NULL)
		{
			err = EdsRelease( _directoryItem);
			_directoryItem = NULL;
		}

		//WriteInfo("EdsRelease, err = %d", err);

		//Release stream
		if(stream != NULL)
		{
			err = EdsRelease(stream);
			stream = NULL;
		}		
		

		//WriteInfo("EdsRelease, err = %d", err);
		// Forwarding completion notification
		if( err == EDS_ERR_OK)
		{
			CameraEvent e("DownloadComplete", &err);
			_model->notifyObservers(&e);
		}

		//WriteInfo("notifyObservers, err = %d", err);

		//Notification of error
		if( err != EDS_ERR_OK)
		{
			CameraEvent e("error", &err);
			_model->notifyObservers(&e);
			incFail();
			return false;
		}

		//WriteInfo("notifyObservers, err = %d", err);

		return true;
	}

private:
	static EdsError EDSCALLBACK ProgressFunc (
						EdsUInt32	inPercent,
						EdsVoid *	inContext,
						EdsBool	*	outCancel
						)
	{
		Command *command = (Command *)inContext;
		CameraEvent e("ProgressReport", &inPercent);
		command->getCameraModel()->notifyObservers(&e);
		return EDS_ERR_OK;
	}


};