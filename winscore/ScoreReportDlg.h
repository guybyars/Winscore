#if !defined(AFX_SCOREREPORTDLG_H__04E77E8F_725F_4ACA_B5A5_BE02FA3829F1__INCLUDED_)
#define AFX_SCOREREPORTDLG_H__04E77E8F_725F_4ACA_B5A5_BE02FA3829F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScoreReportDlg.h : header file
//

#include "ResizingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CScoreReportDlg dialog

class CScoreReportDlg : public CResizingDialog
{
// Construction
public:
	CString m_strText;
	CScoreReportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScoreReportDlg)
	enum { IDD = IDD_SCORE_REPORT };
	CEdit	m_cEditCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScoreReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScoreReportDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOREREPORTDLG_H__04E77E8F_725F_4ACA_B5A5_BE02FA3829F1__INCLUDED_)
