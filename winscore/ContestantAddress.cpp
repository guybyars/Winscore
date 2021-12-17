//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestantAddress.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "ContestantAddress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContestantAddress dialog


CContestantAddress::CContestantAddress(CWnd* pParent /*=NULL*/)
	: CDialog(CContestantAddress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContestantAddress)
	m_strAddress1 = _T("");
	m_strAddress2 = _T("");
	m_strCity = _T("");
	m_strState = _T("");
	m_strZip1 = _T("");
	m_strZip2 = _T("");
	//}}AFX_DATA_INIT
}


void CContestantAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContestantAddress)
	DDX_Text(pDX, IDC_ADDRESS1, m_strAddress1);
	DDV_MaxChars(pDX, m_strAddress1, 40);
	DDX_Text(pDX, IDC_ADDRESS2, m_strAddress2);
	DDV_MaxChars(pDX, m_strAddress2, 40);
	DDX_Text(pDX, IDC_CITY, m_strCity);
	DDV_MaxChars(pDX, m_strCity, 40);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDV_MaxChars(pDX, m_strState, 2);
	DDX_Text(pDX, IDC_ZIP1, m_strZip1);
	DDV_MaxChars(pDX, m_strZip1, 5);
	DDX_Text(pDX, IDC_ZIP2, m_strZip2);
	DDV_MaxChars(pDX, m_strZip2, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContestantAddress, CDialog)
	//{{AFX_MSG_MAP(CContestantAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContestantAddress message handlers

void CContestantAddress::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

