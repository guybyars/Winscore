//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// DescriptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "DescriptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDescriptionDlg dialog


CDescriptionDlg::CDescriptionDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CDescriptionDlg::IDD, pParent)
{
	m_bNoOKButton=false;

	//{{AFX_DATA_INIT(CDescriptionDlg)
	m_Text = _T("");
	m_cInfoBar = _T("");
	//}}AFX_DATA_INIT
	m_cTitle = _T("Description");

    SetControlInfo(IDC_EDIT1,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDOK,               ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,           ANCHORE_BOTTOM);

}


void CDescriptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDescriptionDlg)
	DDX_Control(pDX, IDOK, m_cOKButton);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Text(pDX, IDC_INFO_BAR, m_cInfoBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDescriptionDlg, CDialog)
	//{{AFX_MSG_MAP(CDescriptionDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDescriptionDlg message handlers

BOOL CDescriptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_cTitle);	

	if( m_bNoOKButton )
		m_cOKButton.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDescriptionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

void CDescriptionDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
