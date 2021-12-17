//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSViewPpg.cpp : Implementation of the CWSViewPropPage property page class.

#include "stdafx.h"
#include "WSView.h"
#include "WSViewPpg.h"
#include "winscoredefs.h"
#include "wsviewinterface.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INITIALIZE_BITMAP( cBitmapControl, crBitmapColor )   \
	{					\
	CRect rect;			\
    cBitmapControl.GetWindowRect(rect);			\
    m_bitmap.CreateCompatibleBitmap(&CClientDC(this), rect.right - rect.left, rect.bottom - rect.top);    m_bitmap.SetBitmapDimension(rect.right - rect.left, rect.bottom - rect.top);		\
	FillPropControl( m_bitmap, cBitmapControl, crBitmapColor);		\
	}	


IMPLEMENT_DYNCREATE(CWSViewPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWSViewPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CWSViewPropPage)
	ON_BN_CLICKED(IDC_CHANGE_BACKGROUND_BUTTON, OnChangeBackgroundButton)
	ON_BN_CLICKED(IDC_RESETBUTTON, OnResetbutton)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COLOR_COMBO, OnSelchangeColorCombo)
	ON_BN_CLICKED(IDC_SCENERY_CHECK, OnSceneryCheck)
	ON_CBN_SELCHANGE(IDC_FONT_COMBO, OnSelchangeFontCombo)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWSViewPropPage, "WSVIEW.WSViewPropPage.1",
	0xe0748847, 0x5814, 0x11d3, 0xba, 0x96, 0, 0x10, 0xa4, 0x4, 0x32, 0x81)


/////////////////////////////////////////////////////////////////////////////
// CWSViewPropPage::CWSViewPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CWSViewPropPage

BOOL CWSViewPropPage::CWSViewPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WSVIEW_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CWSViewPropPage::CWSViewPropPage - Constructor

CWSViewPropPage::CWSViewPropPage() :
	COlePropertyPage(IDD, IDS_WSVIEW_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CWSViewPropPage)
	m_bSceneryCheck = FALSE;
	//}}AFX_DATA_INIT
	m_bSceneryState=false;
}


/////////////////////////////////////////////////////////////////////////////
// CWSViewPropPage::DoDataExchange - Moves data between page and properties

void CWSViewPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CWSViewPropPage)
	DDX_Control(pDX, IDC_FONT_COMBO, m_cFontCombo);
	DDX_Control(pDX, IDC_VERTSCALEGROUP, m_cVertScaleGroup);
	DDX_Control(pDX, IDC_SCENERY_CHECK, m_cSceneryCheck);
	DDX_Control(pDX, IDC_COLOR_COMBO, m_cColorCombo);
	DDX_Control(pDX, IDC_ALTSLIDER, m_cSlider);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR, m_cBackgroundColor);
	DDX_Check(pDX, IDC_SCENERY_CHECK, m_bSceneryCheck);
	//}}AFX_DATA_MAP

	if( pDX->m_bSaveAndValidate )
		{
		unsigned long ulDispatchCount;								
		LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
		assert(ulDispatchCount == 1);
		_DWSView wsview;
		wsview.AttachDispatch(plpdispatch[0], FALSE);
		wsview.SetColor(WSC_BACKGROUND_COLOR, m_crBackgroundColor);
		wsview.SetColor(WSC_START_RADIUS_COLOR, m_crStartRadiusColor);
		wsview.SetColor(WSC_FINISH_RADIUS_COLOR, m_crFinishRadiusColor);
		wsview.SetColor(WSC_TASK_COLOR, m_crTaskColor);
		wsview.SetColor(WSC_AIRSPACE_COLOR, m_crAirspaceColor);
		wsview.SetColor(WSC_CLIMB_COLOR, m_crClimbColor);
		wsview.SetColor(WSC_WAYPOINT_COLOR, m_crWaypointColor);
		if( m_bSceneryCheck!=m_bSceneryState )
			{
			wsview.SetDisplayOption( WSO_SHOW_SCENERY,	m_bSceneryCheck );
			wsview.InvalidateGrid();
			m_bSceneryState=m_bSceneryCheck;
			}
		if( m_bFontChanged )
			{
			CString strFontName;
			m_cFontCombo.GetWindowText(strFontName);
			wsview.SetDisplayOptionStr( WSC_FONT_NAME,	strFontName );
			}

		}

	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CWSViewPropPage message handlers

