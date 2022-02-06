//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSBaroCtl.cpp : Implementation of the CWSBaroCtrl ActiveX Control class.

#include "stdafx.h"
#include "WSView.h"
#include "WSBaroCtl.h"
#include "WSBaroPpg.h"
#include "WinscoreDefs.h"
#include "2dtransform.h"
#include "location.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWSBaroCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWSBaroCtrl, COleControl)
	//{{AFX_MSG_MAP(CWSBaroCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
    ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BARO_ZOOM, OnZoom)
	ON_COMMAND(ID_BARO_RESET, OnReset)
	ON_COMMAND(ID_BARO_MEASURE, OnMeasure)
	ON_UPDATE_COMMAND_UI(ID_BARO_MEASURE, OnUpdateMeasure)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWSBaroCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CWSBaroCtrl)
	DISP_FUNCTION(CWSBaroCtrl, "AddPilot", AddPilot, VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I4)
	DISP_FUNCTION(CWSBaroCtrl, "SetCursorPos", SetCursorPos, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CWSBaroCtrl, "AddPoint", AddPoint, VT_EMPTY, VTS_I2 VTS_I4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSBaroCtrl, "SetTimeRange", SetTimeRange, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CWSBaroCtrl, "AddPoints", AddPoints, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CWSBaroCtrl, "ResetAll", ResetAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSBaroCtrl, "AddPointByName", AddPointByName, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSBaroCtrl, "OnControlKey", OnControlKey, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CWSBaroCtrl, "SetTaskOpenTime", SetTaskOpenTime, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CWSBaroCtrl, "RedrawBaro", RedrawBaro, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSBaroCtrl, "SetPilotVisibility", SetPilotVisibility, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CWSBaroCtrl, "GetCursorPos", GetCursorPos, VT_I4, VTS_NONE)
	DISP_FUNCTION(CWSBaroCtrl, "IsCursorEnd", IsCursorEnd, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CWSBaroCtrl, "CursorHome", CursorHome, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSBaroCtrl, "AddFullPoint", AddFullPoint, VT_EMPTY, VTS_I2 VTS_I4 VTS_I2 VTS_I2 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CWSBaroCtrl, "SetDisplayOption", SetDisplayOption, VT_EMPTY, VTS_I4 VTS_I2)
	DISP_FUNCTION(CWSBaroCtrl, "GetDisplayOption", GetDisplayOption, VT_I2, VTS_I4 )
	DISP_FUNCTION(CWSBaroCtrl, "SetTaskCloseTime", SetTaskCloseTime, VT_EMPTY, VTS_I4)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CWSBaroCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWSBaroCtrl, COleControl)
	//{{AFX_EVENT_MAP(CWSBaroCtrl)
	EVENT_CUSTOM("OnMoveCursor", FireOnMoveCursor, VTS_I4)
	EVENT_CUSTOM("UpdateText", FireUpdateText, VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CWSBaroCtrl, 1)
	PROPPAGEID(CWSBaroPropPage::guid)
