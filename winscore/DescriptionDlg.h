//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_DESCRIPTIONDLG_H__84C4E1D1_B853_11D1_B1CC_0080C78BFBD8__INCLUDED_)
#define AFX_DESCRIPTIONDLG_H__84C4E1D1_B853_11D1_B1CC_0080C78BFBD8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DescriptionDlg.h : header file
//

#include "ResizingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDescriptionDlg dialog

class CDescriptionDlg : public CResizingDialog
{
// Construction
public:
	bool m_bNoOKButton;
	CString m_cTitle;
	CDescriptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDescriptionDlg)
	enum { IDD = IDD_DESCRIPTION };
	CButton	m_cOKButton;
	CString	m_Text;
	CString	m_cInfoBar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDescriptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDescriptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESCRIPTIONDLG_H__84C4E1D1_B853_11D1_B1CC_0080C78BFBD8__INCLUDED_)

