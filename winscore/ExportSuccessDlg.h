//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_EXPORTSUCCESSDLG_H__3295D371_6FB7_11D1_AD1A_000000000000__INCLUDED_)
#define AFX_EXPORTSUCCESSDLG_H__3295D371_6FB7_11D1_AD1A_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExportSuccessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportSuccessDlg dialog

class CExportSuccessDlg : public CDialog
{
// Construction
public:
	CExportSuccessDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportSuccessDlg)
	enum { IDD = IDD_EXPORT_STATUS };
	CString	m_cText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportSuccessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportSuccessDlg)
	afx_msg void OnHelpButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTSUCCESSDLG_H__3295D371_6FB7_11D1_AD1A_000000000000__INCLUDED_)

