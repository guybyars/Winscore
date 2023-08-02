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


CGateDlg::CGateDlg(CWinscoreDoc *pDoc, EGateDlgType  eGateDlgType, CGate &cGate, CTask*	pTask, CWnd* pParent /*=NULL*/)
	: CDialog(CGateDlg::IDD, pParent),
	m_cGate(cGate),
	m_eGateDlgType(eGateDlgType),
	m_pDoc(pDoc),
	m_pTask(pTask)
	, m_dMaxSpeed(86.839069079077575) //100mph
	, m_dMaxGroundSpeed(5000)
	, m_iPreStartAltitude(5000)
	, m_iMaxAltitude(5000)
	, m_iPEVWaitTime(0)
	, m_iPEVStartWindow(0)
{
	//{{AFX_DATA_INIT(CGateDlg)
	m_iBase = 0;
	m_iHeading = 0;
	m_iHeight = 0;
	m_dRadius = 0.0;
	m_dMaxSpeed = 0.0;
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
	DDX_Control(pDX, IDC_MAX_GROUND_SPEED, m_cPreStartAltitudeCheckBox);
	DDX_Control(pDX, IDC_MAX_GROUND_SPEED_TEXT, m_cMaxGroundSpeedText);
	DDX_Control(pDX, IDC_GROUND_SPEED_UNITS, m_cMaxGroundSpeedTextUnits);
	DDX_Control(pDX, IDC_MAX_SPEED, m_cSpeedEdit);
	DDX_Text(pDX, IDC_MAX_SPEED, m_dMaxGroundSpeed);
	DDX_Control(pDX, IDC_PRESTART_ALTITUDE, m_cPreStartAltitude);
	DDX_Text(pDX, IDC_PRESTART_ALTITUDE, m_iPreStartAltitude);
	DDX_Text(pDX, IDC_MAX_ALTITUDE, m_iMaxAltitude);
	DDX_Control(pDX, IDC_MAX_START_ALTITUDE_TEXT, m_cMaxStartAltitudeText);
	DDX_Control(pDX, IDC_MSA_UNITS, m_cMSAUnits);
	DDX_Control(pDX, IDC_PRESTART_ALTITUDE_UNITS, m_cPreStartAltitudeUnits);
	DDX_Control(pDX, IDC_MAX_ALTITUDE, m_cMaxAltitude);
	DDX_Control(pDX, IDC_STATIC_PEV_START, m_cPEVStartWindowStatic);
	DDX_Text(pDX, IDC_PEV_START_WINDOW_EDIT, m_iPEVStartWindow);
	DDX_Text(pDX, IDC_WAIT_TIME_EDIT, m_iPEVWaitTime);
	DDX_Control(pDX, IDC_WAIT_TIME_EDIT, m_cPEVWaitTime);
	DDX_Control(pDX, IDC_PEV_START_WINDOW_EDIT, m_cPEVStartWindow);
	DDX_Control(pDX, IDC_ENABLE_PEV, m_cPEVEnable);
	DDX_Control(pDX, IDC_STATIC_WAIT_TIME, m_cPEVWaitTimeStatic);
}