END_PROPPAGEIDS(CWSBaroCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWSBaroCtrl, "WSBARO.WSBaroCtrl.1",
	0x49134336, 0x1ce2, 0x11d4, 0xb1, 0xd2, 0x54, 0xf8, 0x5e, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWSBaroCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWSBaro =
		{ 0x49134334, 0x1ce2, 0x11d4, { 0xb1, 0xd2, 0x54, 0xf8, 0x5e, 0, 0, 0 } };
const IID BASED_CODE IID_DWSBaroEvents =
		{ 0x49134335, 0x1ce2, 0x11d4, { 0xb1, 0xd2, 0x54, 0xf8, 0x5e, 0, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWSBaroOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWSBaroCtrl, IDS_WSBARO, _dwWSBaroOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::CWSBaroCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWSBaroCtrl

BOOL CWSBaroCtrl::CWSBaroCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WSBARO,
			IDB_WSBARO,
			afxRegApartmentThreading,
			_dwWSBaroOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::CWSBaroCtrl - Constructor

CWSBaroCtrl::CWSBaroCtrl():
	m_cCursor(m_cPlotWindow, m_c2DTransform)
{
	InitializeIIDs(&IID_DWSBaro, &IID_DWSBaroEvents);

	m_strFontName	="Arial";
	m_bMeasureMode	=false;
	m_bDragging		=false;

	m_eUnits	=eStatute;
	m_eAltUnits	=eFeet;
	m_cStartTime=0;
	m_cEndTime	=0;
	m_cTaskOpenTime=0;
	m_cTaskCloseTime=0;
	m_cPlotWindow=CRect(0,0,0,0);
	m_cDataWindow=CRect(0,0,0,0);
	m_cInfoWindow=CRect(0,0,0,0);
	m_uDisplayMask= PRESSURETRACE | GPSTRACE;// | ENLTRACE;

	m_hMeasureCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_MEASURE));
	m_pZoom=NULL;
	m_iDataMax	=0;
	m_iDataMin	=0;
	m_bZoomMode	=false;
	m_pMeasuringDlg=NULL;
	m_bDemo		=false;

	m_pFont18	=new CFont;
	m_pFont15	=new CFont;
	m_pFont12	=new CFont;
	m_pFont10	=new CFont;

	m_pFont18->CreateFont(
						18,		//int nHeight, 
						0,		// int nWidth, 
						0,		// int nEscapement, 
						0,		//  int nOrientation, 
						0,	//int nWeight, 
						FALSE,	// BYTE bItalic, 
						0,	// BYTE bUnderline, 
						FALSE,	// BYTE cStrikeOut, 
						ANSI_CHARSET,	// BYTE nCharSet, 
						OUT_DEVICE_PRECIS,		// BYTE nOutPrecision, 
						CLIP_DEFAULT_PRECIS,	// BYTE nClipPrecision, 
						DEFAULT_QUALITY,	// BYTE nQuality, 
						DEFAULT_PITCH,	// BYTE nPitchAndFamily, 
						m_strFontName);	//	 LPCTSTR lpszFacename );

	m_pFont15->CreateFont(
						15,		//int nHeight, 
						0,		// int nWidth, 
						0,		// int nEscapement, 
						0,		//  int nOrientation, 
						0,	//int nWeight, 
						FALSE,	// BYTE bItalic, 
						0,	// BYTE bUnderline, 
						FALSE,	// BYTE cStrikeOut, 
						ANSI_CHARSET,	// BYTE nCharSet, 
						OUT_DEVICE_PRECIS,		// BYTE nOutPrecision, 
						CLIP_DEFAULT_PRECIS,	// BYTE nClipPrecision, 
						DEFAULT_QUALITY,	// BYTE nQuality, 
						DEFAULT_PITCH,	// BYTE nPitchAndFamily, 
						m_strFontName);	//	 LPCTSTR lpszFacename );

	m_pFont12->CreateFont(
						12,		//int nHeight, 
						0,		// int nWidth, 
						0,		// int nEscapement, 
						0,		//  int nOrientation, 
						0,	//int nWeight, 
						FALSE,	// BYTE bItalic, 
						0,	// BYTE bUnderline, 
						FALSE,	// BYTE cStrikeOut, 
						ANSI_CHARSET,	// BYTE nCharSet, 
						OUT_DEVICE_PRECIS,		// BYTE nOutPrecision, 
						CLIP_DEFAULT_PRECIS,	// BYTE nClipPrecision, 
						DEFAULT_QUALITY,	// BYTE nQuality, 
						DEFAULT_PITCH,	// BYTE nPitchAndFamily, 
						m_strFontName);	//	 LPCTSTR lpszFacename );

	m_pFont10->CreateFont(
						10,		//int nHeight, 
						0,		// int nWidth, 
						0,		// int nEscapement, 
						0,		//  int nOrientation, 
						0,	//int nWeight, 
						FALSE,	// BYTE bItalic, 
						0,	// BYTE bUnderline, 
						FALSE,	// BYTE cStrikeOut, 
						ANSI_CHARSET,	// BYTE nCharSet, 
						OUT_DEVICE_PRECIS,		// BYTE nOutPrecision, 
						CLIP_DEFAULT_PRECIS,	// BYTE nClipPrecision, 
						DEFAULT_QUALITY,	// BYTE nQuality, 
						DEFAULT_PITCH,	// BYTE nPitchAndFamily, 
						m_strFontName);	//	 LPCTSTR lpszFacename );

}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::~CWSBaroCtrl - Destructor

CWSBaroCtrl::~CWSBaroCtrl()
{
	m_pFont18->DeleteObject();
	m_pFont15->DeleteObject();
	m_pFont12->DeleteObject();
	m_pFont10->DeleteObject();
	delete m_pFont18;
	delete m_pFont15;
	delete m_pFont12;
	delete m_pFont10;

	if (m_hMeasureCursor) DestroyCursor(m_hMeasureCursor);

	if( m_pMeasuringDlg )
		{
		m_pMeasuringDlg->DestroyWindow();
		delete m_pMeasuringDlg;
		}

}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::OnDraw - Drawing function

void CWSBaroCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CSingleLock cLock(&m_cSemaphore);
	cLock.IsLocked();
	cLock.Lock();

	m_bMeasureMode=false;
	if( m_pMeasuringDlg )
		{
		m_pMeasuringDlg->DestroyWindow();
		delete m_pMeasuringDlg;
		m_pMeasuringDlg=NULL;
		}

	if( m_pZoom )
			{
			delete m_pZoom;
			m_pZoom = NULL;
			}


	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	bool	bFullScreen;

	GetPlotArea(rcBounds, m_cPlotWindow, m_cInfoWindow, bFullScreen);
	GetPlotDimensions( m_cDataWindow);

	m_c2DTransform.Initialize(m_cPlotWindow, m_cDataWindow);

	DrawGrid( pdc, m_c2DTransform, m_cPlotWindow, m_cDataWindow, bFullScreen);

	DrawTraces(pdc, m_c2DTransform);

	m_cCursor.SetPlotWindow(m_cPlotWindow);
	m_cCursor.DrawCursor(pdc );
	UpdateText(pdc );

	cLock.Unlock();

}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::DoPropExchange - Persistence support

void CWSBaroCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::OnResetState - Reset control to default state

void CWSBaroCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl::AboutBox - Display an "About" box to the user

void CWSBaroCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_WSBARO);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl message handlers

short CWSBaroCtrl::AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass, long lColorREF) 
{
	CSingleLock cLock(&m_cSemaphore);
	cLock.IsLocked();
	cLock.Lock();

	short s=m_cBaroTraceGroup.AddTrace( new CBaroTrace( strContestNo, strLastName, strFirstName, (EClass)sClass, lColorREF) );

	cLock.Unlock();
	return s;

}

