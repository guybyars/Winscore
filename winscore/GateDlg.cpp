//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// GateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "GateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGateDlg dialog


CGateDlg::CGateDlg(CWinscoreDoc *pDoc, EGateDlgType  eGateDlgType, CGate &cGate, CWnd* pParent /*=NULL*/)
	: CDialog(CGateDlg::IDD, pParent),
	m_cGate(cGate),
	m_eGateDlgType(eGateDlgType),
	m_pDoc(pDoc)
{
	//{{AFX_DATA_INIT(CGateDlg)
	m_iBase = 0;
	m_iHeading = 0;
	m_iHeight = 0;
	m_dRadius = 0.0;
	m_strUnitsText = _T("");
	//}}AFX_DATA_INIT

	m_eUnits=pDoc->m_eUnits;

}


void CGateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGateDlg)
	DDX_Control(pDX, IDC_LEG_CHECK, m_cFirstLegCheck);
	DDX_Control(pDX, IDC_PERP_CHECK, m_cPerpCheck);
	DDX_Control(pDX, IDC_HEADING_TEXT, m_cHeadingText);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_cHeightEdit);
	DDX_Control(pDX, IDC_TYPECOMBO, m_cTypeCombo);
	DDX_Control(pDX, IDC_TYPE_TEXT, m_cTypeText);
	DDX_Control(pDX, IDC_RADIUS_TEXT, m_cRadiusText);
	DDX_Control(pDX, IDC_RADIUS_EDIT, m_cRadiusEdit);
	DDX_Control(pDX, IDC_POINTCOMBO, m_cPointCombo);
	DDX_Control(pDX, IDC_HEIGHT_TEXT, m_cHeightText);
	DDX_Control(pDX, IDC_BASE_EDIT, m_cBaseEdit);
	DDX_Control(pDX, IDC_HEADING_EDIT, m_cHeadingEdit);
	DDX_Control(pDX, IDC_BASE_TEXT, m_cBaseText);
	DDX_Text(pDX, IDC_BASE_EDIT, m_iBase);
	DDV_MinMaxInt(pDX, m_iBase, 0, 32000);
	DDX_Text(pDX, IDC_HEADING_EDIT, m_iHeading);
	DDV_MinMaxInt(pDX, m_iHeading, 0, 360);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_iHeight);
	DDV_MinMaxInt(pDX, m_iHeight, 0, 32000);
	DDX_Text(pDX, IDC_RADIUS_EDIT, m_dRadius);
	DDV_MinMaxDouble(pDX, m_dRadius, 0., 100.);
	DDX_Text(pDX, IDC_UNITSTEXT, m_strUnitsText);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OUT_THE_TOP_CHECK, m_cOutTheTopCheck);
}


