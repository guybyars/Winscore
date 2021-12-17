//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ContestSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContestSettingsDlg dialog


CContestSettingsDlg::CContestSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContestSettingsDlg::IDD, pParent)
	, m_iDatum(0)
{
	//{{AFX_DATA_INIT(CContestSettingsDlg)
	m_bGuestsAtBottom = FALSE;
	m_iLatLongFormat = -1;
	m_iDistanceUnits = -1;
	m_bTwoGateCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CContestSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContestSettingsDlg)
	DDX_Check(pDX, IDC_GUEST_CHECK, m_bGuestsAtBottom);
	DDX_Radio(pDX, IDC_HHMMSS2, m_iLatLongFormat);
	DDX_Radio(pDX, IDC_UNITS_STATUTE_MILES_RADIO, m_iDistanceUnits);
	DDX_Radio(pDX, IDC_WGS84_RADIO, m_iDatum);
	DDX_Check(pDX, IDC_TWO_GATE_CHECK, m_bTwoGateCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContestSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CContestSettingsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContestSettingsDlg message handlers


