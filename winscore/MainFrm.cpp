//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreDoc.h"
#include "ListVwEx.h"
#include "WinscoreView.h"
#include "Global_Prototypes.h"
#include "ProgressBar.h"

#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CArray<int,int>  saBackFwdArray;
static int				isaBackFwd=0;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DATE, OnUpdateDate)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnUpdateTime)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOTO_BACK, OnUpdateViewGotoBack)
	ON_COMMAND(ID_VIEW_GOTO_BACK, OnViewGotoBack)
	ON_CBN_SELCHANGE(IDC_CONTROL_BAR_VIEW, OnControlBarViewChange)	
	ON_CBN_SELCHANGE(IDC_CONTROL_BAR_DAY, OnControlBarDayChange)
	ON_CBN_SELCHANGE(IDC_CONTROL_BAR_CLASS, OnControlBarClassChange)
	ON_COMMAND(ID_VIEW_GOTO_FORWARD, OnViewGotoForward)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOTO_FORWARD, OnUpdateViewGotoForward)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_VIEW_DLGBAR, &CMainFrame::OnViewDlgbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DLGBAR, &CMainFrame::OnUpdateViewDlgbar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_sStatusBarString="Ready!";
	saBackFwdArray.Add(eContestantView);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndDialogBar.Create( this ) )
	{
		TRACE0("Failed to create dialog bar\n");
		return -1;      // fail to create
	}
	
   if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndWinscoreBar.Create (_T("Winscore Control"), this, CRect (0, 0, 200, 200),
		FALSE, IDS_CONTROL_BAR,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT ))
	{
		TRACE0("Failed to create Winscore bar\n");
		return FALSE;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, 0))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	CProgressBar::SetStaticStatusBar(&m_wndStatusBar);


	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
//	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
//		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndDialogBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndDialogBar);
	DockPane(&m_wndWinscoreBar);

	bool bVis=false;
	AfxGetApp()->GetProfileInt(REGSECTION, CONTROLBARVISIBLE, 0 )==1?bVis=true:bVis=false;
	
	ShowPane (&m_wndDialogBar,
					bVis,
					FALSE, TRUE);
	ShowPane (&m_wndWinscoreBar,
					!bVis,
					FALSE, TRUE);

	

	//m_wndDialogBar.ShowPane(TRUE,TRUE,TRUE);

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// Find out the size of the static variable 'indicators' defined
		// by AppWizard and copy it
		int nOrigSize = sizeof(indicators) / sizeof(UINT);

		UINT* pIndicators = new UINT[nOrigSize + 2];
		memcpy(pIndicators, indicators, sizeof(indicators));

		// Call the Status Bar Component's status bar creation function
		if (!InitStatusBar(pIndicators, nOrigSize, 60))
		{
			TRACE0("Failed to initialize Status Bar\n");
			return -1;
		}
		delete[] pIndicators;
	}

	// Set the default manager to Office 2007 
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

//void CMainFrame::OnFileNew() 
//{
//	CMenu cMenu;
//	int iStat=cMenu.LoadMenu(IDR_NOCONTEST);
//	SetMenu(&cMenu);
//	cMenu.Detach();
//}

void CMainFrame::OnControlBarViewChange()
{
	EViews			eNewView = m_wndDialogBar.GetView();

	ProcessViewChange(eNewView);

	PushView(eNewView);

	}

void CMainFrame::SetViewCombo(EViews eView)
{
	CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
	if( !pcView ) return;
	m_wndDialogBar.SetView(eView);
	PushView(eView);
	m_wndDialogBar.LoadDayCombo(pcView->GetDocument());
	m_wndDialogBar.LoadClassCombo(pcView->GetDocument());
}

void CMainFrame::SetClassCombo(EClass eClass)
{
	m_wndDialogBar.SetClass(eClass);
	SetDefaultClass(eClass);
}

void CMainFrame::SetDateCombo(CTime cDate)
{
	m_wndDialogBar.SetDate(cDate);
}


void CMainFrame::OnControlBarDayChange()
{
	CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );

	EViews eView = m_wndDialogBar.GetView();
	CTime cTime=m_wndDialogBar.GetDayCombo();

	if(	eView==eTaskView )
		{
		pcView->ViewTasks();
		}
	else if(	eView==eFlightLogView )
		{
		EClass eClass=m_wndDialogBar.GetClassCombo();
		pcView->ViewFlightLogs(cTime, eClass);
		}
	else if(	eView==eScoreView )
		{
		EClass eClass=m_wndDialogBar.GetClassCombo();
		pcView->ViewScores(cTime, eClass);
		}
	SetDefaultDay(cTime);
}

