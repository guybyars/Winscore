#if !defined(AFX_UITHREAD_H__4161E89D_3655_46B2_AF28_6DB7E65C5031__INCLUDED_)
#define AFX_UITHREAD_H__4161E89D_3655_46B2_AF28_6DB7E65C5031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIThread.h : header file
//

#include "AutoScoreDlg.h"
#include "WinscoreDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CUIThread thread



#define WM_INFORM_CLOSE		WM_USER + 100

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

protected:
	

// Attributes
public:

// Operations
public:
	void SetParent(CWnd* pParent);
	CString m_szMessage;
	CUIThread();           // protected constructor used by dynamic creation
	virtual ~CUIThread();

	//	Retrieve the status if the interface thread is still running.
	BOOL IsRunning();
	//	The main loop for running the interface thread. In this case, it displays a modal dialog box.
	int Run();
	//	Kill the interface thread by posting a message to the dialog box object.
	void Kill();

private:
	CWnd* m_pParent;
	// Indicates if we want to kill the thread.
	BOOL	m_bKill;
	BOOL	m_bRunning;
	// Object for the displaying the message dialog box.
	CAutoScoreDlg m_Dlg;

public:
	CWinscoreDoc *m_pDocument;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CUIThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UITHREAD_H__4161E89D_3655_46B2_AF28_6DB7E65C5031__INCLUDED_)
