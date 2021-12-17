// LandingLocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "LandingLocationDlg.h"
#include "LandoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLandingLocationDlg dialog


CLandingLocationDlg::CLandingLocationDlg(	CWinscoreDoc* pDoc, 
											CLocation	&LandingcLocation,
											CLocation	&cFurthestProgress,
											int			iLandoutTurnpoint,
											BOOL		bUseFurthestProgress,
											CWnd* pParent)
	: CDialog(CLandingLocationDlg::IDD, pParent),
	m_pDoc(pDoc),
	m_cFurthestProgressLocation(cFurthestProgress),
	m_cLandoutLocation(LandingcLocation),
	m_iLandoutTurnpoint(iLandoutTurnpoint)
{
	//{{AFX_DATA_INIT(CLandingLocationDlg)
	//}}AFX_DATA_INIT

}


void CLandingLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLandingLocationDlg)
	DDX_Control(pDX, IDC_PROGRESSRADIO, m_cProgressRadio);
	DDX_Control(pDX, IDC_LATLONG_TEXT, m_cLatLongText);
	DDX_Control(pDX, IDC_LATLONG_BUTTON, m_cLatLongButton);
	DDX_Control(pDX, IDC_ATLATLONG_RADIO, m_cAtLatLongRadio);
	DDX_Control(pDX, IDC_ATTURNPOINT_RADIO, m_cAtTurnpointRadio);
	DDX_Control(pDX, IDC_LANDOUT_TURNPOINT_COMBO2, m_cLandoutTurnpoint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLandingLocationDlg, CDialog)
	//{{AFX_MSG_MAP(CLandingLocationDlg)
	ON_BN_CLICKED(IDC_ATTURNPOINT_RADIO, OnAtturnpointRadio)
	ON_BN_CLICKED(IDC_ATLATLONG_RADIO, OnAtlatlongRadio)
	ON_BN_CLICKED(IDC_LATLONG_BUTTON, OnLatlongButton)
	ON_CBN_SELCHANGE(IDC_LANDOUT_TURNPOINT_COMBO2, OnSelchangeLandoutTurnpointCombo2)
	ON_BN_CLICKED(IDC_PROGRESSRADIO, OnProgressradio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLandingLocationDlg message handlers

void CLandingLocationDlg::OnOK() 
{
	m_iLandoutTurnpoint= -1;
	m_bUseFurthestProgress=false;

	if( m_cAtTurnpointRadio.GetCheck()==1 )
		{
		CTurnpoint* pcTurnpoint=m_cLandoutTurnpoint.GetCurSel();
		ASSERT( pcTurnpoint );
		m_iLandoutTurnpoint=pcTurnpoint->m_iID;

		m_cLandoutLocation=*pcTurnpoint;
		m_strLandoutTurnpointName=pcTurnpoint->m_strName;
		}
	else if (m_cAtTurnpointRadio.GetCheck()==1 )
		{
		m_bUseFurthestProgress=true;
		}

	CDialog::OnOK();
}


void CLandingLocationDlg::OnAtlatlongRadio() 
{
	m_cAtTurnpointRadio.SetCheck(0);
	m_cProgressRadio.SetCheck(0);
	m_cAtLatLongRadio.SetCheck(1);

	UpdateLandoutRadio();
}

void CLandingLocationDlg::OnAtturnpointRadio() 
{
	EnableLandoutRadios(0);

	CTurnpoint* pcTurnpoint=m_cLandoutTurnpoint.GetCurSel();
	ASSERT(pcTurnpoint);
	m_cLandoutLocation= *pcTurnpoint;
}

void CLandingLocationDlg::UpdateLandoutRadio()
	{
	EnableLandoutRadios(1);
	UpdateData(false);
	}

void CLandingLocationDlg::OnLatlongButton() 
{
	CLandoutDlg	dlg;
	dlg.m_cLocation=m_cLandoutLocation;
	dlg.m_pDoc=m_pDoc;

	if (dlg.DoModal() == IDOK)
		{
		m_cLandoutLocation=dlg.m_cLocation;
		m_cLatLongText.SetWindowText(m_cLandoutLocation.GetFormattedInfo());
		}	
}

BOOL CLandingLocationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cLandoutTurnpoint, TPT_AIRFIELD, 1);

	m_cLatLongText.SetWindowText(m_cLandoutLocation.GetFormattedInfo());

	if( m_iLandoutTurnpoint>0 )
		{
		EnableLandoutRadios(0);
		}
	else if( m_bUseFurthestProgress)
		EnableLandoutRadios(2);
	else
		EnableLandoutRadios(1);

	if( m_iLandoutTurnpoint>0 )
		m_cLandoutTurnpoint.SetCurSel(m_iLandoutTurnpoint);

	CString strTemp="Constructive Landing Point: ";
	strTemp+=m_cFurthestProgressLocation.GetFormattedInfo();
	m_cProgressRadio.SetWindowText(strTemp);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLandingLocationDlg::EnableLandoutRadios(int iOpt)
{
	m_cAtTurnpointRadio.SetCheck(iOpt==0);
	m_cAtLatLongRadio.SetCheck(iOpt==1);
	m_cProgressRadio.SetCheck(iOpt==2);

	if( iOpt==0 ) 
		{
		m_cLandoutTurnpoint.EnableWindow(1);
		m_cLatLongButton.EnableWindow(0);
		m_cLatLongText.EnableWindow(0);
		}
	else if( iOpt==1 ) 
		{
		m_cLandoutTurnpoint.EnableWindow(0);
		m_cLatLongButton.EnableWindow(1);
		m_cLatLongText.EnableWindow(1);
		}
	else if( iOpt==2 ) 
		{
		m_cLandoutTurnpoint.EnableWindow(0);
		m_cLatLongButton.EnableWindow(0);
		m_cLatLongText.EnableWindow(0);
		}

}

void CLandingLocationDlg::OnSelchangeLandoutTurnpointCombo2() 
{
	CTurnpoint* pcTurnpoint=m_cLandoutTurnpoint.GetCurSel();
	ASSERT(pcTurnpoint);
	m_cLandoutLocation= *pcTurnpoint;

}

void CLandingLocationDlg::OnProgressradio() 
{
	EnableLandoutRadios(2);
}
