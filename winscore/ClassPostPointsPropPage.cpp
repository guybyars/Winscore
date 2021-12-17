//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassPostPointsPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "wsclass.h"
#include "ClassPropSheet.h"
#include "ClassPostPointsPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassPostPointsPropPage property page

IMPLEMENT_DYNCREATE(CClassPostPointsPropPage, CPropertyPage)

CClassPostPointsPropPage::CClassPostPointsPropPage() : CPropertyPage(CClassPostPointsPropPage::IDD)
{
	//{{AFX_DATA_INIT(CClassPostPointsPropPage)
	m_iAirfieldBonus = 0;
	m_dMDP_B = 0.0;
	m_dMDP_M = 0.0;
	m_dMSP_B = 0.0;
	m_dMSP_M = 0.0;
	m_dNonFinishPoints = 0.0;
	m_bScoredCompletionRatioCheck = FALSE;
	m_bShortTaskFactor = FALSE;
	//}}AFX_DATA_INIT
}

CClassPostPointsPropPage::~CClassPostPointsPropPage()
{
}

void CClassPostPointsPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassPostPointsPropPage)
	DDX_Text(pDX, IDC_AIRFIELD_BONUS_EDIT, m_iAirfieldBonus);
	DDX_Text(pDX, IDC_MDP_B_EDIT, m_dMDP_B);
	DDX_Text(pDX, IDC_MDP_M_EDIT, m_dMDP_M);
	DDX_Text(pDX, IDC_MSP_B_EDIT, m_dMSP_B);
	DDX_Text(pDX, IDC_MSP_M_EDIT, m_dMSP_M);
	DDX_Text(pDX, IDC_NOFINISH_POINTS_EDIT, m_dNonFinishPoints);
	DDX_Check(pDX, IDC_SCR_CHECK, m_bScoredCompletionRatioCheck);
	DDX_Check(pDX, IDC_STF_CHECK, m_bShortTaskFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassPostPointsPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CClassPostPointsPropPage)
	ON_EN_CHANGE(IDC_NOFINISH_POINTS_EDIT, OnChangeNofinishPointsEdit)
	ON_BN_CLICKED(IDC_STF_CHECK, OnStfCheck)
	ON_BN_CLICKED(IDC_SCR_CHECK, OnScrCheck)
	ON_EN_CHANGE(IDC_MSP_M_EDIT, OnChangeMspMEdit)
	ON_EN_CHANGE(IDC_MSP_B_EDIT, OnChangeMspBEdit)
	ON_EN_CHANGE(IDC_MDP_M_EDIT, OnChangeMdpMEdit)
	ON_EN_CHANGE(IDC_MDP_B_EDIT, OnChangeMdpBEdit)
	ON_EN_CHANGE(IDC_AIRFIELD_BONUS_EDIT, OnChangeAirfieldBonusEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassPostPointsPropPage message handlers

void CClassPostPointsPropPage::OnOK() 
{
	if( !UpdateData() ) return;

	CWSClass	&cClass=((CClassPropSheet*)GetParent())->m_cClass;
	
	cClass.m_iAirfieldBonusPoints	=m_iAirfieldBonus;
	cClass.m_dMDP_B_POST			=m_dMDP_B;
	cClass.m_dMDP_M_POST			=m_dMDP_M;
	cClass.m_dMSP_B_POST			=m_dMSP_B;
	cClass.m_dMSP_M_POST			=m_dMSP_M;
	cClass.m_dNoFinisherPoints_POST	=m_dNonFinishPoints;
	cClass.SetUseShortTaskFactor((m_bShortTaskFactor)?(true):(false));
	cClass.SetUseScoredCompletionRatio((m_bScoredCompletionRatioCheck)?(true):(false));

	CPropertyPage::OnOK();
}

BOOL CClassPostPointsPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClassPostPointsPropPage::OnChangeNofinishPointsEdit() 
{
SetModified(true);
	
}

void CClassPostPointsPropPage::OnStfCheck() 
{
SetModified(true);	
}

void CClassPostPointsPropPage::OnScrCheck() 
{
SetModified(true);	
}

void CClassPostPointsPropPage::OnChangeMspMEdit() 
{
SetModified(true);	
}

void CClassPostPointsPropPage::OnChangeMspBEdit() 
{
SetModified(true);	
}

void CClassPostPointsPropPage::OnChangeMdpMEdit() 
{
SetModified(true);
}

void CClassPostPointsPropPage::OnChangeMdpBEdit() 
{
SetModified(true);
}

void CClassPostPointsPropPage::OnChangeAirfieldBonusEdit() 
{
SetModified(true);
}

