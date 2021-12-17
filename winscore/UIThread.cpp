// UIThread.cpp : implementation file
//

#include "stdafx.h"
#include "UIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
{
	m_bKill = FALSE;
	m_bRunning = FALSE;
}

CUIThread::~CUIThread()
{
}


BOOL CUIThread::InitInstance()
{
	return TRUE;
}

int CUIThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
	//{{AFX_MSG_MAP(CUIThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIThread message handlers


void CUIThread::Kill()
{
	//	Kill the interface thread by posting a message to 
	//	the dialog box object.
	if(m_bKill) return;

	m_Dlg.PostMessage(WM_COMMAND, IDCANCEL);
}


int CUIThread::Run()
{
	//	The main loop for running the interface thread. 
	//	In this case, it displays a modal dialog box.
	
	m_bKill = FALSE;	// Indicates that the UI-thread is activated and it is used for displaying the dialog box.
	m_bRunning = TRUE;
	m_Dlg.m_pDocument=m_pDocument;
	m_Dlg.DoModal();
	m_bKill = TRUE;
	m_bRunning = FALSE;
	
	if(m_pParent)		// Post a message to the parent window to remove the UI-thread object.
		m_pParent->PostMessage(WM_INFORM_CLOSE);


	return 0;
}


BOOL CUIThread::IsRunning()
{
	return m_bRunning;
}

void CUIThread::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

