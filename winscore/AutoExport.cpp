// AutoExport.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "AutoExport.h"
#include "DirDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoExport dialog


CAutoExport::CAutoExport(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoExport)
	//}}AFX_DATA_INIT
}


void CAutoExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoExport)
	DDX_Control(pDX, IDC_CHECK1, m_cCheckControl);
	DDX_Control(pDX, IDC_PATH, m_cPathTextControl);
	DDX_Control(pDX, IDC_BUTTON1, m_cButtonControl);
	DDX_Control(pDX, IDC_EDIT, m_cEditControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoExport, CDialog)
	//{{AFX_MSG_MAP(CAutoExport)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoExport message handlers

void CAutoExport::OnCheck() 
	{
		
	if( m_cCheckControl.GetCheck() ) 
		{	
		m_cPathTextControl.EnableWindow(true);
		m_cButtonControl.EnableWindow(true);
		m_cEditControl.EnableWindow(true);
		}
	else
		{
		m_cPathTextControl.EnableWindow(false);
		m_cButtonControl.EnableWindow(false);
		m_cEditControl.EnableWindow(false);
		}
	}

void CAutoExport::OnOK() 
{
	if( m_cCheckControl.GetCheck() ) 
		{
		CString cText;
		m_cEditControl.GetWindowText(cText);
		
//		CFileStatus rStatus;
//		if( !CFile::GetStatus(cText, rStatus))
//			{
//			AfxMessageBox("Invalid path : " + cText, MB_ICONSTOP);
//			return;
//			}
		AfxGetApp()->WriteProfileString( REGSECTION, AUTOEXPORTPATH, cText);
		AfxGetApp()->WriteProfileInt(REGSECTION, AUTOEXPORTENABLEDFLAG,	1 );
		}
	else
		{
		AfxGetApp()->WriteProfileInt(REGSECTION, AUTOEXPORTENABLEDFLAG,	0 );
		}
	CDialog::OnOK();
}

BOOL CAutoExport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int iFlag;
	iFlag=AfxGetApp()->GetProfileInt(REGSECTION, AUTOEXPORTENABLEDFLAG,	0 );
	CString cText=AfxGetApp()->GetProfileString( REGSECTION, AUTOEXPORTPATH, _T("") );
	m_cEditControl.SetWindowText(cText);
	m_cCheckControl.SetCheck(iFlag);
	if( iFlag==1 ) 
		{
		m_cPathTextControl.EnableWindow(true);
		m_cButtonControl.EnableWindow(true);
		m_cEditControl.EnableWindow(true);
		}
	else
		{
		m_cPathTextControl.EnableWindow(false);
		m_cButtonControl.EnableWindow(false);
		m_cEditControl.EnableWindow(false);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoExport::OnButton() 
{
	CString cText;
	m_cEditControl.GetWindowText(cText);
	CDirDialog cdlg;
	cdlg.m_strSelDir=cText;
	cdlg.m_strTitle=_T("Path for Auto Export");

	if( cdlg.DoBrowse() ==TRUE  ) 
		{
		m_cEditControl.SetWindowText(cdlg.m_strPath);
		}		
	
}