void CWSBaroCtrl::SetCursorPos(long lCursorTime) 
{
		CDC* pDC = GetDC();
		FireOnMoveCursor(  m_cCursor.MoveCursorTo(pDC, CTime(lCursorTime) ) );
		UpdateText(pDC);
		ReleaseDC(pDC);
}

void CWSBaroCtrl::AddPoint(	short sPilotIndex, 
							long lTimeIndex, 
							short sGPSAltitude, 
							short sPressureAltitude) 
	{
	CBaroTrace* pTrace=m_cBaroTraceGroup.GetTrace(sPilotIndex);
	if( !pTrace ) return;

	pTrace->AddPoint(CBaroPoint( lTimeIndex, sGPSAltitude, sPressureAltitude) );
	m_cCursor.IncreaseMaxXLimit( lTimeIndex );
	}


void CWSBaroCtrl::GetPlotDimensions(CRect &cRect)
{
	short sMaxAltitude, sMinAltitude;


	// Size X axis
	if( m_iDataMin==0 || m_iDataMax==0 )
		{
		cRect.left=(long)m_cStartTime.GetTime();
		cRect.right=(long)m_cEndTime.GetTime();
		}
	else
		{
		cRect.left=m_iDataMin;
		cRect.right=m_iDataMax;
		}

	// Size Vertical Axis
	m_cBaroTraceGroup.GetDataMinMax(cRect.left, cRect.right, m_uDisplayMask, sMaxAltitude, sMinAltitude);
	if( sMaxAltitude==0 && sMinAltitude==0 ) //Give something to workwith if no data yet
		{
		if( m_eAltUnits==eMeters )
			sMaxAltitude=5000; 
		else
			sMaxAltitude=1000; 
		}

	if( m_eAltUnits==eMeters )
		{
		cRect.top=(((short)ConvertAltitude(sMaxAltitude,m_eAltUnits)/250)+1)*250;
		cRect.bottom=(((short)ConvertAltitude(sMinAltitude,m_eAltUnits)/250)-1)*250;
		cRect.bottom=max( cRect.bottom, 0);
		}
	else
		{
		cRect.top=(((short)ConvertAltitude(sMaxAltitude,m_eAltUnits)/1000)+1)*1000;
		cRect.bottom=(((short)ConvertAltitude(sMinAltitude,m_eAltUnits)/1000)-1)*1000;
		cRect.bottom=max( cRect.bottom, 0);
		}

	// If ENL is on, then make sure it is scaled accordingly
	if( m_uDisplayMask & ENLTRACE )
		cRect.bottom=0;


}

void CWSBaroCtrl::GetPlotArea(const CRect &cWindow, CRect &cPlotArea, CRect &cInfoWindow, bool &bFullscreen)
	{
	cPlotArea		=cWindow;

	int iHeight=cPlotArea.Height();
	int iDeflate=iHeight/10;
	iDeflate=min(iDeflate,10);

	cPlotArea.DeflateRect(iDeflate,iDeflate);
	cPlotArea.right-=50;
	cPlotArea.bottom-=10;
	bFullscreen		=false;

//	if( cPlotArea.Height()<30 ||
//		cPlotArea.top	> cPlotArea.bottom ||
//		cPlotArea.right	< cPlotArea.left		)
//		{
//		cPlotArea=cWindow;
//		cPlotArea.right-=200;
//		bFullscreen=true;
//		}

	cInfoWindow=cWindow;
	cInfoWindow.left=cPlotArea.right+60;
	cInfoWindow.DeflateRect( iDeflate, iDeflate );

	if( m_uDisplayMask & ENLTRACE )
		{
		m_cPlotWindow.left+=25;
		}
	}


void CWSBaroCtrl::DrawTraces(CDC *pdc, C2DTransform &c2DTransform)
	{
	m_cBaroTraceGroup.DrawTraces( pdc, c2DTransform, false, m_eUnits, m_eAltUnits, m_uDisplayMask);
	}


void CWSBaroCtrl::DrawGrid( CDC* pdc, C2DTransform &c2DTransform, CRect &cPlotWindow, CRect &cDataWindow, bool bFullscreen)
	{
	int iTickHeight=7;

	pdc->FrameRect( m_cPlotWindow,  CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)) );

	if( m_cStartTime==m_cEndTime ) return;

	CFont* pOldFont = pdc->SelectObject( m_pFont15 );

	// draw horizontal labels
	CTime cTest(m_cStartTime.GetYear(),
				m_cStartTime.GetMonth(),
				m_cStartTime.GetDay(),
				0,0,0);
	short sFirstX=0, sLastX=0;
	bool bFirst=true;
	while( 1 )
		{
		cTest+=CTimeSpan(0,0,15,0);
		if( cTest<cDataWindow.left ) continue;
		if( cTest>cDataWindow.right ) break;
		
		short sX=c2DTransform.WorldXToScreen((long)cTest.GetTime());
		if( cTest.GetMinute()==0 )
			{
			if( !bFullscreen )
				{
				if( bFirst )
					{
					sFirstX=sX; 
					bFirst=false;
					}
				sLastX=sX;
				pdc->TextOut( sX, m_cPlotWindow.bottom+4, cTest.Format("%H:%M") );
				}
			pdc->MoveTo( sX, m_cPlotWindow.bottom );
			pdc->LineTo( sX, m_cPlotWindow.top );
			}
		else if( cTest.GetMinute()==30 ) // do a big tick
			{
			pdc->MoveTo( sX, m_cPlotWindow.bottom );
			pdc->LineTo( sX, m_cPlotWindow.bottom-(iTickHeight*2) );
			}
		else // do a little tick
			{
			pdc->MoveTo( sX, m_cPlotWindow.bottom );
			pdc->LineTo( sX, m_cPlotWindow.bottom-iTickHeight );
			}
		}

	if( !bFullscreen )
		{
		if( sFirstX==0 || sFirstX-m_cPlotWindow.left >30 )
			{
			CTime cStart(m_cDataWindow.left);
			pdc->TextOut( m_cPlotWindow.left, m_cPlotWindow.bottom+4, cStart.Format("%H:%M") );
			}
		if( m_cPlotWindow.right-sLastX >30 )
			{
			CTime cEnd(m_cDataWindow.right);
			pdc->TextOut( m_cPlotWindow.right-5, m_cPlotWindow.bottom+4, cEnd.Format("%H:%M") );
			}
		}

