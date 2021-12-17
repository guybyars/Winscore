//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRSettings.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "FDRSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFDRSettings dialog


CFDRSettings::CFDRSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CFDRSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFDRSettings)
	m_dFinishRadius = 0.0;
	m_dStartRadius = 0.0;
	m_iFinishBase = 0;
	m_iStartCeiling = 0;
	m_iStartHeading = 0;
	m_iFinishHeading = 0;
	m_strFinishRadiusText = _T("");
	m_strStartRadiusText = _T("");
	//}}AFX_DATA_INIT
}


void CFDRSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFDRSettings)
	DDX_Control(pDX, IDC_STARTCOMBO, m_cStartTypeCombo);
	DDX_Control(pDX, IDC_STARTHEADINGTEXT, m_cStartHeadingText);
	DDX_Control(pDX, IDC_START_HEADING_EDIT, m_cStartHeadingEdit);
	DDX_Control(pDX, IDC_HEADING_EDIT, m_cHeadingEdit);
	DDX_Control(pDX, IDC_COMBO1, m_cTypeCombo);
	DDX_Control(pDX, IDC_HEADINGTEXT, m_strHeadingText);
	DDX_Text(pDX, IDC_FINISH, m_dFinishRadius);
	DDV_MinMaxDouble(pDX, m_dFinishRadius, 0.25, 100.);
	DDX_Text(pDX, IDC_START, m_dStartRadius);
	DDV_MinMaxDouble(pDX, m_dStartRadius, 0.25, 100.);
	DDX_Text(pDX, IDC_FINISH_BASE, m_iFinishBase);
	DDX_Text(pDX, IDC_START_HEIGHT, m_iStartCeiling);
	DDX_Text(pDX, IDC_START_HEADING_EDIT, m_iStartHeading);
	DDV_MinMaxInt(pDX, m_iStartHeading, 0, 360);
	DDX_Text(pDX, IDC_HEADING_EDIT, m_iFinishHeading);
	DDV_MinMaxInt(pDX, m_iFinishHeading, 0, 360);
	DDX_Text(pDX, IDC_FINISH_RADIUS_TEXT, m_strFinishRadiusText);
	DDX_Text(pDX, IDC_START_RADIUS_TEXT, m_strStartRadiusText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFDRSettings, CDialog)
	//{{AFX_MSG_MAP(CFDRSettings)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_STARTCOMBO, OnSelchangeStartcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDRSettings message handlers

BOOL CFDRSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cTypeCombo.SetCurSel((int)m_eFinishGateType);
	m_cHeadingEdit.EnableWindow( m_eFinishGateType==eLine );
	m_strHeadingText.EnableWindow( m_eFinishGateType==eLine );

	m_cStartTypeCombo.SetCurSel((int)m_eStartGateType);
	m_cStartHeadingEdit.EnableWindow( m_eStartGateType==eLine );
	m_cStartHeadingText.EnableWindow( m_eStartGateType==eLine );
	
	(m_eFinishGateType==eLine)?(m_strFinishRadiusText="Gate Width:"):(m_strFinishRadiusText="Radius:");
	(m_eStartGateType==eLine)?(m_strStartRadiusText="Gate Width:"):(m_strStartRadiusText="Radius:");
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFDRSettings::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

	m_eFinishGateType=(EGateType)m_cTypeCombo.GetCurSel();
	m_eStartGateType=(EGateType)m_cStartTypeCombo.GetCurSel();

}

void CFDRSettings::OnSelchangeCombo1() 
{
	UpdateData(true);
	m_eFinishGateType=(EGateType)m_cTypeCombo.GetCurSel();
	m_cHeadingEdit.EnableWindow( m_eFinishGateType==eLine );
	m_strHeadingText.EnableWindow( m_eFinishGateType==eLine );
	(m_eFinishGateType==eLine)?(m_strFinishRadiusText="Gate Width:"):(m_strFinishRadiusText="Radius:");
	UpdateData(false);
	
}

void CFDRSettings::OnSelchangeStartcombo() 
{
	UpdateData(true);
	m_eStartGateType=(EGateType)m_cStartTypeCombo.GetCurSel();
	m_cStartHeadingEdit.EnableWindow( m_eStartGateType==eLine );
	m_cStartHeadingText.EnableWindow( m_eStartGateType==eLine );
	(m_eStartGateType==eLine)?(m_strStartRadiusText="Gate Width:"):(m_strStartRadiusText="Radius:");
	UpdateData(false);

}