BOOL CWSViewPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
    unsigned long ulDispatchCount;								
    LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
    assert(ulDispatchCount == 1);
    _DWSView wsview;
    wsview.AttachDispatch(plpdispatch[0], FALSE);
	m_crBackgroundColor		=wsview.GetColor(WSC_BACKGROUND_COLOR);
	m_crStartRadiusColor	=wsview.GetColor(WSC_START_RADIUS_COLOR);
	m_crFinishRadiusColor	=wsview.GetColor(WSC_FINISH_RADIUS_COLOR);
	m_crTaskColor			=wsview.GetColor(WSC_TASK_COLOR);
	m_crAirspaceColor		=wsview.GetColor(WSC_AIRSPACE_COLOR);
	m_crClimbColor			=wsview.GetColor(WSC_CLIMB_COLOR);
	m_crWaypointColor		=wsview.GetColor(WSC_WAYPOINT_COLOR);
	
	m_bSceneryCheck			=wsview.GetDisplayOption( WSO_SHOW_SCENERY );
	m_bSceneryState			=m_bSceneryCheck;

	m_cSlider.ShowWindow( wsview.GetDisplayOption( WSO_DEMO_MODE )?SW_HIDE:SW_SHOW );
	m_cVertScaleGroup.ShowWindow( wsview.GetDisplayOption( WSO_DEMO_MODE )?SW_HIDE:SW_SHOW );
	m_cSceneryCheck.ShowWindow( wsview.GetDisplayOption( WSO_DEMO_MODE )?SW_HIDE:SW_SHOW );

	INITIALIZE_BITMAP( m_cBackgroundColor, m_crBackgroundColor );

	m_cSlider.SetRange( 0, 20 );
	m_cSlider.SetPos(10);
	m_iLastPos=10;

	m_cColorCombo.SetCurSel(0);

    m_cFontCombo.Initialize();
	m_cFontCombo.SetWindowText( wsview.GetDisplayOptionStr( WSC_FONT_NAME ) );
	m_bFontChanged=false;

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWSViewPropPage::FillPropControl( CBitmap &bitmap, CStatic &cStatic, COLORREF crColor)
    {
    // set color control


    CRect rect;
 
    // Paint bitmap
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(&CClientDC(this));

    CBitmap *pbitmapPrev = dcMemory.SelectObject(&bitmap);
 
    dcMemory.FillRect(CRect(CPoint(0, 0),
                            CSize(bitmap.GetBitmapDimension().cx, bitmap.GetBitmapDimension().cy)),
                      &CBrush(crColor));

    dcMemory.SelectObject(pbitmapPrev);
    dcMemory.DeleteDC();
    cStatic.SetBitmap(bitmap); 
    cStatic.Invalidate(); 
  
    }

void CWSViewPropPage::OnChangeBackgroundButton() 
{
	COLORREF crColor;
	int iSel=m_cColorCombo.GetCurSel();

	if( iSel==WSC_START_RADIUS_COLOR )
		crColor=m_crStartRadiusColor;
	else if( iSel==WSC_FINISH_RADIUS_COLOR )
		crColor=m_crFinishRadiusColor;
	else if( iSel==WSC_TASK_COLOR )
		crColor=m_crTaskColor;
	else if( iSel==WSC_AIRSPACE_COLOR )
		crColor=m_crAirspaceColor;
	else if( iSel==WSC_BACKGROUND_COLOR )
		crColor=m_crBackgroundColor;
	else if( iSel==WSC_CLIMB_COLOR )
		crColor=m_crClimbColor;
	else if( iSel==WSC_WAYPOINT_COLOR )
		crColor=m_crWaypointColor;

    CColorDialog colordialog(m_crBackgroundColor, CC_ANYCOLOR);
    if (colordialog.DoModal() == IDOK)
        {
        crColor=colordialog.GetColor();

		if( iSel==WSC_START_RADIUS_COLOR )
			m_crStartRadiusColor=crColor;
		else if( iSel==WSC_FINISH_RADIUS_COLOR )
			m_crFinishRadiusColor=crColor;
		else if( iSel==WSC_TASK_COLOR )
			m_crTaskColor=crColor;
		else if( iSel==WSC_AIRSPACE_COLOR )
			m_crAirspaceColor=crColor;
		else if( iSel==WSC_BACKGROUND_COLOR )
			m_crBackgroundColor=crColor;
		else if( iSel==WSC_CLIMB_COLOR )
			m_crClimbColor=crColor;
		else if( iSel==WSC_WAYPOINT_COLOR )
			m_crWaypointColor=crColor;

		FillPropControl(m_bitmap, m_cBackgroundColor, crColor);
        }		
}


