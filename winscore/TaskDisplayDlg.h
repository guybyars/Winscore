//{{AFX_INCLUDES()
#include "wsview.h"
//}}AFX_INCLUDES
#if !defined(AFX_TASKDISPLAYDLG_H__06AF7FB1_5512_4C63_B4D6_BEB2BA1E78E2__INCLUDED_)
#define AFX_TASKDISPLAYDLG_H__06AF7FB1_5512_4C63_B4D6_BEB2BA1E78E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskDisplayDlg.h : header file
//
#include "ResizingDialog.h"
#include "winscore.h"

/////////////////////////////////////////////////////////////////////////////
// CTaskDisplayDlg dialog

class CTaskDisplayDlg : public CResizingDialog
{
// Construction
public:
	CTaskDisplayDlg(CTask *pcTask, CWinscoreDoc *pcDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaskDisplayDlg)
	enum { IDD = IDD_TASKDISPLAY };
	CWSView	m_cWSViewCtrl;
	//}}AFX_DATA

	CTask			*m_pcTask;
	CWinscoreDoc	*m_pDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHelpButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDISPLAYDLG_H__06AF7FB1_5512_4C63_B4D6_BEB2BA1E78E2__INCLUDED_)
