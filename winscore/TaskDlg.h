//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TASKDLG_H__C923E5B3_4F74_11D1_ACE5_000000000000__INCLUDED_)
#define AFX_TASKDLG_H__C923E5B3_4F74_11D1_ACE5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TaskDlg.h : header file
//

#include "WinscoreDoc.h"
#include "TurnpointListCtrl.h"
#include "TurnpointCombo.h"
#include "ResizingDialog.h"
#include "wsview.h"
#include "afxcmn.h"

static _TCHAR *szIntervalHours	=_T("IntervalHours");
static _TCHAR *szIntervalMinutes=_T("IntervalMinutes");
static _TCHAR *szOpenHours		=_T("OpenHours");
static _TCHAR *szOpenMinutes	=_T("OpenMinutes");
static _TCHAR *szPostMinutes	=_T("PostMinutes");
static _TCHAR *szPostHours		=_T("PostHours");
static _TCHAR *szMaxPostMinutes =_T("MaxPostMinutes");
static _TCHAR *szMaxPostHours	=_T("MaxPostHours");
static _TCHAR *szFTIHours		=_T("FTIHours");
static _TCHAR *szFTIMinutes		=_T("FTIMinutes");
static _TCHAR *szSTIFactor		=_T("STIFactor");
static _TCHAR *szFinishRadius	=_T("FinishRadius");
static _TCHAR *szStartRadius	=_T("StartRadius");
static _TCHAR *szFirstLegAtExit =_T("FirstLegAtExit");
static _TCHAR *szStartCeiling	=_T("StartCeiling");
static _TCHAR *szFinishBase		=_T("FinishBase");
static _TCHAR *szFinishType		=_T("FinishType");
static _TCHAR *szFinishHeading	=_T("FinishHeading");
static _TCHAR *szStartType		=_T("StartType");
static _TCHAR *szStartHeading	=_T("StartHeading");

static _TCHAR *szStartID		=_T("StartID");
static _TCHAR *szFinishID		=_T("FinishID");

/////////////////////////////////////////////////////////////////////////////
// CTaskDlg dialog

class CTaskDlg : public CResizingDialog
{
// Construction
public:
	void SetStatusText();
	void InitializeTask();
	CTask* GetTask();
	bool IsContestDay();
	bool ApplyDlg();
	CTask*	m_pPreselect;
	CTime   m_cSelectedDate;

	void MaskForm();
	bool ValidateTask();
	void LoadWSView();
	void UpdateWSView();

	CWinscoreDoc* m_pDoc;
	CTaskList* m_pcTaskList;
	CTaskDlg(CWnd* pParent = NULL);   // standard constructor
	void SetTaskList(CTaskList *pcTaskList);

	CTask m_cTask;

// Dialog Data
	//{{AFX_DATA(CTaskDlg)
	enum { IDD = IDD_TASK };
	CButton	m_c2ndStartGateButton;
	CButton	m_c2ndGateCheck;
	CButton	m_cTypeRadio;
	CButton	m_cTaskTypeText;
	CButton	m_cFinishGateButton;
	CButton	m_cTimeButton;
	CButton	m_cTurnpointButton;
	CButton	m_cStartGateButton;
	CButton	m_cModRadio;
	CStatic m_cPostHoursText;
	CEdit	m_cPostHours;
	CButton	m_cPostRadio;
	CButton	m_cAssignedRadio;
	CComboBox	m_cDateCombo;
	CComboBox	m_cClassCombo;
	int		m_iStatus;
	int		m_iType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeClassCombo();
	afx_msg void OnSelchangeDateCombo();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	afx_msg void OnModRadio();
	afx_msg void OnStartGateButton();
	afx_msg void OnFinishGateButton();
	afx_msg void OnSelchangeStatuscombo();
	afx_msg void OnTypeRadio();
	afx_msg void OnTurnpointButton();
	afx_msg void OnTimeButton();
	afx_msg void OnStatusRadio();
	afx_msg void On2ndGateCheck();
	afx_msg void OnStartGateButtonB();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedImport();
	afx_msg void OnCopyFormatted();
	afx_msg void OnBnClickedTaskPlanner2();
	CWSView	m_cWSViewCtrl;
	CListCtrl m_cStatusListCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDLG_H__C923E5B3_4F74_11D1_ACE5_000000000000__INCLUDED_)

