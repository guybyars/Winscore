//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//{{AFX_INCLUDES()
#include "wsview.h"
//}}AFX_INCLUDES
#if !defined(AFX_REALTIMEDLG_H__DF4C0D50_CE82_11D3_89AB_00C04F79ECB9__INCLUDED_)
#define AFX_REALTIMEDLG_H__DF4C0D50_CE82_11D3_89AB_00C04F79ECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealtimeDlg.h : header file
//

#include "winscoredoc.h"
#include "task.h"

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDlg dialog

class CRealtimeDlg : public CResizingDialog
{
// Construction
public:
	CTask* m_pcTask;
	CWinscoreDoc* m_pDoc;
	CTime m_cDate;
	void Refresh();
	void ReadFixes( CFixList &cFixList, CString &str);

	CRealtimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRealtimeDlg)
	enum { IDD = IDD_REALTIME };
	CWSView	m_cWSView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealtimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealtimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnInfo();
	afx_msg void OnRefresh();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPilotSelection(LPCTSTR strContestNo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEDLG_H__DF4C0D50_CE82_11D3_89AB_00C04F79ECB9__INCLUDED_)

