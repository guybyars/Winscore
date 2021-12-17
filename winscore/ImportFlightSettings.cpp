//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ImportFlightSettings.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ImportFlightSettings.h"
#include "dirdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportFlightSettings dialog


CImportFlightSettings::CImportFlightSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CImportFlightSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportFlightSettings)
	m_strExePath = _T("");
	m_strLogPath = _T("");
	m_strUploadPath = _T("");
	//}}AFX_DATA_INIT
}


void CImportFlightSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportFlightSettings)
	DDX_Control(pDX, IDC_COMBO1, m_cPortCombo);
	DDX_Text(pDX, IDC_EXEPATH, m_strExePath);
	DDX_Text(pDX, IDC_LOGPATH, m_strLogPath);
	DDX_Text(pDX, IDC_UPLOADPATH, m_strUploadPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportFlightSettings, CDialog)
	//{{AFX_MSG_MAP(CImportFlightSettings)
	ON_BN_CLICKED(IDC_BROWSE_EXE, OnBrowseExe)
	ON_BN_CLICKED(IDC_BROWSE_LOG_PATH, OnBrowseLogPath)
	ON_BN_CLICKED(IDC_BROWSE_UPLOAD, OnBrowseUpload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportFlightSettings message handlers

void CImportFlightSettings::OnOK() 
{
	m_iPortID=m_cPortCombo.GetCurSel()+1;
	
	CDialog::OnOK();
}

BOOL CImportFlightSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cPortCombo.SetCurSel(m_iPortID-1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportFlightSettings::OnBrowseExe() 
{
	CDirDialog cdlg;
	cdlg.m_strSelDir=m_strExePath;
	cdlg.m_strTitle="Path for FDR transfer programs";

	if( cdlg.DoBrowse() ==TRUE  ) 
		{
		m_strExePath=cdlg.m_strPath;
		UpdateData(FALSE);
		}
	
}

void CImportFlightSettings::OnBrowseLogPath() 
{
	CDirDialog cdlg;
	cdlg.m_strSelDir=m_strLogPath;
	cdlg.m_strTitle="Path for flight logs";

	if( cdlg.DoBrowse() ==TRUE  ) 
		{
		m_strLogPath=cdlg.m_strPath;
		UpdateData(FALSE);
		}	
}

void CImportFlightSettings::OnBrowseUpload() 
{
	CDirDialog cdlg;
	cdlg.m_strSelDir=m_strUploadPath;
	cdlg.m_strTitle="Temporary directory used for transfering flght logs";
	if( cdlg.DoBrowse() ==TRUE  ) 
		{
		m_strUploadPath=cdlg.m_strPath;
		UpdateData(FALSE);
		}	
}


