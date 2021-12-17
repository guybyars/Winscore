#if !defined(AFX_DONTSHOWDLG_H__D1321857_471F_4571_9A20_F5E896AAE6A3__INCLUDED_)
#define AFX_DONTSHOWDLG_H__D1321857_471F_4571_9A20_F5E896AAE6A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DontShowDlg.h : header file
//
//#include  "defines.h"
/////////////////////////////////////////////////////////////////////////////
// CDontShowDlg dialog

class CDontShowDlg : public CDialog
{
// Construction
public:
	CString m_strRegIdent;
	LPCTSTR  m_strIcon;
	CString m_strDlgTitle;
	void DoModalConditional();
	CDontShowDlg(	CString		strRegIdent,
					CString		strMessage,
					CString		strDlgTitle="Winscore",
					LPCTSTR 	strIcon=IDI_EXCLAMATION,
					CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDontShowDlg)
	enum { IDD = IDD_DONT_SHOW };
	CStatic	m_cDlgIcon;
	BOOL	m_bDontShowAgainCheck;
	CString	m_strMessageText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDontShowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDontShowDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DONTSHOWDLG_H__D1321857_471F_4571_9A20_F5E896AAE6A3__INCLUDED_)
