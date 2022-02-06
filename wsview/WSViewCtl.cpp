//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSViewCtl.cpp : Implementation of the CWSViewCtrl ActiveX Control class.

#include "stdafx.h"
#include "WSView.h"
#include "WSViewCtl.h"
#include "WSViewPpg.h"
#include "gllistinfo.h"
#include <math.h>
#include "winscoredefs.h"
// #include "location.h"
#include "WSViewAboutDlg.h"
#include "ModulVer.h"
#include "HitList.h"
#include "tracesproppage.h"
#include "SUA.h"

#include <atlimage.h>
#include <Gdiplusimaging.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		SILHOUETTE  0
#define		FILL		1

static double fCorrectionMin=1;
static double fCorrectionMax=0;

static BOOL LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal );

static CDC *pdcMap=NULL;


#define DEFAULTZSCALE   .025f


IMPLEMENT_DYNCREATE(CWSViewCtrl, COleControl)


#define		NUMDEFAULTCOLORS	10

static unsigned char aDefaultR[NUMDEFAULTCOLORS]={     0,    0, 255, 155,    0,  55,   0, 128, 128, 128 };
static unsigned char aDefaultG[NUMDEFAULTCOLORS]={	   0,   55,	  0,  55,   55,   0, 155, 128,   0, 128 };
static unsigned char aDefaultB[NUMDEFAULTCOLORS]={	 255,    0,   0,   0,  255,  55, 155,   0, 128, 128 };

/*
// these are used to construct an equilibrated 256 color palette
static unsigned char _threeto8[8] = 
{
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

static unsigned char _twoto8[4] = 
{
	0, 0x55, 0xaa, 0xff
};

static unsigned char _oneto8[2] = 
{
	0, 255
};

static int defaultOverride[13] = 
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

// Windows Default Palette
static PALETTEENTRY defaultPalEntry[20] = 
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

*/

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWSViewCtrl, COleControl)
	//{{AFX_MSG_MAP(CWSViewCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_POPUP_ZOOMWINDOW, OnPopupZoomwindow)
	ON_COMMAND(ID_POPUP_PROPERTIES, OnPopupProperties)
	ON_COMMAND(ID_POPUP_COPY, OnPopupCopy)
	ON_COMMAND(ID_SAVEAS, OnSaveAS)
	ON_COMMAND(ID_POPUP_AUTOSCALE, OnPopupAutoscale)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_ABOUT_BOX, OnAboutBox)
	ON_COMMAND(ID_MEASURE, OnMeasure)
	ON_UPDATE_COMMAND_UI(ID_MEASURE, OnUpdateMeasure)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCPAINT()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWSViewCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CWSViewCtrl)
	DISP_PROPERTY_NOTIFY(CWSViewCtrl, "DisplayLength", m_displayLength, OnDisplayLengthChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CWSViewCtrl, "PointDisplay", m_pointDisplay, OnPointDisplayChanged, VT_BOOL)
	DISP_FUNCTION(CWSViewCtrl, "SetTimeRange", SetTimeRange, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "AddPilot", AddPilot, VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetCurrentTimeIndex", SetCurrentTimeIndex, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetPilotVisibility", SetPilotVisibility, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CWSViewCtrl, "SetViewScale", SetViewScale, VT_EMPTY, VTS_R8)
	DISP_FUNCTION(CWSViewCtrl, "SetViewCenter", SetViewCenter, VT_EMPTY, VTS_R4 VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "AddControlPoint", AddControlPoint, VT_EMPTY, VTS_I2 VTS_BSTR VTS_R4 VTS_R4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetStartGateCylinder", SetStartGateCylinder, VT_EMPTY,   VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetFinishGateCylinder", SetFinishGateCylinder, VT_EMPTY,   VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "ClearTask", ClearTask, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "AddTaskPoint", AddTaskPoint, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetDisplayOption", SetDisplayOption, VT_EMPTY, VTS_I4 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "AddPosition", AddPosition, VT_EMPTY, VTS_I2 VTS_I4 VTS_R4 VTS_R4 VTS_I4  VTS_I2 )
	DISP_FUNCTION(CWSViewCtrl, "SetWorldLimits", SetWorldLimits, VT_EMPTY, VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetColor", SetColor, VT_EMPTY, VTS_I2 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetPilotTraceColor", SetPilotTraceColor, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "ResetAll", ResetAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "ResetContestants", ResetContestants, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "Autoscale", Autoscale, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "InitSite", InitSite, VT_EMPTY, VTS_R4 VTS_R4 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetStartGateLine", SetStartGateLine, VT_EMPTY, VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetFinishGateLine", SetFinishGateLine, VT_EMPTY, VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetPilotTimes", SetPilotTimes, VT_EMPTY, VTS_I2 VTS_I4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetFinishTime", SetFinishTime, VT_EMPTY, VTS_I2 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "AddPositionByName", AddPositionByName, VT_EMPTY, VTS_BSTR VTS_I4 VTS_R4 VTS_R4 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetCurrentTime", SetCurrentTime, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "AddFixes", AddFixes, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CWSViewCtrl, "SetMinMaxAltitudes", SetMinMaxAltitudes, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "IsPilotVisible", IsPilotVisible, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "SetTraceDisplayLength", SetTraceDisplayLength, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetActiveClass", SetActiveClass, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "GetColor", GetColor, VT_I4, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "OnEditProperties", OnEditProperties, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "SetPilotText", SetPilotText, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "SetRotations", SetRotations, VT_EMPTY, VTS_R4 VTS_R4 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "SetZScale", SetZScale, VT_EMPTY, VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "NudgeView", NudgeView, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "GetZScale", GetZScale, VT_R4, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "GetPilotColor", GetPilotColor, VT_I4, VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "ScaleView", ScaleView, VT_EMPTY, VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "GetDisplayOption", GetDisplayOption, VT_I2, VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetSceneryPath", SetSceneryPath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "InvalidateGrid", InvalidateGrid, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "ZoomWindow", ZoomWindow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "AddTaskLocation", AddTaskLocation, VT_EMPTY, VTS_I2 VTS_R4 VTS_R4 VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "SetDisplayOptionD", SetDisplayOptionD, VT_EMPTY, VTS_I4 VTS_R8)
	DISP_FUNCTION(CWSViewCtrl, "GetDisplayOptionD", GetDisplayOptionD, VT_R8, VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "AddSUAirspace", AddSUAirspace, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "DisplayNextTaskPoint", DisplayNextTaskPoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "SetTaskPointRadius", SetTaskPointRadius, VT_EMPTY, VTS_I2 VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "AddTATLocation", AddTATLocation, VT_EMPTY, VTS_I2 VTS_R4 VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "DefineControlPointAttributes", DefineControlPointAttributes, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "GetVersionString", GetVersionString, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "GetBuildDate", GetBuildDate, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "OnMeasureCursor", OnMeasureCursor, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "GetCurrentDisplayTime", GetCurrentDisplayTime, VT_I4, VTS_NONE)
	DISP_FUNCTION(CWSViewCtrl, "Set2ndStartGateCylinder", Set2ndStartGateCylinder, VT_EMPTY, VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2)
	DISP_FUNCTION(CWSViewCtrl, "GetDisplayOptionStr", GetDisplayOptionStr, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetDisplayOptionStr", SetDisplayOptionStr, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "SetStartTime", SetStartTime, VT_EMPTY, VTS_I2 VTS_I4)
	DISP_FUNCTION(CWSViewCtrl, "SetTATLocation", SetTATLocation, VT_EMPTY, VTS_I2 VTS_R4 VTS_R4)
	DISP_FUNCTION(CWSViewCtrl, "WriteBMPFile", WriteBMPFile, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CWSViewCtrl, "AutoscaleTask", AutoscaleTask, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWSViewCtrl, COleControl)
	//{{AFX_EVENT_MAP(CWSViewCtrl)
	EVENT_CUSTOM("PilotSelection", FirePilotSelection, VTS_BSTR)
	EVENT_CUSTOM("OnKeyDown", FireOnKeyDown, VTS_I2)
	EVENT_CUSTOM("UpdateProperties", FireUpdateProperties, VTS_NONE)
	EVENT_CUSTOM("PointSelected", FirePointSelected, VTS_I2  VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CWSViewCtrl, 2)
	PROPPAGEID(CWSViewPropPage::guid)
	PROPPAGEID(CTracesPropPage::guid)
END_PROPPAGEIDS(CWSViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWSViewCtrl, "WSVIEW.WSViewCtrl.1",
	0xe0748846, 0x5814, 0x11d3, 0xba, 0x96, 0, 0x10, 0xa4, 0x4, 0x32, 0x81)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWSViewCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWSView =
		{ 0xe0748844, 0x5814, 0x11d3, { 0xba, 0x96, 0, 0x10, 0xa4, 0x4, 0x32, 0x81 } };
const IID BASED_CODE IID_DWSViewEvents =
		{ 0xe0748845, 0x5814, 0x11d3, { 0xba, 0x96, 0, 0x10, 0xa4, 0x4, 0x32, 0x81 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWSViewOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWSViewCtrl, IDS_WSVIEW, _dwWSViewOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::CWSViewCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWSViewCtrl

BOOL CWSViewCtrl::CWSViewCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_WSVIEW,
			IDB_WSVIEW,
			afxRegApartmentThreading,
			_dwWSViewOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::CWSViewCtrl - Constructor

CWSViewCtrl::CWSViewCtrl()
{
	m_fControlPointRadius=(float)0.25/(float)MIPERDEGREE;
	m_fControlPointOuterRadius=(float)1.0/(float)MIPERDEGREE;

	for( int i=0; i<MAXTASKPOINTS; i++ ) 
		m_fTaskPointRadius[i]=m_fControlPointOuterRadius;

	m_cMousePanPoint=CPoint(-1,-1);

	m_hHandCursor	= ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_WSHAND));
	m_hRotateCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE));
	m_hMeasureCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_MEASURE));

	m_iDisplayOptions=0;

	m_sWorldAltMax	=0;
	m_sWorldAltMin	=32000;

	m_fWorldXMin=1000.;
	m_fWorldXMax= -1000.;
	m_fWorldYMin=1000.;
	m_fWorldYMax= -1000.;

	m_fCPXMin=1000.;
	m_fCPXMax= -1000.;
	m_fCPYMin=1000.;
	m_fCPYMax= -1000.;

	m_fTraceXMin=1000.;
	m_fTraceXMax= -1000.;
	m_fTraceYMin=1000.;
	m_fTraceYMax= -1000.;

	m_fStartGateLatitude	=0;
	m_fStartGateLongitude	=0;
	m_fStartGateLatitude2	=0;
	m_fStartGateLongitude2	=0;
	m_eStartGateType	=eCylinder;
	m_fStartGateSize	=0;
	m_sStartGateHeight	=0;
	m_sStartGateBase	=0;

	m_f2ndStartGateLatitude	=0;
	m_f2ndStartGateLongitude=0;
	m_f2ndStartGateSize		=0;
	m_s2ndStartGateHeight	=0;
	m_s2ndStartGateBase		=0;

	m_fFinishGateLatitude	=0;
	m_fFinishGateLongitude	=0;
	m_fFinishGateLatitude2	=0;
	m_fFinishGateLongitude2	=0;
	m_eFinishGateType	=eCylinder;
	m_fFinishGateSize	=0;
	m_sFinishGateBase	=0;
	m_sFinishGateHeight	=0;

//	m_nControlPointsDisplayed	=0;
//	pcControlPointsDisplayPoint	=NULL;
//	ppcControlPointDisplayed	=NULL;
	m_bEnableControlPointHighlight=true;
	m_pHighlightedControlPoint	=NULL;
	m_pcHit						=NULL;
	m_pcMeasureStartHitPoint	=NULL;
	m_pcMeasureDoneHitPoint		=NULL;
	m_bMeasureMode				=false;
	m_bCopy						=false;
	m_bWrite					=false;;
	m_pMeasuringDlg				=NULL;
	m_pNavpointPropertiesDlg	=NULL;

	m_eUnits	=eStatute;

	m_iHighlightedPilot	= -1;
	m_bHighlightLock	=false;

	InitializeIIDs(&IID_DWSView, &IID_DWSViewEvents);

	m_sTimeIndex=0;
	m_cTimeIndex=CTime(0);
	m_cMinTime=CTime( 0 );
	m_cMaxTime=CTime( 0	 );

	m_eActiveClass=eStandard;

	m_sRotatingDefault=0;	
	m_iTaskPtr=0;

	m_bTexturePresent=false;
	m_strSeneryPath="scenery";

	pxyzoom=NULL;

	m_iTurnpointAttribute	=0x01;
	m_iAirportAttribute		=0x02;
	m_iLandableAttribute	=0x08;
	m_iStartAttribute		=0x10;
	m_iFinishAttribute		=0x20;
	m_iHomeAttribute		=0x40;

	m_b2ndStartGate=false;

	m_strFontName="Arial";
	for( int i=0; i<WSNUMFONTS; i++ )	m_pFonts[i]	=NULL;

	CreateFonts();


/*
// Set the defaults to be the same as the CWP_* defines.
#define CWP_TURNPOINT	0x01
#define CWP_AIRPORT		0x02
#define CWP_LANDABLE	0x08
#define CWP_START		0x10
#define CWP_FINISH		0x20
#define CWP_HOME		0x40
*/

}


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::~CWSViewCtrl - Destructor

CWSViewCtrl::~CWSViewCtrl()
{

	if( m_pMeasuringDlg )
		{
		m_pMeasuringDlg->DestroyWindow();
		delete m_pMeasuringDlg;
		m_pMeasuringDlg=NULL;
		}
	if( m_pNavpointPropertiesDlg )
		{
		m_pNavpointPropertiesDlg->DestroyWindow();
		delete m_pNavpointPropertiesDlg;
		m_pNavpointPropertiesDlg=NULL;
		}

	PurgeControlPoints();
	if (m_hHandCursor) DestroyCursor(m_hHandCursor);
	if (m_hRotateCursor) DestroyCursor(m_hRotateCursor);
	if (m_hMeasureCursor) DestroyCursor(m_hMeasureCursor);

//	if( pcControlPointsDisplayPoint!=NULL )
//		{
//		delete[] pcControlPointsDisplayPoint;
//		delete[] ppcControlPointDisplayed;
//		}

	if( m_caAirspaceSections.GetSize()>0 )
		{
		for( int i=0; i<m_caAirspaceSections.GetSize(); i++ )
			delete (CSUASection*)m_caAirspaceSections[i];
		}

	ClearTask();

	for( int i=0; i<WSNUMFONTS; i++ ) 
		{
		m_pFonts[i]->DeleteObject();
		delete m_pFonts[i];
		}
}


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::OnDraw - Drawing function

void CWSViewCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if( m_pMeasuringDlg )
		{
		m_pMeasuringDlg->DestroyWindow();
		delete m_pMeasuringDlg;
		m_pMeasuringDlg=NULL;
		SetFocus( );
		}

	int iWidth, iHeight;
	GetControlSize( &iWidth, &iHeight );

    // Do screen optimization for screen only
    if (GetDeviceCaps(pdc->m_hDC, TECHNOLOGY) == DT_RASDISPLAY && GetObjectType(pdc) != OBJ_ENHMETADC)
        {
		CDC dcMemory;
		CBitmap bitmap;

		bitmap.CreateCompatibleBitmap(pdc, iWidth, iHeight);

		if( !dcMemory.CreateCompatibleDC(pdc) ) 
			{
			DrawData(pdc, rcBounds);
			return;
			};

		CBitmap *pbitmapPrev = dcMemory.SelectObject(&bitmap);

		DrawData(&dcMemory, rcBounds);
		
		pdc->BitBlt(	0,
						0,
						iWidth,
						iHeight,
						&dcMemory,
						0,
						0,
						SRCCOPY);

		dcMemory.SelectObject(pbitmapPrev);

		//copy to clipboard
		if( m_bCopy	)
			{
			OpenClipboard();
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, bitmap);
			CloseClipboard();
			m_bCopy			=false;
			}
		if( m_bWrite )
			{
			CImage image;
			image.Attach(bitmap);
			image.Save(m_strWriteFileName);
			m_bWrite=false;
			}

		// Restore and destroy memory dc
		dcMemory.DeleteDC();
		}
	else
		DrawData(pdc, rcBounds);


//	dcSect.DeleteDC();

}


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::DoPropExchange - Persistence support

void CWSViewCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	PX_Color( pPX, "BackgroundColor",	m_crBackgroundColor,  WSC_BACKGROUND_COLOR_RGB );
	PX_Color( pPX, "StartRadiusColor",	m_crStartRadiusColor, WSC_START_RADIUS_COLOR_RGB );
	PX_Color( pPX, "FinishRadiusColor", m_crFinishRadiusColor,WSC_FINISH_RADIUS_COLOR_RGB );
	PX_Color( pPX, "TaskColor",			m_crTaskColor,		  WSC_TASK_COLOR_RGB );
	PX_Color( pPX, "AirspaceColor",		m_crAirspaceColor,	  WSC_AIRSPACE_COLOR_RGB );
	PX_Color( pPX, "ClimbColor",		m_crClimbColor,		  WSC_CLIMB_COLOR_RGB );
	PX_Color( pPX, "WaypointColor",		m_crWaypointColor,	  WSC_WAYPOINT_COLOR_RGB );

	PX_Long( pPX, "DisplayLength",		m_displayLength,  30 );
	PX_Bool( pPX, "PointDisplay",		m_pointDisplay,  TRUE );

}


/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl::OnResetState - Reset control to default state

void CWSViewCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}

/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl message handlers


void CWSViewCtrl::SetTimeRange(long lStartTime, long lEndTime) 
{
	m_cMinTime=CTime( lStartTime );
	m_cMaxTime=CTime( lEndTime	 );
	InvalidateRect(NULL,FALSE);
}

short CWSViewCtrl::AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass) 
{
	// Assign a default color from the stack
	BYTE bR=aDefaultR[m_sRotatingDefault];
	BYTE bG=aDefaultG[m_sRotatingDefault];
	BYTE bB=aDefaultB[m_sRotatingDefault++];
	long lColor=RGB(bR, bG, bB);
	if( m_sRotatingDefault==NUMDEFAULTCOLORS )
		m_sRotatingDefault=0;

	short sID=m_cTraceGroup.AddTrace(Winscore::CTrace( strContestNo, strLastName, strFirstName, (EClass)sClass, lColor, this ) );

	Winscore::CTrace *pTrace=m_cTraceGroup.GetTrace(sID);
	if( pTrace )
		pTrace->m_bAltitudeMeters=(m_eAltUnits==eMeters)?(true):(false);

	return sID;

}

BOOL CWSViewCtrl::SetCurrentTimeIndex(short sTime) 
{
	m_sTimeIndex=sTime;	
	InvalidateRect(NULL,FALSE);

	return TRUE;
}

void CWSViewCtrl::SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible) 
{
	m_cTraceGroup.SetPilotVisible(strContestNo, (bVisible)?(true):(false));

}

void CWSViewCtrl::SetViewScale(double dScale) 
{
	// TODO: Add your dispatch handler code here

}

void CWSViewCtrl::SetViewCenter(float dLat, float dLong) 
	{
	int cx, cy;
	GetControlSize( &cx, &cy );
	
	float fToler=fToler=2.f/60.f;

	// First look to see if this is a task point:
	for( int i=0; i<m_TaskPointList.GetCount(); i++ ) 
		{
		CControlPoint* pcPt=GetTaskPoint(i);
		if( !pcPt ) continue;
		if( dLat==pcPt->m_fLat &&
			dLong==pcPt->m_fLong )
			{
			fToler=m_fTaskPointRadius[i-1]*1.10f;

			if( fToler<.00001 ) fToler=1.f/60.f;
			break;
			}
		}


	m_c3DTransformation.AutoScale( 	dLong-fToler, 
									dLong+fToler,
									dLat-fToler, 
									dLat+fToler, 
									cx, 
									cy);
	InvalidateRect(NULL,FALSE);

	// Wasn't a task point, do it manually

//	CPoint cPoint;
//	m_c3DTransformation.WorldToLogical(cPoint, dLong, dLat, 0);
//	m_c3DTransformation.SetTranslation( -(cPoint.x-cx/2), -(cPoint.y/2), 0);
//	InvalidateRect(NULL,FALSE);
	}

void CWSViewCtrl::AddControlPoint(short sID, LPCTSTR strName, float fLatitude, float fLongitude, short sAltitude, short sAttribute) 
{
	CControlPoint *pPoint=new CControlPoint(sID, fLatitude, fLongitude, strName, sAttribute, sAltitude);

	pPoint->UpdateAttributeText(m_iTurnpointAttribute,
								m_iAirportAttribute,
								m_iLandableAttribute,
								m_iStartAttribute,
								m_iFinishAttribute,
								m_iHomeAttribute);

	m_ControlPointList.AddTail( pPoint );

	ExpandWindow(fLatitude, fLongitude, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);

	ExpandWindow(fLatitude, fLongitude, m_fCPXMin, m_fCPXMax, m_fCPYMin, m_fCPYMax, 2);
}

void CWSViewCtrl::ClearTask() 
{
	for( POSITION pos = m_TaskPointList.GetHeadPosition(); pos != NULL; )   
		{
		delete (CControlPoint*)m_TaskPointList.GetNext( pos );   
		}
	m_TaskPointList.RemoveAll();

	for( POSITION pos = m_TATPointList.GetHeadPosition(); pos != NULL; )   
		{
		delete (CControlPoint*)m_TATPointList.GetNext( pos );   
		}
	m_TATPointList.RemoveAll();


	for( int i=0; i<MAXTASKPOINTS; i++ ) 
		m_fTaskPointRadius[i]=m_fControlPointOuterRadius;
}

void CWSViewCtrl::AddTaskPoint(short sControlPoint) 
{
	CControlPoint* pcPt=GetControlPoint(sControlPoint);
	if( pcPt )
		m_TaskPointList.AddTail(new CControlPoint(	pcPt->m_iID, 
													pcPt->m_fLat, 
													pcPt->m_fLong,
													pcPt->m_strName, 
													pcPt->m_iAltitude, 
													pcPt->m_iAttributes) );


}


void CWSViewCtrl::SetDisplayOption(long lOption, short sValue) 
{
	switch(	lOption )
		{
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
		case	WSO_DIFFERENT_CLIMB_COLOR:
		case	WSO_FULL_TRACE_DISPLAY:
		case	WSO_TIME_STEP_METHOD:
		case	WSO_SHOW_SCENERY:
		case	WSO_DEMO_MODE:
		default:
			{
			if( sValue==0 )
				{
				if( m_iDisplayOptions & lOption )
					m_iDisplayOptions ^=lOption;
				}
			else
				{
				if( !(m_iDisplayOptions & lOption) )
					m_iDisplayOptions ^=lOption;
				}
			break;
			}
		}
		InvalidateRect(NULL,FALSE);

}

void CWSViewCtrl::AddPosition(short sPilotIndex, long lTimeIndex, float fLatitude, float fLongitude, long lAltitude, short sMotor) 
{
	m_cTraceGroup.AddFix( sPilotIndex, CFix( CTime(lTimeIndex), fLatitude, fLongitude, lAltitude, sMotor ) );

	//  Update the World Min/Max----------------------------

	ExpandWindow(fLatitude, fLongitude, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax);
	ExpandWindow(fLatitude, fLongitude, m_fTraceXMin, m_fTraceXMax, m_fTraceYMin, m_fTraceYMax);

	// Filter out stray points in the stratosphere
	if ( (short)lAltitude>m_sWorldAltMax )
		{
		if((short)lAltitude-m_sWorldAltMax<2000 )
			m_sWorldAltMax= (short)lAltitude;
		else
			m_sWorldAltMax+=200;
		}
	m_sWorldAltMin	=min( m_sWorldAltMin, (short)lAltitude) ;

	// Filter out stray points from the depths of hell
	m_sWorldAltMin	=max( m_sWorldAltMin, 0);
	
}

void CWSViewCtrl::SetWorldLimits(float fLatitudeBottom, float fLatitudeTop, float fLongitudeLeft, float fLongidudeRight, long lMinAltitude, long lMaxAltitude) 
{
	// TODO: Add your dispatch handler code here

}

void CWSViewCtrl::SetColor(short sOption, long lColor) 
{
	switch(sOption)
		{
		case WSC_BACKGROUND_COLOR:
			m_crBackgroundColor= lColor;
			break;
		case WSC_START_RADIUS_COLOR:
			m_crStartRadiusColor= lColor;
			break;
		case WSC_FINISH_RADIUS_COLOR:
			m_crFinishRadiusColor= lColor;
			break;
		case WSC_TASK_COLOR:
			m_crTaskColor= lColor;
			break;
		case WSC_AIRSPACE_COLOR:
			m_crAirspaceColor= lColor;
			break;
		case WSC_CLIMB_COLOR:
			m_crClimbColor= lColor;
			break;
		case WSC_WAYPOINT_COLOR:
			m_crWaypointColor= lColor;
			break;
		}
		InvalidateRect(NULL,FALSE);

}

void CWSViewCtrl::SetPilotTraceColor(LPCTSTR strContestNo, long lColor) 
{
	m_cTraceGroup.SetPilotColor(strContestNo, lColor);
}

void CWSViewCtrl::ResetAll() 
{
	m_sRotatingDefault=0;
	m_cTraceGroup.Purge();
	PurgeControlPoints();

	m_sWorldAltMax	=0;
	m_sWorldAltMin	=32000;

	m_fWorldXMin=1000.;
	m_fWorldXMax= -1000.;
	m_fWorldYMin=1000.;
	m_fWorldYMax= -1000.;

	m_fTraceXMin=1000.;
	m_fTraceXMax= -1000.;
	m_fTraceYMin=1000.;
	m_fTraceYMax= -1000.;

}

void CWSViewCtrl::ResetContestants() 
{
	m_cTraceGroup.Purge();
	m_sRotatingDefault=0;
}


void CWSViewCtrl::OnPaint() 
	{
	CPaintDC dc(this); // device context for painting
	COleControl::OnPaint(&dc);
	}

void CWSViewCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
}
void CWSViewCtrl::AutoscaleTask() 
	{
	int cx, cy;
	GetControlSize( &cx, &cy );

	m_fWorldXMin=1000.;
	m_fWorldXMax= -1000.;
	m_fWorldYMin=1000.;
	m_fWorldYMax= -1000.;

	float fHomeLat=0;
	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_ControlPointList.GetNext( pos );
		if( pcPt && (pcPt->m_iAttributes & m_iHomeAttribute ))	
			{
			fHomeLat=pcPt->m_fLat;
			break;
			}
		}	

	CPoint cPoint;
	for( int i=0; i<m_TaskPointList.GetCount(); i++ ) 
		{
		CControlPoint* pcPt=GetTaskPoint(i);
		if( !pcPt ) continue;		
		
		float fRad=m_fTaskPointRadius[i]*1.3f;

		ExpandWindow(pcPt->m_fLat+fRad, pcPt->m_fLong+fRad, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);
		ExpandWindow(pcPt->m_fLat-fRad, pcPt->m_fLong-fRad, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);
		}

	ExpandWindow(m_fStartGateLatitude-m_fStartGateSize*1.1f, m_fStartGateLongitude-m_fStartGateSize*1.1f, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);
	ExpandWindow(m_fStartGateLatitude+m_fStartGateSize*1.1f, m_fStartGateLongitude+m_fStartGateSize*1.1f, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);

	ExpandWindow(m_fFinishGateLatitude-m_fFinishGateSize*1.1f, m_fFinishGateLongitude-m_fFinishGateSize*1.1f, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);
	ExpandWindow(m_fFinishGateLatitude+m_fFinishGateSize*1.1f, m_fFinishGateLongitude+m_fFinishGateSize*1.1f, m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 2);


	m_c3DTransformation.AutoScale( 	m_fWorldXMin, 
									m_fWorldXMax,
									m_fWorldYMin,
									m_fWorldYMax,
									cx, 
									cy,
									0.0);//fHomeLat);


	InvalidateRect(NULL,FALSE);
	}


void CWSViewCtrl::Autoscale() 
{
	int cx, cy;
	GetControlSize( &cx, &cy );

	float fHomeLat=0;
	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_ControlPointList.GetNext( pos );
		if( pcPt && (pcPt->m_iAttributes & m_iHomeAttribute ))	
			{
			fHomeLat=pcPt->m_fLat;
			break;
			}
		}	


	if( m_fTraceXMin < m_fTraceXMax )
		m_c3DTransformation.AutoScale( 	m_fTraceXMin*.999, 
										m_fTraceXMax*1.001,
										m_fTraceYMin*.999,
										m_fTraceYMax*1.001,
										cx, 
										cy,
										fHomeLat);
	else
		m_c3DTransformation.AutoScale( 	m_fWorldXMin, 
										m_fWorldXMax,
										m_fWorldYMin,
										m_fWorldYMax,
										cx, 
										cy,
										fHomeLat);


	InvalidateRect(NULL,FALSE);

}

void CWSViewCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_pNavpointPropertiesDlg )
		{
		m_pNavpointPropertiesDlg->DestroyWindow();
		delete m_pNavpointPropertiesDlg;
		m_pNavpointPropertiesDlg=NULL;
		}

	if( m_bMeasureMode )
		{
		if( m_pcMeasureStartHitPoint)
			{
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			m_bMeasureMode=false;
			return;
			}

		if( m_pcHit )
			m_pcMeasureStartHitPoint=m_pcHit;
		else
			m_pcMeasureStartHitPoint=m_cHitPointList.Get(point,8,CHitPoint::eNavPoint);

		if( !m_pcMeasureStartHitPoint )
			{
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			m_bMeasureMode=false;
			return;
			}

		if( m_pMeasuringDlg )
			m_pMeasuringDlg->SetInitialAltitude(m_pcMeasureStartHitPoint->GetAltitude());

		CDC *pDC=GetDC();
		CPen cPen;
		cPen.CreatePen(PS_SOLID | PS_COSMETIC, 1,RGB( 0xFF,0xFF,0xFF ));
		CPen* pOldPen = pDC->SelectObject( &cPen );
		int iOld=pDC->SetROP2( R2_XORPEN );
		pDC->MoveTo(m_pcMeasureStartHitPoint->GetPoint());
		pDC->LineTo(point);
		m_cMeasureEndPoint=point;
		pDC->SetROP2( iOld );
		pDC->SelectObject( pOldPen );
		ReleaseDC(pDC);

		return;
		}

	if( nFlags & MK_CONTROL     )
		{
		m_c3DTransformation.PointZoom( point,  2 );
		InvalidateRect(NULL,FALSE);
		COleControl::OnLButtonDown(nFlags, point);
		return;
		}

    if (pxyzoom != 0)
        pxyzoom->SetBeginning(point);
	else
		{
		int iSelected=m_cTraceGroup.GetHighlightedContestant();

		if( iSelected>=0 && !m_bHighlightLock)
			{
			Winscore::CTrace* pcTrace=m_cTraceGroup.GetTrace(iSelected);
			if( pcTrace ) FirePilotSelection(pcTrace->m_strContestNo);
			m_bHighlightLock=true;
			m_iHighlightedPilot=iSelected;
			}
		else
			{
			m_bHighlightLock=false;
			m_iHighlightedPilot= -1;
			::SetCursor(m_hHandCursor);
			m_cMousePanPoint=point;
			}
		}

	COleControl::OnLButtonDown(nFlags, point);
}

void CWSViewCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{


	if( m_bMeasureMode )
		{
		if( m_pcMeasureStartHitPoint &&
			m_pcMeasureDoneHitPoint      )
			{
			m_pMeasuringDlg->UpdateRunningTotals(m_pcMeasureStartHitPoint, m_pcMeasureDoneHitPoint, m_eUnits);
			m_bEnableControlPointHighlight=false;	
			}
		else
			InvalidateRect(NULL,FALSE);

		m_pcMeasureStartHitPoint=NULL;
		m_pcMeasureDoneHitPoint=NULL;
//		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
//		m_bMeasureMode=false;


		COleControl::OnLButtonUp(nFlags, point);

		return;
		}

	if( m_pcHit ) return;

    if (pxyzoom != 0)
        {
        pxyzoom->SetEnd(point);
        if (pxyzoom->IsValid())
            {
			int cx, cy;
			GetControlSize( &cx, &cy );
			CPoint cCenter(cx/2, cy/2);

            CRect geomrectZoom = pxyzoom->GetZoomRect();
            
			geomrectZoom.NormalizeRect( );

			if( geomrectZoom.BottomRight().x < cx &&
				geomrectZoom.BottomRight().y < cy    )
				{

				CPoint cZoomCenter=geomrectZoom.CenterPoint();
				m_c3DTransformation.SetTranslation( cCenter.x-cZoomCenter.x, cCenter.y-cZoomCenter.y, 0 );

				double dScale=cx/geomrectZoom.Width();
			
				m_c3DTransformation.PointZoom( cCenter, dScale );
				InvalidateRect(NULL,FALSE);
				}

			}
        delete pxyzoom;
        pxyzoom = NULL;
        }
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	InvalidateRect(NULL,FALSE);

	COleControl::OnLButtonUp(nFlags, point);
}

void CWSViewCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
	{

	}

void CWSViewCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bMeasureMode )
		{
		::SetCursor(m_hMeasureCursor);
		if( m_pcMeasureStartHitPoint ) 
			{

			CPoint cEndPoint=point;
			m_pcMeasureDoneHitPoint = m_cHitPointList.Get(point, 15, CHitPoint::eEither );
			if( m_pcMeasureDoneHitPoint )
				cEndPoint=m_pcMeasureDoneHitPoint->GetPoint();

			CDC *pDC=GetDC();
			CPen cPen;
			cPen.CreatePen(PS_SOLID | PS_COSMETIC, 1,RGB( 0xFF,0xFF,0xFF ));
			CPen* pOldPen = pDC->SelectObject( &cPen );
			int iOld=pDC->SetROP2( R2_XORPEN );
			pDC->MoveTo(m_pcMeasureStartHitPoint->GetPoint());
			pDC->LineTo(m_cMeasureEndPoint);
			pDC->MoveTo(m_pcMeasureStartHitPoint->GetPoint());
			pDC->LineTo(cEndPoint);
			m_cMeasureEndPoint=cEndPoint;
			pDC->SetROP2( iOld );
			pDC->SelectObject( pOldPen );
			ReleaseDC(pDC);

			if( m_pMeasuringDlg )
				m_pMeasuringDlg->UpdateTotals(	m_pcMeasureStartHitPoint, 
												m_cMeasureEndPoint, 
												m_c3DTransformation, 
												m_pcMeasureDoneHitPoint, 
												m_eUnits, 
												(m_eAltUnits==eMeters),
												GetUnitsText());
			}
		else
			{
			if( m_pMeasuringDlg )
				m_pMeasuringDlg->UpdateTurnpointText(m_pcHit);
			}
		}

    if (pxyzoom != 0)
		{
        pxyzoom->OnMove(point);

		if( m_pcHit )
			{
			CDC* pDC = GetDC();
			m_pcHit->Highlight(pDC);
			m_pcHit=NULL;
			ReleaseDC( pDC );
			}
		}
	else
		{
		if( !m_bMeasureMode && (nFlags & MK_LBUTTON)  )
			{
			// Left mouse with shift key equals rotate
			if (nFlags & MK_SHIFT)
				{
				if( GetDisplayOption(WSO_DEMO_MODE) ) 
					{
					return;
					}

				::SetCursor(m_hRotateCursor);
				int cx, cy;
				GetControlSize( &cx, &cy );
				CPoint cCenter(cx/2, cy/2);
				CPoint cTwoThirds((int)(cx*.66), (int)(cy*.66) );

				double dXAngle=0, dZAngle=0, dYAngle=0;

				if( m_cMousePanPoint.x<cTwoThirds.x )
					dXAngle=(m_cMousePanPoint.y-point.y)/3;
				else
					dZAngle= -(m_cMousePanPoint.y-point.y)/3;
				dYAngle= -(m_cMousePanPoint.x-point.x)/3;


				if( m_c3DTransformation.CheckRotations( dXAngle, dYAngle, dZAngle) )
					{
					m_c3DTransformation.SetTranslation( -cCenter.x, -cCenter.y, 0);
					m_c3DTransformation.SetRotation( dXAngle, dYAngle, dZAngle);
					m_c3DTransformation.SetTranslation( cCenter.x, cCenter.y, 0);
					}
				// try again with a smaller rotation
				else if( m_c3DTransformation.CheckRotations( dXAngle/2, dYAngle/2, dZAngle/2) )
					{
					m_c3DTransformation.SetTranslation( -cCenter.x, -cCenter.y, 0);
					m_c3DTransformation.SetRotation( dXAngle/2, dYAngle/2, dZAngle/2);
					m_c3DTransformation.SetTranslation( cCenter.x, cCenter.y, 0);
					}

				InvalidateRect(NULL,FALSE);
				m_cMousePanPoint=point;
				return;
				}
			else if( !m_bMeasureMode )
				{ // PAN
				::SetCursor(m_hHandCursor);
				if(m_cMousePanPoint!=CPoint(-1,-1) )
					m_c3DTransformation.SetTranslation(	point.x-m_cMousePanPoint.x,
														point.y-m_cMousePanPoint.y,
														0);
				m_cMousePanPoint=point;
				InvalidateRect(NULL,FALSE);
				}
			}
		else
			{
			if( !m_bMeasureMode )
				{
			    if( m_bEnableControlPointHighlight )
					{

					if( !m_bHighlightLock )
						{
						CPoint cHitPoint=point;
						m_iHighlightedPilot=m_cTraceGroup.GetClosestPilot(point, 10 );
						if( m_iHighlightedPilot<0 )
							{
							if( m_cTraceGroup.AnyoneHighlighted(true) )
								InvalidateRect(NULL,FALSE);
							}
						else
								InvalidateRect(NULL,FALSE);

						}

					// Now highlight the NavPoints
					CPoint cTemp=point;
					CControlPoint *pOld=m_pHighlightedControlPoint;
					m_pHighlightedControlPoint=GetClosestControlPoint(cTemp, 8);
					if( pOld!=m_pHighlightedControlPoint  )
						{
						InvalidateRect(NULL,FALSE);
						}
					}
				}

			// Now highlight the trace points
			CDC* pDC = GetDC();
			CHitPoint *pcHit = m_cHitPointList.Get(point, m_bMeasureMode?(15):(25), m_bMeasureMode?(CHitPoint::eEither):(CHitPoint::eLogPoint) );
			if( !pcHit )
				{
				// Nothing selected, erase any previous one.
				if( m_pcHit && m_pcHit!=m_pcMeasureStartHitPoint)
					m_pcHit->Highlight(pDC);
				}
			else if( pcHit!=m_pcHit )
				{
				// Point is selected and it is different from old one
				// Erase the old, and highlight the new.
				if( m_pcHit && m_pcHit!=m_pcMeasureStartHitPoint ) 
					m_pcHit->Highlight(pDC);

				pcHit->Highlight(pDC);

				if( !m_bMeasureMode )
					{
					CFix* pFix=(CFix*)pcHit->m_ptr;
					if( pFix  )
						FirePointSelected( 0, (long)pFix->m_cTime.GetTime() );
					}
				}
			m_pcHit=pcHit;

			ReleaseDC( pDC );
			}
		}

	COleControl::OnMouseMove(nFlags, point);
}

void CWSViewCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	COleControl::OnKillFocus(pNewWnd);
	
}

void CWSViewCtrl::LoadControlPoints(CDC *pDC, C3DTransformation &c3DTrans )
{
	CPen cGreenPen, cRedPen;
	cGreenPen.CreatePen( PS_SOLID, 1, RGB(0,255,0) );
	cRedPen.CreatePen( PS_SOLID, 1, RGB(255,0,0) );
	CPen *pcOldPen=pDC->SelectObject( &cGreenPen );
	CControlPoint *pHome=NULL;
	int cx, cy;
	GetControlSize( &cx, &cy );

	CFont* pOldFont=SelectFontByScale(pDC);


// Draw start cylinder------------------------------------------------
	if( m_eStartGateType==eCylinder )
		{
		LoadDisk(pDC, c3DTrans,m_fStartGateLatitude, m_fStartGateLongitude, m_sStartGateBase, m_fStartGateSize, m_bTexturePresent?(SILHOUETTE):(FILL), m_crStartRadiusColor);
		}

	if( m_b2ndStartGate )
		{
		LoadDisk(pDC, c3DTrans,m_f2ndStartGateLatitude, m_f2ndStartGateLongitude, m_s2ndStartGateBase, m_f2ndStartGateSize, m_bTexturePresent?(SILHOUETTE):(FILL), m_crStartRadiusColor);
		}

// Draw finish cylinder-------------------------------------------------
	if( m_eFinishGateType==eCylinder )
		LoadDisk(pDC, c3DTrans, m_fFinishGateLatitude, m_fFinishGateLongitude, m_sFinishGateBase,  m_fFinishGateSize, m_bTexturePresent?(SILHOUETTE):(FILL), m_crFinishRadiusColor);


// Draw control point disks first, so they will be on top top of start/ finish cylinders
// Do them all now so the green borders will have display priority.
	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_ControlPointList.GetNext( pos );  
		
		CPoint cPoint=m_c3DTransformation.WorldToLogical(	pcPt->m_fLong, 
															pcPt->m_fLat, 
															pcPt->m_iAltitude );
		if( IsPointClipped( cPoint ) ) continue;

		LoadDisk(pDC, c3DTrans,pcPt->m_fLat, pcPt->m_fLong, pcPt->m_iAltitude, m_fControlPointRadius, FILL, m_crWaypointColor );
		}


//  Draw Control Points ------------------------------------------------------------------	
	CPoint cTemp1, cTemp2;
	bool bFirst=true;
	POSITION pos=NULL;

	for( pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_ControlPointList.GetNext( pos );
		if( !pcPt ) continue;

		CPoint cPoint=m_c3DTransformation.WorldToLogical(	pcPt->m_fLong, 
															pcPt->m_fLat, 
															pcPt->m_iAltitude );
		if( IsPointClipped( cPoint ) ) continue;

		if(!pHome && (pcPt->m_iAttributes & m_iHomeAttribute ))	pHome=pcPt;

		// Draw the green border around the disk
		LoadDisk(pDC, c3DTrans, pcPt->m_fLat, pcPt->m_fLong, pcPt->m_iAltitude, m_fControlPointRadius, SILHOUETTE, RGB(0,255,0) );

		// If the center of the waypoint isn't visible, stop now.
		if( !IsPointVisible(cx, cy, cPoint ) ) continue;

		// Draw the black center point
		LoadVertex(pDC, c3DTrans, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude, 2, RGB(0,0,0) );

		// Add the hitpoint
		m_cHitPointList.AddHitPoint(new CHitPoint(cPoint, pcPt, CHitPoint::eNavPoint));

		// Add the text, if text is large enough
		CString strText;
		strText.Format( "%d - %s",pcPt->m_iID, pcPt->m_strName );

		if( pcPt==m_pHighlightedControlPoint || pcPt==pHome )
			{
			int iOldStyle=pDC->SetTextAlign( TA_LEFT | TA_BOTTOM );
			COLORREF oldColor=pDC->SetTextColor( 0 );
			CFont *pFont=pDC->SelectObject( m_pFonts[WSLARGEFONT] );
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );
			pDC->SelectObject( pFont );
			pDC->SetTextColor( oldColor );
			pDC->SetTextAlign( iOldStyle );
			}
		else if( pOldFont )
			{
			int iOldStyle=pDC->SetTextAlign( TA_LEFT | TA_BOTTOM );
			COLORREF oldColor=pDC->SetTextColor( 0 );
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );	
			pDC->SetTextColor( oldColor );
			pDC->SetTextAlign( iOldStyle );
			}

		}

	// Draw Verticies at the center of the start/finish cyl.
	LoadVertex(pDC, c3DTrans,  m_fStartGateLongitude, m_fStartGateLatitude, m_sStartGateBase, 2, RGB(255,255,255));
	LoadVertex(pDC, c3DTrans,  m_fFinishGateLongitude, m_fFinishGateLatitude, m_sFinishGateBase, 2, RGB(255,255,255));

	if( m_b2ndStartGate )
		LoadVertex(pDC, c3DTrans,  m_f2ndStartGateLongitude, m_f2ndStartGateLatitude, m_s2ndStartGateBase, 2, RGB(255,255,255));

