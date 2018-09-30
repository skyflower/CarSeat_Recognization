/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : EVFPictureBox.h	                                              *
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

#include "Observer.h"
#include "ActionSource.h"
#include "EDSDK.h"

// CEVFPictureBox
class CEVFPictureBox : public CStatic, public ActionSource , public Observer
{
	DECLARE_DYNAMIC(CEVFPictureBox)
	
private:
	BOOL active;
	EdsFocusInfo	m_focusInfo;
	EdsBool			m_bDrawZoomFrame;
	volatile bool m_bStopUpdate;
	int rotateZ;
	int m_nImageWidth;
	int m_nImageHeight;
	void OnDrawImage(CDC *pDC, unsigned char* pbyteImage, int size);
	void OnDrawFocusRect(CDC *pDC, CRect zoomRect, CSize sizeJpegLarge);

	void auxRotateZ(char *byte, unsigned int size, int degree);
	CImage auxRotateZ(CImage &src, int degree);

public:
	CEVFPictureBox();
	virtual ~CEVFPictureBox();

	void stopUpdate();

	//observer
	virtual void update(Observable* from, CameraEvent *e);

	void reverseRotateZ();

protected:
	afx_msg LRESULT OnEvfDataChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


