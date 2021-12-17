#if !defined(AFX_CONTESTANTSFROMSSA_H__3D1BA25B_FBFA_47BD_8933_E482C854D271__INCLUDED_)
#define AFX_CONTESTANTSFROMSSA_H__3D1BA25B_FBFA_47BD_8933_E482C854D271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContestantsFromSSA.h : header file
//

#include "winscoredoc.h"
#include "resizingdialog.h"

/////////////////////////////////////////////////////////////////////////////
// CContestantsFromSSA dialog

class CContestantsFromSSA : public CResizingDialog
{
// Construction
public:
	CWinscoreDoc* m_pDoc;
	CContestantList		m_cContestantList;
	CContestantsFromSSA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContestantsFromSSA)
	enum { IDD = IDD_CONTESTANTS_FROM_SSA };
	CButton	m_cOK;
	CButton	m_cLookupButton;
	CComboBox	m_cContestCombo;
	CListCtrl	m_cCListCtrlBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContestantsFromSSA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContestantsFromSSA)
	afx_msg void OnLookupButton();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTESTANTSFROMSSA_H__3D1BA25B_FBFA_47BD_8933_E482C854D271__INCLUDED_)