void CMainFrame::OnControlBarClassChange()
{
	CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );

	EViews eView = m_wndDialogBar.GetView();

	EClass eClass=m_wndDialogBar.GetClassCombo();
	SetDefaultClass(eClass);

	if(	eView==eFlightLogView )
		{
		CTime cTime=m_wndDialogBar.GetDayCombo();
		pcView->ViewFlightLogs(cTime, eClass);
		}
	else if(	eView==eScoreView )
		{
		CTime cTime=m_wndDialogBar.GetDayCombo();
		pcView->ViewScores(cTime, eClass);
		}
	else if(	eView==eTakeoffGridView )
		{
		pcView->ViewTakeoffGrid(eClass);
		}
}

CTime CMainFrame::GetDateCombo()
{
return m_wndDialogBar.GetDayCombo();
}

EClass CMainFrame::GetClassCombo()
{
return m_wndDialogBar.GetClassCombo();
}

void CMainFrame::OnUpdateDate(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strDate = time.Format(_T("%A, %B %d, %Y "));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strDate);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current date
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(m_nDatePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nDatePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strDate);
	pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%H:%M  "));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strTime);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current time
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(m_nTimePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nTimePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strTime);
	pCmdUI->Enable(TRUE);

}

BOOL CMainFrame::InitStatusBar(UINT *pIndicators, int nSize, int nSeconds)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Create an index for the DATE pane
	m_nDatePaneNo = nSize++;
	pIndicators[m_nDatePaneNo] = ID_INDICATOR_DATE;
	// Create an index for the TIME pane
	m_nTimePaneNo = nSize++;
	nSeconds = 10;
	pIndicators[m_nTimePaneNo] = ID_INDICATOR_TIME;

	// TODO: Select an appropriate time interval for updating
	// the status bar when idling.
	m_wndStatusBar.SetTimer(0x1000, nSeconds * 1000, NULL);

	return m_wndStatusBar.SetIndicators(pIndicators, nSize);

}

void CMainFrame::GreyAll()
{
 m_wndDialogBar.GreyAll();
}


EViews CMainFrame::GetViewCombo()
{
	return m_wndDialogBar.GetView();
}

void CMainFrame::ShowDialogBar(int iShowCmd)
{
	m_wndDialogBar.ShowWindow(iShowCmd);
}

void CMainFrame::SetStatusText(CString cText)
	{
	m_wndStatusBar.SetPaneText( 0, LPCTSTR(cText) );
	}


void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
  if (nID == AFX_IDS_IDLEMESSAGE && m_sStatusBarString.GetLength())
  {
    rMessage = m_sStatusBarString;

	CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
	if( pcView )
		{
		rMessage = pcView->GetStatusLine();
		}
	else
		rMessage = "Ready";
  }
  else
  {
    CFrameWnd::GetMessageString(nID, rMessage);
  }
}



void CMainFrame::ProcessViewChange(EViews eNewView)
{
	CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );

	pcView->SaveColumnWidths();

	SetViewCombo(eNewView);
	if( eNewView==eContestantView )
		pcView->ViewContestantList();

	else if( eNewView==eTurnpointView ) 
		pcView->ViewTurnpointList();

	else if( eNewView==eTurnpointGridView ) 
		pcView->ViewTurnpointGrid();

	else if( eNewView==eTaskView  )
		{
		pcView->ViewTasks();
		}
	else if( eNewView==eScoreView  )
		{
		CTime cTime=m_wndDialogBar.GetDayCombo();
		EClass eClass=m_wndDialogBar.GetClassCombo();
		pcView->ViewScores(cTime, eClass);
		}
	else if( eNewView==eTakeoffGridView  )
		{
		EClass eClass=m_wndDialogBar.GetClassCombo();
		pcView->ViewTakeoffGrid(eClass);
		}
	else if( eNewView==eFlightLogView  )
		{
		CTime cTime=m_wndDialogBar.GetDayCombo();
		EClass eClass=m_wndDialogBar.GetClassCombo();
		pcView->ViewFlightLogs(cTime, eClass);
		}
}
void CMainFrame::OnUpdateViewGotoForward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(isaBackFwd<saBackFwdArray.GetSize()-1);
}

void CMainFrame::OnUpdateViewGotoBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(saBackFwdArray.GetSize()>1 && isaBackFwd>0);
}

void CMainFrame::OnViewGotoForward() 
{
	if( isaBackFwd<0 ||
		isaBackFwd+1>saBackFwdArray.GetSize()-1 ) return;

	EViews eNewView=(EViews)saBackFwdArray[isaBackFwd+1];
	isaBackFwd++;

	ProcessViewChange(eNewView);
}

void CMainFrame::OnViewGotoBack() 
{
	ProcessViewChange(PopView());
}

void CMainFrame::PushView(EViews eNewView)
	{

	// Check that we Dont Push a duplicate
	if( saBackFwdArray.GetSize()>0 &&
		saBackFwdArray[isaBackFwd]==eNewView ) return;

	isaBackFwd++;
	saBackFwdArray.SetAtGrow(isaBackFwd,eNewView);

	}

EViews CMainFrame::PopView()
{
	EViews eView=eContestantView;
	if( isaBackFwd<1  ) return eView;
	eView=(EViews)saBackFwdArray[isaBackFwd-1];
	isaBackFwd--;
	if( isaBackFwd<0 ) isaBackFwd=0;
	return eView;
}


