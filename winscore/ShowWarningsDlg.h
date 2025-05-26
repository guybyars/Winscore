//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#include "afxwin.h"
#include "afxcmn.h"
#include "WinscoreView.h"
#include "WinscoreDoc.h"


#if !defined(AFX_SHOWWARNINGSDLG_H__959F3C63_F49B_11D2_B0F1_0080C84B0161__INCLUDED_)
#define AFX_SHOWWARNINGSDLG_H__959F3C63_F49B_11D2_B0F1_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowWarningsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowWarningsDlg dialog

class CShowWarningsDlg : public CResizingDialog
{
// Construction
public:
	CFlight*		m_pcFlight;
	CWinscoreDoc*	m_pDoc;
	CWinscoreView*  m_pView;
	CShowWarningsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowWarningsDlg)
	enum { IDD = IDD_SHOWWARNINGS };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowWarningsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowWarningsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRecheckLatest();
	afx_msg void OnBnClickedApplyPenalty();
	afx_msg void OnBnClickedSecurityPassed();
	afx_msg void OnBnClickedClear();
	CButton m_cLatestStartButton;
	CButton m_cPenaltyButton;
	CButton m_cSecurityButton;
	CButton m_cClearButton;
	CButton m_cFDRButton;

	void LoadWarningsFromFlight(int iSel);

	afx_msg void OnBnClickedClose();
	afx_msg void OnLvnItemchangingListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedUpdateFdrId();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWWARNINGSDLG_H__959F3C63_F49B_11D2_B0F1_0080C84B0161__INCLUDED_)