/////// now do the vertical grid

	int iBigTick;
	int iSmallTick;

	if( m_eAltUnits==eMeters )
		{
		iBigTick=1000;
		iSmallTick=500;
		}
		else
		{
		iBigTick=5000;
		iSmallTick=1000;
		}

	int iOldStyle=pdc->SetTextAlign( TA_BOTTOM );

	int iTest=0;
	int nOutput=0;
	while( 1 )
		{
		short sY=c2DTransform.WorldYToScreen(iTest);
		if( iTest<cDataWindow.bottom ) 
			{
			iTest+=iSmallTick;
			continue;
			}
		if( iTest>cDataWindow.top ) 
			{
			if( nOutput==1 )
				{
				pdc->SetTextAlign( TA_TOP );
				CString cText;
				cText.Format(m_eAltUnits==eMeters?("%d m"):("%d ft"), cDataWindow.top);
				CSize cSize=pdc->GetTextExtent( cText );
				pdc->TextOut( m_cPlotWindow.right+4, sY+(cSize.cy/2), cText );
				}
			break;
			}
		
		if( (iTest%iBigTick)==0 || nOutput==0 )
			{
			CString cText;
			cText.Format(m_eAltUnits==eMeters?("%d m"):("%d ft"), iTest);
			CSize cSize=pdc->GetTextExtent( cText );
			pdc->TextOut( m_cPlotWindow.right+4, sY+(cSize.cy/2), cText );
			pdc->MoveTo( m_cPlotWindow.left, sY );
			pdc->LineTo( m_cPlotWindow.right, sY);
			nOutput++;
			}
		else if( (iTest%iSmallTick)==0 ) // do a tick
			{
			pdc->MoveTo( m_cPlotWindow.right,  sY);
			pdc->LineTo( m_cPlotWindow.right-(iTickHeight), sY );
			}
		iTest+=iSmallTick;
		}

// Draw the left vertical grid info for enl
	if( m_uDisplayMask & ENLTRACE )
		{
		pdc->SetTextAlign( TA_RIGHT );
		CString cText="150";
		CSize cSize=pdc->GetTextExtent( cText );
		pdc->TextOut( m_cPlotWindow.left-5, m_cPlotWindow.top-(cSize.cy/2), cText );
		pdc->MoveTo( m_cPlotWindow.left, m_cPlotWindow.top );
		pdc->LineTo( m_cPlotWindow.left-4, m_cPlotWindow.top);

		cText="0";
		cSize=pdc->GetTextExtent( cText );
		pdc->TextOut( m_cPlotWindow.left-5, m_cPlotWindow.bottom-(cSize.cy/2), cText );
		pdc->MoveTo( m_cPlotWindow.left, m_cPlotWindow.bottom );
		pdc->LineTo( m_cPlotWindow.left-4, m_cPlotWindow.bottom);

		cText="ENL";
		cSize=pdc->GetTextExtent( cText );
		pdc->TextOut( m_cPlotWindow.left-5, (m_cPlotWindow.bottom+m_cPlotWindow.top)/2, cText );
		}	
	
	
	pdc->SetTextAlign( iOldStyle );
	pdc->SelectObject( pOldFont );

	if( m_cTaskOpenTime.GetTime()>0 )
		{
		CPen cPen;
		cPen.CreatePen( PS_SOLID, 3, RGB(255, 0, 0) );

		CPen* pOldPen = pdc->SelectObject( &cPen );

		pdc->MoveTo(	c2DTransform.WorldXToScreen((long)m_cTaskOpenTime.GetTime()), 
						m_cPlotWindow.bottom+2 );

		pdc->LineTo(	m_cTaskCloseTime.GetTime()==0?m_cPlotWindow.right:c2DTransform.WorldXToScreen((long)m_cTaskCloseTime.GetTime()), 
						m_cPlotWindow.bottom+2 );

		pdc->SelectObject( pOldPen );
		cPen.DeleteObject();
		}

	}


void CWSBaroCtrl::SetTimeRange(long lStartTime, long lEndTime) 
{
	if( m_cStartTime.GetTime()==0 || m_cStartTime.GetTime() > lStartTime )
		{
		m_cStartTime=lStartTime;
		}
	if( m_cEndTime.GetTime()==0 || m_cEndTime.GetTime() < lEndTime )
		{
		m_cEndTime=lEndTime;
		}
	m_cCursor.SetTimeRange((long)m_cStartTime.GetTime(), (long)m_cEndTime.GetTime());
}

