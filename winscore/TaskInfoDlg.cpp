//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "resizingdialog.h"
#include "TaskInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskInfoDlg dialog


CTaskInfoDlg::CTaskInfoDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CTaskInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaskInfoDlg)
	m_strTaskText = _T("");
	//}}AFX_DATA_INIT

    SetControlInfo(IDOK,               ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_RESET,          ANCHORE_BOTTOM);
    SetControlInfo(IDC_EDIT,    RESIZE_HOR | RESIZE_VER);

	m_strTaskText=ResetText();
}


void CTaskInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskInfoDlg)
	DDX_Control(pDX, IDC_EDIT, m_cEdit);
	DDX_Text(pDX, IDC_EDIT, m_strTaskText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskInfoDlg)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskInfoDlg message handlers

void CTaskInfoDlg::OnReset() 
{
	m_strTaskText=ResetText();
	UpdateData(FALSE);
}

void CTaskInfoDlg::OnOK() 
{
	UpdateData(TRUE);


	CDialog::OnOK();
}

void CTaskInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
		
}

void CTaskInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();
	
}

BOOL CTaskInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( m_strTaskText.GetLength()==0 ) 	m_strTaskText=ResetText();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CTaskInfoDlg::ResetText()
{
	CString strTaskText ="Remarks:\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="Start Direction: _______  Finish Direction _______ :\r\n";
	strTaskText+="\r\n";
	strTaskText+="Grid Time: ________  Launch Time: ________  Sunset: ________\r\n";
	strTaskText+="\r\n";
	strTaskText+="Weather Synopsis:\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="Visibility:_________________________   Clouds:________________________\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="                                                  Winds Aloft\r\n";
	strTaskText+="\r\n";
	strTaskText+="  Time                 3K                  6K                  9K                 12K             18K\r\n";
	strTaskText+="\r\n";
	strTaskText+="  12:00 _________________________________________________________\r\n";
	strTaskText+="\r\n";
	strTaskText+="  18:00 _________________________________________________________\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	strTaskText+="\r\n";
	return strTaskText;
}

