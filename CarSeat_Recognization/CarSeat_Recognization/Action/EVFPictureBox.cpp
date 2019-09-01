/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : EVFPictureBox.cpp                                                *
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

#include "../stdafx.h"
#include <atlimage.h>

#include "EVFPictureBox.h"
#include "../Command/DownloadEvfCommand.h"
#include "../../common/Log.h"

#define WM_USER_EVF_DATA_CHANGED		WM_APP+1
// CEVFPictureBox

IMPLEMENT_DYNAMIC(CEVFPictureBox, CStatic)
CEVFPictureBox::CEVFPictureBox():ActionSource(),
active(FALSE),
m_bStopUpdate(false),
rotateZ(0),
m_nImageWidth(0),
m_nImageHeight(0)
{
	memset(&m_focusInfo, 0, sizeof(EdsFocusInfo));
	m_pPixel = new COLORREF[PixelLength];
	memset(m_pPixel, 0, sizeof(COLORREF) * PixelLength);

	m_pExBuffer = new COLORREF[PixelLength];
	memset(m_pExBuffer, 0, sizeof(COLORREF) * PixelLength);

}

CEVFPictureBox::~CEVFPictureBox()
{
	if (m_pPixel != NULL)
	{
		delete[]m_pPixel;
		m_pPixel = NULL;
	}
	if (m_pExBuffer != NULL)
	{
		delete[]m_pExBuffer;
		m_pExBuffer = NULL;
	}
}

void CEVFPictureBox::stopUpdate()
{
	m_bStopUpdate = true;
}


BEGIN_MESSAGE_MAP(CEVFPictureBox, CStatic)
	ON_MESSAGE(WM_USER_EVF_DATA_CHANGED, OnEvfDataChanged)
END_MESSAGE_MAP()



// CEVFPictureBox messge handler

void CEVFPictureBox::update(Observable* from, CameraEvent *e)
{
	if (m_bStopUpdate == true)
	{
		return;
	}
	std::string event = e->getEvent();

	if(event == "EvfDataChanged")
	{
		EVF_DATASET data = *static_cast<EVF_DATASET *>(e->getArg());
	
		//The update processing can be executed from another thread. 
		::SendMessage(this->m_hWnd, WM_USER_EVF_DATA_CHANGED, (WPARAM) &data, NULL);

		EdsInt32 propertyID = kEdsPropID_FocusInfo;
		fireEvent("get_Property", &propertyID);

		// Download image data.
		fireEvent("downloadEVF");
	}
	
	if (event == "PropertyChanged")
	{
		EdsInt32 proeprtyID = *static_cast<EdsInt32 *>(e->getArg());
		if(proeprtyID == kEdsPropID_Evf_OutputDevice)
		{
			CameraModel* model = (CameraModel *)from;
			EdsUInt32 device = model->getEvfOutputDevice();

			// PC live view has started.
			if (!active && (device & kEdsEvfOutputDevice_PC) != 0)
			{
				active = TRUE;
				// Start download of image data.
				fireEvent("downloadEVF");
			}

			// PC live view has ended.
			if(active && (device & kEdsEvfOutputDevice_PC) == 0)
			{
				active = FALSE;
			}
		}
		if(proeprtyID == kEdsPropID_FocusInfo)
		{
			CameraModel* model = (CameraModel *)from;
			float xRatio = 1;
			float yRatio = 1;
			CRect rect;
			GetWindowRect(&rect);

			EdsFocusInfo focusInfo = {0};
			focusInfo = model->getFocusInfo();
			m_focusInfo = focusInfo;
			xRatio = (float)(rect.Width())/(float)(focusInfo.imageRect.size.width);
			yRatio = (float)(rect.Height())/(float)(focusInfo.imageRect.size.height);
			for(EdsUInt32 i = 0; i < focusInfo.pointNumber; i++)
			{
				m_focusInfo.focusPoint[i].rect.point.x = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.x * xRatio);
				m_focusInfo.focusPoint[i].rect.point.y = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.y * yRatio);
				m_focusInfo.focusPoint[i].rect.size.width = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.width * xRatio);
				m_focusInfo.focusPoint[i].rect.size.height = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.height * yRatio);
			}
		}
		if(proeprtyID==kEdsPropID_Evf_AFMode)
		{
			CameraModel* model = (CameraModel *)from;
			m_bDrawZoomFrame = model->getEvfAFMode()!=2;
		}
	}	
}

