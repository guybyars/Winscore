//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CLASSLISTCTRL_H__509D9A35_7007_11D3_B10D_0080C84B0161__INCLUDED_)
#define AFX_CLASSLISTCTRL_H__509D9A35_7007_11D3_B10D_0080C84B0161__INCLUDED_

#include "Winscore.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassListCtrl.h : header file
//
#include "wsclass.h"

/////////////////////////////////////////////////////////////////////////////
// CClassListCtrl window

class CClassListCtrl : public CListCtrl
{
// Construction
public:
	CClassListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassListCtrl)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetDayDisplay(bool b);
	bool m_bDays;
	int GetDay(EClass eClass);
	void CheckClass(CString strClass);
	void IncrementSelectedDay(bool bIncrease);
	void AddColumn(CString cColumnTitle);
	bool GetSelectedClass( EClass &eClass);
	bool IsChecked(EClass eClass);
	EClass GetCheckedClass(int i);
	int GetNumCheckedClasses();
	virtual ~CClassListCtrl();
	void SetAvailableClasses( bool bAll=false );
	void SetAvailableClasses( int nClasses, CWSClass acClasses[], EContest eContest, bool bAll=false);

protected:
	CImageList* m_pDragImage;
	BOOL		m_bDragging;
	BOOL		m_bDragChecked;
	int			m_nDragIndex, m_nDropIndex;
	CWnd*		m_pDropWnd;
	CPoint		m_ptDropPoint;
	void		DropItemOnList();
	int			m_aiDays[NUMCLASSES];


	// Generated message map functions
protected:
	//{{AFX_MSG(CClassListCtrl)	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSLISTCTRL_H__509D9A35_7007_11D3_B10D_0080C84B0161__INCLUDED_)