void CWSBaroCtrl::AddPoints(const VARIANT FAR& vlTime, 
							const VARIANT FAR& vstrContestNo, 
							const VARIANT FAR& vsPressureAlt, 
							const VARIANT FAR& vsGPSAlt) 
{
	CSingleLock cLock(&m_cSemaphore);
	cLock.IsLocked();
	cLock.Lock();

    COleSafeArray olesafearrayTime(vlTime);
    COleSafeArray olesafearrayCID(vstrContestNo);
    COleSafeArray olesafearrayPAlt(vsPressureAlt);
    COleSafeArray olesafearrayGAlt(vsGPSAlt);

	long  *paTime;
	short *paPAltitude;
	short *paGAltitude;
	char  *paCID;

	olesafearrayTime.AccessData((void **)&paTime);
    olesafearrayCID.AccessData((void **)&paCID);
    olesafearrayPAlt.AccessData((void **)&paPAltitude);
    olesafearrayGAlt.AccessData((void **)&paGAltitude);

    int nFixes = olesafearrayTime.GetOneDimSize();

	for( int i=0, ic=0; i<nFixes; i++, ic+=3 )
		{
		char cCID[4];
		strncpy_s( cCID, 4, "   ", 3);
		cCID[3]='\0';
		strncpy_s( cCID, 4, &(const char)paCID[ic], 3);
		CString strCID=cCID;
		strCID.TrimRight();
		AddPointByName( strCID, paTime[i], paPAltitude[i], paGAltitude[i]);
		}

	olesafearrayTime.UnaccessData();
    olesafearrayCID.UnaccessData();
    olesafearrayPAlt.UnaccessData();
    olesafearrayGAlt.UnaccessData();

	cLock.Unlock();

    return;
}

void CWSBaroCtrl::ResetAll() 
{
	m_cBaroTraceGroup.Purge();
}

void CWSBaroCtrl::AddPointByName(LPCTSTR strContestNo, long lTimeIndex, short sGPSAltitude, short sPressureAltitude) 
{
	short sIndex=m_cBaroTraceGroup.GetPilotIndex(strContestNo);
	if( sIndex<0 ) return;
	AddPoint( sIndex, lTimeIndex, sGPSAltitude, sPressureAltitude);
}

void CWSBaroCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bZoomMode )
		{
		CDC* pDC = GetDC();
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		if( m_pZoom ) m_pZoom->OnMove(*pDC, point);
		ReleaseDC(pDC);
		}
	else if( m_bMeasureMode ) 
		{
		if( m_bDragging )
			{
			CDC* pDC = GetDC();
			::SetCursor(m_hMeasureCursor);
			if( m_pZoom ) m_pZoom->OnMove(*pDC, point);

			if( m_pZoom && m_pMeasuringDlg )
				{
				m_pMeasuringDlg->SetRange(	m_c2DTransform.ScreenXToWorld(m_pZoom->m_lStartPoint),
											m_c2DTransform.ScreenXToWorld(m_pZoom->m_lEndPoint)  );
				}
			ReleaseDC(pDC);
			}
		else if( m_pMeasuringDlg && !m_pMeasuringDlg->IsWindowVisible() )
			{
			m_bMeasureMode=false;
			if( m_pZoom )
				{
				CDC* pDC = GetDC();
				m_pZoom->Cancel(*pDC);
				ReleaseDC(pDC);
				delete m_pZoom;
				m_pZoom = NULL;
				}
			}
		}
	else
		{
		if( nFlags & MK_LBUTTON )
			{
			CDC* pDC = GetDC();
			int lClosest=GetClosestPoint(CTime(m_c2DTransform.ScreenXToWorld((short)point.x)));
			int iNewPos=m_cCursor.MoveCursorTo(pDC, CTime(lClosest) );
			UpdateText(pDC);
			if( iNewPos>=0 )
				FireOnMoveCursor( lClosest );
			ReleaseDC(pDC);

			/*
			CDC* pDC = GetDC();
			int iNewPos=m_cCursor.MoveCursorTo(pDC, (short)point.x);
			UpdateText(pDC);
			if( iNewPos>=0 )
				FireOnMoveCursor( m_c2DTransform.ScreenXToWorld(iNewPos) );
			ReleaseDC(pDC);
			*/

			}
		}
	COleControl::OnMouseMove(nFlags, point);
}

void CWSBaroCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bDragging=true;
	m_cDragPoint=point;

	if( m_bZoomMode )
		{
		if( !m_pZoom )
			m_pZoom = new CBaroZoom( m_cPlotWindow );

		m_pZoom->SetBeginning( point.x );

		m_iDataMin	= m_c2DTransform.ScreenXToWorld((short)point.x);
		}
	else if( m_bMeasureMode )
		{
		::SetCursor(m_hMeasureCursor);
		CDC* pDC = GetDC();
		if( m_pZoom )
			{
			m_pZoom->Cancel(*pDC);
			delete m_pZoom;
			}
		m_pZoom = new CBaroZoom( m_cPlotWindow );
		m_pZoom->SetBeginning( point.x );
		}
	else
		{
		CDC* pDC = GetDC();
		int iNewPos=m_cCursor.MoveCursorTo(pDC,(short)point.x);
		UpdateText(pDC);
		FireOnMoveCursor( m_c2DTransform.ScreenXToWorld(iNewPos) );
		ReleaseDC(pDC);

		if( m_pMeasuringDlg )
			{
			m_pMeasuringDlg->DestroyWindow();
			delete m_pMeasuringDlg;
			m_pMeasuringDlg=NULL;
			}

		}

	COleControl::OnLButtonDown(nFlags, point);
}


void CWSBaroCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	m_bDragging=false;
	if( m_bMeasureMode  )
		{
		::SetCursor(m_hMeasureCursor);
		CSize size=m_cDragPoint-point;
		if( abs(size.cx)<5  ) 
			{
			m_bMeasureMode=false;
			InvalidateRect(NULL,FALSE);
			}
		}
	else if( m_bZoomMode && m_pZoom!=NULL )
		{
		m_iDataMin	= m_c2DTransform.ScreenXToWorld((short)m_pZoom->m_lStartPoint);
		m_iDataMax	= m_c2DTransform.ScreenXToWorld((short)m_pZoom->m_lEndPoint);
		if( m_iDataMax<m_iDataMin )
			{
			int iTemp=m_iDataMin;
			m_iDataMin=m_iDataMax;
			m_iDataMax=m_iDataMin;
			}
		if( m_iDataMax-m_iDataMin<120 )
			{
			m_iDataMax=m_iDataMin+120;
			}

		InvalidateRect(NULL,FALSE);
		}

	m_bZoomMode=false;
	COleControl::OnLButtonUp(nFlags, point);
}

void CWSBaroCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	OnControlKey(nChar);

	COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CWSBaroCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
//    if (m_cToolTip.Create(this) && m_cToolTip.AddTool(this, LPCTSTR(_T("Tool tip"))))
//      m_cToolTip.Activate(TRUE);

    return 0;
    }

void CWSBaroCtrl::OnControlKey(short nChar) 
{
	CDC* pDC = GetDC();
	if( nChar==39 || nChar==38 ) // right arrow or up arrow
		{
		MoveCursorNext();
		}
	if( nChar==37 || nChar==40 )// left arrow or down arrow
		{
//		FireOnMoveCursor(  m_cCursor.MoveCursorInc(pDC,-10) );
		MoveCursorNext(true);
		}
	if( nChar==34 ) //pgdn
		{
		FireOnMoveCursor(  m_cCursor.MoveCursorInc(pDC,-120) );
		}
	if( nChar==33 ) //pgup
		{
		FireOnMoveCursor(  m_cCursor.MoveCursorInc(pDC,120) );
		}
	if( nChar==36 ) //home
		{
		FireOnMoveCursor(  m_cCursor.MoveCursorTo(pDC, m_cStartTime ) );
		}
	if( nChar==35 ) //end
		{
		FireOnMoveCursor(  m_cCursor.MoveCursorTo(pDC, m_cEndTime ) );
		}
	UpdateText(pDC);
	ReleaseDC(pDC);
}

void CWSBaroCtrl::SetTaskOpenTime(long lTime) 
{
	m_cTaskOpenTime=lTime;
}

void CWSBaroCtrl::SetTaskCloseTime(long lTime) 
{
	m_cTaskCloseTime=lTime;
}

void CWSBaroCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	COleControl::OnKillFocus(pNewWnd);		
}

void CWSBaroCtrl::RedrawBaro() 
{
	CSingleLock cLock(&m_cSemaphore);
	cLock.IsLocked();
	cLock.Lock();

	CDC* pDC = GetDC();
	m_cBaroTraceGroup.DrawTraces(pDC, m_c2DTransform, true, m_eUnits, m_eAltUnits );
	ReleaseDC(pDC);

	cLock.Unlock();
}

void CWSBaroCtrl::SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible) 
{
	short sIndex=m_cBaroTraceGroup.GetPilotIndex(strContestNo);
	if( sIndex<0 ) return;
	CBaroTrace* pTrace=m_cBaroTraceGroup.GetTrace(sIndex);
	if( pTrace ) pTrace->m_bVisible=(bVisible)?(true):(false);
}

long CWSBaroCtrl::GetCursorPos() 
{
	return m_cCursor.m_lLocation;
}

BOOL CWSBaroCtrl::IsCursorEnd() 
{
	return m_cCursor.IsCursorAtEnd();

	return TRUE;
}

void CWSBaroCtrl::CursorHome() 
{
	CDC* pDC = GetDC();
	FireOnMoveCursor(  m_cCursor.MoveCursorTo(pDC, m_cStartTime ) );
	UpdateText(pDC);
	ReleaseDC(pDC);
}

