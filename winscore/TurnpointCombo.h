//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TURNPOINTCOMBO_H__A7D93BE1_536C_11D1_ACE9_000000000000__INCLUDED_)
#define AFX_TURNPOINTCOMBO_H__A7D93BE1_536C_11D1_ACE9_000000000000__INCLUDED_

#include "Turnpoint.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TurnpointCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTurnpointCombo window

class CTurnpointCombo : public CComboBox
{
// Construction
public:
	CTurnpointCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurnpointCombo)
	//}}AFX_VIRTUAL

	CTurnpoint* m_apSelTpts[12];
	int nSel;

// Implementation
public:
	void SetCurSel(int iID);
	CTurnpoint* GetCurSel();
	virtual ~CTurnpointCombo();
	int GetNumSel();
	CTurnpoint*  GetSelAt(int i);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTurnpointCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNPOINTCOMBO_H__A7D93BE1_536C_11D1_ACE9_000000000000__INCLUDED_)

