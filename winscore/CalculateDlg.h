//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CALCULATEDLG_H__043FB520_5B43_11D1_ACF6_000000000000__INCLUDED_)
#define AFX_CALCULATEDLG_H__043FB520_5B43_11D1_ACF6_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CalculateDlg.h : header file
//

#include "WinscoreDoc.h"
#include "ResizingDialog.h"	// Added by ClassView
#include "Winscore.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CCalculateDlg dialog

class CCalculateDlg : public CResizingDialog
{
// Construction
public:
	EClass m_eClass;
	CTime m_cDate;
	void MaskCalculateButton();
	void CreateStatusColumns();
	void Calculate();
	CWinscoreDoc* m_pDoc;
	CCalculateDlg(CWnd* pParent = NULL);   //  constructor

// Dialog Data
	//{{AFX_DATA(CCalculateDlg)
	enum { IDD = IDD_CALCULATE };
	CWinscoreListCtrl	m_cStatusListCtrl;
	CComboBox			m_cDateCombo;
	CClassTabCtrl		m_cClassTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalculateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalculateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeClassTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDateCombo();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCULATEDLG_H__043FB520_5B43_11D1_ACF6_000000000000__INCLUDED_)

