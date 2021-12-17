//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassAssignedPointsPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "wsclass.h"
#include "ClassPropSheet.h"
#include "ClassAssignedPointsPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassAssignedPointsPropPage property page

IMPLEMENT_DYNCREATE(CClassAssignedPointsPropPage, CPropertyPage)

CClassAssignedPointsPropPage::CClassAssignedPointsPropPage() : CPropertyPage(CClassAssignedPointsPropPage::IDD)
, m_dD1(0)
, m_dDm(0)
{
	//{{AFX_DATA_INIT(CClassAssignedPointsPropPage)
	//}}AFX_DATA_INIT
}

CClassAssignedPointsPropPage::~CClassAssignedPointsPropPage()
{
}

void CClassAssignedPointsPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassAssignedPointsPropPage)
	DDX_Text(pDX, IDC_EDIT_D1, m_dD1);
	DDX_Text(pDX, IDC_EDIT_DM, m_dDm);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_UNITS_DM, m_cUnitsTextDm);
	DDX_Control(pDX, IDC_UNITS_D1, m_cUnitsTextD1);
}


BEGIN_MESSAGE_MAP(CClassAssignedPointsPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CClassAssignedPointsPropPage)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassAssignedPointsPropPage message handlers

void CClassAssignedPointsPropPage::OnOK() 
{
	if( !UpdateData() ) return;

	CWSClass	&cClass=((CClassPropSheet*)GetParent())->m_cClass;
	
	cClass.m_dD1			=ConvertDistance(m_dD1, m_eUnits, SYSTEMUNITS);
	cClass.m_dDm			=ConvertDistance(m_dDm, m_eUnits, SYSTEMUNITS);

	CPropertyPage::OnOK();
}

BOOL CClassAssignedPointsPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_cUnitsTextDm.SetWindowText(m_strUnits);
	m_cUnitsTextD1.SetWindowText(m_strUnits);

	m_dD1=ConvertDistance(m_dD1, SYSTEMUNITS, m_eUnits);
	m_dDm=ConvertDistance(m_dDm, SYSTEMUNITS, m_eUnits);

	UpdateData(FALSE);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


