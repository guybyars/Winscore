//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WINDDLG_H__1B21AEE1_5107_11D1_ACE6_000000000000__INCLUDED_)
#define AFX_WINDDLG_H__1B21AEE1_5107_11D1_ACE6_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WindDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWindDlg dialog

class CWindDlg : public CDialog
{
// Construction
public:
	CWindDlg(CWnd* pParent = NULL);   // standard constructor

	EUnits m_eUnits;

// Dialog Data
	//{{AFX_DATA(CWindDlg)
	enum { IDD = IDD_WINDS };
	double	m_dDirection;
	double	m_dSpeed;
	CString	m_cSpeedText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDDLG_H__1B21AEE1_5107_11D1_ACE6_000000000000__INCLUDED_)