//  ----  Draw the borders to the start and finish circles ----------------------------------

	if( m_eStartGateType==eCylinder )
		{
		LoadDisk(pDC, c3DTrans,m_fStartGateLatitude, m_fStartGateLongitude, m_sStartGateHeight, m_fStartGateSize, SILHOUETTE, RGB(155,0,0));
		LoadDisk(pDC, c3DTrans, m_fStartGateLatitude, m_fStartGateLongitude, m_sStartGateBase, m_fStartGateSize, SILHOUETTE, m_bTexturePresent?(RGB(255,0,0)):(RGB(0,0,0)) );
		}

	if( m_eFinishGateType==eCylinder )
		{
		LoadDisk(pDC, c3DTrans,m_fFinishGateLatitude, m_fFinishGateLongitude, m_sFinishGateHeight, m_fFinishGateSize, SILHOUETTE, RGB(0,255,0));
		LoadDisk(pDC, c3DTrans,m_fFinishGateLatitude, m_fFinishGateLongitude, m_sFinishGateBase, m_fFinishGateSize, SILHOUETTE, RGB(0,255,0));
		}
	
	if( m_b2ndStartGate )
		{
		LoadDisk(pDC, c3DTrans, m_f2ndStartGateLatitude, m_f2ndStartGateLongitude, m_s2ndStartGateHeight, m_f2ndStartGateSize, SILHOUETTE, RGB(155,0,0));
		LoadDisk(pDC, c3DTrans, m_f2ndStartGateLatitude, m_f2ndStartGateLongitude, m_s2ndStartGateBase, m_f2ndStartGateSize, SILHOUETTE, m_bTexturePresent?(RGB(255,0,0)):(RGB(0,0,0)) );
		}


//---- Draw the start line  ----------------------------------------------------------

	if( m_eStartGateType==eLine )
		{

		// Draw the start line
		pDC->SelectObject( &cRedPen );
		pDC->SetTextColor( RGB(255,0,0) );

		CPoint cPoint;
		m_c3DTransformation.WorldToLogical( cPoint, m_fStartGateLongitude, m_fStartGateLatitude, m_sStartGateHeight);
		pDC->MoveTo(SafePoint(cPoint));
		m_c3DTransformation.WorldToLogical( cPoint, m_fStartGateLongitude2, m_fStartGateLatitude2, m_sStartGateHeight );
		pDC->LineTo(SafePoint(cPoint));

		//  Draw the text
		CString strText;
		strText.Format( "S" );
		m_c3DTransformation.WorldToLogical( cPoint, m_fStartGateLongitude, m_fStartGateLatitude, m_sStartGateHeight);
		if( !IsPointClipped(cPoint) ) 
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );

		m_c3DTransformation.WorldToLogical( cPoint, m_fStartGateLongitude2, m_fStartGateLatitude2, m_sStartGateHeight);
		if( !IsPointClipped(cPoint) ) 
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );
		}


//---- Draw the finish line  ----------------------------------------------------------

	if( m_eFinishGateType==eLine )
		{	
		pDC->SelectObject( &cGreenPen );
		pDC->SetTextColor( RGB(0,255,0) );

		CPoint cPoint;
		m_c3DTransformation.WorldToLogical( cPoint, m_fFinishGateLongitude, m_fFinishGateLatitude, m_sFinishGateBase);
		pDC->MoveTo(SafePoint(cPoint));
		m_c3DTransformation.WorldToLogical( cPoint, m_fFinishGateLongitude2, m_fFinishGateLatitude2, m_sFinishGateBase );
		pDC->LineTo(SafePoint(cPoint));

		CString strText;
		strText.Format( "F" );
		m_c3DTransformation.WorldToLogical( cPoint, m_fFinishGateLongitude, m_fFinishGateLatitude, m_sFinishGateBase);
		if( !IsPointClipped(cPoint) ) 
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );
		m_c3DTransformation.WorldToLogical( cPoint, m_fFinishGateLongitude2, m_fFinishGateLatitude2, m_sFinishGateBase );
		if( !IsPointClipped(cPoint) ) 
			pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );
		}

	// cleanup
	pDC->SelectObject( pcOldPen );
	cRedPen.DeleteObject();
	cGreenPen.DeleteObject();
	if( pOldFont )
		pDC->SelectObject( pOldFont );

}


void CWSViewCtrl::LoadDisk(CDC *pDC, C3DTransformation &c3DTrans,  float dLat, float dLong, short sElevation, float dRadius, int style, COLORREF crColor )
{

	if( dRadius<.0000001 ) return;


	int cx, cy;
	GetControlSize( &cx, &cy );


	double dAng=(dLat/180)*PI;
	dAng=min(dAng, 80);
	double dCorr=1/cos(dAng);

	CPoint cPoint;
	c3DTrans.WorldToLogical( cPoint,  dLong, dLat, sElevation );
	if( IsPointClipped( cPoint ) ) 
		return;

	// Use a circle for the overhead view.
	CPoint topLeft, bottomRight;
	c3DTrans.WorldToLogical( topLeft,		dLong-(dRadius*dCorr), dLat+(dRadius), sElevation );
	c3DTrans.WorldToLogical( bottomRight,	dLong+(dRadius*dCorr), dLat-(dRadius), sElevation );

	if( topLeft.x>cx ) 
		return;
	if( topLeft.y>cy ) 
		return;

	if( bottomRight.y<0 ) 
		return;
	if( bottomRight.x<0 ) 
		return;


	CBrush cBrush;
	cBrush.CreateSolidBrush( crColor );
	CBrush *pcOldBrush=pDC->SelectObject( &cBrush );

	CPen cPen;
	cPen.CreatePen( PS_SOLID, m_bTexturePresent?(3):(1), crColor );
	CPen *pcOldPen=pDC->SelectObject( &cPen );


	if( !c3DTrans.IsViewOrthogonal() )
		{
		// Make a Bezier ellipse to handle arbitrary rotations.

	//	const double EToBConst =	0.2761423749154; 
		const double EToBConst =	0.554; 


		CPoint cCtlPt[13];

		c3DTrans.WorldToLogical( cCtlPt[0],  dLong-(dRadius*dCorr), dLat, sElevation );
		c3DTrans.WorldToLogical( cCtlPt[12], dLong-(dRadius*dCorr), dLat, sElevation );
		
		c3DTrans.WorldToLogical( cCtlPt[2], dLong-(dRadius*dCorr)*(EToBConst), dLat+(dRadius), sElevation );
		c3DTrans.WorldToLogical( cCtlPt[3], dLong,						 dLat+(dRadius), sElevation );
		c3DTrans.WorldToLogical( cCtlPt[4], dLong+(dRadius*dCorr)*(EToBConst), dLat+(dRadius), sElevation );

		c3DTrans.WorldToLogical( cCtlPt[5], dLong+(dRadius*dCorr),			dLat+(dRadius)*(EToBConst), sElevation );
		c3DTrans.WorldToLogical( cCtlPt[6], dLong+(dRadius*dCorr),			dLat,			sElevation );
		c3DTrans.WorldToLogical( cCtlPt[7], dLong+(dRadius*dCorr),			dLat-(dRadius)*(EToBConst), sElevation );

		c3DTrans.WorldToLogical( cCtlPt[ 8], dLong+(dRadius*dCorr)*(EToBConst), dLat-(dRadius),					sElevation );
		c3DTrans.WorldToLogical( cCtlPt[ 9], dLong,		  dLat-(dRadius),				    sElevation  );
		c3DTrans.WorldToLogical( cCtlPt[10], dLong-(dRadius*dCorr)*(EToBConst), dLat-(dRadius),					sElevation );

		c3DTrans.WorldToLogical(  cCtlPt[1], dLong-(dRadius*dCorr),			  dLat+(dRadius)*(EToBConst),	sElevation );
		c3DTrans.WorldToLogical( cCtlPt[11], dLong-(dRadius*dCorr),			  dLat-(dRadius)*(EToBConst),	sElevation );

	/*

												   //------------------------/
												  //                        /
		cCtlPt[11].x =                            //        2___3___4       /
		cCtlPt[12].x = r.left;                    //     1             5    /
		cCtlPt[5].x  =                            //     |             |    /
		cCtlPt[6].x  =                            //     |             |    /
		cCtlPt[7].x  = r.right;                   //     0,12          6    /
		cCtlPt[2].x  =                            //     |             |    /
		cCtlPt[10].x = centre.x - offset.cx;      //     |             |    /
		cCtlPt[4].x  =                            //    11             7    /
		cCtlPt[8].x  = centre.x + offset.cx;      //       10___9___8       /
		cCtlPt[3].x  =                            //                        /
		cCtlPt[9].x  = centre.x;                  //------------------------*

	*/

		for( int i=0; i<13; i++ )
			cCtlPt[i]=SafePoint(cCtlPt[i]);

		if( style==SILHOUETTE )
			pDC->PolyBezier( cCtlPt, 13 );
		else
			{
			pDC->BeginPath();
			pDC->PolyBezier( cCtlPt, 13 );
			pDC->EndPath();
			pDC->StrokeAndFillPath();
			}
		}
	else
		{

		if( !IsPointClipped(topLeft) && !IsPointClipped(bottomRight) ) 
			{

			if( style==SILHOUETTE )
				{
				pDC->Arc( CRect(topLeft, bottomRight), topLeft, topLeft );
				int iSize=abs(topLeft.y-bottomRight.y);
				iSize/=10;
				if( iSize>5 )
					{
					CFont* pOldFont;
					if( iSize<15 )
						pOldFont=pDC->SelectObject( m_pFonts[WSSMALLFONT] );
					else 
						pOldFont=pDC->SelectObject( m_pFonts[WSMEDFONT] );


					int iOldStyle=pDC->SetTextAlign( TA_BOTTOM |TA_CENTER  );
					COLORREF oldColor=pDC->SetTextColor( 0 );

					double dDist= ConvertDistance(dRadius*60, eNautical, m_eUnits );
					CString strText;
					strText.Format("%5.2lf %s", dDist, GetUnitsText());

					pDC->TextOut(cPoint.x, topLeft.y, strText, strText.GetLength() );
					
					pDC->SetTextColor( oldColor );
					pDC->SetTextAlign( iOldStyle );
					pDC->SelectObject( pOldFont );
					}
				}
			else
				pDC->Ellipse( CRect(topLeft, bottomRight) );
			}
		}

	pDC->SelectObject( pcOldBrush );
	pDC->SelectObject( pcOldPen );

	cBrush.DeleteObject();
	cPen.DeleteObject();

}




void CWSViewCtrl::LoadTask(CDC *pDC, C3DTransformation &c3DTrans )
{
	int cx, cy;
	GetControlSize( &cx, &cy );

	CPoint cPoint, cCPoint;

	CPen cPen;
	cPen.CreatePen( PS_SOLID, 1, m_crTaskColor );
	CPen *pcOldPen=pDC->SelectObject( &cPen );
	pDC->SetTextColor( m_crTaskColor );

//	CFont* pOldFont=pDC->SelectObject( m_pFonts[WSMEDFONT] );
	CFont* pOldFont=SelectFontByScale(pDC);

	bool bTurnAreaTaskDisplayed = m_TATPointList.GetCount()>0;

	CPoint cPrevious;
	for( int i=0; i<m_TaskPointList.GetCount(); i++ ) 
		{
		CControlPoint* pcPt=GetTaskPoint(i);
		if( !pcPt ) continue;
		
		m_c3DTransformation.WorldToLogical( cPoint, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude);
		if( i==0 )
			cPrevious=cPoint;
		else
			DrawClippedLine(pDC, cx, cy, cPrevious, cPoint);

		m_cHitPointList.AddHitPoint(new CHitPoint(cPoint, pcPt, CHitPoint::eNavPoint));

		cPrevious=cPoint;
		}


	if( m_b2ndStartGate )
		{
		m_c3DTransformation.WorldToLogical( cPrevious, m_f2ndStartGateLongitude, m_f2ndStartGateLatitude, m_s2ndStartGateBase);

		CControlPoint* pcPt=GetTaskPoint(1);
		if( pcPt )
			{
			m_c3DTransformation.WorldToLogical( cPoint, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude);
			DrawClippedLine(pDC, cx, cy, cPrevious, cPoint);
			}
		}


	pDC->SelectObject( pcOldPen );
	cPen.DeleteObject();

	cPen.CreatePen( PS_DASHDOT, 1, m_crTaskColor );
	pcOldPen=pDC->SelectObject( &cPen );
	pDC->SetTextColor( m_crTaskColor );

	for( int i=0; i<m_TATPointList.GetCount(); i++ ) 
		{
		CControlPoint* pcPt=GetTATPoint(i);
		if( !pcPt ) continue;
		
		m_c3DTransformation.WorldToLogical( cPoint, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude);
		if( i>0 )
			DrawClippedLine(pDC, cx, cy, cPrevious, cPoint);

		LoadVertex(pDC, c3DTrans, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude, 4, RGB(0,0,0) );

		m_cHitPointList.AddHitPoint(new CHitPoint(cPoint, pcPt, CHitPoint::eNavPoint));
		cPrevious=cPoint;
		}

	pDC->SelectObject( pcOldPen );
	cPen.DeleteObject();

	CPen cPen2;
	cPen2.CreatePen( PS_SOLID, 1, RGB(0,0,0) );
	pcOldPen=pDC->SelectObject( &cPen2 );

	for( int i=1; i<m_TaskPointList.GetCount()-1; i++ ) 
		{
		CControlPoint* pcPt=GetTaskPoint(i);
		if( !pcPt ) continue;
		
		float fRadius=m_fTaskPointRadius[i];

		// Draw the shadow of the disk
		LoadDisk(pDC, c3DTrans, pcPt->m_fLat, pcPt->m_fLong, 0, fRadius, SILHOUETTE, RGB(155,155,155));

		// Draw the outer turnpoint radius
		LoadDisk(pDC, c3DTrans, pcPt->m_fLat, pcPt->m_fLong, pcPt->m_iAltitude, fRadius, SILHOUETTE, RGB(0,0,0));


		if( !bTurnAreaTaskDisplayed )
			{

			// Load the FAI sector indicators

			double dAng=(pcPt->m_fLat/180)*PI;
			dAng=min(dAng, 80);
			double dCorr=1/cos(dAng);

			CControlPoint* pcPtPrev=GetTaskPoint(i-1);
			CLocation cPrevious(pcPtPrev->m_fLat, pcPtPrev->m_fLong);
			CLocation cCenter(pcPt->m_fLat, pcPt->m_fLong);
			CControlPoint* pcPtNext=GetTaskPoint(i+1);
			if( !pcPtNext ) continue;
			CLocation cNext(pcPtNext->m_fLat, pcPtNext->m_fLong);


			double dBearingPrev=cCenter.CourseTo(cPrevious);
			double dBearingNext=cCenter.CourseTo(cNext);

			double dAverage=(dBearingPrev+dBearingNext)/2.;
			double dDifference=dBearingPrev-dBearingNext;
			if( dDifference<180 && dDifference> -180 )
				dAverage+=180;
			if( dAverage>360 ) dAverage-=360;
			CLocation cEnd1(&cCenter), cEnd2(&cCenter);
			cEnd1.Move(fRadius*60, TORAD*(dAverage+45), dCorr);
			cEnd2.Move(fRadius*60, TORAD*(dAverage-45), dCorr);

			m_c3DTransformation.WorldToLogical( cCPoint, cCenter.GetLong(), cCenter.GetLat(),  0);
			pDC->MoveTo( SafePoint(cCPoint) );
			m_c3DTransformation.WorldToLogical( cPoint, cEnd1.GetLong(), cEnd1.GetLat(),  0);
			DrawClippedLine(pDC, cx, cy, cPoint, cCPoint);
			m_c3DTransformation.WorldToLogical( cPoint, cEnd2.GetLong(), cEnd2.GetLat(),  0);
			DrawClippedLine(pDC, cx, cy, cPoint, cCPoint);
			}
		}

	pDC->SelectObject( pcOldPen );
	cPen2.DeleteObject();

	if( pOldFont )
		{
		for( int i=0; i<m_TaskPointList.GetCount(); i++ ) 
			{
			CControlPoint* pcPt=GetTaskPoint(i);
			if( !pcPt ) continue;
			
			// now draw the characters in a string 
			CString strText=pcPt->m_strName;
			m_c3DTransformation.WorldToLogical( cPoint, pcPt->m_fLong, pcPt->m_fLat, pcPt->m_iAltitude);
			if( !IsPointClipped(cPoint) ) 
				pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength() );
			}

		pDC->SelectObject( pOldFont );
		}

}


