//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_GRIDPOSITIONSDLG_H__3B574B11_67F8_11D1_AD0B_000000000000__INCLUDED_)
#define AFX_GRIDPOSITIONSDLG_H__3B574B11_67F8_11D1_AD0B_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GridPositionsDlg.h : header file
//

#include "ClassTabCtrl.h"
#include "WinscoreDoc.h"
#include "Winscore.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CGridPositionsDlg dialog

class CGridPositionsDlg : public CDialog
{
// Construction
public:
	EClass m_eClass;
	void UpdateStatus();
	CWinscoreDoc* m_pDoc;
	CGridPositionsDlg(CWnd* pParent = NULL);   //  constructor

// Dialog Data
	//{{AFX_DATA(CGridPositionsDlg)
	enum { IDD = IDD_GRID_POSITIONS };
	CClassTabCtrl	m_cClassTab;
	CString	m_cStatusText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridPositionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridPositionsDlg)
	virtual void OnOK();
	afx_msg void OnSelchangeClassTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDPOSITIONSDLG_H__3B574B11_67F8_11D1_AD0B_000000000000__INCLUDED_)