BEGIN_MESSAGE_MAP(CGateDlg, CDialog)
	//{{AFX_MSG_MAP(CGateDlg)
	ON_BN_CLICKED(IDC_PERP_CHECK, OnBnClickedPerpCheck)
	ON_CBN_SELCHANGE(IDC_TYPECOMBO, OnSelchangeTypecombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGateDlg message handlers

void CGateDlg::OnGpscheck() 
	{
	MaskForm();	
	}

BOOL CGateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nCtlPts= m_pDoc->m_turnpointArray.LoadTurnpointComboBox(m_cPointCombo, (m_eGateDlgType==CGateDlg::eFinish)?(TPT_FINISH):(TPT_START), m_cGate.GetPointID() );
	if( nCtlPts==0 ) 
		{
		if( m_eGateDlgType==CGateDlg::eFinish ) 
			AfxMessageBox(_T("Warning, no control points with FINISH attribute have been entered."));
		else
			AfxMessageBox(_T("Warning, no control points with START attribute have been entered."));
		}

	m_strUnitsText=m_pDoc->GetUnitsText();

	if( m_eGateDlgType==CGateDlg::eFinish ) 
			SetWindowText(_T("Finish Gate Settings"));
	else
			SetWindowText(_T("Start Gate Settings"));

	m_cFirstLegCheck.SetCheck(m_cGate.IsFirstLegAtExit());
	m_cPerpCheck.SetCheck(m_cGate.IsPerpToCourse());
	m_cOutTheTopCheck.SetCheck(m_cGate.ScoreOutTheTop());

	m_iBase		= m_cGate.GetBase();
	m_iHeading	= (int)m_cGate.GetHeading();
	m_iHeight	= m_cGate.GetHeight();
	m_dRadius	= ConvertDistance(m_cGate.GetRadius() ,SYSTEMUNITS, m_eUnits);
	if( m_cGate.GetGateType()==eLine ) m_dRadius*=2.;
	m_cTypeCombo.SetCurSel((int)m_cGate.GetGateType() );

	MaskForm();

	if( m_eGateDlgType==CGateDlg::eFinish )
		{
		m_cFirstLegCheck.EnableWindow(false);
		m_cPerpCheck.EnableWindow(false);
        m_cOutTheTopCheck.EnableWindow(false);
		}


	UpdateData(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGateDlg::OnOK() 
{
	UpdateData(true);
	m_cGate.SetFirstLegAtExit((m_cFirstLegCheck.GetCheck())?(true):(false));
	m_cGate.SetGateType( (EGateType)m_cTypeCombo.GetCurSel() );
	m_cGate.SetBase(m_iBase);
	m_cGate.SetHeading(m_iHeading);
	m_cGate.SetHeight(m_iHeight);
    if( m_eGateDlgType==CGateDlg::eStart )
	  {
	  bool b=m_cPerpCheck.GetCheck()==BST_CHECKED;
      m_cGate.SetPerpToCourse(b);

	  b=m_cOutTheTopCheck.GetCheck()==BST_CHECKED;
	  m_cGate.SetScoreOutTheTop(b);
	  }
	if( m_cGate.GetGateType()==eLine ) m_dRadius/=2.;
	m_cGate.SetRadius( ConvertDistance(m_dRadius, m_eUnits, SYSTEMUNITS) );
	CTurnpoint *pcTurnpoint=m_cPointCombo.GetCurSel();
	if( pcTurnpoint ) 
		m_cGate.SetPointID( pcTurnpoint->m_iID );
	

	CDialog::OnOK();
}

void CGateDlg::MaskForm()
{
	m_cPointCombo.SetCurSel(m_cGate.GetPointID());

	BOOL bGPS=true;
	bool bPerpCheck=false;
	EGateType eGateType=(EGateType)m_cTypeCombo.GetCurSel();
	if( m_eGateDlgType==eStart )
		{
		bPerpCheck=m_cPerpCheck.GetCheck()==BST_CHECKED;
		}

	m_cHeadingText.EnableWindow(!bPerpCheck && eGateType==eLine);
	m_cPerpCheck.EnableWindow(m_eGateDlgType==CGateDlg::eStart && eGateType==eLine);
	m_cHeadingEdit.EnableWindow(!bPerpCheck && eGateType==eLine );


	m_cHeightEdit.EnableWindow(bGPS );
	m_cHeightText.EnableWindow(bGPS );
	m_cBaseEdit.EnableWindow(bGPS && eGateType==eCylinder );
	m_cBaseText.EnableWindow(bGPS && eGateType==eCylinder);

	m_cTypeCombo.EnableWindow(bGPS);
	m_cTypeText.EnableWindow(bGPS);

	m_cRadiusText.EnableWindow(bGPS);
	m_cRadiusEdit.EnableWindow(bGPS);

	if( eGateType==eLine ) 
		{
		m_cRadiusText.SetWindowText(_T("Width"));
		}
	else
		{
		m_cRadiusText.SetWindowText(_T("Radius"));
		}

}

void CGateDlg::OnSelchangeTypecombo() 
{
	MaskForm();
}



void CGateDlg::OnBnClickedPerpCheck()
{
	MaskForm();
}
