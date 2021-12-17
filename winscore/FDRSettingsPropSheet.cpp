//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRSettingsPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "FDRSettingsPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropSheet

IMPLEMENT_DYNAMIC(CFDRSettingsPropSheet, CPropertySheet)

CFDRSettingsPropSheet::CFDRSettingsPropSheet(UINT nIDCaption,  CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
			AddPage(&m_Page1);
			m_Page1.m_pDoc		=m_pDoc;
}

CFDRSettingsPropSheet::CFDRSettingsPropSheet(LPCTSTR pszCaption, CWinscoreDoc *pDoc,CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	m_pDoc(pDoc)
{
			AddPage(&m_Page1);
			m_Page1.m_pDoc		=m_pDoc;
			AddPage(&m_Page2);
//			AddPage(&m_Page3);
}

CFDRSettingsPropSheet::~CFDRSettingsPropSheet()
{
}


BEGIN_MESSAGE_MAP(CFDRSettingsPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CFDRSettingsPropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropSheet message handlers

EUnits CFDRSettingsPropSheet::GetUnits()
{
	return m_pDoc->m_eUnits;
}