void CWSBaroCtrl::UpdateText(CDC *pDC)
{
	if(!pDC) return;

	int iHeight=m_cInfoWindow.Height();
	CBaroPoint *pPoint=NULL;
	CFont* pOldFont=NULL;
	CString strText;
	int iGap=3;
	CTime	cCursorTime=m_cCursor.m_lLocation;
	CString strCursorTime=cCursorTime.Format("Time: %H:%M:%S");
	CString strPosition, strGPSAltitude, strPresAltitude, strSpeed, strVSpeed, strMotor;


	CBaroTrace* pcTrace=IsSinglePilotVisible();
	if( pcTrace )
		pPoint=pcTrace->GetBaroPoint( cCursorTime );

	int iYPos= m_cInfoWindow.top;
	pDC->FillRect(m_cInfoWindow, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	if( pcTrace )
		{
		if( iHeight<30 )
			{
			iGap=1;
			pOldFont = pDC->SelectObject( m_pFont10 );
			}
		else if( iHeight<50 )
			{
			iGap=2;
			pOldFont = pDC->SelectObject( m_pFont12 );
			}
		else if( iHeight<80 )
			{
			iGap=3;
			pOldFont = pDC->SelectObject( m_pFont15 );
			}
		else
			{
			iGap=3;
			pOldFont = pDC->SelectObject( m_pFont18 );
			}
		}

//	pDC->TextOut(m_cInfoWindow.left, iYPos, strCursorTime );
//	CSize cSize=pDC->GetTextExtent(strCursorTime);
//	iYPos+=cSize.cy+iGap;

#define kts_to_kph 1.85  
#define m_to_ft  3.281  /* m to feet. */

	if( pcTrace && pPoint )
		{
		CLocation cLoc(pPoint->m_fLat, pPoint->m_fLong);
		strPosition=cLoc.GetFormattedInfo();
		if( m_eUnits==eKilometers )
			{
			float fSpeed=pPoint->m_fSpeed;
			fSpeed=fSpeed*(float)kts_to_kph;
			strSpeed.Format("%6.2f kph",fSpeed);
			}
		else if( m_eUnits==eNautical )
			{
			strSpeed.Format("%6.2f kts", pPoint->m_fSpeed);
			}
		else if( m_eUnits==eStatute )
			{
			strSpeed.Format("%6.2f mph", pPoint->m_fSpeed);
			}

		if( m_eAltUnits==eMeters )
			{
			strGPSAltitude.Format("GPS Alt: %d m",(short)ConvertAltitude(pPoint->m_sGPSAltitude,m_eAltUnits));
			strPresAltitude.Format("Press Alt: %d m",(short)ConvertAltitude(pPoint->m_sPressure,m_eAltUnits));
			strVSpeed.Format("%6.2f m/s", pPoint->m_fVSpeed/m_to_ft);
			}
		else
			{
			strGPSAltitude.Format("GPS Alt: %d ft",pPoint->m_sGPSAltitude);
			strPresAltitude.Format("Press Alt: %d ft",pPoint->m_sPressure);
			strVSpeed.Format("%6.0f ft/min.", pPoint->m_fVSpeed*60.);
			}
		strMotor.Format("%d", pPoint->m_sENL);

//		pDC->SetTextColor( GPSTRACECOLOR );
//		pDC->TextOut(m_cInfoWindow.left, iYPos, strGPSAltitude );
//		cSize=pDC->GetTextExtent(strGPSAltitude);
//		iYPos+=cSize.cy+iGap;

//		pDC->SetTextColor( PRESSTRACECOLOR );
//		pDC->TextOut(m_cInfoWindow.left, iYPos, strPresAltitude );
//		cSize=pDC->GetTextExtent(strPresAltitude);
//		iYPos+=cSize.cy+iGap;

//		strText.Format("Lat : %s",cLoc.GetFormattedLat());
//		pDC->SetTextColor( RGB(0,0,0) );
//		pDC->TextOut(m_cInfoWindow.left, iYPos, strText );
//		cSize=pDC->GetTextExtent(strText);
//		iYPos+=cSize.cy+iGap;

//		strText.Format("Long: %s",cLoc.GetFormattedLong());
//		pDC->SetTextColor( RGB(0,0,0) );
//		pDC->TextOut(m_cInfoWindow.left, iYPos, strText );
//		cSize=pDC->GetTextExtent(strText);
//		iYPos+=cSize.cy+iGap;

		}
	if( pOldFont ) pDC->SelectObject( pOldFont );


	FireUpdateText( strCursorTime, 
					strPosition,  
					strGPSAltitude,  
					strPresAltitude, 
					strSpeed, 
					strVSpeed,
					strMotor);

}

CBaroTrace* CWSBaroCtrl::IsSinglePilotVisible()
{
	return m_cBaroTraceGroup.IsSinglePilotVisible();
}

void CWSBaroCtrl::MoveCursorNext(bool bBack)
	{
	CBaroTrace* pTrace=IsSinglePilotVisible();
	if( !pTrace )
		{
		CDC* pDC = GetDC();
		FireOnMoveCursor(  m_cCursor.MoveCursorInc(pDC,bBack?-10:10) );
		ReleaseDC(pDC);
		}
	else
		{
		long lNext=0;
		if( bBack )
			lNext=pTrace->GetPrevPoint( CTime(m_cCursor.m_lLocation) );
		else
			lNext=pTrace->GetNextPoint( CTime(m_cCursor.m_lLocation) );
		SetCursorPos(lNext);
		}
	}

void CWSBaroCtrl::AddFullPoint(	short sPilotIndex, 
								long lTimeIndex, 
								short sGPSAltitude, 
								short sPressAltitude, 
								float fLat, 
								float fLong, 
								float fSpeed, 
								float fVSpeed, 
								short sENL) 
{
	CBaroTrace* pTrace=m_cBaroTraceGroup.GetTrace(sPilotIndex);
	if( !pTrace ) return;

	pTrace->AddPoint(CBaroPoint(	lTimeIndex, 
									sGPSAltitude, 
									sPressAltitude,
									fLat, 
									fLong, 
									fSpeed, 
									fVSpeed,
									sENL) );
	m_cCursor.IncreaseMaxXLimit( lTimeIndex );
}

short CWSBaroCtrl::GetDisplayOption(long lOption) 
{
	switch(	lOption )
		{
		case	WSO_DEMO_MODE:
			{
			return m_bDemo;
			break;
			}
		case	WSC_PRESS_ALT_DISPLAY:
			{
			if( m_uDisplayMask & PRESSURETRACE ) 
				return 1;
			else
				return 0;
			break;
			}
		case	WSC_GPS_ALT_DISPLAY:
			{
			if( m_uDisplayMask & GPSTRACE ) 
				return 1;
			else
				return 0;
			break;
			}
		case	WSC_ENL_DISPLAY:
			{
			if( m_uDisplayMask & ENLTRACE ) 
				return 1;
			else
				return 0;
			break;
			}
		default:
			{
			break;
			}
		}
		return 0;
}

void CWSBaroCtrl::SetDisplayOption(long lOption, short sValue) 
{
	switch(	lOption )
		{
		case	WSO_DEMO_MODE:
			{
			m_bDemo=sValue==1?true:false;
			break;
			}
		case	WSC_UNITS:
			{
			m_eUnits=(EUnits)sValue;
			break;
			}
		case	WSC_ALTUNITS:
			{
			m_eAltUnits=(EAltUnits)sValue;
			break;
			}
		case	WSC_PRESS_ALT_DISPLAY:
			{
			if( sValue==0 )
				{
				if( m_uDisplayMask & PRESSURETRACE )
					m_uDisplayMask ^=PRESSURETRACE;
				}
			else
				{
				if( !(m_uDisplayMask & PRESSURETRACE) )
					m_uDisplayMask ^=PRESSURETRACE;
				}
			InvalidateRect(NULL,FALSE);
			break;
			}
		case	WSC_GPS_ALT_DISPLAY:
			{
			if( sValue==0 )
				{
				if( m_uDisplayMask & GPSTRACE )
					m_uDisplayMask ^=GPSTRACE;
				}
			else
				{
				if( !(m_uDisplayMask & GPSTRACE) )
					m_uDisplayMask ^=GPSTRACE;
				}
			InvalidateRect(NULL,FALSE);
			break;
			}
		case	WSC_ENL_DISPLAY:
			{
			if( sValue==0 )
				{
				if( m_uDisplayMask & ENLTRACE )
					m_uDisplayMask ^=ENLTRACE;
				}
			else
				{
				if( !(m_uDisplayMask & ENLTRACE) )
					m_uDisplayMask ^=ENLTRACE;
				}
			InvalidateRect(NULL,FALSE);
			break;
			}
		default:
			{
			break;
			}
		}
}

float ConvertAltitude(short sAltitude,  EAltUnits eUnitsTo )
	{
	if( eUnitsTo==eMeters ) 
		return (((float)sAltitude)/(float)3.2808333333464565);

	return sAltitude;
	}

BOOL CWSBaroCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if( zDelta>0 )
		OnControlKey(34);
	else
		OnControlKey(33);

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

void CWSBaroCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
		if( m_bMeasureMode )
			{
			m_bMeasureMode=false;
			InvalidateRect(NULL,FALSE);
			}

        CMenu menuResource, menuPopup;
#ifdef _DEBUG
			menuResource.LoadMenu(IDR_BARO_POPUP);
#else
		if( GetDisplayOption(WSO_DEMO_MODE) )
			menuResource.LoadMenu(IDR_BARO_POPUP_DEMO);
		else
			menuResource.LoadMenu(IDR_BARO_POPUP);
#endif
        menuPopup.CreatePopupMenu();

        int iItemCount = menuResource.GetMenuItemCount();
        for (int iMenuItemIndex = 0; iMenuItemIndex < iItemCount; iMenuItemIndex++)
            {
            int iID=menuResource.GetMenuItemID(iMenuItemIndex);
        
            if (iID == 0)
                menuPopup.AppendMenu(MF_SEPARATOR);
            else
                {
                char szTemp[80];
                menuResource.GetMenuString(iMenuItemIndex, szTemp, sizeof(szTemp), MF_BYPOSITION);
                menuPopup.AppendMenu(MF_ENABLED, iID, szTemp);
                }
            }

        ClientToScreen(&point);
        menuPopup.TrackPopupMenu(TPM_LEFTALIGN, point.x,  point.y, this);

		COleControl::OnRButtonDown(nFlags, point);
		ReleaseCapture();

}

