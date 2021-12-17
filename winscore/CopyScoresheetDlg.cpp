//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// CopyScoresheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "CopyScoresheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyScoresheetDlg dialog


CCopyScoresheetDlg::CCopyScoresheetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyScoresheetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyScoresheetDlg)
	m_fSummaryCheck = FALSE;
	m_fTabCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CCopyScoresheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyScoresheetDlg)
	DDX_Control(pDX, IDC_TABCHECK, m_cTabCheck);
	DDX_Check(pDX, IDC_SUMMARYCHECK, m_fSummaryCheck);
	DDX_Check(pDX, IDC_TABCHECK, m_fTabCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyScoresheetDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyScoresheetDlg)
	ON_BN_CLICKED(IDC_TEXTRADIO, OnTextradio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyScoresheetDlg message handlers

BOOL CCopyScoresheetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyScoresheetDlg::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}

void CCopyScoresheetDlg::OnTextradio() 
{
	UpdateData(TRUE);
	m_cTabCheck.EnableWindow(TRUE);	
}