BEGIN_MESSAGE_MAP(CGateDlg, CDialog)
	//{{AFX_MSG_MAP(CGateDlg)
	ON_BN_CLICKED(IDC_PERP_CHECK, OnBnClickedPerpCheck)
	ON_CBN_SELCHANGE(IDC_TYPECOMBO, OnSelchangeTypecombo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MAX_GROUND_SPEED, &CGateDlg::OnBnClickedMaxGroundSpeed)
	ON_BN_CLICKED(IDC_ENABLE_PEV, &CGateDlg::OnBnClickedEnablePev)
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
	m_strUnitsSpeedText=m_pDoc->GetUnitsSpeedText();

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

	if( m_eGateDlgType==eStart && m_pTask->IsFAITask() )
		{
		m_cPreStartAltitudeCheckBox.SetCheck(m_cGate.IsPreStartAltitude());
		m_dMaxGroundSpeed= ConvertDistance(m_cGate.GetMaxSpeed() ,SYSTEMUNITS, m_eUnits);
		m_iPreStartAltitude=m_cGate.GetPreStartAltitude();
		m_iMaxAltitude=m_cGate.GetMaxAltitude();
		}
	else
		{
		m_cPreStartAltitudeCheckBox.SetCheck(false);
		m_dMaxGroundSpeed= 0.0;
		m_iPreStartAltitude=0;
		m_iMaxAltitude=0;
		}

	m_iPEVStartWindow=m_cGate.GetPEVStartWindow();
	m_iPEVWaitTime=m_cGate.GetPEVWaitTime();

	bool bPEV=m_cGate.IsPEVStart();;
	m_cPEVEnable.SetCheck(bPEV);


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
	
	if( m_eGateDlgType==eStart && m_pTask->IsFAITask() )
		{
		m_cGate.SetMaxSpeed( ConvertDistance(m_dMaxGroundSpeed, m_eUnits, SYSTEMUNITS) );
		m_cGate.SetIsPreStartAltitude(m_cPreStartAltitudeCheckBox.GetCheck()==BST_CHECKED);
		m_cGate.SetPreStartAltitude(m_iPreStartAltitude);
		m_cGate.SetMaxAltitude(m_iMaxAltitude);
		}

	if( m_cPEVEnable.GetCheck()==BST_CHECKED) 
		{
		m_cGate.SetPEVStart(true);
		if( m_iPEVWaitTime<5 || m_iPEVWaitTime>10 )
			{
			AfxMessageBox(_T("Warning, invalid PEV wait time entered."));
			}
		if( m_iPEVStartWindow<5 || m_iPEVStartWindow>10 )
			{
			AfxMessageBox(_T("Warning, invalid PEV start window entered."));
			}
		m_cGate.SetPEVStartWindow(m_iPEVStartWindow);
		m_cGate.SetPEVWaitTime(m_iPEVWaitTime);
		}
	else
		m_cGate.SetPEVStart(false);

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
	m_cBaseEdit.EnableWindow(bGPS );
	m_cBaseText.EnableWindow(bGPS );

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

	bool bFAI = m_eGateDlgType==eStart && m_pTask->IsFAITask();

	m_cPreStartAltitudeCheckBox.EnableWindow(bFAI);
	m_cMaxGroundSpeedText.EnableWindow(bFAI);
	m_cMaxGroundSpeedTextUnits.EnableWindow(bFAI);
	m_cSpeedEdit.EnableWindow(bFAI);
	m_cMaxGroundSpeedText.EnableWindow(bFAI);
	m_cMaxGroundSpeedTextUnits.EnableWindow(bFAI);
	m_cMaxStartAltitudeText.EnableWindow(bFAI);
	m_cMaxAltitude.EnableWindow(bFAI);
	m_cMSAUnits.EnableWindow(bFAI);
	m_cPreStartAltitudeUnits.EnableWindow(bFAI);
	m_cPreStartAltitude.EnableWindow(bFAI);
	if( bFAI )
		{
		bool b=m_cPreStartAltitudeCheckBox.GetCheck()==BST_CHECKED;
		m_cPreStartAltitudeUnits.EnableWindow(b);
		m_cPreStartAltitude.EnableWindow(b);
		}
	m_cMaxGroundSpeedTextUnits.SetWindowTextA(m_strUnitsSpeedText);

	BOOL bPEV=m_cPEVEnable.GetCheck()==BST_CHECKED;
	m_cPEVWaitTime.EnableWindow(bPEV);
	m_cPEVStartWindow.EnableWindow(bPEV);
	m_cPEVStartWindowStatic.EnableWindow(bPEV);
	m_cPEVWaitTimeStatic.EnableWindow(bPEV);


}

void CGateDlg::OnSelchangeTypecombo() 
{
	MaskForm();
}



void CGateDlg::OnBnClickedPerpCheck()
{
	MaskForm();
}



void CGateDlg::OnBnClickedMaxGroundSpeed()
{
	MaskForm();
}



void CGateDlg::OnBnClickedEnablePev()
{
	MaskForm();
}