void CMainFrame::OnViewDlgbar()
{
	BOOL bIsVis=m_wndDialogBar.IsVisible ();
	ShowPane (&m_wndDialogBar,
					!bIsVis,
					FALSE, TRUE);
	ShowPane (&m_wndWinscoreBar,
			  bIsVis,
			  FALSE, TRUE);
	RecalcLayout ();

	int iVis=(int)!m_wndWinscoreBar.IsVisible();
	AfxGetApp()->WriteProfileInt(REGSECTION, CONTROLBARVISIBLE,	iVis );
}


void CMainFrame::OnUpdateViewDlgbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (!m_wndWinscoreBar.IsVisible ());
}


BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR *pHeader=(NMHDR*)lParam;

//	if(pHeader->code==NM_DBLCLK ) AfxMessageBox("Double Click");
 

	if(pHeader->code==TVN_SELCHANGED )
		{
		CWSTreeItem *pItem=m_wndWinscoreBar.GetSelectedItem();
		if( pItem )
			{
			if( pItem->m_eTreeType==CWSTreeItem::eContestants )
				{
				ProcessViewChange(eContestantView);
				}
			else if( pItem->m_eTreeType==CWSTreeItem::eControlPoints )
				{
				ProcessViewChange(eTurnpointView);
				}

			else if( pItem->m_eTreeType==CWSTreeItem::eTasks )
				{
				ProcessViewChange(eTaskView);
				}
			else if( pItem->m_eTreeType==CWSTreeItem::eDistanceGrid )
				{
				ProcessViewChange(eTurnpointGridView);
				}

			else if( pItem->m_eTreeType==CWSTreeItem::eGridPositions )
				{
				SetClassCombo(pItem->m_eClass);
				ProcessViewChange(eTakeoffGridView);
				}
			else if( pItem->m_eTreeType==CWSTreeItem::eScores )
				{
                SetViewCombo(eScoreView);
				SetClassCombo(pItem->m_eClass);
				SetDateCombo(pItem->m_cTime);
				ProcessViewChange(eScoreView);
				}
			else if( pItem->m_eTreeType==CWSTreeItem::eLogs )
				{
                SetViewCombo(eFlightLogView);
				SetClassCombo(pItem->m_eClass);
				SetDateCombo(pItem->m_cTime);
				ProcessViewChange(eFlightLogView);
				}
			else if( pItem->m_eTreeType==CWSTreeItem::eTask )
				{
				SetClassCombo(pItem->m_eClass);
				SetDateCombo(pItem->m_cTime);
				ProcessViewChange(eTaskView);
				}
			}
		}
	else if(pHeader->code==NM_DBLCLK )
		{
		CWnd* pFocus=GetFocus();
		CTreeCtrl &cTree=m_wndWinscoreBar.GetTreeCtrl();
	
		if( pFocus->m_hWnd == cTree.m_hWnd )
			{

			CWSTreeItem *pItem=m_wndWinscoreBar.GetSelectedItem();
			if( pItem )
				{
				if( pItem->m_eTreeType==CWSTreeItem::eContestants )
					{
					CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
					pcView->ContestInfoContestants();
					}
				else if( pItem->m_eTreeType==CWSTreeItem::eControlPoints )
					{
					CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
					pcView->ContestInfoTurnpoint();
					}

				else if( pItem->m_eTreeType==CWSTreeItem::eTasks )
					{
					//ProcessViewChange(eTaskView);
					}
				else if( pItem->m_eTreeType==CWSTreeItem::eDistanceGrid )
					{
					//ProcessViewChange(eTurnpointGridView);
					}

				else if( pItem->m_eTreeType==CWSTreeItem::eGridPositions )
					{
					CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
					pcView->ContestInfoTakeoffGrid();
					}
				else if( pItem->m_eTreeType==CWSTreeItem::eScores )
					{
					//SetClassCombo(pItem->m_eClass);
					//SetDateCombo(pItem->m_cTime);
					//ProcessViewChange(eScoreView);
					}
				else if( pItem->m_eTreeType==CWSTreeItem::eLogs )
					{
					//SetClassCombo(pItem->m_eClass);
					//SetDateCombo(pItem->m_cTime);
					//ProcessViewChange(eFlightLogView);
					}
				else if( pItem->m_eTreeType==CWSTreeItem::eTask )
					{
					SetClassCombo(pItem->m_eClass);
					SetDateCombo(pItem->m_cTime);
					CWinscoreView*	pcView=(CWinscoreView*)GetActiveView( );
					pcView->ManageTasks();
					}
				}	
			}
		}
	return CFrameWnd::OnNotify(wParam, lParam, pResult);
}


void CMainFrame::LoadTreeCtrl(CWinscoreDoc* pDoc)
{
	m_wndWinscoreBar.LoadTreeCtrl(pDoc);
}


