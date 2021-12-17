//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TASKINFODLG_H__4B13623D_CB72_11D2_88FE_00C04F79ECB9__INCLUDED_)
#define AFX_TASKINFODLG_H__4B13623D_CB72_11D2_88FE_00C04F79ECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaskInfoDlg dialog

class CTaskInfoDlg : public CResizingDialog
{
// Construction
public:
	CString ResetText();
	CTaskInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaskInfoDlg)
	enum { IDD = IDD_TASKINFO };
	CEdit	m_cEdit;
	CString	m_strTaskText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskInfoDlg)
	afx_msg void OnReset();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKINFODLG_H__4B13623D_CB72_11D2_88FE_00C04F79ECB9__INCLUDED_)

