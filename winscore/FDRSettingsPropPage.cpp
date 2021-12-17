//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRSettingsPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "FDRSettingsPropPage.h"
#include "dirdialog.h"
#include "suautil.h"
#include "FlightSearchFoldersDlg.h"
#include "global_prototypes.h"
#include "IGCDirectories.h"
#include "WinscoreDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropPage property page

IMPLEMENT_DYNCREATE(CFDRSettingsPropPage, CPropertyPage)

CFDRSettingsPropPage::CFDRSettingsPropPage() : CPropertyPage(CFDRSettingsPropPage::IDD)
{
	//{{AFX_DATA_INIT(CFDRSettingsPropPage)
	m_bSUACheck = FALSE;
	m_strSUAFile = _T("");
	m_strSUAFile2 = _T("");
	m_bSUAIgnore = TRUE;
	m_pDoc=NULL;
	//}}AFX_DATA_INIT
}

CFDRSettingsPropPage::~CFDRSettingsPropPage()
{
}

void CFDRSettingsPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFDRSettingsPropPage)
	DDX_Control(pDX, IDC_SUACHECK2, m_cSUAIgnore);
	DDX_Control(pDX, IDC_SUAFILETEXT, m_cSUAFileText);
	DDX_Control(pDX, IDC_SUAFILETEXT2, m_cSUAFileText2);
	DDX_Control(pDX, IDC_SUAFILENAME, m_cSUAFileEdit);
	DDX_Control(pDX, IDC_SUAFILENAME2, m_cSUAFileEdit2);
	DDX_Control(pDX, IDC_BROWSE_SUAFILE, m_cSUAButton);
	DDX_Control(pDX, IDC_BROWSE_SUAFILE2, m_cSUAButton2);
	DDX_Control(pDX, IDC_COMBO1, m_cPortCombo);
	DDX_Check(pDX, IDC_SUACHECK, m_bSUACheck);
	DDX_Text(pDX, IDC_SUAFILENAME, m_strSUAFile);
	DDX_Text(pDX, IDC_SUAFILENAME2, m_strSUAFile2);
	DDX_Check(pDX, IDC_SUACHECK2, m_bSUAIgnore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFDRSettingsPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFDRSettingsPropPage)
	ON_BN_CLICKED(IDC_BROWSE_LOG_PATH, OnBrowseLogPath)
	ON_BN_CLICKED(IDC_BROWSE_SUAFILE, OnBrowseSuafile)
	ON_BN_CLICKED(IDC_SUACHECK, OnSuacheck)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_SUAFILENAME, &CFDRSettingsPropPage::OnEnChangeSuafilename)
	ON_BN_CLICKED(IDC_BROWSE_SUAFILE2, &CFDRSettingsPropPage::OnBnClickedBrowseSuafile2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropPage message handlers


void CFDRSettingsPropPage::OnOK() 
{
	UpdateData(true);

	m_iPortID=m_cPortCombo.GetCurSel()+1;
	
	AfxGetApp()->WriteProfileInt(	 REGSECTION, COMPORT,	 m_iPortID);
	WriteWinscoreInt(  SUA_IGNORE, m_bSUAIgnore);

	if( m_bSUACheck )
		{
		GetSUAArray()->Purge();
		CStringArray strAirspaceArray;
		int nSections=ReadSUAFile(m_strSUAFile, strAirspaceArray);
		if( nSections==0  ) 
			{
			AfxMessageBox("No Valid Airspace in : "+m_strSUAFile);
			WriteWinscoreInt(  SUADISPLAY, 0);
			}
		else
			{
			WriteWinscoreInt(  SUADISPLAY, 1);
			WriteWinscoreString( SUADISPLAYFILE,  m_strSUAFile);
			}

		CStringArray strAirspaceArray2;
		int nSections2=ReadSUAFile(m_strSUAFile2, strAirspaceArray2);
		if( nSections2==0 && m_strSUAFile2.GetLength()>0 ) 
			{
			AfxMessageBox("No Valid 2ndary Airspace in : "+m_strSUAFile2);
			}
		WriteWinscoreString( SUADISPLAYFILE2,  m_strSUAFile2);
		}
	else
		WriteWinscoreInt(  SUADISPLAY, 0);

	CPropertyPage::OnOK();
}

BOOL CFDRSettingsPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString cTemp;

	m_iPortID		=AfxGetApp()->GetProfileInt(	REGSECTION, COMPORT,  1 );
	m_strSUAFile	=GetWinscoreString( SUADISPLAYFILE,  DEFAULT_SUADISPLAYFILE);
	m_strSUAFile2	=GetWinscoreString( SUADISPLAYFILE2,  "");

	m_bSUAIgnore = GetWinscoreInt( SUA_IGNORE, DEFAULT_SUA_IGNORE);
	m_bSUACheck  = GetWinscoreInt( SUADISPLAY, DEFAULT_SUADISPLAY);
	m_cPortCombo.SetCurSel(m_iPortID-1);
	UpdateData(false);
	
	MasqueForm();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFDRSettingsPropPage::OnBrowseLogPath() 
	{
	CStringArray	strArray;

	CIGCDirectories cIGCDir;
	cIGCDir.GetFlightLogFolders( strArray );


	CFlightSearchFoldersDlg	dlg(strArray);
	dlg.m_pDoc=m_pDoc;
	if( dlg.DoModal()==IDOK )
		{
		cIGCDir.SetFlightLogFolders(strArray);
		cIGCDir.SaveFlightLogFolders();
		}
}


void CFDRSettingsPropPage::OnBrowseSuafile() 
{
	static TCHAR BASED_CODE szFilter[] = _T("SUA Files (*.sua)|*.sua|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("sua"), NULL, OFN_HIDEREADONLY, szFilter);

	CString cTrans=_T("Select Special Use Airspace File");
	cFileDlg.m_ofn.lpstrTitle=cTrans;

	if( cFileDlg.DoModal()==IDOK )
		{
		CStringArray strAirspaceArray;
		int nSections=ReadSUAFile(cFileDlg.GetPathName(), strAirspaceArray);
		if( nSections==0  ) 
			AfxMessageBox(_T("No Valid Airspace in : ")+cFileDlg.GetPathName());
		else
			{
			// If the SUA file is in, or a sub directory of the wsc file, store the relative path
			CString strPath=GetRelativePath(cFileDlg.GetPathName());
			m_cSUAFileEdit.SetWindowText(strPath);
			}
		}	
}

void CFDRSettingsPropPage::OnSuacheck() 
	{
	UpdateData(true);
	MasqueForm();	
	}

void CFDRSettingsPropPage::MasqueForm()
	{
	m_cSUAButton.EnableWindow(m_bSUACheck);
	m_cSUAButton2.EnableWindow(m_bSUACheck);
	m_cSUAFileEdit.EnableWindow(m_bSUACheck);
	m_cSUAFileEdit2.EnableWindow(m_bSUACheck);
	m_cSUAFileText.EnableWindow(m_bSUACheck);
	m_cSUAFileText2.EnableWindow(m_bSUACheck);
	m_cSUAIgnore.EnableWindow(m_bSUACheck);
	}


void CFDRSettingsPropPage::OnEnChangeSuafilename()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CFDRSettingsPropPage::OnBnClickedBrowseSuafile2()
{
	static TCHAR BASED_CODE szFilter[] = _T("SUA Files (*.sua)|*.sua|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("sua"), NULL, OFN_HIDEREADONLY, szFilter);

	CString cTrans=_T("Select Special Use Airspace File");
	cFileDlg.m_ofn.lpstrTitle=cTrans;

	if( cFileDlg.DoModal()==IDOK )
		{
		CStringArray strAirspaceArray;
		int nSections=ReadSUAFile(cFileDlg.GetPathName(), strAirspaceArray);
		if( nSections==0  ) 
			AfxMessageBox(_T("No Valid Airspace in : ")+cFileDlg.GetPathName());
		else
			{
			// If the SUA file is in, or a sub directory of the wsc file, store the relative path
			CString strPath=GetRelativePath(cFileDlg.GetPathName());
			m_cSUAFileEdit2.SetWindowText(strPath);
			}
		}	
}
