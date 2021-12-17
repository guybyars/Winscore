//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CLASSTABCTRL_H__E1D1ABD4_5870_11D1_ACF1_000000000000__INCLUDED_)
#define AFX_CLASSTABCTRL_H__E1D1ABD4_5870_11D1_ACF1_000000000000__INCLUDED_

#include "Winscore.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClassTabCtrl.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CClassTabCtrl window

class CClassTabCtrl : public CTabCtrl
{
// Construction
public:
	CClassTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	EClass GetCurClass();
	virtual ~CClassTabCtrl();
	EClass m_aeClasses[NUMCLASSES];

	// Generated message map functions
protected:
	//{{AFX_MSG(CClassTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSTABCTRL_H__E1D1ABD4_5870_11D1_ACF1_000000000000__INCLUDED_)

