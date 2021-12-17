//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ClassPropSheet.h"
#include "Global_Prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassPropSheet

IMPLEMENT_DYNAMIC(CClassPropSheet, CPropertySheet)

CClassPropSheet::CClassPropSheet(UINT nIDCaption, CWSClass &cClass, EUnits eUnits, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	m_cClass(cClass),
	m_eUnits(eUnits)
{
}

CClassPropSheet::CClassPropSheet(LPCTSTR pszCaption, CWSClass &cClass, EUnits eUnits, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	m_cClass(cClass),
	m_eUnits(eUnits)
{
		AddPage(&m_Page1);
		m_Page1.m_bHandicappedCheck		=cClass.IsHandicapped();
		m_Page1.m_bWorstDay				=cClass.IsUsingWDSA();
		m_Page1.m_strName				=cClass.GetName();
		m_Page1.m_dSMTD					=cClass.GetMinTaskDistance(eUnits);
		m_Page1.m_dSMTT					=cClass.GetMinTaskTime().GetHours()+(double)cClass.GetMinTaskTime().GetMinutes()/60.;
		m_Page1.m_strUnits				=UnitsText(eUnits);
		m_Page1.m_iAirfieldBonusPoints	=cClass.m_iAirfieldBonusPoints;


		AddPage(&m_Page2);
		m_Page2.m_dD1		=cClass.m_dD1;
		m_Page2.m_dDm		=cClass.m_dDm;
		m_Page2.m_eUnits	=eUnits;
		m_Page2.m_strUnits	=UnitsText(eUnits);

		// It's too big a pain in the ass keeping these prop sheets in sync
		// with the rules.  No one uses them anyway... so scrap them.
		/*
		if( cClass.GetType()!=e126 )
			{
			AddPage(&m_Page2);
			m_Page2.m_iAirfieldBonus	=cClass.m_iAirfieldBonusPoints;
			m_Page2.m_dMDP_B			=cClass.m_dMDP_B_ASGNED;
			m_Page2.m_dMDP_M			=cClass.m_dMDP_M_ASGNED;
			m_Page2.m_dMSP_B			=cClass.m_dMSP_B_ASGNED;
			m_Page2.m_dMSP_M			=cClass.m_dMSP_M_ASGNED;
			m_Page2.m_dNonFinishPoints	=cClass.m_dNoFinisherPoints_ASGNED;
			m_Page2.m_bSTFCheck			=cClass.m_bUseShortTaskFactor;

			AddPage(&m_Page3);
			m_Page3.m_iAirfieldBonus	=cClass.m_iAirfieldBonusPoints;
			m_Page3.m_dMDP_B			=cClass.m_dMDP_B_POST;
			m_Page3.m_dMDP_M			=cClass.m_dMDP_M_POST;
			m_Page3.m_dMSP_B			=cClass.m_dMSP_B_POST;
			m_Page3.m_dMSP_M			=cClass.m_dMSP_M_POST;
			m_Page3.m_dNonFinishPoints	=cClass.m_dNoFinisherPoints_POST;
			m_Page3.m_bShortTaskFactor	=cClass.m_bUseShortTaskFactor;
			m_Page3.m_bScoredCompletionRatioCheck	=cClass.m_bUseShortTaskFactor;
			}

*/

}

CClassPropSheet::~CClassPropSheet()
{
}


BEGIN_MESSAGE_MAP(CClassPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CClassPropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassPropSheet message handlers

