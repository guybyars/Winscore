// ScoreReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ScoreReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScoreReportDlg dialog


CScoreReportDlg::CScoreReportDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CScoreReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScoreReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   SetControlInfo(IDC_EDIT,    RESIZE_HOR | RESIZE_VER);
   SetControlInfo(IDOK,		   ANCHORE_BOTTOM);
   SetControlInfo(IDCANCEL,    ANCHORE_BOTTOM | ANCHORE_RIGHT);
}


void CScoreReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScoreReportDlg)
	DDX_Control(pDX, IDC_EDIT, m_cEditCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScoreReportDlg, CDialog)
	//{{AFX_MSG_MAP(CScoreReportDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScoreReportDlg message handlers

void CScoreReportDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CResizingDialog::OnSize(nType, cx, cy);	// TODO: Add your message handler code here
	
}

void CScoreReportDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();

}

BOOL CScoreReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cEditCtrl.SetWindowText(m_strText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScoreReportDlg::OnClose() 
{
	CDialog::OnClose();
}

void CScoreReportDlg::OnOK() 
{
	m_cEditCtrl.GetWindowText(m_strText);	
	CDialog::OnOK();
}
