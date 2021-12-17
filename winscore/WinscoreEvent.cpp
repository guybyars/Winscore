
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WinscoreEvent.cpp: implementation of the CWinscoreEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreEvent.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char TAB = '\t';

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#if 0

IMPLEMENT_SERIAL( CEvent, CEvent, VERSIONABLE_SCHEMA | 2)

CEvent::CEvent()
{
	// This empty constructor should be used by serialization only
}

using namespace Winscore

CEvent::CEvent( Winscore::CEvent* pcEvent)
{
	m_eEventType	=pcEvent->m_eEventType;
	m_cContestNo	=pcEvent->m_cContestNo;
	m_cTime			=pcEvent->m_cTime;
}

CEvent::CEvent(CString cContestNo, CTime cTime, EEventType eEventType)
{
	m_eEventType	=eEventType;
	m_cContestNo	=cContestNo;
	m_cTime			=cTime;
}


CEvent::~CEvent()
{

}

CString Winscore::CEvent::ListLocalText()
{

CString cTemp = m_cTime.Format( "%H : %M : %S - %b %d, %y" );

return cTemp;
}


CString Winscore::CEvent::ListGMTText()
{
CString cTemp = m_cTime.FormatGmt( "%H : %M : %S - %b %d, %y" );
return cTemp;
}

CString Winscore::CEvent::EventText()
{
  if( m_eEventType==eRoll   ) return "Rolled";
  if( m_eEventType==eStart  ) return "Started";
  if( m_eEventType==eFinish ) return "Finished";
  if( m_eEventType==eLanding ) return "Landed";
  if( m_eEventType==eDocumentation ) return "Documented";
 
  return " ";
}


int CALLBACK CompareEvents(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	int iR=0;
	switch( abs(iColumn)-1 )
		{
	case 0:
		iR  =strcmp(  ((Winscore::CEvent*)lParam1)->m_cContestNo,
						 ((Winscore::CEvent*)lParam2)->m_cContestNo  );
		if( iR==0 ) 
			{
			if( ((Winscore::CEvent*)lParam1)->m_cTime <
				((Winscore::CEvent*)lParam2)->m_cTime  ) iR=  -1;
			if( ((Winscore::CEvent*)lParam1)->m_cTime >
				((Winscore::CEvent*)lParam2)->m_cTime  ) iR=   1;
			}
		break;
	case 1:		
		if( ((Winscore::CEvent*)lParam1)->m_eEventType <
			((Winscore::CEvent*)lParam2)->m_eEventType  ) iR=  -1;
		if( ((Winscore::CEvent*)lParam1)->m_eEventType >
			((Winscore::CEvent*)lParam2)->m_eEventType  ) iR=   1;
		if( ((Winscore::CEvent*)lParam1)->m_eEventType ==
			((Winscore::CEvent*)lParam2)->m_eEventType  ) 
			{
			if( ((Winscore::CEvent*)lParam1)->m_cTime <
				((Winscore::CEvent*)lParam2)->m_cTime  ) iR=  -1;
			if( ((Winscore::CEvent*)lParam1)->m_cTime >
				((Winscore::CEvent*)lParam2)->m_cTime  ) iR=   1;
			}
		break;
	case 2:
	case 3:
		if( ((Winscore::CEvent*)lParam1)->m_cTime <
			((Winscore::CEvent*)lParam2)->m_cTime  ) iR=  -1;
		if( ((Winscore::CEvent*)lParam1)->m_cTime >
			((Winscore::CEvent*)lParam2)->m_cTime  ) iR=   1;
		break;
	
		}
	return (iColumn<0)?(-iR):(iR);
}

void Winscore::CEvent::Serialize(CArchive & ar)
{
	WORD w;
	if (ar.IsStoring())
	{
		ar << m_cContestNo;
		SerializeTime( ar, m_cTime);
		ar << (WORD)m_eEventType;
	}
	else
	{
		UINT iSchema=ar.GetObjectSchema( );
		ar >> m_cContestNo;
		if( iSchema==1 )
			ar >> m_cTime;
		else
			SerializeTime( ar, m_cTime);
		ar >> w;
		m_eEventType=(EEventType)w;;
	}

}

Winscore::CEvent* Winscore::CEvent::operator =(Winscore::CEvent* pcNewEvent)
{
return pcNewEvent;
}

int Winscore::CEvent::AddToList(CListCtrl & ListCtrl, BOOL fVisible)
	{

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;

	lvi.lParam = (LPARAM)this;
	int iItem=ListCtrl.InsertItem(&lvi);
	ListCtrl.SetItemText(iItem,0,m_cContestNo );
	ListCtrl.SetItemText(iItem,1,EventText() );
	ListCtrl.SetItemText(iItem,2,ListLocalText() );

//	if( fVisible) ListCtrl.EnsureVisible( iItem, FALSE);
	return iItem;
	}


CString Winscore::CEvent::GetText()
{
	CString str;
	str+=m_cContestNo+TAB;
	str+=EventText()+TAB;
	str+=m_cTime.Format( "%b %d, %y" )+TAB;
	str+=m_cTime.Format( "%H : %M : %S  Local" )+TAB;
	str+=m_cTime.FormatGmt( "%H : %M : %S  GMT" );
	return str;
}


#endif