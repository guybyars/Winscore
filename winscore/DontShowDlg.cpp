// DontShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "DontShowDlg.h"
//#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDontShowDlg dialog


CDontShowDlg::CDontShowDlg(	CString		strRegIdent,
							CString		strMessage,
							CString		strDlgTitle,
							LPCTSTR 	strIcon,
							CWnd* pParent)
	: CDialog(CDontShowDlg::IDD, pParent),
	m_strDlgTitle(strDlgTitle),
	m_strRegIdent(strRegIdent),
	m_strIcon(strIcon)
{
	//{{AFX_DATA_INIT(CDontShowDlg)
	m_bDontShowAgainCheck = FALSE;
	m_strMessageText = strMessage;
	//}}AFX_DATA_INIT
}


void CDontShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDontShowDlg)
	DDX_Control(pDX, IDC_DLG_ICON, m_cDlgIcon);
	DDX_Check(pDX, IDC_CHECK1, m_bDontShowAgainCheck);
	DDX_Text(pDX, IDC_TEXT, m_strMessageText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDontShowDlg, CDialog)
	//{{AFX_MSG_MAP(CDontShowDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDontShowDlg message handlers

BOOL CDontShowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_strDlgTitle);

	HICON hIcon=LoadIcon(NULL, m_strIcon );

	m_cDlgIcon.SetIcon( hIcon );


	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDontShowDlg::DoModalConditional()
	{
	if( AfxGetApp()->GetProfileInt( DIALOGDISPLAY, m_strRegIdent, 0)==0)
		{
		DoModal();
		}
	}

void CDontShowDlg::OnOK() 
{
	UpdateData(true);

	if( m_bDontShowAgainCheck )
		{
		AfxGetApp()->WriteProfileInt( DIALOGDISPLAY, m_strRegIdent, 1);
		}

	CDialog::OnOK();
}

