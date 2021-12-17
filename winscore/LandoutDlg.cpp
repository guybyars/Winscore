//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// CLandoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "LandoutDlg.h"
#include "Turnpoint.h"
#include "Location.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	ELatDirection eLatDirection=eNORTH;
static 	ELongDirection eLongDirection=eWEST;
static  ECoordFormat  eCoordFormat=eHHMMSS;


/////////////////////////////////////////////////////////////////////////////
// CLandoutDlg dialog


CLandoutDlg::CLandoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLandoutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLandoutDlg)
	m_iLatDegDlg = 0;
	m_dLatMinDlg = 0.0;
	m_iLatSecDlg = 0;
	m_iLongDegDlg = 0;
	m_dLongMinDlg = 0.0;
	m_iLongSecDlg = 0;
	//}}AFX_DATA_INIT
}


void CLandoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLandoutDlg)
	DDX_Control(pDX, IDC_WEST, m_cWestRadio);
	DDX_Control(pDX, IDC_SOUTH, m_cSouthRadio);
	DDX_Control(pDX, IDC_NORTH, m_cNorthRadio);
	DDX_Control(pDX, IDC_LONGSEC_TEXT, m_cLongSecText);
	DDX_Control(pDX, IDC_LONGMIN_TEXT, m_cLongMinText);
	DDX_Control(pDX, IDC_LONGDEG_TEXT, m_cLongDegText);
	DDX_Control(pDX, IDC_LONG_TEXT, m_cLongText);
	DDX_Control(pDX, IDC_LONG_SEC, m_cLongSec);
	DDX_Control(pDX, IDC_LONG_MIN, m_cLongMin);
	DDX_Control(pDX, IDC_LONG_DEG, m_cLongDeg);
	DDX_Control(pDX, IDC_LATSEC_TEXT, m_cLatSecText);
	DDX_Control(pDX, IDC_LATMIN_TEXT, m_cLatMinText);
	DDX_Control(pDX, IDC_FORMAT_TEXT, m_cFormatText);
	DDX_Control(pDX, IDC_LATDEG_TEXT, m_cLatDegText);
	DDX_Control(pDX, IDC_LAT_TEXT, m_cLatText);
	DDX_Control(pDX, IDC_LAT_SEC, m_cLatSec);
	DDX_Control(pDX, IDC_LAT_MIN, m_cLatMin);
	DDX_Control(pDX, IDC_LAT_DEG, m_cLatDegrees);
	DDX_Control(pDX, IDC_HHMMSS, m_cHHMMSS);
	DDX_Control(pDX, IDC_HHMMMM, m_cHHMMMM);
	DDX_Control(pDX, IDC_EAST, m_cEastRadio);
	DDX_Text(pDX, IDC_LAT_DEG, m_iLatDegDlg);
	DDV_MinMaxInt(pDX, m_iLatDegDlg, 0, 59);
	DDX_Text(pDX, IDC_LAT_MIN, m_dLatMinDlg);
	DDV_MinMaxDouble(pDX, m_dLatMinDlg, 0., 59.9999);
	DDX_Text(pDX, IDC_LAT_SEC, m_iLatSecDlg);
	DDV_MinMaxInt(pDX, m_iLatSecDlg, 0, 59);
	DDX_Text(pDX, IDC_LONG_DEG, m_iLongDegDlg);
	DDV_MinMaxInt(pDX, m_iLongDegDlg, 0, 180);
	DDX_Text(pDX, IDC_LONG_MIN, m_dLongMinDlg);
	DDV_MinMaxDouble(pDX, m_dLongMinDlg, 0., 59.9999);
	DDX_Text(pDX, IDC_LONG_SEC, m_iLongSecDlg);
	DDV_MinMaxInt(pDX, m_iLongSecDlg, 0, 59);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLandoutDlg, CDialog)
	//{{AFX_MSG_MAP(CLandoutDlg)
	ON_BN_CLICKED(IDC_EAST, OnEast)
	ON_BN_CLICKED(IDC_HHMMMM, OnHhmmmm)
	ON_BN_CLICKED(IDC_HHMMSS, OnHhmmss)
	ON_BN_CLICKED(IDC_NORTH, OnNorth)
	ON_BN_CLICKED(IDC_SOUTH, OnSouth)
	ON_BN_CLICKED(IDC_WEST, OnWest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLandoutDlg message handlers

void CLandoutDlg::OnHhmmss() 
{
		eCoordFormat=eHHMMSS;
		UpdateHMSButtons();
}
void CLandoutDlg::OnHhmmmm() 
{
		eCoordFormat=eHHMMMM;
		UpdateHMSButtons();
}

void CLandoutDlg::OnWest() 
{
	eLongDirection=eWEST;
	UpdateDirectionButtons();
}

void CLandoutDlg::OnSouth() 
{
    eLatDirection=eSOUTH;
	UpdateDirectionButtons();
}

void CLandoutDlg::OnNorth() 
{
    eLatDirection=eNORTH;
    UpdateDirectionButtons();
}

void CLandoutDlg::OnEast() 
{
    eLongDirection=eEAST;
    UpdateDirectionButtons();
}


void CLandoutDlg::UpdateDirectionButtons()
{
	if( eLatDirection==eNORTH )
		{
	    m_cNorthRadio.SetCheck(1);
		m_cSouthRadio.SetCheck(0);
		}
	else
		{	    
		m_cNorthRadio.SetCheck(0);
		m_cSouthRadio.SetCheck(1);
		}

	if( eLongDirection==eEAST )
		{
	    m_cEastRadio.SetCheck(1);
		m_cWestRadio.SetCheck(0);
		}
	else
		{	    
		m_cEastRadio.SetCheck(0);
		m_cWestRadio.SetCheck(1);
		}
}

void CLandoutDlg::UpdateHMSButtons() 
		{
		UpdateData();	
		if( eCoordFormat==eHHMMSS )
			{
			m_cHHMMSS.SetCheck(1);
			m_cHHMMMM.SetCheck(0);
			m_cLatSec.EnableWindow(TRUE);
			m_cLongSec.EnableWindow(TRUE);
			m_cLatSecText.EnableWindow(TRUE);
			m_cLongSecText.EnableWindow(TRUE);
			modf( m_dLongMinDlg, &m_dLongMinDlg );
			modf( m_dLatMinDlg, &m_dLatMinDlg );
			}
		else
			{
			m_dLongMinDlg+=m_iLongSecDlg/60.;
			m_dLatMinDlg+=m_iLatSecDlg/60.;
			m_cHHMMSS.SetCheck(0);
			m_cHHMMMM.SetCheck(1);
			m_cLatSec.EnableWindow(FALSE);
			m_cLongSec.EnableWindow(FALSE);
			m_cLatSecText.EnableWindow(FALSE);
			m_cLongSecText.EnableWindow(FALSE);
			}
		UpdateData(FALSE);	
		}

BOOL CLandoutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	eCoordFormat=m_pDoc->m_eCoordinateFormat;

	UpdateDirectionButtons();
	UpdateHMSButtons();

	m_iLatDegDlg=m_cLocation.m_iLatDegrees;
	m_iLatSecDlg=m_cLocation.GetLatSeconds();

	m_iLongDegDlg=m_cLocation.m_iLongDegrees;
	m_iLongSecDlg=m_cLocation.GetLongSeconds();

	eLatDirection=m_cLocation.m_eLatDirection;
	eLongDirection=m_cLocation.m_eLongDirection;


	if( eCoordFormat==eHHMMMM )
		{
		m_dLongMinDlg=m_cLocation.m_dLongMinutes;
		m_dLatMinDlg=m_cLocation.m_dLatMinutes;
		}
	else
		{
		m_dLongMinDlg=m_cLocation.GetLongMinutes();
		m_dLatMinDlg=m_cLocation.GetLatMinutes();
		}

	UpdateData(FALSE);

	return TRUE;  
}


void CLandoutDlg::OnOK() 
{

	UpdateData();

	CLocation* pcLocation;

		if(  eCoordFormat==eHHMMSS )
			{
			 pcLocation=new CLocation(m_iLatDegDlg, m_dLatMinDlg, m_iLatSecDlg,
									m_iLongDegDlg, m_dLongMinDlg, m_iLongSecDlg,
									eLatDirection, eLongDirection);
			}
		else
			{	
			pcLocation =new CLocation(m_iLatDegDlg, m_dLatMinDlg,
										m_iLongDegDlg, m_dLongMinDlg,
										eLatDirection, eLongDirection);
			}
		m_cLocation=*pcLocation;
		delete pcLocation;


	CDialog::OnOK();
}

