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
	int m_nBpp;

	const static size_t PixelLength = 10 * 1024 * 1024 * 3;
	COLORREF *m_pPixel;
	COLORREF *m_pExBuffer;

	void OnDrawImage(CDC *pDC, unsigned char* pbyteImage, int size);
	void OnDrawFocusRect(CDC *pDC, CRect zoomRect, CSize sizeJpegLarge);

	void auxRotateZ(COLORREF *byte, unsigned int size, int degree);
	void auxRotateZ(CImage &src, int degree);

	void OnRotate90Image(CImage &srcImage);
	void OnRotate180Image(CImage &srcImage);
	void OnRotate270Image(CImage &srcImage);

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
};


