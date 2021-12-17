//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// GridPositionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "GridPositionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridPositionsDlg dialog


CGridPositionsDlg::CGridPositionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridPositionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridPositionsDlg)
	m_cStatusText = _T("");
	//}}AFX_DATA_INIT
}


void CGridPositionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridPositionsDlg)
	DDX_Control(pDX, IDC_CLASS_TAB, m_cClassTab);
	DDX_Text(pDX, IDC_STATUS_TEXT, m_cStatusText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridPositionsDlg, CDialog)
	//{{AFX_MSG_MAP(CGridPositionsDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CLASS_TAB, OnSelchangeClassTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridPositionsDlg message handlers

void CGridPositionsDlg::OnOK() 
{

	m_eClass=m_cClassTab.GetCurClass();

	int iClass=(int)m_cClassTab.GetCurClass();

	if( m_pDoc->m_bGridPositionCalculated[iClass] )
		{
		if( AfxMessageBox( OVERWRITE_TAKEOFF_GRID, MB_OKCANCEL )==2 )return;
		}

	m_pDoc->CalculateGridPositions(m_eClass, true);

	CDialog::OnOK();
}

void CGridPositionsDlg::OnSelchangeClassTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateStatus();
	*pResult = 0;
}

BOOL CGridPositionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc->SetAvailableClasses( m_cClassTab );

	UpdateStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridPositionsDlg::UpdateStatus()
{
	int iClass=(int)m_cClassTab.GetCurClass();

	if( m_pDoc->m_bGridPositionCalculated[iClass] )
		{
		m_cStatusText="Status: Already Calculated";
		}
	else
		{
		m_cStatusText="Status: Not Yet Calculated";
		}

	UpdateData(FALSE);

}