void CWSBaroCtrl::OnReset() 
	{
	m_iDataMax	=0;
	m_iDataMin	=0;
	InvalidateRect(NULL,FALSE);
	}

void CWSBaroCtrl::OnZoom() 
	{
	m_bZoomMode=true;	
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	ReleaseCapture();
	}

void CWSBaroCtrl::OnMeasure() 
{
	m_bMeasureMode=true;	
	::SetCursor(m_hMeasureCursor);

	if( !m_pMeasuringDlg )
		{
		m_pMeasuringDlg=new CBaroMeasureDlg( m_cBaroTraceGroup.GetTrace(0),
											 m_eUnits,
											 m_eAltUnits,
											 m_uDisplayMask);

		m_pMeasuringDlg->Create(IDD_BARO_MEASURE, this);

		CRect rc;
		GetClientRect(rc);
		CPoint point(rc.right/2,rc.bottom);

		ClientToScreen(&point);

		m_pMeasuringDlg->SetWindowPos( &wndTopMost, point.x, point.y, 0, 0, SWP_NOSIZE );

		m_pMeasuringDlg->ShowWindow(SW_SHOW);
		}

	ReleaseCapture();
}

void CWSBaroCtrl::OnUpdateMeasure(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_cBaroTraceGroup.IsSinglePilotVisible()!=NULL );
}


long CWSBaroCtrl::GetClosestPoint(CTime &cTime)
	{
	CBaroTrace* pTrace=IsSinglePilotVisible();
	if( pTrace )
		{
		return pTrace->GetClosestPoint(cTime);
		}
	return (long)cTime.GetTime();
}
