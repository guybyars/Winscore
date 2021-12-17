//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WINSCORELISTCTRL_H__04D4C771_4864_11D1_ACDA_000000000000__INCLUDED_)
#define AFX_WINSCORELISTCTRL_H__04D4C771_4864_11D1_ACDA_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WinscoreListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWinscoreListCtrl window

class CWinscoreListCtrl : public CListCtrl
{
// Construction
public:
	CWinscoreListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinscoreListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Select(int i);
	void DeselectAll();
	BOOL m_fReport;
	void AddTwoStringItem(CString, CString);
	void RepaintSelectedItems();
	virtual ~CWinscoreListCtrl();
	void EnsureVisibleCenter( int iItem  );
	void EnsureVisibleTop( int iItem  );

	// Generated message map functions
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	static LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);

// Implementation - client area width
	int m_cxClient;

// Implementation - state icon width
	int m_cxStateImageOffset;

// Implementation - list view colors
	COLORREF m_clrText;
	COLORREF m_clrTextBk;
	COLORREF m_clrBkgnd;

	//{{AFX_MSG(CWinscoreListCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINSCORELISTCTRL_H__04D4C771_4864_11D1_ACDA_000000000000__INCLUDED_)