void CEVFPictureBox::reverseRotateZ()
{
	rotateZ += 90;
	rotateZ %= 360;
}

LRESULT CEVFPictureBox::OnEvfDataChanged(WPARAM wParam, LPARAM lParam)
{
	EVF_DATASET data = *(EVF_DATASET *)wParam;
	EdsUInt64 size = 0;

	unsigned char* pbyteImage = NULL; 

	// Get image (JPEG) pointer.
	EdsGetPointer(data.stream, (EdsVoid**)&pbyteImage );
	
	if(pbyteImage != NULL)
	{
		EdsGetLength(data.stream, &size);

		CDC *pDC = GetDC();
	
		// Display image data.
		OnDrawImage(pDC, pbyteImage, size);

		// Display the focus border if displaying the entire image.
		if(data.zoom == 1 && (data.sizeJpegLarge.width != 0 && data.sizeJpegLarge.height != 0))
		{
			OnDrawFocusRect(pDC, CRect(data.zoomRect.point.x, data.zoomRect.point.y, data.zoomRect.point.x + data.zoomRect.size.width, data.zoomRect.point.y + data.zoomRect.size.height), CSize(data.sizeJpegLarge.width, data.sizeJpegLarge.height));
		}

		ReleaseDC(pDC);
	}
	else 
	{
		static time_t preLogTime = time(NULL);
		if (time(NULL) - preLogTime >= 1)
		{
			WriteInfo("pbyteimage is null");
			preLogTime = time(NULL);
		}
	}

	return 0;
}



void CEVFPictureBox::OnDrawImage(CDC *pDC, unsigned char* pbyteImage, int size)
{
	CImage image;

	CComPtr<IStream> stream;
	stream = NULL;

	HGLOBAL hMem = ::GlobalAlloc(GHND, size); 
	LPVOID pBuff = ::GlobalLock(hMem);

	memcpy(pBuff, pbyteImage, size);

	::GlobalUnlock(hMem);
	CreateStreamOnHGlobal(hMem, TRUE, &stream);

	image.Load(stream);

	auxRotateZ(image, rotateZ);

	int tmpImageHeight = image.GetHeight();
	int tmpImageWidth = image.GetWidth();
	if ((tmpImageHeight != m_nImageHeight) || (tmpImageWidth != m_nImageWidth))
	{
		m_nImageHeight = tmpImageHeight;
		m_nImageWidth = tmpImageWidth;
		m_nBpp = image.GetBPP();
		WriteInfo("size = %d, width = %d, height = %d, bpp = %d", size, tmpImageWidth,
			tmpImageHeight, m_nBpp);
	}
	
	CRect rect;
	GetWindowRect(&rect);
	// Drawing
	SetStretchBltMode(pDC->GetSafeHdc() , COLORONCOLOR);

	//WriteInfo("rect width = %d, height = %d", rect.Width(), rect.Height());
	//WriteInfo("image width = %d, hegith = %d", image.GetWidth(), image.GetHeight());
	
	image.StretchBlt(pDC->GetSafeHdc(), 0, 0, rect.Width(),rect.Height(),0,0,image.GetWidth(), image.GetHeight(),SRCCOPY);
	
	//image.BitBlt(hDC, 0, 0);
	image.Destroy();

	::GlobalFree(hMem);
}


