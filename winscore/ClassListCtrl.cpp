//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ClassListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassListCtrl

CClassListCtrl::CClassListCtrl()
{
   m_pDragImage = NULL;	
   m_bDragging = FALSE;
   m_bDragChecked=FALSE;
   for( int i=0; i<NUMCLASSES; i++ )  m_aiDays[i]=1;
   m_bDays=false;
}

CClassListCtrl::~CClassListCtrl()
{
	if( m_pDragImage ) delete m_pDragImage;
}


BEGIN_MESSAGE_MAP(CClassListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CClassListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CClassListCtrl message handlers



int CClassListCtrl::GetNumCheckedClasses()
{
	int nClasses=0;
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL   );
		if( iItem<0 ) break;
		if(  ListView_GetCheckState( this->m_hWnd,  iItem) ) 
			nClasses++;
		}
	return nClasses;

}

EClass CClassListCtrl::GetCheckedClass(int i)
	{
	int nClasses=0;
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		if(  !ListView_GetCheckState( this->m_hWnd,  iItem) ) continue;
		if( i==nClasses )
			{
			return (EClass)GetItemData( iItem );
			}
		nClasses++;
		}
	return eStandard;
	}


  void CClassListCtrl::SetAvailableClasses( bool bAll )
		{
		LV_ITEM lvi;

		SetExtendedStyle( LVS_EX_CHECKBOXES  );

		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM | LVIF_STATE;
		lvi.state		= LVIS_SELECTED;
		lvi.stateMask	= LVIS_SELECTED;
		lvi.iItem = 0;
		int iItem=0;
		int i=0;
		
		DeleteAllItems( );

		for( int iClass=0; iClass<NUMCLASSES; iClass++)
			{

			if( (bAll || GetClass(iClass).IsActive()) )
				{
				lvi.iItem=iItem;
				int i=InsertItem(&lvi);
				CString strName=GetClass(iClass).GetName();
				SetItemText(iItem,0, strName );
				SetItemData(iItem++, GetClass(iClass).GetType() );
 				if(GetClass(iClass).IsActive())
					ListView_SetCheckState(this->m_hWnd, i, 1);
				if( m_bDays )
					{
					CString cText;
					cText.Format(_T("%d"),m_aiDays[iClass]);
					SetItem( i, 1, LVIF_TEXT, LPCTSTR(cText), 0, 0, 0, 0 );
					}

   				}
			}
		}

  void CClassListCtrl::SetAvailableClasses( int nClasses, CWSClass acClasses[],EContest eContest, bool bAll )
		{
		LV_ITEM lvi;
		bool bChecked[20];

		SetExtendedStyle( LVS_EX_CHECKBOXES  );

		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM | LVIF_STATE;
		lvi.state		= LVIS_SELECTED;
		lvi.stateMask	= LVIS_SELECTED;
		lvi.iItem = 0;
		int iItem=0;
		int i=0;
		
		//Remember any previously checked classes
		int nCount=min(20,GetItemCount());
		for( int i=0; i<nCount; i++)
			{
			bChecked[i]=ListView_GetCheckState( this->m_hWnd,  i)==0?(false):(true);
			}

		DeleteAllItems( );

		for( int iClass=0; iClass<nClasses; iClass++)
			{
			
			if( eContest!=acClasses[iClass].GetContest() ) continue;

			if (bAll || acClasses[iClass].IsActive() )
				{
				CString strName=acClasses[iClass].GetName();

				bool bAlreadyThere=false;
				for( int iCheck=0; iCheck<GetItemCount(); iCheck++ )
					{
					if( acClasses[iClass].GetType()==(EClass)GetItemData( iCheck ) )
						{
						bAlreadyThere=true;
						}
					}
				if( bAlreadyThere ) continue;
				lvi.iItem=iItem;
				int i=InsertItem(&lvi);
				SetItemText(iItem,0, strName );
				SetItemData(iItem++, acClasses[iClass].GetType() );
   				}
			}

		for( int iChecked=0; iChecked<GetItemCount(); iChecked++ )
			{
			for( int iC=0; iC<nClasses; iC++)
				{
				if( acClasses[iC].IsActive() &&
					acClasses[iC].GetType()==(EClass)GetItemData(iChecked) )
					{
					ListView_SetCheckState(this->m_hWnd, iChecked, 1);
					}
				}
			}

		//Reset previous checks
		for( int i=0; i<nCount; i++)
			{
			if( bChecked[i] ) ListView_SetCheckState(this->m_hWnd, i, 1);
			}

		}

bool CClassListCtrl::IsChecked(EClass eClass)
	{
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		if ((EClass)GetItemData( iItem )==eClass &&
			ListView_GetCheckState( this->m_hWnd,  iItem) ) return true;
		}
	return false;
	}

void CClassListCtrl::CheckClass(CString strClass)
	{
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		CString str=GetItemText(iItem,0);
		str.MakeLower();
		strClass.MakeLower();
		if( str.Find(strClass)>=0 || (str.Find("fai")>=0 && strClass.Find("fai")>=0) )
			{
			ListView_SetCheckState(this->m_hWnd, iItem, 1);
			break;
			}
		}
	return;
	}

bool CClassListCtrl::GetSelectedClass(EClass &eClass)
{
	int iItem=GetNextItem(-1, LVNI_SELECTED);
	if( iItem<0 ) return false;
	eClass=(EClass)GetItemData( iItem );
	return true;
}

void CClassListCtrl::AddColumn(CString cColumnTitle)
{
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.pszText = (TCHAR*)LPCTSTR(cColumnTitle);
	lvc.cx =  200;
	lvc.fmt =LVCFMT_LEFT;
	InsertColumn(0,&lvc);


	if( m_bDays )
		{
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = _T("Contest Day");
		lvc.cx =  200;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(1,&lvc);
		}
}


