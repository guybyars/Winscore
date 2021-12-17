//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_FDRSETTINGSPROPSHEET_H__210D2F74_B798_11D3_B132_0080C84B0161__INCLUDED_)
#define AFX_FDRSETTINGSPROPSHEET_H__210D2F74_B798_11D3_B132_0080C84B0161__INCLUDED_


#include "winscoredoc.h"
#include "FDRSettingsPropPage.h"
#include "FDRAnalysisSettingsPropPage.h"
#include "WinscoreDefs.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FDRSettingsPropSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropSheet

class CFDRSettingsPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CFDRSettingsPropSheet)

// Construction
public:
	CFDRSettingsPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFDRSettingsPropSheet(LPCTSTR pszCaption, CWinscoreDoc *pDoc, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

	CFDRSettingsPropPage			m_Page1;
	CFDRAnalysisSettingsPropPage	m_Page2;
	CWinscoreDoc *m_pDoc;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDRSettingsPropSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	EUnits GetUnits();
	virtual ~CFDRSettingsPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFDRSettingsPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDRSETTINGSPROPSHEET_H__210D2F74_B798_11D3_B132_0080C84B0161__INCLUDED_)