void CEVFPictureBox::OnDrawFocusRect(CDC *pDC, CRect zoomRect, CSize sizeJpegLarge)
{
	// Get window size.
	CRect rect;
	GetWindowRect(&rect);

	// Draw Zoom Frame
	if(m_bDrawZoomFrame)
	{
		int cx = rect.Width();
		int cy = rect.Height();

		// The zoomPosition is given by the coordinates of the upper left of the focus border using Jpeg Large size as a reference.
		
		// The size of the focus border is one fifth the size of Jpeg Large.
		// Because the image fills the entire window, the height and width to be drawn is one fifth of the window size.

		int iw = sizeJpegLarge.cx;
		int ih = sizeJpegLarge.cy;

		// Upper left coordinate to be drawn.
		CPoint topLeft(zoomRect.left * cx / iw, zoomRect.top * cy / ih);

		// Upper right coordinate to be drawn.
		CPoint topRight(zoomRect.right * cx / iw, zoomRect.top * cy / ih);

		// Lower right coordinate to be drawn.
		CPoint bottomRight(zoomRect.right * cx / iw, zoomRect.bottom * cy / ih);

		// Lower left coordinate to be drawn.
		CPoint bottomLeft(zoomRect.left * cx / iw, zoomRect.bottom * cy / ih);

		// Draw.
		CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen *oldPen = pDC->SelectObject(&pen);
	 	
		pDC->MoveTo(topLeft);
		pDC->LineTo(topRight);
		pDC->LineTo(bottomRight);
		pDC->LineTo(bottomLeft);
		pDC->LineTo(topLeft);
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
	}

	// Draw AF Frames
	HPEN defaultPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN justPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN disablePen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	HPEN oldPenH;
	LOGBRUSH  logBrush = {0};
	logBrush.lbStyle = BS_NULL;
	HBRUSH newBrush = CreateBrushIndirect(&logBrush);
	HBRUSH oldBrush = (HBRUSH)SelectObject(pDC->m_hDC, newBrush);

	for(EdsUInt32 i = 0; i < m_focusInfo.pointNumber; i++)
	{
		if(m_focusInfo.focusPoint[i].valid==1) {
			// Selecte Just Focus Pen
			if(m_focusInfo.focusPoint[i].justFocus&1) {
				oldPenH = (HPEN)SelectObject(pDC->m_hDC, justPen);
			} else {
				oldPenH = (HPEN)SelectObject(pDC->m_hDC, defaultPen);
			}
			if(m_focusInfo.focusPoint[i].selected!=1)
				oldPenH = (HPEN)SelectObject(pDC->m_hDC, disablePen);


			// Set Frame Rect
			RECT frame;
			frame.left = (int)(m_focusInfo.focusPoint[i].rect.point.x);
			frame.top = (int)(m_focusInfo.focusPoint[i].rect.point.y);
			frame.right = frame.left + (int)(m_focusInfo.focusPoint[i].rect.size.width);
			frame.bottom = frame.top + (int)(m_focusInfo.focusPoint[i].rect.size.height);
			Rectangle(pDC->m_hDC, frame.left, frame.top, frame.right, frame.bottom);
			SelectObject(pDC->m_hDC, oldPenH);
		}
	}
	SelectObject(pDC->m_hDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(disablePen);
	DeleteObject(justPen);
	DeleteObject(defaultPen);
}

void CEVFPictureBox::auxRotateZ(COLORREF * byte, unsigned int size, int degree)
{
	if (size != m_nImageHeight * m_nImageWidth)
	{
		return;
	}
	memset(m_pPixel, 0, sizeof(COLORREF) * PixelLength);
	if (degree == 90)
	{
		//dst.SetPixel(tmpHeight - j - 1, i, src.GetPixel(i, j));
		for (int i = 0; i < m_nImageWidth; ++i)
		{
			for (int j = 0; j < m_nImageHeight; ++j)
			{
				unsigned int dstIndex = (m_nImageHeight - j - 1) * m_nImageWidth + i;
				unsigned int srcIndex = i * m_nImageHeight + j;
				m_pPixel[dstIndex] = byte[srcIndex];
			}
		}
	}
	else if (degree == 180)
	{
		for (int i = 0; i < m_nImageWidth; ++i)
		{
			for (int j = 0; j < m_nImageHeight; ++j)
			{
				unsigned int dstIndex = (m_nImageWidth - i - 1) * m_nImageHeight + (m_nImageHeight - 1 - j);
				unsigned int srcIndex = i * m_nImageHeight + j;
				m_pPixel[dstIndex] = byte[srcIndex];
			}
		}
	}
	else if(degree == 270)
	{
		for (int i = 0; i < m_nImageWidth; ++i)
		{
			for (int j = 0; j < m_nImageHeight; ++j)
			{
				unsigned int dstIndex = j * m_nImageWidth + (m_nImageWidth - 1 - i);
				unsigned int srcIndex = i * m_nImageHeight + j;
				m_pPixel[dstIndex] = byte[srcIndex];
			}
		}
	}
	memcpy(byte, m_pPixel, size * sizeof(COLORREF));
}

void CEVFPictureBox::auxRotateZ(CImage &src, int degree)
{
	if ((degree == 0) || (m_pExBuffer == nullptr))
	{
		return;
	}
	if (degree == 90)
	{
		OnRotate90Image(src);
	}
	else if (degree == 180)
	{
		OnRotate180Image(src);
	}
	else if (degree == 270)
	{
		OnRotate270Image(src);
	}
}



void CEVFPictureBox::OnRotate90Image(CImage &srcImage)
{
	// TODO: 在此添加控件通知处理程序代码
	if ((HBITMAP)srcImage == NULL)
	{
		return;
	}

	int pit = srcImage.GetPitch();
	int bitCount = srcImage.GetBPP() / 8;
	byte *pRealData = (byte*)srcImage.GetBits();

	int tmpHeight = srcImage.GetHeight();
	int tmpWidth = srcImage.GetWidth();
	int tmpBpp = srcImage.GetBPP();

	memset(m_pExBuffer, 0, PixelLength);

	memcpy(m_pExBuffer, pRealData + (tmpHeight - 1) * pit, tmpHeight * tmpWidth * bitCount);

	srcImage.Destroy();
	pRealData = NULL;

	srcImage.Create(tmpHeight, tmpWidth, tmpBpp, 0);

	int dstPit = srcImage.GetPitch();
	byte *dstData = (byte*)srcImage.GetBits();

	for (size_t i = 0; i < tmpHeight * tmpWidth * bitCount; i = i + bitCount)
	{
		size_t j = i / bitCount;
		int x = j % tmpWidth;
		int y = j / tmpWidth;
		y = tmpHeight - 1 - y;

		unsigned char *tmp = (unsigned char*)m_pExBuffer;
		byte *curData = dstData + dstPit * x + (tmpHeight - 1 - y) * bitCount;
		curData[0] = tmp[i];
		curData[1] = tmp[i + 1];
		curData[2] = tmp[i + 2];
	}

}


void CEVFPictureBox::OnRotate180Image(CImage &srcImage)
{
	// TODO: 在此添加控件通知处理程序代码
	if ((HBITMAP)srcImage == NULL)
	{
		return;
	}

	unsigned int tmpBase = 0;
	int tmpImageHeight = srcImage.GetHeight();
	int tmpImageWidth = srcImage.GetWidth();
	int pit = srcImage.GetPitch();
	int tmpBpp = srcImage.GetBPP();
	int bitCount = tmpBpp / 8;
	byte *pRealData = (byte*)srcImage.GetBits();

	memcpy((char*)m_pExBuffer, pRealData + pit * (tmpImageHeight - 1), tmpImageHeight * tmpImageWidth * bitCount);

	char *left = (char*)pRealData + pit * (tmpImageHeight - 1);
	char *right = left + (tmpImageHeight * tmpImageWidth - 1) * bitCount;

	while (left < right)
	{
		for (int i = 0; i < bitCount; ++i)
		{
			*left ^= *right;
			*right ^= *left;
			*left ^= *right;

			left++;
			right++;
		}

		right = right - 2 * bitCount;
	}
	
}


void CEVFPictureBox::OnRotate270Image(CImage &srcImage)
{
	// TODO: 在此添加控件通知处理程序代码
	if ((HBITMAP)srcImage == NULL)
	{
		return;
	}

	int pit = srcImage.GetPitch();
	int bitCount = srcImage.GetBPP() / 8;
	byte *pRealData = (byte*)srcImage.GetBits();

	int tmpHeight = srcImage.GetHeight();
	int tmpWidth = srcImage.GetWidth();
	int tmpBpp = srcImage.GetBPP();

	memset(m_pExBuffer, 0, PixelLength);

	memcpy(m_pExBuffer, pRealData + (tmpHeight - 1) * pit, tmpHeight * tmpWidth * bitCount);

	srcImage.Destroy();
	pRealData = NULL;
	srcImage.Create(tmpHeight, tmpWidth, tmpBpp, 0);

	int dstPit = srcImage.GetPitch();
	byte *dstData = (byte*)srcImage.GetBits();

	for (size_t i = 0; i < tmpHeight * tmpWidth * bitCount; i = i + bitCount)
	{
		size_t j = i / bitCount;
		int x = j % tmpWidth;
		int y = j / tmpWidth;
		y = tmpHeight - 1 - y;

		unsigned char *tmp = (unsigned char*)m_pExBuffer;
		byte *curData = dstData + dstPit * (tmpWidth - 1 - x) + y * bitCount;
		curData[0] = tmp[i];
		curData[1] = tmp[i + 1];
		curData[2] = tmp[i + 2];
	}
}
