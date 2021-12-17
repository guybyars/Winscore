//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Winscorethis->cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreListCtrl.h"
#include "ListVwEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinscoreListCtrl

CWinscoreListCtrl::CWinscoreListCtrl()
{
	m_cxClient = 0;
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_fReport=TRUE;
}

CWinscoreListCtrl::~CWinscoreListCtrl()
{
}


BEGIN_MESSAGE_MAP(CWinscoreListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CWinscoreListCtrl)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinscoreListCtrl message handlers

// offsets for first and other columns
#define OFFSET_FIRST	2
#define OFFSET_OTHER	6

void CWinscoreListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//	CListCtrl& ListCtrl=GetListCtrl();

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	UINT uiFlags = ILD_TRANSPARENT;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	COLORREF clrTextSave, clrBkSave;
//	COLORREF clrImage = m_clrBkgnd;
	static _TCHAR szBuff[MAX_PATH];
	LPCTSTR pszText;

// get item data

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	lvi.stateMask = 0xFFFF;		// get all state flags
	this->GetItem(&lvi);

	BOOL bSelected = (bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

// set colors if item is selected

	CRect rcAllLabels;
	this->GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

	CRect rcLabel;
	this->GetItemRect(nItem, rcLabel, LVIR_LABEL);

	rcAllLabels.left = rcLabel.left;
	if (rcAllLabels.right<m_cxClient)
		rcAllLabels.right = m_cxClient;

	if (bSelected)
	{
		clrTextSave = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrBkSave = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(rcAllLabels, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
		{
		pDC->FillRect(rcAllLabels, &CBrush(m_clrTextBk));
		}

// set color and mask for the icon

	if (lvi.state & LVIS_CUT)
	{
		uiFlags |= ILD_BLEND50;
	}
	else if (bSelected)
	{
		uiFlags |= ILD_BLEND50;
	}

// draw item label

	this->GetItemRect(nItem, rcItem, LVIR_LABEL);

	pszText = MakeShortString(pDC, szBuff,
				rcItem.right-rcItem.left, 2*OFFSET_FIRST);

	rcLabel = rcItem;
	rcLabel.left += OFFSET_FIRST;
	rcLabel.right -= OFFSET_FIRST;

	pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
				   
	for(int nColumn = 1; this->GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		int nRetLen = this->GetItemText(nItem, nColumn,
						szBuff, sizeof(szBuff));
		if (nRetLen == 0)
			continue;

		pszText = MakeShortString(pDC, szBuff,
			rcItem.right - rcItem.left, 2*OFFSET_OTHER);

		UINT nJustify = DT_LEFT;

		if(pszText == szBuff)
		{
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify = DT_CENTER;
				break;
			default:
				break;
			}
		}

		rcLabel = rcItem;
		rcLabel.left += OFFSET_OTHER;
		rcLabel.right -= OFFSET_OTHER;

		pDC->DrawText(pszText, -1, rcLabel,
			nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	}

// draw focus rectangle if item has focus

	if (lvi.state & LVIS_FOCUSED && bFocus)
		pDC->DrawFocusRect(rcAllLabels);

// set original colors if item was selected

	if (bSelected)
	{
        pDC->SetTextColor(clrTextSave);
		pDC->SetBkColor(clrBkSave);
	}
}

LPCTSTR CWinscoreListCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}

	lstrcat(szShort, szThreeDots);
	return(szShort);
}

void CWinscoreListCtrl::OnSize(UINT nType, int cx, int cy) 
{	
	m_cxClient = cx;
	CListCtrl::OnSize(nType, cx, cy); 
}


void CWinscoreListCtrl::RepaintSelectedItems()
{

	CListCtrl& ListCtrl = *this;
	CRect rcItem, rcLabel;

// invalidate focused item so it can repaint properly

	int nItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

	if(nItem != -1)
	{
		ListCtrl.GetItemRect(nItem, rcItem, LVIR_BOUNDS);
		ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
		rcItem.left = rcLabel.left;

		InvalidateRect(rcItem, FALSE);
	}

// if selected items should not be preserved, invalidate them

	if(!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = ListCtrl.GetNextItem(-1, LVNI_SELECTED);
			nItem != -1; nItem = ListCtrl.GetNextItem(nItem, LVNI_SELECTED))
		{
			ListCtrl.GetItemRect(nItem, rcItem, LVIR_BOUNDS);
			ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
			rcItem.left = rcLabel.left;

			InvalidateRect(rcItem, FALSE);
		}
	}

// update changes 

	UpdateWindow();
}


void CWinscoreListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
	RepaintSelectedItems();	
}

void CWinscoreListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	RepaintSelectedItems();	
}


void CWinscoreListCtrl::AddTwoStringItem( CString cstr1, CString cstr2)
	{
	if( !m_fReport || m_hWnd==NULL ) return;
	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = 0;

    int iItem=GetItemCount();

	//  Load up the listbox
	lvi.iItem = iItem;
	InsertItem(&lvi);
	SetItemText(iItem,0,cstr1);
	SetItemText(iItem,1,cstr2);
	}

void CWinscoreListCtrl::EnsureVisibleCenter( int iItem )
	{
	CListCtrl::EnsureVisible( iItem+(GetCountPerPage()/2) , FALSE);
	CListCtrl::EnsureVisible( iItem-(GetCountPerPage()/2), FALSE);
	CListCtrl::EnsureVisible( iItem, FALSE);
	}

void CWinscoreListCtrl::EnsureVisibleTop( int iItem )
	{
	CListCtrl::EnsureVisible( iItem+(GetCountPerPage()) , FALSE);
	CListCtrl::EnsureVisible( iItem-1, FALSE);
	}





void CWinscoreListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CWinscoreListCtrl::DeselectAll()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
	   return;
	else
	{
	   while (pos)
	   {
		  int nItem = GetNextSelectedItem(pos);
		  SetItemState(nItem, 0, LVIS_SELECTED );
//		  SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
	   }
	}

}

void CWinscoreListCtrl::Select(int i)
{
	SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	SetItemState(i, LVIS_FOCUSED, LVIS_FOCUSED);
}

