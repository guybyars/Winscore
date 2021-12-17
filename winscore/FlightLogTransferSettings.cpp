// FlightLogTransferSettings.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "FlightLogTransferSettings.h"
#include "ExcludeContestantsDlg.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlightLogTransferSettings dialog


CFlightLogTransferSettings::CFlightLogTransferSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CFlightLogTransferSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlightLogTransferSettings)
	m_iTime = 0;
	m_iTop = 0;
	//}}AFX_DATA_INIT
}


void CFlightLogTransferSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlightLogTransferSettings)
	DDX_Control(pDX, IDC_TRANSFER_TEXT2, m_cTransferText2);
	DDX_Control(pDX, IDC_TOP, m_cTop);
	DDX_Control(pDX, IDC_TRANSFER_TEXT, m_cTransferText);
	DDX_Control(pDX, IDC_CONDENSE_CHECK, m_cCondenseCheck);
	DDX_Control(pDX, IDC_MINTIMETEXT, m_cMinTimeText);
	DDX_Control(pDX, IDC_TIME, m_cTime);
	DDX_Text(pDX, IDC_TIME, m_iTime);
	DDV_MinMaxInt(pDX, m_iTime, 1, 60);
	DDX_Text(pDX, IDC_TOP, m_iTop);
	DDV_MinMaxInt(pDX, m_iTop, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlightLogTransferSettings, CDialog)
	//{{AFX_MSG_MAP(CFlightLogTransferSettings)
	ON_BN_CLICKED(IDC_EXCLUDE, OnExcludeSpecific)
	ON_BN_CLICKED(IDC_CONDENSE_CHECK, OnCondenseCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlightLogTransferSettings message handlers

void CFlightLogTransferSettings::OnExcludeSpecific() 
{
	CExcludeContestantsDlg dlg;	
	dlg.m_pDoc=m_pDoc;
	if( dlg.DoModal()==IDOK )
		{
		}	
}


BOOL CFlightLogTransferSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int iCondense=GetWinscoreInt( FTPCONDENSE, FALSE ); 

	m_cMinTimeText.EnableWindow(iCondense);
	m_cTime.EnableWindow(iCondense);
	m_cTransferText.EnableWindow(iCondense);
	m_cTransferText2.EnableWindow(iCondense);
	m_cTop.EnableWindow(iCondense);

	m_cCondenseCheck.SetCheck(iCondense);
	m_iTime=GetWinscoreInt(  FTPCONDENSESEC, 4 ); 
	m_iTop=GetWinscoreInt( FTPTRANSFERTOP, 10 ); 
	
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlightLogTransferSettings::OnOK() 
{
	UpdateData();

	WriteWinscoreInt( FTPCONDENSE, m_cCondenseCheck.GetCheck() ); 
	WriteWinscoreInt( FTPCONDENSESEC, m_iTime ); 
	WriteWinscoreInt( FTPTRANSFERTOP, m_iTop ); 
	
	CDialog::OnOK();
}

void CFlightLogTransferSettings::OnCondenseCheck() 
{
	int iCondense=m_cCondenseCheck.GetCheck();
	m_cMinTimeText.EnableWindow(iCondense);
	m_cTime.EnableWindow(iCondense);
	m_cTransferText.EnableWindow(iCondense);
	m_cTransferText2.EnableWindow(iCondense);
	m_cTop.EnableWindow(iCondense);
	
}
