#if !defined(AFX_SCORESERVERDLG_H__EB9157BC_DC43_4AF9_9C79_A4A46FF34108__INCLUDED_)
#define AFX_SCORESERVERDLG_H__EB9157BC_DC43_4AF9_9C79_A4A46FF34108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScoreServerDlg.h : header file
//
#include <afxinet.h>

#include "WinscoreDoc.h"
#include "ScoreSheetListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CScoreServerDlg dialog

class CScoreServerDlg : public CDialog
{
// Construction
public:
	int m_iSSA_ID;
	int SendFlightLogs(CString &strFileName, CString &strUsername, CString &strPassword );
	void SetStatusText(CString str);
	CScoreServerDlg(CWnd* pParent = NULL);   // standard constructor
	CWinscoreDoc* m_pDoc;


	bool  SendFile(   CString strContents,
					  CString strFileName);

// Dialog Data
	//{{AFX_DATA(CScoreServerDlg)
	enum { IDD = IDD_SERVER_SCORES };
	CComboBox	m_cSSACombo;
	CComboBox	m_cDateCombo;
	CButton	m_cCommentButton;
	CButton	m_cExcludeButton;
	CButton	m_cSendButton;
	CEdit	m_cStatus;
	CScoreSheetListCtrl	m_cClassListCtrl;
	int		m_iType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScoreServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScoreServerDlg)
	afx_msg void OnSend();
	afx_msg void OnCommentButton();
	afx_msg void OnSettings();
	virtual BOOL OnInitDialog();
	afx_msg void OnTest();
	afx_msg void OnClose();
	afx_msg void OnExclude();
	afx_msg void OnRadio();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCORESERVERDLG_H__EB9157BC_DC43_4AF9_9C79_A4A46FF34108__INCLUDED_)
