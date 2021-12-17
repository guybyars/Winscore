#if !defined(AFX_AUTOEXPORT_H__B2A204BF_FB98_4D1A_9297_DA447B4E4BAE__INCLUDED_)
#define AFX_AUTOEXPORT_H__B2A204BF_FB98_4D1A_9297_DA447B4E4BAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoExport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoExport dialog

class CAutoExport : public CDialog
{
// Construction
public:
	CAutoExport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoExport)
	enum { IDD = IDD_AUTOEXPORT_DIALOG };
	CButton	m_cCheckControl;
	CStatic	m_cPathTextControl;
	CButton	m_cButtonControl;
	CEdit	m_cEditControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoExport)
	afx_msg void OnCheck();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOEXPORT_H__B2A204BF_FB98_4D1A_9297_DA447B4E4BAE__INCLUDED_)