CControlPoint* CWSViewCtrl::GetControlPoint(int iID)
{
	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_ControlPointList.GetNext( pos );   

		if( pcPt && iID==pcPt->m_iID ) return pcPt;

		}
	return NULL;
}


/*
BOOL CWSViewCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CS_OWNDC; 

	BOOL B=COleControl::PreCreateWindow(cs);
	return B;
}
*/

void CWSViewCtrl::InitSite(float fLatitude, float fLongitude, short iAltitude) 
{
}

void CWSViewCtrl::SetStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase) 
{
	m_fStartGateLatitude	=fGateLatitude;
	m_fStartGateLongitude	=fGateLongitude;
	m_eStartGateType		=eCylinder;
	m_fStartGateSize		=(float)(fGateSize/60.);
	m_sStartGateHeight		=sGateHeight;
	m_sStartGateBase		=sGateBase;

}

void CWSViewCtrl::SetFinishGateCylinder(float fGateLatitude, float fGateLongitude,  float fGateSize, short sGateHeight, short sGateBase) 
{
	m_fFinishGateLatitude	=fGateLatitude;
	m_fFinishGateLongitude	=fGateLongitude;
	m_eFinishGateType		=eCylinder;
	m_fFinishGateSize		=(float)(fGateSize/60.);
	m_sFinishGateHeight		=sGateHeight;
	m_sFinishGateBase		=sGateBase;
}

void CWSViewCtrl::SetStartGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iStartCeiling) 
{
	m_fStartGateLatitude	=fLat1;
	m_fStartGateLongitude	=flong1;
	m_fStartGateLatitude2	=fLat2;
	m_fStartGateLongitude2	=fLong2;
	m_eStartGateType		=eLine,
	m_sStartGateHeight		=(short)iStartCeiling;

}

void CWSViewCtrl::SetFinishGateLine(float fLat1, float fLong1, float fLat2, float fLong2, long iFinishGateBase) 
{
	m_fFinishGateLatitude	=fLat1;
	m_fFinishGateLongitude	=fLong1;
	m_fFinishGateLatitude2	=fLat2;
	m_fFinishGateLongitude2	=fLong2;
	m_eFinishGateType		=eLine;
	m_sFinishGateBase		=(short)iFinishGateBase;

}

void CWSViewCtrl::SetPilotTimes(short sPilotID, long lStartTime, long lFinishTime) 
{
	m_cTraceGroup.AddTimes(sPilotID, CTime(lStartTime), CTime(lFinishTime) );
}



void CWSViewCtrl::AddPositionByName(LPCTSTR strContestNo, long lTime, float fLat, float fLong, short sAltitude) 
{
	short sPilotIndex=m_cTraceGroup.GetPilotIndex(strContestNo);
	if( sPilotIndex>=0 )  
		AddPosition( sPilotIndex, lTime, fLat, fLong, sAltitude, 0);


}

void CWSViewCtrl::SetCurrentTime(long lTime) 
{
	m_cTimeIndex=lTime;
	InvalidateRect(NULL,FALSE);

}

void CWSViewCtrl::AddFixes(const VARIANT FAR& vlTime, 
						   const VARIANT FAR& vstrContestNo, 
						   const VARIANT FAR& vfLatitude, 
						   const VARIANT FAR& vfLongidude, 
						   const VARIANT FAR& vsAltitude) 
{
    COleSafeArray olesafearrayTime(vlTime);
    COleSafeArray olesafearrayLat(vfLatitude);
    COleSafeArray olesafearrayLon(vfLongidude);
    COleSafeArray olesafearrayAlt(vsAltitude);
    COleSafeArray olesafearrayCID(vstrContestNo);

    float *paLat;
	float *paLong;
	long  *paTime;
	short *paAltitude;
	char  *paCID;

	olesafearrayTime.AccessData((void **)&paTime);
    olesafearrayCID.AccessData((void **)&paCID);
    olesafearrayLat.AccessData((void **)&paLat);
    olesafearrayLon.AccessData((void **)&paLong);
    olesafearrayAlt.AccessData((void **)&paAltitude);

    int nFixes = olesafearrayTime.GetOneDimSize();

	for( int i=0, ic=0; i<nFixes; i++, ic+=3 )
		{
		char cCID[4];
		strncpy_s( cCID, 4, "   ", 3);
		cCID[3]='\0';
		strncpy_s( cCID, 4, &(const char)paCID[ic], 3);
		CString strCID=cCID;
		strCID.TrimRight();
		AddPositionByName( strCID, paTime[i], paLat[i], paLong[i], paAltitude[i]);
		}

	olesafearrayTime.UnaccessData();
    olesafearrayCID.UnaccessData();
    olesafearrayLat.UnaccessData();
    olesafearrayLon.UnaccessData();
    olesafearrayAlt.UnaccessData();

    return;
}

void CWSViewCtrl::SetMinMaxAltitudes(short sMinAltitude, short sMaxAltitude) 
{
	m_sWorldAltMax	=sMaxAltitude;
	m_sWorldAltMin	=sMinAltitude;
}

BOOL CWSViewCtrl::IsPilotVisible(LPCTSTR strContestNo) 
{
	if( !m_cTraceGroup.IsPilotVisible(strContestNo ) ) return false;

	return m_cTraceGroup.GetNumFixes(strContestNo)>0;

}

void CWSViewCtrl::SetTraceDisplayLength(long lDisplayLength) 
{
	m_cTraceGroup.SetDisplayLength(lDisplayLength );

}

void CWSViewCtrl::PurgeControlPoints()
	{
	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		delete (CControlPoint*)m_ControlPointList.GetNext( pos );   
		}
	m_ControlPointList.RemoveAll();
	}

void CWSViewCtrl::SetActiveClass(short sClass) 
{
	m_eActiveClass=(EClass)sClass;
}


long CWSViewCtrl::GetColor(short sOption) 
{
	switch(sOption)
		{
		case WSC_BACKGROUND_COLOR:
			return m_crBackgroundColor;
			break;
		case WSC_START_RADIUS_COLOR:
			return m_crStartRadiusColor;
			break;
		case WSC_FINISH_RADIUS_COLOR:
			return m_crFinishRadiusColor;
			break;
		case WSC_TASK_COLOR:
			return m_crTaskColor;
			break;
		case WSC_AIRSPACE_COLOR:
			return m_crAirspaceColor;
			break;
		case WSC_CLIMB_COLOR:
			return m_crClimbColor;
			break;
		case WSC_WAYPOINT_COLOR:
			return m_crWaypointColor;
			break;
		}
	return 0;
}


void CWSViewCtrl::OnDisplayLengthChanged() 
{
	m_cTraceGroup.SetDisplayLength(m_displayLength);
	SetModifiedFlag();
}

void CWSViewCtrl::OnEditProperties() 
{
		if( m_pHighlightedControlPoint )
			{
			// Show properties of highlighted navpoint
			if( !m_pNavpointPropertiesDlg )
				{
				m_pNavpointPropertiesDlg=new CMeasuringDlg;
				m_pNavpointPropertiesDlg->Create(IDD_MEASURING, this);
				}
			m_pNavpointPropertiesDlg->ResetText();
			m_pNavpointPropertiesDlg->DisplayControlPointInfo(m_pHighlightedControlPoint, m_eAltUnits);
			m_pNavpointPropertiesDlg->ShowWindow(SW_SHOW);
			}
		else
			{
			// Show Display Options
			CRect rect;
			GetRectInContainer(&rect);
			OnProperties((LPMSG)GetCurrentMessage(), NULL, rect);
			FireUpdateProperties();
			}
}

void CWSViewCtrl::SetPilotText(LPCTSTR strContestNo, LPCTSTR strTitle) 
{
	Winscore::CTrace* pcTrace=m_cTraceGroup.GetTrace(strContestNo);
	if( pcTrace )
		{
		pcTrace->m_strAdditionalText=strTitle;
		}

}

void CWSViewCtrl::OnPointDisplayChanged() 
{
	m_cTraceGroup.m_bPointDisplay=m_pointDisplay?(true):(false);

	SetModifiedFlag();
}

void CWSViewCtrl::LoadGrid(CDC *pDC, C3DTransformation c3DTransformation )
{
	CPoint cPoint;
	if( m_fCPYMin>m_fCPYMax  ) return;
	int iLatStart	=(int)m_fCPYMin;
	int iLatEnd		=(int)m_fCPYMax;
	if( iLatStart<0 ) 
		iLatStart--;
	else
		iLatEnd++;

	int iLonStart	=(int)m_fCPXMin;
	int iLonEnd		=(int)m_fCPXMax;
	if( iLonStart<0 ) 
		iLonStart--;
	else
		iLonEnd++;


	int ilong=0, ilat=0;
	int cx, cy;
	GetControlSize( &cx, &cy );

	m_bTexturePresent=false;

	BOOL bDemoMode=GetDisplayOption( WSO_DEMO_MODE );

	if( GetDisplayOption( WSO_SHOW_SCENERY ) )
		{

	//	CWaitCursor cCursor;
		for ( ilong= iLonStart; ilong<iLonEnd+1; ilong++ )
			{
			for ( ilat= iLatStart; ilat<iLatEnd+1; ilat++ )
				{

				if( !IsGridVisible( c3DTransformation, ilat, ilong ) ) continue;

				CString strFileName;
				strFileName.Format("%s%02d%s%03d.bmp",(ilat<0)?("S"):("N"),abs(ilat),(ilong<0)?("W"):("E"),abs(ilong));

				// Only permit mifflin in demo mode.
				if( bDemoMode )
					{
					if ( strFileName!="N40W078.bmp" ) continue;
					}

				CDC dcSect;
				CPalette cPalette;
				CBitmap	cSecbitmap;

				dcSect.CreateCompatibleDC(pDC);
				
				bool bLoaded=false;
				bLoaded=LoadBMPImage( m_strSeneryPath+"\\"+strFileName, cSecbitmap, &cPalette )?true:false;
				if( bLoaded )
					{
					CPoint cPointUL,cPointLR;
					m_c3DTransformation.WorldToLogical( cPointLR,  ilong+1,		ilat,	0);
					m_c3DTransformation.WorldToLogical( cPointUL,  ilong,		ilat+1,	0);
					CRect cRec(cPointUL, cPointLR);

					CBitmap *pbitmapPrev = dcSect.SelectObject(&cSecbitmap);
					BOOL bsec=pDC->StretchBlt(		cPointUL.x, cPointUL.y,
													cRec.Width(), cRec.Height(),
													&dcSect, 
													0, 0, 1024, 1024, 
													SRCCOPY );
					m_bTexturePresent=true;
					}
				dcSect.DeleteDC();
				}
			}
		}

	// Get ballpark zoom scale to decide whether to show minor tick marks
	CPoint cPoint1, cPoint2;
	c3DTransformation.WorldToLogical(cPoint1, 0, 0, 0);
	c3DTransformation.WorldToLogical(cPoint2, 1, 0, 0);
	int iZoom=abs(cPoint1.x-cPoint2.x);
	bool bShowMinor= iZoom>400;



	CPen cPen;
	cPen.CreatePen( PS_SOLID, m_bTexturePresent?(3):(1), RGB(155,155,155) );
	CPen *pcOldPen=pDC->SelectObject( &cPen );

	for ( ilong= iLonStart; ilong<iLonEnd+1; ilong++ )
		{
		CPoint cPoint1;
		m_c3DTransformation.WorldToLogical( cPoint1,  ilong, iLatStart, 0);
		m_c3DTransformation.WorldToLogical( cPoint, ilong, iLatEnd, 0 );

		DrawClippedLine(pDC, cx, cy, cPoint1, cPoint);


		// Add Longitude ticks
		double dPos=iLatStart;
		for( int iTick=0; iTick<(iLatEnd-iLatStart)*60; iTick++)
			{
			if( fmod(double(iTick), 10.)==0 )
				{
				m_c3DTransformation.WorldToLogical( cPoint, ilong-.01, dPos, 0 );
				if( !IsPointClipped(cPoint) ) 
					{
					pDC->MoveTo( cPoint );
					m_c3DTransformation.WorldToLogical( cPoint, ilong+.01, dPos, 0 );
					pDC->LineTo( cPoint );
					}
				}
			else
				{
				if( bShowMinor ) 
					{
					m_c3DTransformation.WorldToLogical( cPoint, ilong-.005, dPos, 0 );
					if( !IsPointClipped(cPoint) ) 
						{
						pDC->MoveTo( cPoint );
						m_c3DTransformation.WorldToLogical( cPoint, ilong, dPos, 0);
						pDC->LineTo( cPoint );
						}
					}

				}
			dPos+=(1./60.);
			}
		}

	CFont* pOldFont=pDC->SelectObject( m_pFonts[WSLARGEFONT] );

	for ( ilat= iLatStart; ilat<iLatEnd+1; ilat++ )
		{

		CPoint cPoint1;
		m_c3DTransformation.WorldToLogical( cPoint1, iLonStart, ilat, 0  );
		m_c3DTransformation.WorldToLogical( cPoint, iLonEnd, ilat, 0 );
		DrawClippedLine(pDC, cx, cy, cPoint1, cPoint);


		// Add Longitude ticks

		double dPos=iLonStart;
		for( int iTick=0; iTick<(iLonEnd-iLonStart)*60; iTick++)
			{
			if( fmod(double(iTick), 10.)==0 )
				{
				m_c3DTransformation.WorldToLogical( cPoint, dPos, ilat-.01, 0 );
				if( !IsPointClipped(cPoint) ) 
					{
					pDC->MoveTo( cPoint );
					m_c3DTransformation.WorldToLogical( cPoint, dPos, ilat+.01, 0);
					pDC->LineTo( cPoint );
					}
				}
			else
				{
				if( bShowMinor ) 
					{
					m_c3DTransformation.WorldToLogical( cPoint, dPos, ilat+.005, 0);
					if( !IsPointClipped(cPoint) ) 
						{
						pDC->MoveTo( cPoint );
						m_c3DTransformation.WorldToLogical( cPoint, dPos, ilat,      0 );
						pDC->LineTo( cPoint );
						}
					}
				}
			dPos+=(1./60.);
			}
		}

	pDC->SelectObject( pcOldPen );
	cPen.DeleteObject();


		{
		CString strText;
		for ( ilat= iLatStart+1; ilat<iLatEnd; ilat++ )
			{
			CString cLat=(ilat>0)?("N"):("S");
			strText.Format( "%d° %s", abs(ilat), cLat );
			CPoint cPoint;

			m_c3DTransformation.WorldToLogical( cPoint, (float)iLonStart-.1f, (float)ilat+.005f, 0 );
			if( !IsPointClipped(cPoint) ) 
				pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength());

			m_c3DTransformation.WorldToLogical( cPoint, (float)iLonEnd+.005f, (float)ilat+.005f, 0 );
			if( !IsPointClipped(cPoint) ) 
				pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength());
			}
			
		for ( ilong= iLonStart; ilong<iLonEnd+1; ilong++ )
			{
			CString cLon=(ilong>0)?("E"):("W");
			strText.Format( "%d° %s", abs(ilong), cLon );
			m_c3DTransformation.WorldToLogical( cPoint,  (float)ilong+.005f, (float)iLatStart+.005f,0 );
			if( !IsPointClipped(cPoint) ) 
				pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength());

			m_c3DTransformation.WorldToLogical( cPoint, (float)ilong+.005f, (float)iLatEnd+.005f, 0 );
			if( !IsPointClipped(cPoint) ) 
				pDC->TextOut( cPoint.x, cPoint.y, strText, strText.GetLength());
			}
		}
	pDC->SelectObject( pOldFont );


	
}

