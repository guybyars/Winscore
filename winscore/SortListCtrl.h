//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_SORTLISTCTRL_H__104384C6_C9A1_11D3_B137_0080C84B0161__INCLUDED_)
#define AFX_SORTLISTCTRL_H__104384C6_C9A1_11D3_B137_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

BOOL SortTextItems( int nCol, BOOL bAscending, 
                                 int low = 0,
								 int high= -1 );

BOOL SortIntItems( int nCol, BOOL bAscending, 
                                 int low = 0,
								 int high= -1 );


	// Generated message map functions
protected:
	//{{AFX_MSG(CSortListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTLISTCTRL_H__104384C6_C9A1_11D3_B137_0080C84B0161__INCLUDED_)

