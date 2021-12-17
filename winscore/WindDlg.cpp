//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "WindDlg.h"
#include "Global_Prototypes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindDlg dialog


CWindDlg::CWindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindDlg)
	m_dDirection = 0.0;
	m_dSpeed = 0.0;
	m_cSpeedText = _T("");
	//}}AFX_DATA_INIT
}


void CWindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindDlg)
	DDX_Text(pDX, IDC_DIRECTION, m_dDirection);
	DDV_MinMaxDouble(pDX, m_dDirection, 0., 359.);
	DDX_Text(pDX, IDC_SPEED, m_dSpeed);
	DDV_MinMaxDouble(pDX, m_dSpeed, 0., 100.);
	DDX_Text(pDX, IDC_SPEED_TEXT, m_cSpeedText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindDlg, CDialog)
	//{{AFX_MSG_MAP(CWindDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindDlg message handlers

void CWindDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( UpdateData()==0 ) return;
	CDialog::OnOK();
}

BOOL CWindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cSpeedText=UnitsText(m_eUnits)+"/Hour";
	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

