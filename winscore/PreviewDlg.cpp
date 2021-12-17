//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "PreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int iDefault=0;

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog


CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreviewDlg)
	m_iRadio = 0;
	//}}AFX_DATA_INIT
}


void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewDlg)
	DDX_Radio(pDX, IDC_RADIO, m_iRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CPreviewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg message handlers

void CPreviewDlg::OnOK() 
{
	UpdateData(TRUE);
    m_eScoresheetStatus=(EScoresheetStatus)m_iRadio;
	iDefault=m_iRadio;
	CDialog::OnOK();
}


BOOL CPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_iRadio=iDefault;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

