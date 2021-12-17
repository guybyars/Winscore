//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_REALTIMEINFODLG_H__83B06C23_D4A7_11D3_B13D_D040AE000000__INCLUDED_)
#define AFX_REALTIMEINFODLG_H__83B06C23_D4A7_11D3_B13D_D040AE000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealtimeInfoDlg.h : header file
//

#include "fixlist.h"
#include "resizingdialog.h"
#include "SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRealtimeInfoDlg dialog

class CRealtimeInfoDlg : public CResizingDialog
{
// Construction
public:
	CString m_strContestNo;
	CFixList& m_cFixList;
	CRealtimeInfoDlg(CFixList &cFixList, CString strContestNo, CWnd* pParent = NULL);   // standard constructor

	void CreateControlColumns(CListCtrl& ListCtrl);


// Dialog Data
	//{{AFX_DATA(CRealtimeInfoDlg)
	enum { IDD = IDD_REALTIME_INFO };
	CSortListCtrl	m_cList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealtimeInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealtimeInfoDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEINFODLG_H__83B06C23_D4A7_11D3_B13D_D040AE000000__INCLUDED_)

