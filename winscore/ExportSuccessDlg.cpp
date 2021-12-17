//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ExportSuccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "ExportSuccessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg dialog


CExportSuccessDlg::CExportSuccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportSuccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportSuccessDlg)
	m_cText =_T(" Your scoresheet was successfully exported.  Please share your\n good work with others on the Internet by emailing it to the SSA at: \n\n scores@ssa.org \n\n Be advised that if your contest is sanctioned, then this is a\n requirement.");
	//}}AFX_DATA_INIT
}


void CExportSuccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportSuccessDlg)
	DDX_Text(pDX, IDC_STATIC_TEXT, m_cText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportSuccessDlg, CDialog)
	//{{AFX_MSG_MAP(CExportSuccessDlg)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg message handlers

void CExportSuccessDlg::OnHelpButton() 
{
	// TODO: Add your control notification handler code here
	
}

