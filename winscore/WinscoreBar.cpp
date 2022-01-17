// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "WINSCORE.h"
#include "WinscoreBar.h"
#include "WStreeitem.h"
#include "WinscoreDoc.h"
#include "Global_Prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWinscoreBar

BEGIN_MESSAGE_MAP(CWinscoreBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_PARENTNOTIFY()
	ON_WM_NOTIFYFORMAT()
	ON_WM_CONTEXTMENU()
	ON_WM_NCRBUTTONDOWN()
//	ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinscoreBar construction/destruction

CWinscoreBar::CWinscoreBar()
{
	m_hRoot=NULL;
	m_hTurnpointItem=NULL;
	m_hContestantItem=NULL;
}

CWinscoreBar::~CWinscoreBar()
{
if( m_hRoot ) DeleteTreeChildren(m_hRoot,true);
}

/////////////////////////////////////////////////////////////////////////////
// CWinscoreBar message handlers

int CWinscoreBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_HASBUTTONS;							
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0( "Failed to create workspace view\n" );
		return -1;      // fail to create
	}
	
	BOOL bSuccess=m_cImageList.Create( IDB_TREE_BMPS, 16, 1, RGB(255, 255, 255 ) );
	GetTreeCtrl().SetImageList(&m_cImageList, TVSIL_NORMAL);

	// Setup trees content:
	m_hRoot = m_wndTree.InsertItem (_T("No Contest Defined"));
	GetTreeCtrl().SetItemData(m_hRoot,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eContestRoot));

	return 0;
}

void CWinscoreBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWinscoreBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}


void CWinscoreBar::OnParentNotify(UINT message, LPARAM lParam)
{
	CDockablePane::OnParentNotify(message, lParam);

}




UINT CWinscoreBar::OnNotifyFormat(CWnd *pWnd, UINT nCommand)
{
	// TODO: Add your message handler code here and/or call default

	return CDockablePane::OnNotifyFormat(pWnd, nCommand);
}


BOOL CWinscoreBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//NMHDR *pHeader=(NMHDR*)lParam;

	return CDockablePane::OnNotify(wParam, lParam, pResult);
}


BOOL CWinscoreBar::OnShowControlBarMenu(CPoint )
{
	return FALSE;
}

void CWinscoreBar::OnContextMenu(CWnd* /*pWnd*/, CPoint ptMousePos )
{
	// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
		ptMousePos = (CPoint) GetMessagePos();

	ScreenToClient(&ptMousePos);

	UINT uFlags;
	HTREEITEM htItem;
	
	htItem = m_wndTree.HitTest( ptMousePos, &uFlags );

	if( htItem == NULL )
		return;
	
	//m_hActiveItem = htItem;
	UINT uMenu=0;	

	CWSTreeItem *pItem=(CWSTreeItem*)GetTreeCtrl().GetItemData(htItem);
	if( pItem )
		{
		if( pItem->m_eTreeType==CWSTreeItem::eContestants )
			{
			uMenu=IDR_CONTESTANT_TREE_MENU;
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eContestRoot )
			{
			uMenu=IDR_CONTEST_TREE_MENU;
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eControlPoints )
			{
			uMenu=IDR_CONTROL_TREE_MENU;
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eTasks )
			{
			//AfxMessageBox("eTasks Contest Menu");
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eDistanceGrid )
			{
			//AfxMessageBox("eDistanceGrid Contest Menu");
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eGridPositions )
			{
			//AfxMessageBox("eGridPositions Contest Menu");
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eScores )
			{
			uMenu=IDR_SCORE_TREE_MENU;
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eLogs )
			{
			uMenu=IDR_LOG_TREE_MENU;
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eTask )
			{
			//AfxMessageBox("eTask Contest Menu");
			}
		else if( pItem->m_eTreeType==CWSTreeItem::eSetup )
			{
			uMenu=IDR_SETUP_TREE_MENU;
			}
		}

	if( uMenu==0 ) return;
	
	// the font popup is stored in a resource
	CMenu menu;
	CMenu* pPopup;
	menu.LoadMenu(uMenu);
	pPopup = menu.GetSubMenu(0);
	ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );

}


void CWinscoreBar::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default




	CDockablePane::OnNcRButtonDown(nHitTest, point);
}


CTreeCtrl& CWinscoreBar::GetTreeCtrl(void)
{
	return m_wndTree;
}


