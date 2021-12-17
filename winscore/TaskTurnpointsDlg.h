//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TASKTURNPOINTSDLG_H__FFC5D704_7264_4B21_9CB1_1A430331DF79__INCLUDED_)
#define AFX_TASKTURNPOINTSDLG_H__FFC5D704_7264_4B21_9CB1_1A430331DF79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TaskTurnpointsDlg.h : header file
//

#include "winscoredoc.h"
#include "task.h"
#include "turnpointedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaskTurnpointsDlg dialog

class CTaskTurnpointsDlg : public CDialog 
{
	
	

// Construction
public:
	void RefreshTaskList();
	CTaskTurnpointsDlg( CWinscoreDoc *pDoc, CTask &cTask, CWnd* pParent = NULL);   // standard constructor


	CWinscoreDoc *m_pDoc;
	CTask		&m_cTask;

// Dialog Data
	//{{AFX_DATA(CTaskTurnpointsDlg)
	enum { IDD = IDD_TASK_TURNPOINTS };
	CStatic	m_cRadiiText;
	CTurnpointEdit	m_cRadiiEdit;
	CTurnpointEdit	m_cTurnpointEdit;
	CTurnpointListCtrl	m_cTurnpointListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskTurnpointsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskTurnpointsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTaskPlanner();
	afx_msg void OnSelectpointsbutton();
	afx_msg void OnChangeTurnpointEdit();
	virtual void OnOK();
	afx_msg void OnChangeRadiiEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKTURNPOINTSDLG_H__FFC5D704_7264_4B21_9CB1_1A430331DF79__INCLUDED_)

