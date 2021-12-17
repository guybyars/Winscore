//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "wsclass.h"
#include "ClassPropPage.h"
#include "ClassPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassPropPage property page

IMPLEMENT_DYNCREATE(CClassPropPage, CPropertyPage)

CClassPropPage::CClassPropPage() : CPropertyPage(CClassPropPage::IDD)
, m_iAirfieldBonusPoints(0)
{
	//{{AFX_DATA_INIT(CClassPropPage)
	m_bHandicappedCheck = FALSE;
	m_strName = _T("");
	m_dSMTD = 0.0;
	m_dSMTT = 0.0;
	m_strUnits = _T("");
	m_bWorstDay = FALSE;
	//}}AFX_DATA_INIT
}

CClassPropPage::~CClassPropPage()
{
}

void CClassPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassPropPage)
	DDX_Check(pDX, IDC_HANDICAPPING_CHECK, m_bHandicappedCheck);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_SMTD_EDIT, m_dSMTD);
	DDX_Text(pDX, IDC_SMTT_EDIT, m_dSMTT);
	DDX_Text(pDX, IDC_UNIT_TEXT, m_strUnits);
	DDX_Text(pDX, IDC_AIRFIELD_BONUS_EDIT, m_iAirfieldBonusPoints);
	DDX_Check(pDX, IDC_WORSTCHECK, m_bWorstDay);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CClassPropPage)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_AIRFIELD_BONUS_EDIT, &CClassPropPage::OnEnChangeAirfieldBonusEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassPropPage message handlers

void CClassPropPage::OnOK() 
{

	if( !UpdateData() ) return;

	CWSClass	&cClass=((CClassPropSheet*)GetParent())->m_cClass;
	EUnits		eUnits=((CClassPropSheet*)GetParent())->m_eUnits;

	cClass.SetUsingWDSA( (m_bWorstDay)?(true):(false) );
	cClass.SetHandicapped( (m_bHandicappedCheck)?(true):(false) );
	cClass.SetName(m_strName);
	cClass.SetMinTaskDistance(ConvertDistance(m_dSMTD, eUnits, SYSTEMUNITS) );
	cClass.SetMinTaskTime(m_dSMTT);
	cClass.m_iAirfieldBonusPoints=m_iAirfieldBonusPoints;



	CPropertyPage::OnOK();
}

BOOL CClassPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CClassPropPage::OnEnChangeAirfieldBonusEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
