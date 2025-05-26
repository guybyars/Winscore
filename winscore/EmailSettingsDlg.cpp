// EmailSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "EmailSettingsDlg.h"
#include "afxdialogex.h"
#include "AutoScoreDlg.h"


// CEmailSettingsDlg dialog

IMPLEMENT_DYNAMIC(CEmailSettingsDlg, CDialogEx)

CEmailSettingsDlg::CEmailSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmailSettingsDlg::IDD, pParent)
	, m_strServer(_T(""))
	, m_strUsername(_T(""))
	, m_strPassword(_T(""))
	, m_strMailbox(_T(""))
	, m_iPortID(0)
	, m_bUseSSL(FALSE)
{

}

CEmailSettingsDlg::~CEmailSettingsDlg()
{
}

void CEmailSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVER_EDIT, m_strServer);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_strUsername);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	DDX_Text(pDX, IDC_MAILBOX_EDIT, m_strMailbox);
	DDX_Text(pDX, IDC_PORT_EDIT, m_iPortID);
	DDX_Check(pDX, IDC_SSL_CHECK, m_bUseSSL);
}


BEGIN_MESSAGE_MAP(CEmailSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEmailSettingsDlg::OnTest)
	ON_BN_CLICKED(IDOK, &CEmailSettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEmailSettingsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEmailSettingsDlg message handlers


void CEmailSettingsDlg::OnTest()
{
	UpdateData();
	CTime cTime;
	CString strError;
	int iStatus= ((CAutoScoreDlg*)m_pParentDlg)->ScanEmailForLogs( cTime, 1, m_strServer, m_strUsername, m_strPassword, m_strMailbox, m_iPortID, m_bUseSSL, "", strError, true );
	if( iStatus<0 ) 
		{
		AfxMessageBox("The email server returned the following error:\n\n"+strError);
		}
	else
		AfxMessageBox("Email settings are OK.",MB_ICONINFORMATION );

}


void CEmailSettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CEmailSettingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

BOOL CEmailSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}