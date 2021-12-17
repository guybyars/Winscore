// ScoreSheetListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ScoreSheetListCtrl.h"
#include "wsclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScoreSheetListCtrl

CScoreSheetListCtrl::CScoreSheetListCtrl()
{
}

CScoreSheetListCtrl::~CScoreSheetListCtrl()
{
}


BEGIN_MESSAGE_MAP(CScoreSheetListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CScoreSheetListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScoreSheetListCtrl message handlers

void CScoreSheetListCtrl::AddColumns(bool bClassesOnly )
{
	SetExtendedStyle( LVS_EX_CHECKBOXES  );


	int nColumnCount = GetHeaderCtrl()->GetItemCount();
	// Delete all of the columns.
	for (int i=0;i < nColumnCount;i++)
	{
	  DeleteColumn(0);
	}

	if( bClassesOnly )
		{
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = _T("Class");
		lvc.cx =  100;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(0,&lvc);
		}
	else
		{
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = _T("Date");
		lvc.cx =  100;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(0,&lvc);

		lvc.iSubItem = 0;
		lvc.pszText = _T("Class");
		lvc.cx =  100;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(1,&lvc);

		lvc.iSubItem = 0;
		lvc.pszText = _T("Status");
		lvc.cx =  100;
		lvc.fmt =LVCFMT_LEFT;
		InsertColumn(2,&lvc);
		}

}

void CScoreSheetListCtrl::LoadTasks(CTaskList &cTaskList)
	{
	LV_ITEM lvi;
	CTask* pcTask;

	SetExtendedStyle( LVS_EX_CHECKBOXES  );

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM | LVIF_STATE;
	lvi.state		= LVIS_SELECTED;
	lvi.stateMask	= LVIS_SELECTED;
	lvi.iItem = 0;
	int iItem=0;
	int i=0;
	
	DeleteAllItems( );

	lvi.iSubItem = 0;
	lvi.mask = 0;

	//  Load up the listbox
	POSITION pos = cTaskList.GetHeadPosition();
	while( pos!=NULL)
		{
		pcTask=cTaskList.GetNext(pos);
		if( !pcTask ) continue;

		lvi.iItem=iItem;
		int i=InsertItem(&lvi);
		SetItemText(iItem,0, pcTask->DateText() );
		SetItemText(iItem,1, GetClass(pcTask->m_eClass).GetName() );
		SetItemText(iItem,2, pcTask->GetStatusText() );
		SetItemData(iItem++, (long)pcTask );
		}

	}

  void CScoreSheetListCtrl::LoadAvailableClasses(  )
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
			if( ( GetClass(iClass).IsActive()) )
				{
				lvi.iItem=iItem;
				int i=InsertItem(&lvi);
				CString strName=GetClass(iClass).GetName();
				SetItemText(iItem,0, strName );
				SetItemData(iItem++, GetClass(iClass).GetType() );
   				}
			}
		}


int CScoreSheetListCtrl::GetNumChecked()
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

CTask* CScoreSheetListCtrl::GetCheckedTask(int i)
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
			return (CTask*)GetItemData( iItem );
			}
		nClasses++;
		}
	return NULL;
	}

EClass CScoreSheetListCtrl::GetCheckedClass(int i)
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

void CScoreSheetListCtrl::LoadContestDates(int nDates, CTime caTimes[])
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

	lvi.iSubItem = 0;
	lvi.mask = 0;

	//  Load up the listbox
	for( i=0; i<nDates; i++ )
		{
		lvi.iItem=iItem;
		int i=InsertItem(&lvi);

		SetItemText(iItem,0, caTimes[i].Format(_T("%b %d, %y")) );
		SetItemText(iItem, 1, _T("All") );
		SetItemText(iItem, 2, _T("-") );


		SetItemData(iItem++, (DWORD_PTR)caTimes[i].GetTime() );
		}

	}

CTime CScoreSheetListCtrl::GetCheckedDate(int i)
{
	int nDates=0;
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		if(  !ListView_GetCheckState( this->m_hWnd,  iItem) ) continue;
		if( i==nDates )
			{
			return CTime(GetItemData( iItem ));
			}
		nDates++;
		}
	return CTime(0);
}

void CScoreSheetListCtrl::CheckToday()
{
	CTime cToday = CTime::GetCurrentTime();
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		CTime cCheck=CTime(GetItemData( iItem ));



		if( cCheck.GetDay()== cToday.GetDay() &&
			cCheck.GetMonth()== cToday.GetMonth() &&
			cCheck.GetYear()== cToday.GetYear() )
			{
			ListView_SetCheckState( this->m_hWnd,  iItem, TRUE);
			return;
			}
		}
}


void CScoreSheetListCtrl::CheckAll()
{
	int iItem= -1;
	while(1)
		{
		iItem=GetNextItem( iItem, LVNI_ALL  );
		if( iItem<0 ) break;
		ListView_SetCheckState( this->m_hWnd,  iItem, TRUE);
		}
}
