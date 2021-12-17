// TaskDisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "winscoredoc.h"
#include "TaskDisplayDlg.h"
#include "Task.h"
#include "ResizingDialog.h"
#include "Global_Prototypes.h"
#include "suautil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskDisplayDlg dialog


CTaskDisplayDlg::CTaskDisplayDlg(CTask *pcTask, CWinscoreDoc *pcDoc, CWnd* pParent /*=NULL*/)
	: CResizingDialog(CTaskDisplayDlg::IDD, pParent),
	m_pDoc(pcDoc),
	m_pcTask(pcTask)
{
    SetControlInfo(IDC_WSVIEWCTRL1,		RESIZE_HOR		| RESIZE_VER);
 	SetControlInfo(IDCANCEL,			ANCHORE_RIGHT	| ANCHORE_BOTTOM );
 	SetControlInfo(IDC_HELP_BUTTON,		ANCHORE_RIGHT	| ANCHORE_BOTTOM );

	//{{AFX_DATA_INIT(CTaskDisplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaskDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskDisplayDlg)
	DDX_Control(pDX, IDC_WSVIEWCTRL1, m_cWSViewCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskDisplayDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskDisplayDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDisplayDlg message handlers

BOOL CTaskDisplayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDialog::OnInitDialog();

	CString cWinText=_T("Task Display ");
	cWinText+=_T("    (Press right mouse button for view commands)");
	SetWindowText(cWinText);

	float fTurnpointRadius		= (float)GetWinscoreDouble( INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	float fTurnpointOuterRadius	= (float)GetWinscoreDouble( OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );
	m_cWSViewCtrl.SetDisplayOptionD(WSO_INNER_RADIUS, fTurnpointRadius);
	m_cWSViewCtrl.SetDisplayOptionD(WSO_OUTER_RADIUS, fTurnpointOuterRadius);
	m_cWSViewCtrl.SetDisplayOption(WSC_UNITS, m_pDoc->m_eUnits);

	// Add the control points
	m_pDoc->m_turnpointArray.AddToControl(m_cWSViewCtrl);

	if( m_pcTask)
		m_pcTask->AddToControl(m_pDoc->m_turnpointArray, m_cWSViewCtrl );


	// Add the SUA Stuff
	BOOL bSUADisplay = GetWinscoreInt( SUADISPLAY, DEFAULT_SUADISPLAY );
	if( bSUADisplay )
		{
		CString strAirspaceFilename=GetWinscoreString( SUADISPLAYFILE, _T("") );
		CStringArray strAirspaceArray;
//		int nSections=ReadSUAFile(strAirspaceFilename, strAirspaceArray);
		int nSections=LoadSUA(m_pDoc->m_turnpointArray);
		if( nSections>0  ) 
			{
			m_cWSViewCtrl.SetColor(WSC_AIRSPACE_COLOR, RGB(255,16,16));
			for( int i=0; i<GetSUAArray()->GetSize(); i++ )
				{
				m_cWSViewCtrl.AddSUAirspace( GetSUAArray()->GetAt(i)->m_strOriginalText );
				}
			}
		}
	
	m_cWSViewCtrl.AutoscaleTask();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaskDisplayDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	
}

void CTaskDisplayDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CTaskDisplayDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowWindow( SW_SHOWMAXIMIZED  );
	
	m_cWSViewCtrl.Autoscale();
	
}

void CTaskDisplayDlg::OnHelpButton() 
{
    AfxGetApp()->WinHelp( 0x10000 + ID_FLIGHT_DISPLAY,  HELP_CONTEXT);	
}
