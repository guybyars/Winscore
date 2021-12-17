//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TASKTIMEDLG_H__77D66861_A3F4_4785_B2B3_0BD84342B5CA__INCLUDED_)
#define AFX_TASKTIMEDLG_H__77D66861_A3F4_4785_B2B3_0BD84342B5CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskTimeDlg.h : header file
//

#include <afxdtctl.h>
#include "winscoredoc.h"
#include "task.h"
#include "turnpointedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaskTimeDlg dialog

class CTaskTimeDlg : public CDialog
{
// Construction
public:
	CTaskTimeDlg(CWinscoreDoc *pDoc, CTask &cTask, CWnd* pParent = NULL);   // standard constructor

	CWinscoreDoc *m_pDoc;
	CTask		&m_cTask;

// Dialog Data
	//{{AFX_DATA(CTaskTimeDlg)
	enum { IDD = IDD_TASK_TIMES };
	CStatic	m_cPostTimeText;
	CDateTimeCtrl	m_cTaskOpenTimePicker;
	CDateTimeCtrl	m_cMinTimePicker;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskTimeDlg)
	afx_msg void OnMaxpostCheck();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKTIMEDLG_H__77D66861_A3F4_4785_B2B3_0BD84342B5CA__INCLUDED_)