void CClassListCtrl::OnBeginDrag(NMHDR* pnmhdr, LRESULT* pResult)
{ 
	//RECORD THE INDEX OF THE ITEM BEIGN DRAGGED AS m_nDragIndex
	m_nDragIndex = ((NM_LISTVIEW *)pnmhdr)->iItem;
	
	m_bDragChecked=ListView_GetCheckState( this->m_hWnd,  m_nDragIndex);

	//CREATE A DRAG IMAGE FROM THE CENTER POINT OF THE ITEM IMAGE
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	m_pDragImage = CreateDragImage(m_nDragIndex, &pt);
	m_pDragImage->BeginDrag(0, CPoint (8, 8));
	m_pDragImage->DragEnter(
		GetDesktopWindow(), ((NM_LISTVIEW *)pnmhdr)->ptAction);
	
	//SET THE FLAGS INDICATING A DRAG IN PROGRESS
	m_bDragging = TRUE;
//	m_hDropItem = NULL;
	m_nDropIndex = -1;
//	m_pDropWnd = &m_ListControl;
	m_pDropWnd = this;
	
	//CAPTURE ALL MOUSE MESSAGES IN CASE THE USER DRAGS OUTSIDE OF THE VIEW
	SetCapture();
}


void CClassListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bDragging )
	{
		m_ptDropPoint = point;
		ClientToScreen(&m_ptDropPoint);
		
		//MOVE THE DRAG IMAGE
		m_pDragImage->DragMove(m_ptDropPoint);
		
		//TEMPORARILY UNLOCK WINDOW UPDATES
		m_pDragImage->DragShowNolock(FALSE);
		
		//CONVERT THE DROP POINT TO CLIENT CO-ORDIANTES
		m_pDropWnd = WindowFromPoint(m_ptDropPoint);
		m_pDropWnd->ScreenToClient(&m_ptDropPoint);
		
		//LOCK WINDOW UPDATES
		m_pDragImage->DragShowNolock(TRUE);
	}
	
//	CView::OnMouseMove(nFlags, point);
}

void CClassListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bDragging )
	{
		//RELEASE THE MOUSE CAPTURE AND END THE DRAGGING
		::ReleaseCapture();
		m_bDragging = FALSE;
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
		
		//GET THE WINDOW UNDER THE DROP POINT
		CPoint pt(point);
		ClientToScreen(&pt);
		m_pDropWnd = WindowFromPoint(pt);
		
		//DROP THE ITEM ON THE LIST
		if( m_pDropWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) )
			DropItemOnList();

		delete m_pDragImage;//this delete call will stop leak
		m_pDragImage=NULL;

	}
//	CView::OnLButtonUp(nFlags, point);
}


void CClassListCtrl::DropItemOnList()
{
	//GET THE DROP INDEX
	m_ptDropPoint.y += 10;
	m_nDropIndex = HitTest(m_ptDropPoint);
	
	//GET INFORMATION ON THE DRAGGED ITEM BY SETTING AN LV_ITEM STRUCTURE
	//AND THEN CALLING GetItem TO FILL IT IN
	TCHAR szLabel[256];
	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
	lvi.stateMask = LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
	lvi.pszText = szLabel;
	lvi.iItem = m_nDragIndex;
	lvi.cchTextMax = 255;
	GetItem(&lvi);
	
	//INSERT THE DROPPED ITEM
	if(m_nDropIndex < 0) m_nDropIndex = GetItemCount();
	lvi.iItem = m_nDropIndex;
	InsertItem(&lvi);
	
	//FILL IN ALL OF THE COLUMNS
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	lvi.mask = LVIF_TEXT;
	lvi.iItem = m_nDropIndex;
	//INDEX OF DRAGGED ITEM WILL CHANGE IF ITEM IS DROPPED ABOVE ITSELF
	if(m_nDragIndex > m_nDropIndex) m_nDragIndex++;
	for(int col=1; col < nColumnCount; col++)
	{
		strcpy_s(lvi.pszText, 255, (LPCTSTR)(GetItemText(m_nDragIndex,
			col)));
		lvi.iSubItem = col;
		SetItem(&lvi);
	}

	ListView_SetCheckState(this->m_hWnd, m_nDropIndex, m_bDragChecked);

	
	//DELETE THE ITEM THAT WAS DRAGGED FROM ITS ORIGINAL LOCATION
	DeleteItem(m_nDragIndex);
}


void CClassListCtrl::IncrementSelectedDay(bool bIncrease)
	{
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
	   return;
	else
		{
		while (pos)
			{
			int nItem = GetNextSelectedItem(pos);
			int iClass=GetItemData(nItem);
			ASSERT( iClass>=0 && iClass<NUMCLASSES );
			if( bIncrease )
				m_aiDays[iClass]++;
			else
				{
				m_aiDays[iClass]--;
				m_aiDays[iClass]=max( m_aiDays[iClass], 1);
				}
			CString cText;
			cText.Format(_T("%d"),m_aiDays[iClass]);
			SetItem( nItem, 1, LVIF_TEXT, LPCTSTR(cText), 0, 0, 0, 0 );
			}
		}

}

int CClassListCtrl::GetDay(EClass eClass)
	{
	return m_aiDays[(int)eClass];
	}

void CClassListCtrl::SetDayDisplay(bool b)
{
	m_bDays=b;
}

void CClassListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int iSelectedItem = HitTest(point);
	if( iSelectedItem>=0 ) 
		SetItemState(iSelectedItem, LVIS_SELECTED, LVIS_SELECTED );
	CListCtrl::OnLButtonDown(nFlags, point);
}

