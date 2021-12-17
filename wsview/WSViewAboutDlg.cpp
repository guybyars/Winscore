//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSViewAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wsview.h"
#include "WSViewAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWSViewAboutDlg dialog


CWSViewAboutDlg::CWSViewAboutDlg(CString &strVersion, CWnd* pParent /*=NULL*/)
	: CDialog(CWSViewAboutDlg::IDD, pParent)
{
	m_strVersion=strVersion;
	//{{AFX_DATA_INIT(CWSViewAboutDlg)
	m_strVersionText = _T("");
	//}}AFX_DATA_INIT
}


void CWSViewAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWSViewAboutDlg)
	DDX_Text(pDX, IDC_VERSION_TEXT, m_strVersionText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWSViewAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CWSViewAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWSViewAboutDlg message handlers

BOOL CWSViewAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(true);

	m_strVersionText+=m_strVersion;

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

