//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//{{AFX_INCLUDES()
#include "taskcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_PLANNERDLG_H__D75D2A96_C464_11D2_B653_0080C84B0161__INCLUDED_)
#define AFX_PLANNERDLG_H__D75D2A96_C464_11D2_B653_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlannerDlg.h : header file
//

	#include "ResizingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPlannerDlg dialog

class CPlannerDlg : public CResizingDialog
{
// Construction
public:
	int m_nTaskPoints;
	int m_aiTaskPoints[MAXTASKTPS];
	CWinscoreDoc* m_pDoc;
	CGate m_cStartGate, m_cFinishGate;

	CPlannerDlg(CWinscoreDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	CPlannerDlg(CWinscoreDoc* pDoc, CGate &cStartGate, CGate &cFinishGate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlannerDlg)
	enum { IDD = IDD_TASK_PLANNER };
	CSliderCtrl	m_cSliderCtrl;
	CTaskControl	m_cTaskControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlannerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlannerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCancelMode();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReset();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnZoomChangedTaskcontrol(double dZoom);
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPlanTasks();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANNERDLG_H__D75D2A96_C464_11D2_B653_0080C84B0161__INCLUDED_)