int CWinscoreBar::LoadTreeCtrl(CDocument *pD)
{
	CWinscoreDoc* pDoc=(CWinscoreDoc*)pD;
	HTREEITEM iTtem;
	HTREEITEM iExpandMe=0;
	CTime cToday=CTime::GetCurrentTime();

	// Select the root, so we won't get wild onnotify messages when the child branches are deleted
	if( m_hRoot ) GetTreeCtrl().Select( m_hRoot, TVGN_CARET );

	if( m_hRoot ) DeleteTreeChildren(m_hRoot,true);

	m_hRoot = m_wndTree.InsertItem (pDoc->m_strContestName);
	GetTreeCtrl().SetItemData(m_hRoot,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eContestRoot));

	HTREEITEM m_hSetup= m_wndTree.InsertItem (_T("Contest Setup"),m_hRoot);;
	GetTreeCtrl().SetItemData(m_hSetup,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eSetup));
	GetTreeCtrl().SetItemImage(m_hSetup, 13, 13 );
	iExpandMe=m_hSetup;

	m_hContestantItem= m_wndTree.InsertItem (_T("Contestants"),m_hSetup);;
	GetTreeCtrl().SetItemData(m_hContestantItem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eContestants));
	GetTreeCtrl().SetItemImage(m_hContestantItem, 6, 6 );

    m_hTurnpointItem = m_wndTree.InsertItem (_T("Control Points"),m_hSetup);
	GetTreeCtrl().SetItemData(m_hTurnpointItem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eControlPoints));
	GetTreeCtrl().SetItemImage(m_hTurnpointItem, 2, 2 );

	iTtem = m_wndTree.InsertItem (_T("Distance Grid"),m_hSetup);
	GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eDistanceGrid));
	GetTreeCtrl().SetItemImage(iTtem, 1, 1 );

	iTtem = m_wndTree.InsertItem (_T("Takeoff Grid Positions"),m_hSetup);
	GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eNone));
	LoadClassesInTree(iTtem,false,CWSTreeItem::eGridPositions, 0);
	GetTreeCtrl().SetItemImage(iTtem, 5, 5 );

	HTREEITEM hPracticeDays = m_wndTree.InsertItem (_T("Practice Days"),m_hRoot);
	GetTreeCtrl().SetItemData(hPracticeDays,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eNone));
	GetTreeCtrl().SetItemImage(hPracticeDays, 12, 12 );


	for( int i=pDoc->m_iNumPracticeDays-1; i>=0; i-- )
    	{
    	CString s = pDoc->m_caPracticeDays[i].Format( _T("Practice Day %A, %B %d, %Y") );
		iTtem = m_wndTree.InsertItem (s,hPracticeDays);
	    GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eNone, pDoc->m_caPracticeDays[i]));
		LoadClassesInTree(iTtem, true,CWSTreeItem::eNone, pDoc->m_caPracticeDays[i]);
		GetTreeCtrl().SetItemImage(iTtem, 10, 10 );
		if( DatesEqual(cToday, pDoc->m_caPracticeDays[i]) ) iExpandMe=iTtem;
    	}

    for( int i=0;i<pDoc->m_iNumContestDays;i++ )
    	{
    	CString s = pDoc->m_caContestDays[i].Format( _T("%A, %B %d, %Y") );
		iTtem = m_wndTree.InsertItem (s,m_hRoot);
	    GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eNone, pDoc->m_caContestDays[i]));
		LoadClassesInTree(iTtem, true,CWSTreeItem::eNone, pDoc->m_caContestDays[i]);
		GetTreeCtrl().SetItemImage(iTtem, 10, 10 );
		if( DatesEqual(cToday, pDoc->m_caContestDays[i]) ) iExpandMe=iTtem;
    	}

	GetTreeCtrl().Expand(m_hRoot, TVE_EXPAND );		
	GetTreeCtrl().Expand(iExpandMe, TVE_EXPAND );
	GetTreeCtrl().Select( m_hRoot, TVGN_CARET );

	return 0;
}


void CWinscoreBar::DeleteTreeChildren(HTREEITEM hItem, bool bDeleteParent)
{
	if( hItem==NULL ) return;

	if( GetTreeCtrl().m_hWnd==NULL ) return;

	if( GetTreeCtrl().ItemHasChildren( hItem ) )
		{
	 	HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetTreeCtrl().GetChildItem(hItem);

		while(hChildItem != NULL)
			{
			hNextItem = GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
			DeleteTreeChildren( hChildItem, true  );
			hChildItem = hNextItem;
			}
		}

	if( bDeleteParent ) 
		{
		DeleteWSTreeItem(hItem);
		}

}

void CWinscoreBar::DeleteWSTreeItem( HTREEITEM hItem)
	{
	if( !hItem ) return;
	CWSTreeItem *pItem=(CWSTreeItem*)GetTreeCtrl().GetItemData(hItem);
	delete pItem;
	GetTreeCtrl().DeleteItem( hItem );
	}



void CWinscoreBar::OnDestroy()
{
	CDockablePane::OnDestroy();

	if( m_hRoot ) DeleteTreeChildren(m_hRoot,true);
}


CWSTreeItem* CWinscoreBar::GetSelectedItem(void)
	{
		HTREEITEM hItem=GetTreeCtrl().GetSelectedItem();
		return (CWSTreeItem*)GetTreeCtrl().GetItemData(hItem);
	}


void CWinscoreBar::LoadClassesInTree(HTREEITEM hItem, bool bkids, CWSTreeItem::ETreeType eType, CTime cTime)
{
		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			{
			if( GetClass(iClass).IsActive() )
    			{
				HTREEITEM iTtem = m_wndTree.InsertItem (GetClass(iClass).GetName(),hItem);
				GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(eType, cTime, GetClass(iClass).GetType() ));
				GetTreeCtrl().SetItemImage(iTtem, 9, 9 );
				if( bkids ) LoadDateClassTreeChildren(iTtem, cTime,  GetClass(iClass).GetType());
				GetTreeCtrl().Expand(iTtem, TVE_EXPAND );
    			}
			}
}


void CWinscoreBar::LoadDateClassTreeChildren(HTREEITEM hItem, CTime cDate, EClass eClass)
{
	HTREEITEM iTtem;

	iTtem = m_wndTree.InsertItem (_T("Task"),hItem);
	GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eTask, cDate, eClass  ));
	GetTreeCtrl().SetItemImage(iTtem, 4, 4 );

	iTtem = m_wndTree.InsertItem (_T("Scores"),hItem);
	GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eScores, cDate, eClass  ));
	GetTreeCtrl().SetItemImage(iTtem, 7, 7 );

	iTtem = m_wndTree.InsertItem (_T("Flight Logs"),hItem);
	GetTreeCtrl().SetItemData(iTtem,(DWORD_PTR)new CWSTreeItem(CWSTreeItem::eLogs, cDate, eClass ));
	GetTreeCtrl().SetItemImage(iTtem, 8, 8 );
}