void CWSViewCtrl::SetRotations(float fRX, float fRY, short fRZ) 
	{

	double sx, sy, sz;
	double tx, ty, tz;
	double rx, ry, rz;
	int cx, cy;
	GetControlSize( &cx, &cy );
	CPoint cCenter(cx/2, cy/2);

	m_c3DTransformation.GetScale(sx, sy, sz);
	m_c3DTransformation.GetTranslations(tx, ty, tz);
	m_c3DTransformation.GetRotations(rx, ry, rz);

	m_c3DTransformation.Reset();

	m_c3DTransformation.SetScale(sx, sy, sz);
	m_c3DTransformation.SetTranslation(tx, ty, tz);

	m_c3DTransformation.SetTranslation( -cCenter.x, -cCenter.y, 0);
	m_c3DTransformation.SetRotation( fRX, fRY, fRZ);
	m_c3DTransformation.SetTranslation( cCenter.x, cCenter.y, 0);

	InvalidateRect(NULL,FALSE);

	}

void CWSViewCtrl::SetZScale(float fZScale) 
{
	// Not the best solution to force a fixed view for the Z scaling, but it 
	// is the best I can do for now.

	double sx, sy, sz;

	int cx, cy;
	GetControlSize( &cx, &cy );

	m_c3DTransformation.GetScale(sx, sy, sz);

	m_c3DTransformation.AutoScale( 	m_fWorldXMin, 
									m_fWorldXMax,
									m_fWorldYMin,
									m_fWorldYMax,
									cx, 
									cy, max(sz*fZScale,.0005));

	m_c3DTransformation.SetTranslation( -cx, -cy, 0);
	m_c3DTransformation.SetRotation(60, 20, -20);
	m_c3DTransformation.SetTranslation( cx, cy, 0);

//	m_c3DTransformation.SetScale(1, 1, sz*fZScale);

	/*
	double tx, ty, tz;
	double rx, ry, rz;
	double dx, dy, dz;
	int cx, cy;
	GetControlSize( &cx, &cy );
	CPoint cCenter(cx/2, cy/2), cNewCenter;

	m_c3DTransformation.LogicalToWorld(cCenter, dx, dy, dz);

	m_c3DTransformation.GetScale(sx, sy, sz);
	m_c3DTransformation.GetTranslations(tx, ty, tz);
	m_c3DTransformation.GetRotations(rx, ry, rz);

	m_c3DTransformation.Reset();

	m_c3DTransformation.SetScale(sx, sy, sz*fZScale);
	m_c3DTransformation.SetTranslation(tx, ty, tz);

	m_c3DTransformation.SetTranslation( -cCenter.x, -cCenter.y, 0);
//	m_c3DTransformation.SetRotation(rx, ry, rz);
	m_c3DTransformation.SetRotation(60, 20, -20);
	m_c3DTransformation.SetTranslation( cCenter.x, cCenter.y, 0);


	m_c3DTransformation.WorldToLogical(cNewCenter, dx, dy, 0);

	m_c3DTransformation.SetTranslation( cNewCenter.x-cCenter.x, cNewCenter.y-cCenter.y, 0);

*/
	InvalidateRect(NULL,FALSE);
}

void CWSViewCtrl::NudgeView(short sViewType) 
	{
	int cx, cy;
	GetControlSize( &cx, &cy );
	CPoint cCenter(cx/2, cy/2);

	m_c3DTransformation.SetTranslation( -cCenter.x, -cCenter.y, 0);

	if( (ENudgeView)sViewType==eNegXRotation ) 
		{
		if( m_c3DTransformation.CheckRotations( -10, 0, 0) )
			m_c3DTransformation.SetRotation(-10, 0, 0);
		}
	else if( (ENudgeView)sViewType==ePosXRotation ) 
		{
		if( m_c3DTransformation.CheckRotations( 10, 0, 0) )
			m_c3DTransformation.SetRotation(10, 0, 0);
		}
	else if( (ENudgeView)sViewType==eNegYRotation ) 
		{
		if( m_c3DTransformation.CheckRotations( 0, -10, 0) )
			m_c3DTransformation.SetRotation(0, -10, 0);
		}
	else if( (ENudgeView)sViewType==ePosYRotation ) 
		{
		if( m_c3DTransformation.CheckRotations( 0, 10, 0) )
			m_c3DTransformation.SetRotation( 0, 10, 0);
		}
	m_c3DTransformation.SetTranslation( cCenter.x, cCenter.y, 0);

	InvalidateRect(NULL,FALSE);

	}

float CWSViewCtrl::GetZScale() 
{
//	return	m_zScaling/DEFAULTZSCALE;
	return 1.0;
}

long CWSViewCtrl::GetPilotColor(short sPilotIndex) 
{
	Winscore::CTrace* pTrace=m_cTraceGroup.GetTrace(sPilotIndex);
	if( pTrace ) 
		return RGB(	pTrace->m_R, 
					pTrace->m_G, 
					pTrace->m_B );
	else 
		return 0;
}

void CWSViewCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
        {
		  if (pxyzoom != 0)
            {
            pxyzoom->Cancel();
            delete pxyzoom;
            pxyzoom = 0;
            }
		ReleaseCapture();
        }

	if( nChar==78 )
		ZoomOnTaskPoint();

	FireOnKeyDown(nChar);
	
	COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CWSViewCtrl::ScaleView(float fScale) 
{
	int cx, cy;
	GetControlSize( &cx, &cy );
	CPoint cCenter(cx/2, cy/2);
	m_c3DTransformation.PointZoom( cCenter,  fScale );

	InvalidateRect(NULL,FALSE);
}

short CWSViewCtrl::GetDisplayOption(long lOption) 
{
	switch(	lOption )
		{
		case	WSO_DIFFERENT_CLIMB_COLOR:
		case	WSO_FULL_TRACE_DISPLAY:
		case	WSO_TIME_STEP_METHOD:
		case	WSO_SHOW_SCENERY:
		case	WSO_DEMO_MODE:
		default:
			{
			return (m_iDisplayOptions & lOption)?(TRUE):(FALSE);
			}
		}
	return 0;
}



void CWSViewCtrl::SetSceneryPath(LPCTSTR strPath) 
	{
	m_strSeneryPath=strPath;
	}

void CWSViewCtrl::InvalidateGrid() 
	{
	}

bool CWSViewCtrl::IsGridVisible(C3DTransformation &c3DTrans, short sLat, short sLong)
	{
	CPoint cPoint1, cPoint2, cPoint3, cPoint4;

	int iWidth, iHeight;
	GetControlSize( &iWidth, &iHeight );

	CRect cViewRect( CPoint(0,0), CPoint(iWidth, iHeight) );
	cViewRect.NormalizeRect( );


	c3DTrans.WorldToLogical( cPoint1, sLong, sLat, 0 );
	if( cViewRect.PtInRect(cPoint1) ) return true;

	c3DTrans.WorldToLogical( cPoint2, sLong+1, sLat, 0 );
	if( cViewRect.PtInRect(cPoint2) ) return true;

	c3DTrans.WorldToLogical( cPoint3, sLong+1, sLat+1, 0 );
	if( cViewRect.PtInRect(cPoint3) ) return true;

	c3DTrans.WorldToLogical( cPoint4, sLong, sLat+1, 0 );
	if( cViewRect.PtInRect(cPoint4) ) return true;


	CRect cGridRect( cPoint1, cPoint3);	
	cGridRect.NormalizeRect();

	BOOL bIntersected=cViewRect.IntersectRect( &cGridRect, &cViewRect );

	return bIntersected?(true):(false);
	}

void CWSViewCtrl::LoadVertex(CDC *pDC, C3DTransformation &c3DTransform,  double dLong, double dLat,double dAltitude, int iSize, COLORREF crColor)
{

	CPoint centerPoint, upperLeft, lowerRight;
	c3DTransform.WorldToLogical( centerPoint, dLong, dLat, dAltitude );
	if( IsPointClipped( centerPoint ) ) return;


	CPen cPen;
	cPen.CreatePen( PS_SOLID, 1, crColor );
	CPen *pcOldPen=pDC->SelectObject( &cPen );

	CBrush cBrush;
	cBrush.CreateSolidBrush( crColor );
	CBrush *pcOldBrush=pDC->SelectObject( &cBrush );

	upperLeft=centerPoint;
	upperLeft.Offset(-iSize, iSize);
	lowerRight=centerPoint;
	lowerRight.Offset(iSize, -iSize);

	upperLeft=SafePoint(upperLeft);
	lowerRight=SafePoint(lowerRight);
	CRect cRect(upperLeft, lowerRight);

	pDC->Ellipse( cRect );
	pDC->SelectObject( pcOldPen );

	cPen.DeleteObject();
	cBrush.DeleteObject();

}

void CWSViewCtrl::DrawData(CDC *pdc, const CRect &rcBounds)
	{
	if(!pdc) return;

	pdc->SetBkMode( TRANSPARENT );

	CBrush cBackgroundBrush;
	cBackgroundBrush.CreateSolidBrush( m_crBackgroundColor );
	pdc->FillRect(rcBounds, &cBackgroundBrush);
	cBackgroundBrush.DeleteObject();

	int cx, cy;
	GetControlSize( &cx, &cy );

	m_cHitPointList.Initialize(cx,cy);
	m_pcHit=NULL;
	m_pcMeasureStartHitPoint=NULL;
	m_pcMeasureDoneHitPoint=NULL;
	m_bMeasureMode=false;
	m_bEnableControlPointHighlight=true;

//	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	LoadGrid(pdc, m_c3DTransformation );

	LoadControlPoints(pdc, m_c3DTransformation);

	LoadAirspace(pdc, m_c3DTransformation );

	LoadTask(pdc, m_c3DTransformation);

	if( GetDisplayOption(WSO_TIME_STEP_METHOD) ) 
		m_cTraceGroup.Draw( pdc, m_c3DTransformation, m_cMinTime, m_cTimeIndex, m_eActiveClass, m_sWorldAltMax, m_sWorldAltMin, m_cHitPointList  );
	else
		m_cTraceGroup.Draw( pdc, m_c3DTransformation, 0, m_sTimeIndex, m_sWorldAltMax, m_sWorldAltMin, m_cHitPointList);

	if( m_iHighlightedPilot>=0 )
		m_cTraceGroup.Highlight(m_iHighlightedPilot, pdc);


	// If we are locked on a contestant, then follow him around
	if (m_iHighlightedPilot>=0 && m_bHighlightLock )
		{
		Winscore::CTrace* pTrace=m_cTraceGroup.GetTrace(m_iHighlightedPilot);
		if( pTrace )
			{
			int cx, cy;
			GetControlSize( &cx, &cy );
			int iFrame=100;

			if( pTrace->m_dLabelPositionX-iFrame<0 )
				m_c3DTransformation.SetTranslation( -(pTrace->m_dLabelPositionX-iFrame), 0, 0);
			
			if( pTrace->m_dLabelPositionY-iFrame<0 )
				m_c3DTransformation.SetTranslation( 0, -(pTrace->m_dLabelPositionY-iFrame), 0);

			if( pTrace->m_dLabelPositionY+iFrame>cy )
				m_c3DTransformation.SetTranslation( 0, (cy-pTrace->m_dLabelPositionY-iFrame), 0);

			if( pTrace->m_dLabelPositionX+iFrame>cx )
				m_c3DTransformation.SetTranslation( (cx-iFrame-pTrace->m_dLabelPositionX), 0, 0);
			}
		}

	DrawScale(pdc);

    if (pxyzoom != 0)
		{
        pxyzoom->Show(*pdc);
		}
	}



