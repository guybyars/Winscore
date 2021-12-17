#if !defined(AFX_DETAILEDSCOREINFODLG_H__E940232B_07E6_4314_9816_705BF261E73F__INCLUDED_)
#define AFX_DETAILEDSCOREINFODLG_H__E940232B_07E6_4314_9816_705BF261E73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailedScoreInfoDlg.h : header file
//
#include "resizingdialog.h"
#include "scorerecord.h"
#include "WinscoreDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDetailedScoreInfoDlg dialog

class CDetailedScoreInfoDlg : public CResizingDialog
{
// Construction
public:
	CWinscoreDoc* m_pDoc;
	CDetailedScoreInfoDlg(CWinscoreDoc* pDoc, CScoreRecord *pcRecord, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailedScoreInfoDlg)
	enum { IDD = IDD_DETAILED_SCORES };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailedScoreInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailedScoreInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void AddTwoStringItem( CString cstr1, CString cstr2);
	CScoreRecord *m_pcRecord;
	void CreateStatusColumns();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILEDSCOREINFODLG_H__E940232B_07E6_4314_9816_705BF261E73F__INCLUDED_)
