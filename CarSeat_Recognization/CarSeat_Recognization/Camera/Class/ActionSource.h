#pragma once

#include <vector>
#include <algorithm>

#include "ActionEvent.h"
#include "ActionListener.h" 
#include "../../common/utils.h"
#include "CameraModel.h"


class ActionSource {
private:
	std::vector<ActionListener*> _listeners;
	std::string _command;
	
protected:

	CameraModel *getCameraModel()
	{
		return nullptr;// m_pHwnd->getAppCameraModel();
	}


public:
	void addActionListener(ActionListener* listener)
	{
		if ( std::find(_listeners.begin(), _listeners.end(), listener) == _listeners.end() )
		{
			_listeners.push_back(listener);
		}
	}

	void setActionCommand(std::string command){ _command = command; }
	
	void fireEvent(void* arg = 0)
	{
		std::vector<ActionListener*>::iterator i = _listeners.begin();
		
		ActionEvent event(_command, arg);
		
		while ( i != _listeners.end() )
		{
			(*i++)->actionPerformed(event);
		}
	}
	
	void fireEvent(const char* command, void* arg = 0)
	{
		std::vector<ActionListener*>::iterator i = _listeners.begin();
		
		ActionEvent event(command, arg);
		
		while ( i != _listeners.end() )
		{
			(*i++)->actionPerformed(event);
		}
	}

	ActionSource() : _listeners(0){}
	virtual ~ActionSource(){}
};