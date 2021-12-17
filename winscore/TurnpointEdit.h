//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TURNPOINTEDIT_H__9C9B8F43_9DB4_11D3_B124_7030F7000000__INCLUDED_)
#define AFX_TURNPOINTEDIT_H__9C9B8F43_9DB4_11D3_B124_7030F7000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurnpointEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTurnpointEdit window

class CTurnpointEdit : public CEdit
{
// Construction
public:
	CTurnpointEdit();

// Attributes
public:

// Operations
public:

int ParseTurnpoints( int MaxTurnpoints, int aiTurnpoints[] );
int ParseTurnpointRadii( int MaxTurnpoints, double adTurnpoints[] );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurnpointEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Clear();
	void AddTurnpoint(int iID);
	void AddTurnpoint(double dID);
	virtual ~CTurnpointEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTurnpointEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNPOINTEDIT_H__9C9B8F43_9DB4_11D3_B124_7030F7000000__INCLUDED_)

