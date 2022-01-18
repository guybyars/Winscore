//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BAD124DE_4095_11D1_ACCF_000000000000__INCLUDED_)
#define AFX_MAINFRM_H__BAD124DE_4095_11D1_ACCF_000000000000__INCLUDED_

#include "WinscoreDlgBar.h"
#include "WinscoreBar.h"
#include "Winscore.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CFrameWnd CFrameWndEx

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	EViews GetViewCombo();
	void GreyAll();
	int m_nDatePaneNo;
	int m_nTimePaneNo;
	CTime GetDateCombo();
	EClass GetClassCombo();
	void SetViewCombo(EViews eView);
	void SetClassCombo(EClass eClass);
	void SetDateCombo(CTime cDate);

	void OnControlBarViewChange();
	void OnControlBarDayChange();
	void OnControlBarClassChange();

	void SetStatusText(CString cText);

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void GetMessageString(UINT nID, CString& rMessage) const;

protected:  // control bar embedded members
	CMFCStatusBar	m_wndStatusBar;
	CMFCToolBar		m_wndToolBar;
	CMFCMenuBar		m_wndMenuBar;
	//CWinscoreDlgBar	m_wndDialogBar;
	CWinscoreBar	m_wndWinscoreBar;

// Generated message map functions
protected:
	afx_msg void OnUpdateDate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClass(CCmdUI* pCmdUI);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewGotoBack(CCmdUI* pCmdUI);
	afx_msg void OnViewGotoBack();
	afx_msg void OnViewGotoForward();
	afx_msg void OnUpdateViewGotoForward(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitStatusBar(UINT *pIndicators, int nSize, int nSeconds);

public:
	EViews PopView();
	void PushView(EViews eView);
	void ProcessViewChange(EViews eNewView);
	CString m_sStatusBarString;
	void UpdateStatusLine();
	afx_msg void OnViewDlgbar();
	afx_msg void OnUpdateViewDlgbar(CCmdUI *pCmdUI);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void LoadTreeCtrl(CWinscoreDoc* pDoc);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BAD124DE_4095_11D1_ACCF_000000000000__INCLUDED_)

