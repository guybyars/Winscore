// ChangeTurnpointLocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ChangeTurnpointLocDlg.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeTurnpointLocDlg dialog


CChangeTurnpointLocDlg::CChangeTurnpointLocDlg( CWinscoreDoc* pDoc,
					   							CContestant*	pcContestant,
												CTask*			pcTask,
											    CFlight*		 pcFlight,
											    CWnd* pParent /*=NULL*/)
	: CResizingDialog(CChangeTurnpointLocDlg::IDD, pParent),
	m_pDoc(pDoc),
	m_pcContestant(pcContestant),
	m_pcTask(pcTask),
	m_pcFlight(pcFlight)
{
	//{{AFX_DATA_INIT(CChangeTurnpointLocDlg)
	m_strControlPointDistanceText = _T("");
	m_strDistanceFlown = _T("Distance:");
	m_strLogPointEdit = _T("");
	m_dDist = 0.0;
	//}}AFX_DATA_INIT

    SetControlInfo(IDC_ZOOMIN,                ANCHORE_BOTTOM);
    SetControlInfo(IDC_ZOOMOUT,               ANCHORE_BOTTOM);

    SetControlInfo(IDC_TURNPOINTTEXT,               ANCHORE_BOTTOM);
    SetControlInfo(IDC_CONTROLPOINTDISTANCETEXT,    ANCHORE_BOTTOM);
    SetControlInfo(IDC_DISTANCEFLOWNTEXT,			ANCHORE_BOTTOM);
	SetControlInfo(IDOK,							ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,						ANCHORE_BOTTOM);
    SetControlInfo(IDC_LOGPOINTEDIT,				ANCHORE_BOTTOM);
    SetControlInfo(IDC_TURNPOINT_COMBO,				ANCHORE_BOTTOM);
    SetControlInfo(IDC_SPIN1,						ANCHORE_BOTTOM);
    SetControlInfo(IDC_EDIT1,						ANCHORE_BOTTOM);
    SetControlInfo(IDC_WSVIEWCTRL1,    RESIZE_HOR | RESIZE_VER);

	SetControlInfo(IDC_CHANGEPLUSBUTTON,     ANCHORE_BOTTOM);
    SetControlInfo(IDC_CHANGEMINUSBUTTON,	 ANCHORE_BOTTOM);



}


void CChangeTurnpointLocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeTurnpointLocDlg)
	DDX_Control(pDX, IDC_SPIN1, m_cSpin);
	DDX_Control(pDX, IDC_TURNPOINT_COMBO, m_cTurnpointCombo);
	DDX_Text(pDX, IDC_CONTROLPOINTDISTANCETEXT, m_strControlPointDistanceText);
	DDX_Text(pDX, IDC_DISTANCEFLOWNTEXT, m_strDistanceFlown);
	DDX_Text(pDX, IDC_LOGPOINTEDIT, m_strLogPointEdit);
	DDX_Control(pDX, IDC_WSVIEWCTRL1, m_cWSViewCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_dDist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeTurnpointLocDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeTurnpointLocDlg)
	ON_CBN_SELCHANGE(IDC_TURNPOINT_COMBO, OnSelchangeTurnpointCombo)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ZOOMIN, OnZoomin)
	ON_BN_CLICKED(IDC_ZOOMOUT, OnZoomout)
	ON_BN_CLICKED(IDC_CHANGEPLUSBUTTON, OnChangeplusbutton)
	ON_BN_CLICKED(IDC_CHANGEMINUSBUTTON, OnChangeminusbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeTurnpointLocDlg message handlers

void CChangeTurnpointLocDlg::OnSelchangeTurnpointCombo() 
	{
	int iSel=m_cTurnpointCombo.GetCurSel();

	CTurnpoint* pcTurnpoint=(CTurnpoint*)m_cTurnpointCombo.GetItemData(iSel);
	m_cWSViewCtrl.SetViewCenter((float)pcTurnpoint->GetLat(), (float)pcTurnpoint->GetLong());

	int iFix=m_pcFlight->GetTurnAreaFixID(iSel);

	CPosition *pcPos=m_pcFlight->GetPosition(iFix);
	if( iFix!=0 ) 
		m_cWSViewCtrl.SetViewCenter((float)pcPos->GetLat(), (float)pcPos->GetLong());

	m_cSpin.SetPos(iFix);
	UpdateFix();

	}


BOOL CChangeTurnpointLocDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	LoadViewControl( m_cWSViewCtrl,NULL, m_pDoc, m_pcFlight, m_pcTask );

	m_cWSViewCtrl.Autoscale();

	for ( int i=0; i<m_pcFlight->GetNumTaskTurnpoints(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = m_pDoc->GetTurnpointByID(m_pcFlight->GetTaskPointID(i));
		ASSERT( pcTurnpoint);
		if( pcTurnpoint )  
			{
			CString strText;
			strText.Format(_T("%d:  %s"),i+1,pcTurnpoint->GetNoNameText());
			int iItem=m_cTurnpointCombo.AddString(strText );
			m_cTurnpointCombo.SetItemData(iItem, (DWORD)pcTurnpoint);
			}	
		m_cTurnpointCombo.SetCurSel(0);
		m_cSpin.SetRange32(0,m_pcFlight->GetNumPoints() );

		int iFix=m_pcFlight->GetTurnAreaFixID(0);
		m_cSpin.SetPos(iFix);

		OnSelchangeTurnpointCombo();

		UpdateFix();

		}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeTurnpointLocDlg::UpdateFix()
	{
	int iFix=LOWORD( m_cSpin.GetPos() );
	int iSel=m_cTurnpointCombo.GetCurSel();
	CTurnpoint* pcTurnpoint=(CTurnpoint*)m_cTurnpointCombo.GetItemData(iSel);

	iFix=min(iFix,m_pcFlight->GetNumPoints()-1);
	iFix=max(iFix,0);

	CPosition *pcPos=m_pcFlight->GetPosition(iFix);
	if( !pcPos )
		{
		m_strLogPointEdit=_T("Invalid Fix");
		m_strControlPointDistanceText=_T("");
		}
	else
		{
		double dDistToCP=pcTurnpoint->DistanceTo(	pcPos,
													m_pDoc->m_eUnits);
		m_strControlPointDistanceText.Format(_T("Distance from control point: %6.3lf %s"), dDistToCP, m_pDoc->GetUnitsText());

		m_pcFlight->SetTurnAreaLocation(iSel,*pcPos);
		m_pcFlight->SetTurnAreaFixID(iSel,iFix);

		m_pcFlight->SetDistance(m_pcFlight->ComputeDistance(m_pcTask, m_pDoc->m_turnpointArray));
		m_dDist=ConvertDistance(m_pcFlight->GetDistance(),SYSTEMUNITS, m_pDoc->m_eUnits);

	//	m_strDistanceFlown.Format("Total Distance Flown: %6.2lf %s", dDist, m_pDoc->GetUnitsText());
		m_strLogPointEdit.Format(	_T("Fix: %d - %s - %s"),
									iFix, 
									pcPos->GetTimeString(),
									pcPos->GetFormattedInfo());

		m_cWSViewCtrl.SetTATLocation(iSel+1, (float)pcPos->GetLat(), (float)pcPos->GetLong());
		m_cWSViewCtrl.Invalidate();

		}

	UpdateData(false);

	}

void CChangeTurnpointLocDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	UpdateFix();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CChangeTurnpointLocDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();
}

void CChangeTurnpointLocDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	
}


void CChangeTurnpointLocDlg::OnZoomin() 
{
		m_cWSViewCtrl.ScaleView(1.5);
	
}

void CChangeTurnpointLocDlg::OnZoomout() 
{
		m_cWSViewCtrl.ScaleView(.75);
	
}

void CChangeTurnpointLocDlg::OnChangeplusbutton() 
{
	int iFix=LOWORD( m_cSpin.GetPos() );
	iFix++;
	m_cSpin.SetPos(iFix);
	UpdateFix();
	
}

void CChangeTurnpointLocDlg::OnChangeminusbutton() 
{
	int iFix=LOWORD( m_cSpin.GetPos() );
	iFix--;
	if( iFix<0 ) iFix=0;
	m_cSpin.SetPos(iFix);
	UpdateFix();
	
}