// LoadBMPImage	- Loads a BMP file and creates a bitmap GDI object
//		  also creates logical palette for it.
// Returns	- TRUE for success
// sBMPFile	- Full path of the BMP file
// bitmap	- The bitmap object to initialize
// pPal		- Will hold the logical palette. Can be NULL
static BOOL LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal )
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = DWORD(file.GetLength() - sizeof(BITMAPFILEHEADER));
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
//	if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
//	{
//		::GlobalFree(hDIB);
//		return FALSE;
//	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,		// handle to device context 
				&bmiHeader,	// pointer to bitmap size and format data 
				CBM_INIT,	// initialization flag 
				lpDIBBits,	// pointer to initialization data 
				&bmInfo,	// pointer to bitmap color-format data 
				DIB_RGB_COLORS);		// color-data usage 
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;

}
void CWSViewCtrl::OnPopupZoomwindow() 
{
	m_bMeasureMode=false;
    if (pxyzoom == 0)
		{
		int iWidth, iHeight;
		GetControlSize( &iWidth, &iHeight );
        pxyzoom = new XYZoom(*this, CRect( CPoint(0,0), CPoint(iWidth,iHeight)) );
		}
	
}
void CWSViewCtrl::OnSaveAS() 
	{
	static TCHAR BASED_CODE szFilter[] = _T("JPEG format|*.jpg|Bitmap format|*.bmp|GIF format|*.gif|PNG format|*.png||");
	CString strFileName("image.jpg");
	CFileDialog  cFileDlg(FALSE, _T("jpg"), strFileName, OFN_OVERWRITEPROMPT,szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		WriteBMPFile(cFileDlg.GetPathName());
	
		}
	}

void CWSViewCtrl::OnPopupCopy() 
	{
	m_bMeasureMode=false;
	m_bCopy=true;
	InvalidateRect(NULL,FALSE);	
	}

void CWSViewCtrl::OnPopupProperties() 
	{
	m_bMeasureMode=false;
	ReleaseCapture();
	OnEditProperties();	
	}

void CWSViewCtrl::OnPopupAutoscale() 
	{
	m_bMeasureMode=false;
	Autoscale();
	FireOnKeyDown(35); // Fire off an "End" character to display the entire trace.
    ReleaseCapture();
	}

void CWSViewCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{

		if( nFlags & MK_CONTROL     )
			{
			int cx, cy;
			GetControlSize( &cx, &cy );
			CPoint cCenter(cx/2, cy/2);
			m_c3DTransformation.PointZoom( cCenter,  .5 );
			InvalidateRect(NULL,FALSE);
			return;
			}

        CMenu menuResource, menuPopup;
#ifdef _DEBUG
			menuResource.LoadMenu(IDR_POPUP_MENU);
#else
		if( GetDisplayOption(WSO_DEMO_MODE) )
			menuResource.LoadMenu(IDR_POPUP_MENU_DEMO);
		else
			menuResource.LoadMenu(IDR_POPUP_MENU);
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


void CWSViewCtrl::OnZoomout() 
{
	m_bMeasureMode=false;
	int cx, cy;
	GetControlSize( &cx, &cy );
	CPoint cCenter(cx/2, cy/2);

	m_c3DTransformation.PointZoom( cCenter,  .33 );

	InvalidateRect(NULL,FALSE);
	ReleaseCapture();


}
CControlPoint* CWSViewCtrl::GetClosestControlPoint(CPoint &point, int iToler)
{
	CHitPoint* pcHit=m_cHitPointList.Get(point, iToler, CHitPoint::eNavPoint );
	if( !pcHit ) return NULL;

	return (CControlPoint*)(pcHit->m_ptr);
/*


	for( int i=0; i<m_nControlPointsDisplayed; i++ )
		{
		double d1 =(pcControlPointsDisplayPoint[i].y-point.y);
			   d1*=(pcControlPointsDisplayPoint[i].y-point.y);
		double d2 =(pcControlPointsDisplayPoint[i].x-point.x);
			   d2*=(pcControlPointsDisplayPoint[i].x-point.x);
		double dDist=sqrt( d1+d2 );  

		if( dDist<iToler ) 
			{
			point=pcControlPointsDisplayPoint[i];
			return ppcControlPointDisplayed[i];
			}
		}		*/

	return NULL;
}

void CWSViewCtrl::ZoomWindow() 
	{
	OnPopupZoomwindow();
	}

void CWSViewCtrl::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	COleControl::OnDropFiles(hDropInfo);
}

void CWSViewCtrl::OnAboutBox() 
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CModuleVersion ver;
    ver.GetFileVersionInfo("wsview.ocx");

	CString cVers;
	cVers="WSView ActiveX Control Version: "+ ver.GetValue(_T("FileVersion"));
	CWSViewAboutDlg dlg(cVers);

	ReleaseCapture();

	dlg.DoModal();
}

void CWSViewCtrl::ExpandWindow(float fLatitude, float fLongitude, float &fXMin, float &fXMax, float &fYMin, float &fYMax, int iToler )
{
	//  If the point to expand is more than a tolerance away from the current min/max, it is probably a
	//  bad point.  Don't use it in min/max calculations

	if(		fXMin<fXMax	&&
			fYMin<fYMax   )
		{
		if( fLongitude>(fXMin-iToler) &&
			fLongitude<(fXMax+iToler)		)
			{
			fXMin=min(fXMin, fLongitude);
			fXMax=max(fXMax, fLongitude);
			}
		if( fLatitude>(fYMin-iToler) &&
			fLatitude<(fYMax+iToler)		)
			{
			fYMin=min(fYMin, fLatitude);
			fYMax=max(fYMax, fLatitude);	
			}
		}
	else
		{
		fXMin=min(fXMin, fLongitude);
		fXMax=max(fXMax, fLongitude);
		fYMin=min(fYMin, fLatitude );
		fYMax=max(fYMax, fLatitude );
		}

}

void CWSViewCtrl::AddTaskLocation(short sPointID, float fLat, float fLong, LPCTSTR strComment) 
	{
	m_TaskPointList.AddTail(new CControlPoint(sPointID, fLat, fLong, strComment, 0, 0) );
	}

CControlPoint* CWSViewCtrl::GetTaskPoint(int iPoint)
	{
	int iCnt=0;
	for( POSITION pos = m_TaskPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_TaskPointList.GetNext( pos );   
		if( pcPt ) 
			{
			if(iCnt==iPoint ) return pcPt;
			iCnt++;
			}

		}
	return NULL;
	}

CControlPoint* CWSViewCtrl::GetTATPoint(int iPoint)
	{
	int iCnt=0;
	for( POSITION pos = m_TATPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pcPt=(CControlPoint*)m_TATPointList.GetNext( pos );   
		if( pcPt ) 
			{
			if(iCnt==iPoint ) return pcPt;
			iCnt++;
			}
		}
	return NULL;
	}

void CWSViewCtrl::SetDisplayOptionD(long lOption, double dValue) 
{
	switch(	lOption )
		{
		case WSO_OUTER_RADIUS:
			{
			m_fControlPointOuterRadius=(float)(dValue/60.);
			for( int i=0; i<MAXTASKPOINTS; i++ ) 
				m_fTaskPointRadius[i]=m_fControlPointOuterRadius;
			break;
			}
		case WSO_INNER_RADIUS:
			{
			m_fControlPointRadius=(float)(dValue/60.);
			break;
			}
		default:
			{
			break;
			}
		}
}

double CWSViewCtrl::GetDisplayOptionD(long lOption) 
{
	switch(	lOption )
		{
		case WSO_OUTER_RADIUS:
			{
			return m_fControlPointOuterRadius*60;
			}
		case WSO_INNER_RADIUS:
			{
			return m_fControlPointRadius*60;
			}
		default:
			{
			break;
			}
		}
	return 0.0;
}

void CWSViewCtrl::AddSUAirspace(LPCTSTR strAirspace) 
	{
	CSUASection* pSection=new CSUASection(strAirspace);
	m_caAirspaceSections.Add( pSection );

	for ( int iSub=0; iSub<pSection->m_aSubSections.GetSize(); iSub++ )
			{		
			CSUASubSection *pSub=(CSUASubSection*)pSection->m_aSubSections[iSub];

			if( pSub->m_eType==CSUASubSection::eCIRCLE )
				{
				ExpandWindow((float)pSub->m_cCenter.GetLat()+(float)pSub->m_dRadius/(float)60., (float)pSub->m_cCenter.GetLong()+(float)pSub->m_dRadius/(float)60., m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 3);
				ExpandWindow((float)pSub->m_cCenter.GetLat()-(float)pSub->m_dRadius/(float)60., (float)pSub->m_cCenter.GetLong()-(float)pSub->m_dRadius/(float)60., m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 3);
				}
			else if( pSub->m_eType==CSUASubSection::ePOINT )
				{
				ExpandWindow((float)pSub->m_cLocation.GetLat(), (float)pSub->m_cLocation.GetLong(), m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 3);
				}
			else if( pSub->m_eType==CSUASubSection::eCLOCKWISE )
				{
				ExpandWindow((float)pSub->m_cLocation.GetLat(), (float)pSub->m_cLocation.GetLong(), m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 3);
				ExpandWindow((float)pSub->m_cCenter.GetLat(), (float)pSub->m_cCenter.GetLong(), m_fWorldXMin, m_fWorldXMax, m_fWorldYMin, m_fWorldYMax, 3);
				}
			}
	}

void CWSViewCtrl::LoadAirspace(CDC *pDC, C3DTransformation &c3DTrans )
	{
	if( m_caAirspaceSections.GetSize()==0 ) return;

//	CFont* pOldFont=pDC->SelectObject( m_pFonts[WSMEDFONT] );
	CFont* pOldFont=SelectFontByScale(pDC);

	CPen cPen;
	cPen.CreatePen( PS_SOLID, 1, m_crAirspaceColor );
	CPen* pOldPen = pDC->SelectObject( &cPen );

	CPoint cPrevPoint1, cPrevPoint2;
	CPoint cTitlePoint;

	for ( int iSection=0; iSection<m_caAirspaceSections.GetSize(); iSection++ )
		{
		CSUASection *pSection=(CSUASection*)m_caAirspaceSections[iSection];
		bool bFirstPoint=true;
		int		nAverages=0;
		CLocation cPrevLocation, cLoc;
		CPoint cPoint1, cPoint2;

		for ( int iSub=0; iSub<pSection->m_aSubSections.GetSize(); iSub++ )
			{		
			CSUASubSection *pSub=(CSUASubSection*)pSection->m_aSubSections[iSub];

			if( pSub->m_eType==CSUASubSection::eCLOCKWISE		||
				pSub->m_eType==CSUASubSection::eANTICLOCKWISE	   )
				{
				pDC->MoveTo(cPrevPoint1);
				pDC->MoveTo(cPrevPoint2);
				double dOriginalBearing=pSub->m_cCenter.CourseTo(cPrevLocation);
				double dFinalBearing=pSub->m_cCenter.CourseTo(pSub->m_cLocation);

				if ( pSub->m_eType==CSUASubSection::eCLOCKWISE )
					{
					if( dFinalBearing<dOriginalBearing ) dFinalBearing+=360;
					}
				else
					{
					if( dFinalBearing>dOriginalBearing ) dOriginalBearing+=360;
					}

				double dBearing=dOriginalBearing;

				while( (pSub->m_eType==CSUASubSection::eCLOCKWISE)?(dBearing<dFinalBearing):(dBearing>dFinalBearing) )
					{
					cLoc=pSub->m_cCenter;

					double dAng=(cLoc.GetLat()/180)*PI;
					dAng=min(dAng, 80);
					double dCorr=1/cos(dAng);

					cLoc.Move( pSub->m_dRadius, dBearing*TORAD, dCorr);
					c3DTrans.WorldToLogical( cPoint1, (float)cLoc.GetLong(),  (float)cLoc.GetLat(),  (float)pSub->m_iTop );
					c3DTrans.WorldToLogical( cPoint2, (float)cLoc.GetLong(),  (float)cLoc.GetLat(),  (float)pSub->m_iBase );
					pDC->MoveTo(SafePoint(cPrevPoint1));
					pDC->LineTo(SafePoint(cPoint1));
					pDC->MoveTo(SafePoint(cPrevPoint2));
					pDC->LineTo(SafePoint(cPoint2));
					cPrevPoint1=cPoint1;
					cPrevPoint2=cPoint2;
					(pSub->m_eType==CSUASubSection::eCLOCKWISE)?(dBearing++):(dBearing--);
					}
				c3DTrans.WorldToLogical( cPoint1, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iTop );
				c3DTrans.WorldToLogical( cPoint2, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iBase );
				pDC->MoveTo(SafePoint(cPrevPoint1));
				pDC->LineTo(SafePoint(cPoint1));
				pDC->MoveTo(SafePoint(cPrevPoint2));
				pDC->LineTo(SafePoint(cPoint2));
				pDC->MoveTo(SafePoint(cPoint1));
				pDC->LineTo(SafePoint(cPoint2));
				cPrevPoint1=cPoint1;
				cPrevPoint2=cPoint2;
				cPrevLocation=pSub->m_cLocation;

				}
/*			else if(pSub->m_eType==CSUASubSection::eANTICLOCKWISE)
				{
				pSub->m_dRadius=pSub->m_cCenter.DistanceTo(&(pSub->m_cLocation), SYSTEMUNITS);

				double dOriginalBearing=pSub->m_cCenter.CourseTo(cPrevLocation);
				double dFinalBearing=pSub->m_cCenter.CourseTo(pSub->m_cLocation);
				if( dFinalBearing>dOriginalBearing ) dOriginalBearing+=360;

				double  dBearing=(int)dOriginalBearing;
				while( dBearing > dFinalBearing )
					{
					cLoc=pSub->m_cCenter;

					double dAng=(cLoc.GetLat()/180)*PI;
					dAng=min(dAng, 80);
					double dCorr=1/cos(dAng);

					cLoc.Move( pSub->m_dRadius, dBearing*TORAD, dCorr);
					c3DTrans.WorldToLogical( cPoint1, (float)cLoc.GetLong(),  (float)cLoc.GetLat(),  (float)pSub->m_iTop );
					c3DTrans.WorldToLogical( cPoint2, (float)cLoc.GetLong(),  (float)cLoc.GetLat(),  (float)pSub->m_iBase );
					pDC->MoveTo(cPrevPoint1);
					pDC->LineTo(cPoint1);
					pDC->MoveTo(cPrevPoint2);
					pDC->LineTo(cPoint2);
					cPrevPoint1=cPoint1;
					cPrevPoint2=cPoint2;
					dBearing--;
					}
				c3DTrans.WorldToLogical( cPoint1, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iTop );
				c3DTrans.WorldToLogical( cPoint2, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iBase );
				pDC->MoveTo(cPrevPoint1);
				pDC->LineTo(cPoint1);
				pDC->MoveTo(cPrevPoint2);
				pDC->LineTo(cPoint2);
				cPrevPoint1=cPoint1;
				cPrevPoint2=cPoint2;
				cPrevLocation=pSub->m_cLocation;

				}*/
			else if( pSub->m_eType==CSUASubSection::eCIRCLE )
				{
				LoadDisk(pDC, c3DTrans, (float)pSub->m_cCenter.GetLat(), (float)pSub->m_cCenter.GetLong(), pSub->m_iBase, (float)(pSub->m_dRadius/60.), SILHOUETTE, m_crAirspaceColor);
				LoadDisk(pDC, c3DTrans, (float)pSub->m_cCenter.GetLat(), (float)pSub->m_cCenter.GetLong(), pSub->m_iTop, (float)(pSub->m_dRadius/60.), SILHOUETTE, m_crAirspaceColor);
				c3DTrans.WorldToLogical( cTitlePoint, (float)pSub->m_cCenter.GetLong(),  (float)pSub->m_cCenter.GetLat(),  (float)pSub->m_iTop );
				}
			else if( pSub->m_eType==CSUASubSection::ePOINT )
				{
				cPrevLocation=pSub->m_cLocation;
				c3DTrans.WorldToLogical( cPoint1, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iTop );
				c3DTrans.WorldToLogical( cPoint2, (float)pSub->m_cLocation.GetLong(),  (float)pSub->m_cLocation.GetLat(),  (float)pSub->m_iBase );
				if( bFirstPoint )
					{
					cTitlePoint=cPoint2;
					pDC->MoveTo(SafePoint(cPoint1));
					pDC->LineTo(SafePoint(cPoint2));
					bFirstPoint=false;
					nAverages=1;
					}
				else
					{
					pDC->MoveTo(SafePoint(cPoint1));
					pDC->LineTo(SafePoint(cPoint2));
					pDC->MoveTo(SafePoint(cPrevPoint1));
					pDC->LineTo(SafePoint(cPoint1));
					pDC->MoveTo(SafePoint(cPrevPoint2));
					pDC->LineTo(SafePoint(cPoint2));

					cTitlePoint.x+=cPoint1.x;
					cTitlePoint.y+=cPoint1.y;
					nAverages++;
					}
				cPrevPoint1=cPoint1;
				cPrevPoint2=cPoint2;
				}			
			}

		if( nAverages>0 )
			{
			cTitlePoint.x /=nAverages;
			cTitlePoint.y /=nAverages;
			}

		int iOldStyle=pDC->SetTextAlign( TA_CENTER   );
		pDC->SetTextColor( m_crAirspaceColor);
		if( !IsPointClipped(cTitlePoint) && pOldFont) 
			pDC->TextOut( cTitlePoint.x, cTitlePoint.y,  pSection->m_strTitle, pSection->m_strTitle.GetLength() );
		pDC->SetTextAlign( iOldStyle );
		}

	if( pOldFont ) pDC->SelectObject( pOldFont );

	pDC->SelectObject( pOldPen );
	cPen.DeleteObject();
	}


