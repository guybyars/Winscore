//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CLASSPROPSHEET_H__864B55D3_AB0C_11D3_B129_0080C84B0161__INCLUDED_)
#define AFX_CLASSPROPSHEET_H__864B55D3_AB0C_11D3_B129_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassPropSheet.h : header file
//

#include	"wsclass.h"
#include	"classpostpointsproppage.h"
#include	"classassignedpointsproppage.h"
#include	"classproppage.h"
#include "WinscoreDefs.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CClassPropSheet

class CClassPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CClassPropSheet)

// Construction
public:
	CClassPropSheet(UINT nIDCaption, CWSClass &cClass, EUnits eUnits, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CClassPropSheet(LPCTSTR pszCaption, CWSClass &cClass, EUnits eUnits, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	CClassPropPage					m_Page1;
	CClassAssignedPointsPropPage	m_Page2;
//	CClassPostPointsPropPage		m_Page3;
	CWSClass					   &m_cClass;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassPropSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	EUnits m_eUnits;
	virtual ~CClassPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClassPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSPROPSHEET_H__864B55D3_AB0C_11D3_B129_0080C84B0161__INCLUDED_)

