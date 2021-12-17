//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_PROGRESSBAR_H__F3C234B7_FF4D_11D0_BE9A_0020AFD208B9__INCLUDED_)
#define AFX_PROGRESSBAR_H__F3C234B7_FF4D_11D0_BE9A_0020AFD208B9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressBar window

class CProgressBar : public CProgressCtrl
{
private:
	UINT uEndValue;
	UINT uStartOffset;
	CFrameWnd* pcwndMainFrame;
	CProgressCtrl wndProgress;

public:

// Construction
public:
	CProgressBar();
	CProgressBar::CProgressBar(UINT uStart, UINT uEnd, CString text="" );
	void SetRange(UINT uStart, UINT uEnd);
	void UpdateProgressBar(UINT uCurrent);

	static void SetStaticStatusBar(CMFCStatusBar* pBar);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgressBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProgressBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void SetStaticStatusBar(CMFCStatusBar* pBar);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSBAR_H__F3C234B7_FF4D_11D0_BE9A_0020AFD208B9__INCLUDED_)

