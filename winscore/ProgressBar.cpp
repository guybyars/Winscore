//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "ProgressBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static CMFCStatusBar  *pStatusBar=NULL;


/////////////////////////////////////////////////////////////////////////////
// CProgressBar

void CProgressBar::SetStaticStatusBar(CMFCStatusBar* pBar)
	{
	pStatusBar=pBar;
	};

CProgressBar::CProgressBar()
	{

	if( !pStatusBar ) return;
    RECT rc;
    pStatusBar->GetItemRect (0, &rc);
	rc.left += (rc.right - rc.left)/2;
    VERIFY (wndProgress.Create(WS_CHILD | WS_VISIBLE, rc,
                                            pStatusBar, 1));
	}


CProgressBar::CProgressBar(UINT uStart, UINT uEnd, CString text )
	{
/*
	CWinApp* theApp = AfxGetApp();
	pcwndMainFrame = (CFrameWnd *) theApp->m_pMainWnd;
	pStatusBar = (CStatusBar*)pcwndMainFrame->GetMessageBar();
    RECT rc;
    pStatusBar->GetItemRect (0, &rc);
	rc.left += (rc.right - rc.left)/2;
    VERIFY (wndProgress.Create(WS_CHILD | WS_VISIBLE, rc,
                                            pStatusBar, 1));
	if( text.GetLength()>0 ) pStatusBar->SetPaneText(0,text);
	SetRange( uStart, uEnd); */

	if( !pStatusBar ) return;
     RECT rc;
    pStatusBar->GetItemRect (0, &rc);
	rc.left += (rc.right - rc.left)/2;
    VERIFY (wndProgress.Create(WS_CHILD | WS_VISIBLE, rc,
                                            pStatusBar, 1));
	if( text.GetLength()>0 ) pStatusBar->SetPaneText(0,text);
	SetRange( uStart, uEnd); 


	}


CProgressBar::~CProgressBar()
{
	if( !pStatusBar ) return;
	pStatusBar->SetPaneText(0,_T("Ready"));
}


BEGIN_MESSAGE_MAP(CProgressBar, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CProgressBar::SetRange(UINT uStart, UINT uEnd)
	{
	uStartOffset = __min(uStart,uEnd);
	uEndValue = __max(uStart,uEnd) - uStartOffset;
	wndProgress.SetRange(0,50);
	wndProgress.SetStep(50);
	}

void CProgressBar::UpdateProgressBar(UINT uNext)
	{
	int iPos = (int)(((float)(uNext-uStartOffset)/((float)uEndValue/50.f)) + .5);
	iPos = __min(iPos,50);
	wndProgress.SetPos(iPos);
	}

/////////////////////////////////////////////////////////////////////////////
// CProgressBar message handlers

