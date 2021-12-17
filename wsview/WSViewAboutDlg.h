//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSVIEWABOUTDLG_H__7554CF69_0195_47A7_8B08_769DE83C2C4A__INCLUDED_)
#define AFX_WSVIEWABOUTDLG_H__7554CF69_0195_47A7_8B08_769DE83C2C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WSViewAboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWSViewAboutDlg dialog

class CWSViewAboutDlg : public CDialog
{
// Construction
public:
	CWSViewAboutDlg(CString &strVersion, CWnd* pParent = NULL);   // standard constructor

	CString m_strVersion;
// Dialog Data
	//{{AFX_DATA(CWSViewAboutDlg)
	enum { IDD = IDD_ABOUTBOX_WSVIEWCTL };
	CString	m_strVersionText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSViewAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWSViewAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSVIEWABOUTDLG_H__7554CF69_0195_47A7_8B08_769DE83C2C4A__INCLUDED_)

