//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// RealTimePropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "RealTimePropPage.h"
#include "dirdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealTimePropPage property page

IMPLEMENT_DYNCREATE(CRealTimePropPage, CPropertyPage)

CRealTimePropPage::CRealTimePropPage() : CPropertyPage(CRealTimePropPage::IDD)
{
	//{{AFX_DATA_INIT(CRealTimePropPage)
	m_iRefreshRate = 0;
	m_strRealTimePath = _T("");
	//}}AFX_DATA_INIT
}

CRealTimePropPage::~CRealTimePropPage()
{
}

void CRealTimePropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealTimePropPage)
	DDX_Text(pDX, IDC_EDIT1, m_iRefreshRate);
	DDV_MinMaxInt(pDX, m_iRefreshRate, 0, 1000);
	DDX_Text(pDX, IDC_REALTIMEPATH, m_strRealTimePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealTimePropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRealTimePropPage)
	ON_BN_CLICKED(IDC_BROWSE_REALTIME_PATH, OnBrowseRealtimePath)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimePropPage message handlers

void CRealTimePropPage::OnBrowseRealtimePath() 
{
	CDirDialog cdlg;
	cdlg.m_strSelDir=m_strRealTimePath;
	cdlg.m_strTitle="Temporary directory used for transfering flght logs";
	if( cdlg.DoBrowse() ==TRUE  ) 
		{
		m_strRealTimePath=cdlg.m_strPath;
		UpdateData(FALSE);
		}			
}

BOOL CRealTimePropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
		
	m_strRealTimePath	=AfxGetApp()->GetProfileString( REGSECTION, REALTIMEPATH, ""  );

	m_iRefreshRate		=AfxGetApp()->GetProfileInt( REGSECTION, REFRESHRATE, 60 ); 

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealTimePropPage::OnOK() 
{
	char cBuff[512];
	GetShortPathName(m_strRealTimePath, cBuff, 512);
	AfxGetApp()->WriteProfileString( REGSECTION, REALTIMEPATH,	 cBuff );
	AfxGetApp()->WriteProfileInt(REGSECTION, REFRESHRATE, m_iRefreshRate );

	CPropertyPage::OnOK();
}


