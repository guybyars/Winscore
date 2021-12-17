//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_COPYSCORESHEETDLG_H__A2473EF3_EEE8_11D2_B0EF_0080C84B0161__INCLUDED_)
#define AFX_COPYSCORESHEETDLG_H__A2473EF3_EEE8_11D2_B0EF_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyScoresheetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyScoresheetDlg dialog

class CCopyScoresheetDlg : public CDialog
{
// Construction
public:
	CCopyScoresheetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyScoresheetDlg)
	enum { IDD = IDD_COPYSCORESHEET };
	CButton	m_cTabCheck;
	BOOL	m_fSummaryCheck;
	BOOL	m_fTabCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyScoresheetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCopyScoresheetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTextradio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYSCORESHEETDLG_H__A2473EF3_EEE8_11D2_B0EF_0080C84B0161__INCLUDED_)