void CWSViewPropPage::OnResetbutton() 
{
	m_crBackgroundColor		=WSC_BACKGROUND_COLOR_RGB;
	m_crStartRadiusColor	=WSC_START_RADIUS_COLOR_RGB;
	m_crFinishRadiusColor	=WSC_FINISH_RADIUS_COLOR_RGB;
	m_crTaskColor			=WSC_TASK_COLOR_RGB;
	m_crAirspaceColor		=WSC_AIRSPACE_COLOR_RGB;
	m_crClimbColor			=WSC_CLIMB_COLOR_RGB;
	m_crWaypointColor		=WSC_WAYPOINT_COLOR_RGB;


	FillPropControl(m_bitmap, m_cBackgroundColor, m_crBackgroundColor);
	m_cColorCombo.SetCurSel(0);

	unsigned long ulDispatchCount;								
	LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
	assert(ulDispatchCount == 1);
	_DWSView wsview;
	wsview.AttachDispatch(plpdispatch[0], FALSE);
	wsview.Autoscale();
}

void CWSViewPropPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( pScrollBar->GetDlgCtrlID()==IDC_ALTSLIDER)
		{
		unsigned long ulDispatchCount;								
		LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
		assert(ulDispatchCount == 1);
		_DWSView wsview;
		wsview.AttachDispatch(plpdispatch[0], FALSE);
		int iPos=m_cSlider.GetPos();

		wsview.SetZScale( 1+.3f*(m_iLastPos-iPos) );
		m_iLastPos=iPos;
		}
	
	COlePropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CWSViewPropPage::OnSelchangeColorCombo() 
{
	COLORREF crColor;
	int iSel=m_cColorCombo.GetCurSel();

	if( iSel==WSC_START_RADIUS_COLOR )
		crColor=m_crStartRadiusColor;
	else if( iSel==WSC_FINISH_RADIUS_COLOR )
		crColor=m_crFinishRadiusColor;
	else if( iSel==WSC_TASK_COLOR )
		crColor=m_crTaskColor;
	else if( iSel==WSC_AIRSPACE_COLOR )
		crColor=m_crAirspaceColor;
	else if( iSel==WSC_BACKGROUND_COLOR )
		crColor=m_crBackgroundColor;
	else if( iSel==WSC_CLIMB_COLOR )
		crColor=m_crClimbColor;
	else if( iSel==WSC_WAYPOINT_COLOR )
		crColor=m_crWaypointColor;

	FillPropControl(m_bitmap, m_cBackgroundColor, crColor);
	
}

void CWSViewPropPage::OnSceneryCheck() 
{
		unsigned long ulDispatchCount;								
		LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
		assert(ulDispatchCount == 1);
		_DWSView wsview;
		wsview.AttachDispatch(plpdispatch[0], FALSE);
		if( wsview.GetDisplayOption(WSO_DEMO_MODE) && m_cSceneryCheck.GetCheck() )
			AfxMessageBox("Demo version only displays selected maps at the Mifflin contest site.");
	
}


void CWSViewPropPage::OnSelchangeFontCombo() 
	{
	m_bFontChanged=true;
	}

