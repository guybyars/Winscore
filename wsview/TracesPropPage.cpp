// TracesPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "wsview.h"
#include "TracesPropPage.h"
#include "winscoredefs.h"
#include "wsviewinterface.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage dialog

IMPLEMENT_DYNCREATE(CTracesPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CTracesPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CTracesPropPage)
	ON_BN_CLICKED(IDC_FULL_TRACE_CHECK, OnFullTraceCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {3006CB4A-FE27-4D16-8D9C-EFC9A2DCEF81}
IMPLEMENT_OLECREATE_EX(CTracesPropPage, "WSView.CTracesPropPage",
	0x3006cb4a, 0xfe27, 0x4d16, 0x8d, 0x9c, 0xef, 0xc9, 0xa2, 0xdc, 0xef, 0x81)


/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage::CTracesPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CTracesPropPage

BOOL CTracesPropPage::CTracesPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.

	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TRACEPROPPAGE);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage::CTracesPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CTracesPropPage::CTracesPropPage() :
	COlePropertyPage(IDD, IDS_WSVIEW_TRACE_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CTracesPropPage)
	m_bDifferentColorCheck = FALSE;
	m_iDisplayLenfth = 0;
	m_bPointDisplay = FALSE;
	m_bFullTraceCheck = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage::DoDataExchange - Moves data between page and properties

void CTracesPropPage::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CTracesPropPage)
	DDX_Control(pDX, IDC_FULL_TRACE_CHECK, m_cFullTraceCheck);
	DDX_Control(pDX, IDC_DISPLAY_LENGTH, m_cDisplayLength);
	DDX_Control(pDX, IDC_MINUTES_TEXT, m_cMinutesText);
	DDX_Control(pDX, IDC_DISPLAY_LENGTH_TEXT, m_cDisplayLengthText);
	DDX_Check(pDX, IDC_DIFFERENT_COLOR_CHECK, m_bDifferentColorCheck);
	DDP_Text(pDX, IDC_DISPLAY_LENGTH, m_iDisplayLenfth, _T("DisplayLength") );
	DDX_Text(pDX, IDC_DISPLAY_LENGTH, m_iDisplayLenfth);
	DDP_Check(pDX, IDC_FIX_POINTS_CHECK, m_bPointDisplay, _T("PointDisplay") );
	DDX_Check(pDX, IDC_FIX_POINTS_CHECK, m_bPointDisplay);
	DDX_Check(pDX, IDC_FULL_TRACE_CHECK, m_bFullTraceCheck);
	//}}AFX_DATA_MAP

	if( pDX->m_bSaveAndValidate )
		{
		unsigned long ulDispatchCount;								
		LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
		assert(ulDispatchCount == 1);
		_DWSView wsview;
		wsview.AttachDispatch(plpdispatch[0], FALSE);
		wsview.SetDisplayOption( WSO_FULL_TRACE_DISPLAY, m_bFullTraceCheck );
		wsview.SetDisplayOption( WSO_DIFFERENT_CLIMB_COLOR,	m_bDifferentColorCheck );
		wsview.InvalidateGrid();
		}




	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage message handlers

BOOL CTracesPropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
   unsigned long ulDispatchCount;								
    LPDISPATCH *plpdispatch =  GetObjectArray(&ulDispatchCount);
    assert(ulDispatchCount == 1);
    _DWSView wsview;
    wsview.AttachDispatch(plpdispatch[0], FALSE);
	
	m_bFullTraceCheck		=wsview.GetDisplayOption( WSO_FULL_TRACE_DISPLAY );
	m_cMinutesText.EnableWindow(!m_bFullTraceCheck);
	m_cDisplayLengthText.EnableWindow(!m_bFullTraceCheck);
	m_cDisplayLength.EnableWindow(!m_bFullTraceCheck);

	m_bDifferentColorCheck	=wsview.GetDisplayOption( WSO_DIFFERENT_CLIMB_COLOR );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTracesPropPage::OnFullTraceCheck() 
{
	m_cMinutesText.EnableWindow(!m_cFullTraceCheck.GetCheck());
	m_cDisplayLengthText.EnableWindow(!m_cFullTraceCheck.GetCheck());
	m_cDisplayLength.EnableWindow(!m_cFullTraceCheck.GetCheck());	
}
