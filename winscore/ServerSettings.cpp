// ServerSettings.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ServerSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSettings dialog


CServerSettings::CServerSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerSettings)
	m_strRootDir = _T("");
	m_strPassword = _T("");
	m_strServerName = _T("");
	m_strUsername = _T("");
	m_bCheck = FALSE;
	m_bPassiveMode = FALSE;
	//}}AFX_DATA_INIT
}


void CServerSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerSettings)
	DDX_Control(pDX, IDC_PASSIVE_CHECK, m_cPassiveCheck);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_cUsername);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_cServer);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_cPassword);
	DDX_Control(pDX, IDC_EDIT_DIR, m_cRootDir);
	DDX_Control(pDX, IDC_STATIC4, m_cStatic4);
	DDX_Control(pDX, IDC_STATIC3, m_cStatic3);
	DDX_Control(pDX, IDC_STATIC2, m_cStatic2);
	DDX_Control(pDX, IDC_STATIC1, m_cStatic1);
	DDX_Control(pDX, IDC_CHECK1, m_cCheckBox);
	DDX_Text(pDX, IDC_EDIT_DIR, m_strRootDir);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServerName);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Check(pDX, IDC_CHECK1, m_bCheck);
	DDX_Check(pDX, IDC_PASSIVE_CHECK, m_bPassiveMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerSettings, CDialog)
	//{{AFX_MSG_MAP(CServerSettings)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSettings message handlers

BOOL CServerSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cCheckBox.SetWindowText(SERVER);

	bool bMyServer=m_strServerName==SERVER;

	m_cCheckBox.SetCheck(bMyServer);


	if(bMyServer)
		{
		MasqueForm(false);

		}
	else
		MasqueForm(true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerSettings::OnCheck() 
{
	if(m_cCheckBox.GetCheck() )
		{
		MasqueForm(false);
		}
	else
		{
		MasqueForm(true);
		m_strServerName="";
		m_strUsername="";
		m_strPassword="";
		m_strRootDir="";
		UpdateData(false);
		}
	
}

void CServerSettings::MasqueForm(bool bMasque)
	{
	m_cStatic1.EnableWindow(bMasque);
	m_cStatic2.EnableWindow(bMasque);
	m_cStatic3.EnableWindow(bMasque);
	m_cStatic4.EnableWindow(bMasque);

	m_cServer.EnableWindow(bMasque);
	m_cUsername.EnableWindow(bMasque);
	m_cPassword.EnableWindow(bMasque);
	m_cRootDir.EnableWindow(bMasque);
	//m_cPassiveCheck.EnableWindow(bMasque);
	}


void CServerSettings::OnOK() 
{
	m_bPassiveMode=m_cPassiveCheck.GetCheck();
	CDialog::OnOK();
}

void CServerSettings::OnCancel() 
{
	
	CDialog::OnCancel();
}