void CWSViewCtrl::ZoomOnTaskPoint()
	{
	if( m_TaskPointList.GetCount()==0 ) return;

	if( m_iTaskPtr>m_TaskPointList.GetCount()-1 )
		m_iTaskPtr=0;

	m_iTaskPtr++;

	CControlPoint* pcPt=GetTaskPoint(m_iTaskPtr-1);
	if( !pcPt ) return;

	int cx, cy;
	GetControlSize( &cx, &cy );

	float fToler;
	if( m_iTaskPtr==1 )
		{
		if( m_eStartGateType==eCylinder )
			fToler=(float)m_fStartGateSize*1.1f;
		else
			{
			fToler=(float)CLocation(m_fStartGateLatitude, m_fStartGateLongitude).DistanceTo(&CLocation(m_fStartGateLatitude2, m_fStartGateLongitude2),SYSTEMUNITS );
			fToler/=60;
			}
		}
	else if( m_iTaskPtr==m_TaskPointList.GetCount() )
		{
		if( m_eFinishGateType==eCylinder )
			fToler=m_fFinishGateSize*1.1f;
		else
			{
			fToler=(float)CLocation(m_fFinishGateLatitude, m_fFinishGateLongitude).DistanceTo(&CLocation(m_fFinishGateLatitude2, m_fFinishGateLongitude2),SYSTEMUNITS );
			fToler/=60;
			}
		}
	else
		fToler=m_fTaskPointRadius[m_iTaskPtr-1]*1.1f;

	if( fToler<.00001 ) fToler=1.f/60.f;

	m_c3DTransformation.AutoScale( 	pcPt->m_fLong-fToler, 
									pcPt->m_fLong+fToler,
									pcPt->m_fLat-fToler, 
									pcPt->m_fLat+fToler, 
									cx, 
									cy);

	InvalidateRect(NULL,FALSE);

	}

void CWSViewCtrl::DisplayNextTaskPoint() 
{
	ZoomOnTaskPoint();
}

void CWSViewCtrl::SetTaskPointRadius(short sTaskPointID, float fRadius) 
{
	m_fTaskPointRadius[sTaskPointID]=fRadius/60.f;
}

void CWSViewCtrl::AddTATLocation(short sPointID, float fLat, float fLong) 
{
	m_TATPointList.AddTail(new CControlPoint(sPointID, fLat, fLong, "", 0, 0) );
}

void CWSViewCtrl::DefineControlPointAttributes(long iTurnpoint, long iAirport, 
											   long iLandable, long iStart, 
											   long iFinish, long iHome, 
											   long iReserved1, long iReserved2, long iReserved3) 
	{
	 m_iTurnpointAttribute	=iTurnpoint;
	 m_iAirportAttribute	=iAirport;
	 m_iLandableAttribute	=iLandable;
	 m_iStartAttribute		=iStart;
	 m_iFinishAttribute		=iFinish;
	 m_iHomeAttribute		=iHome;

	for( POSITION pos = m_ControlPointList.GetHeadPosition(); pos != NULL; )   
		{
		CControlPoint* pPoint=(CControlPoint*)m_ControlPointList.GetNext( pos );  
		pPoint->UpdateAttributeText(m_iTurnpointAttribute,
									m_iAirportAttribute,
									m_iLandableAttribute,
									m_iStartAttribute,
									m_iFinishAttribute,
									m_iHomeAttribute);
		}
	}

BSTR CWSViewCtrl::GetVersionString() 
{
	CString strResult;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CModuleVersion ver;
    ver.GetFileVersionInfo("wsview.ocx");

	strResult=ver.GetValue(_T("FileVersion"));

	return strResult.AllocSysString();
}

BSTR CWSViewCtrl::GetBuildDate() 
{
	CString strResult;
	strResult=__DATE__;

	return strResult.AllocSysString();
}

void CWSViewCtrl::DrawScale(CDC *pDC)
	{
	if( !m_c3DTransformation.IsViewOrthogonal() ) return;

	int cx, cy;
	GetControlSize( &cx, &cy );

	CPoint cPoint1, cPoint2;
	double fX, fY, fZ;
	double fX2;

	if( !m_c3DTransformation.LogicalToWorld(CPoint(0,cy), fX, fY, fZ) ) return;
	if( !m_c3DTransformation.LogicalToWorld(CPoint(cx,cy), fX2,fY, fZ) ) return;

	if( fY>85 || fY< -85 ) return;

	double dAng=(fY/180)*PI;
	dAng=min(dAng, 80);
	double dCorr=1/cos(dAng);

	double dDist=fabs(fX-fX2)*60.;

	dDist= ConvertDistance(dDist/dCorr, eNautical, m_eUnits );

	double dWorldToLogicalScale=dDist/cx;

	CPoint CScaleStartPoint( 10, cy-10);
	pDC->MoveTo(CScaleStartPoint);


	double dScaleDistance = 150*dWorldToLogicalScale;
	int iScale=1;
	if( dScaleDistance<.1)
		{
		iScale=(int)(dScaleDistance*10.);
		iScale=max(iScale,1);
		dScaleDistance=((float)iScale)/10;
		}
	else if( dScaleDistance>.1 && dScaleDistance<1.)
		{
		iScale=(int)(dScaleDistance*5.);
		iScale=max(iScale,1);
		dScaleDistance=((float)iScale)/5;
		}
	else if( dScaleDistance>10. && dScaleDistance<50.)
		{
		iScale=(int)(dScaleDistance/5.);
		iScale=max(iScale,1);
		dScaleDistance=((float)iScale)*5;
		}
	else if( dScaleDistance>50. && dScaleDistance<250.)
		{
		iScale=(int)(dScaleDistance/10.);
		iScale=max(iScale,1);
		dScaleDistance=((float)iScale)*10;
		}
	else if( dScaleDistance>250. && dScaleDistance<1500. )
		{
		iScale=(int)(dScaleDistance/100.);
		iScale=max(iScale,1);
		dScaleDistance=((float)iScale)*100;
		}
	else if( dScaleDistance>1500. )
		return;
	else
		{
		iScale=(int)dScaleDistance;
		iScale=max(iScale,1);
		dScaleDistance=iScale;
		}

	int iXPos=(int)(dScaleDistance/dWorldToLogicalScale);
	
	if( iXPos>cx ) return;

	CString strText;
	if( dScaleDistance<1.)
		strText.Format("%2.1lf %s",dScaleDistance, GetUnitsText());
	else
		strText.Format("%5.0lf %s",dScaleDistance, GetUnitsText());

	COLORREF oldColor=pDC->SetTextColor( 0 );
	int iOldStyle=pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );

	CFont* pOldFont=pDC->SelectObject( m_pFonts[WSMEDFONT] );
	pDC->TextOut( (iXPos+10)/2, cy-10, strText, strText.GetLength());
	pDC->SelectObject( pOldFont );
	pDC->SetTextAlign( iOldStyle );
	pDC->SetTextColor( oldColor );

	pDC->MoveTo(CScaleStartPoint);
	pDC->LineTo( iXPos+10, cy-10);

	pDC->MoveTo(CScaleStartPoint.x, CScaleStartPoint.y-3 );
	pDC->LineTo(CScaleStartPoint.x, CScaleStartPoint.y+3 );
	pDC->MoveTo(iXPos+10, cy-7 );
	pDC->LineTo(iXPos+10, cy-13 );

	}


CString CWSViewCtrl::GetUnitsText()
{
	if( m_eUnits==eNautical )
		return "Nm";
	else if( m_eUnits==eStatute )
		return "Miles";
	else if( m_eUnits==eKilometers )
		return "Km";

	return "";
}

void CWSViewCtrl::OnMeasure() 
{
	m_bMeasureMode=true;	
	::SetCursor(m_hMeasureCursor);

	if( !m_pMeasuringDlg )
		{
		m_pMeasuringDlg=new CMeasuringDlg;
		m_pMeasuringDlg->Create(IDD_MEASURING, this);
		}

	m_pMeasuringDlg->ResetText();
	m_pMeasuringDlg->ShowWindow(SW_SHOW);

	ReleaseCapture();
}

void CWSViewCtrl::OnUpdateMeasure(CCmdUI* pCmdUI) 
{
#ifdef _DEBUG
	pCmdUI->Enable( true );
	return;
#endif
	pCmdUI->Enable( !GetDisplayOption(WSO_DEMO_MODE) );
}

void CWSViewCtrl::OnMeasureCursor() 
	{
	OnMeasure();
	}

int CWSViewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CWSViewCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if( m_pcHit )
		{
		CFix* pFix=(CFix*)m_pcHit->m_ptr;
		if( pFix )
			FirePointSelected( 1, (long)pFix->m_cTime.GetTime() );

		}	
	COleControl::OnLButtonDblClk(nFlags, point);
}

BOOL CWSViewCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	m_bMeasureMode=false;
	if( nFlags & MK_CONTROL     )
		{
		ScreenToClient(&pt);
		if( zDelta<0 )
			m_c3DTransformation.PointZoom( pt,  .9  );
		else
			m_c3DTransformation.PointZoom( pt,  1.1  );
		InvalidateRect(NULL,FALSE);
		ReleaseCapture();
		}
	else
		{
	if( zDelta>0 )
		FireOnKeyDown(34);
	else
		FireOnKeyDown(33);
		}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

long CWSViewCtrl::GetCurrentDisplayTime() 
{
	return (long)m_cTimeIndex.GetTime();
}


void CWSViewCtrl::Set2ndStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase) 
{

	m_b2ndStartGate=true;

	m_f2ndStartGateLatitude	=fGateLatitude;
	m_f2ndStartGateLongitude=fGateLongitude;
	m_f2ndStartGateSize		=(float)(fGateSize/60.);
	m_s2ndStartGateHeight	=sGateHeight;
	m_s2ndStartGateBase		=sGateBase;
}

BSTR CWSViewCtrl::GetDisplayOptionStr(long lOption) 
{
	CString strResult;

	switch(	lOption )
		{
		case WSC_FONT_NAME:
			{
			strResult=m_strFontName;
			}
		default:
			{
			break;
			}
		}
	return strResult.AllocSysString();
}

void CWSViewCtrl::SetDisplayOptionStr(long lOption, LPCTSTR strValue) 
{
	switch(	lOption )
		{
		case WSC_FONT_NAME:
			{
			m_strFontName=strValue;
			CreateFonts();
			}
		default:
			{
			break;
			}
		}
}

void CWSViewCtrl::CreateFonts()
{
	int iaSize[WSNUMFONTS]={ 10, 12, 18, 22, 28 };

	for( int i=0; i<WSNUMFONTS; i++ )
		{
		if( m_pFonts[i] )
			{
			m_pFonts[i]->DeleteObject();
			delete m_pFonts[i];
			}

		m_pFonts[i]	=new CFont;

		m_pFonts[i]->CreateFont(
							iaSize[i],		//int nHeight, 
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
	}

CFont* CWSViewCtrl::SelectFontByScale(CDC* pDC)
	{
	double sx, sy, sz;
	m_c3DTransformation.GetScale(sx, sy, sz);

	if( sx<200 )
		{
		return NULL;
		}
	if( sx<500 )
		{
		return pDC->SelectObject( m_pFonts[WSTINYFONT] );
		}
	if( sx<1000 )
		{
		return pDC->SelectObject( m_pFonts[WSSMALLFONT] );
		}
	if( sx<2500 )
		{
		return pDC->SelectObject( m_pFonts[WSMEDFONT] );
		}
	if( sx<4000 )
		{
		return pDC->SelectObject( m_pFonts[WSLARGEFONT] );
		}

	return pDC->SelectObject( m_pFonts[WSHUGEFONT] );
	}

void CWSViewCtrl::SetStartTime(short sPilotID, long lTime) 
{
	Winscore::CTrace* pTrace=m_cTraceGroup.GetTrace(sPilotID);
	if(pTrace)
		pTrace->m_cStartTime=CTime(lTime);

}

void CWSViewCtrl::SetFinishTime(short sPilotID, long lTime) 
{

	Winscore::CTrace* pTrace=m_cTraceGroup.GetTrace(sPilotID);
	if(pTrace)
		pTrace->m_cFinishTime=CTime(lTime);

}

void CWSViewCtrl::SetTATLocation(short sTaskPointID, float fLat, float fLong) 
{

	CControlPoint* pcPt=GetTATPoint(sTaskPointID);
	if( !pcPt ) return;

	pcPt->m_fLat =fLat;
	pcPt->m_fLong=fLong;


}
boolean CWSViewCtrl::WriteBMPFile(LPCTSTR bFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_bMeasureMode=false;
	m_bWrite=true;
	m_strWriteFileName=bFileName;
	InvalidateRect(NULL,FALSE);	
	
	return 0;
}
