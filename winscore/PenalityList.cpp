//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PenalityList.cpp: implementation of the CPenalityList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "PenalityList.h"
#include "global_prototypes.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPenalityList::CPenalityList()
{
m_pos=NULL;
}

CPenalityList::~CPenalityList()
{
	Purge();
}


CPenalityList& CPenalityList::operator =(CPenalityList& cInputPenalityList)
{
	CPenality *pcNewPenality, *pcPenality;

	//  Clean out the destination  
	Purge();

	if( cInputPenalityList.IsEmpty() !=0  )  return *this;

	POSITION pos = cInputPenalityList.GetHeadPosition();

	while(1)
		{
		pcPenality=(CPenality*)cInputPenalityList.GetNext(pos);
		if( pcPenality ) 
			{
			pcNewPenality=new CPenality(pcPenality);
			(CPenality*)AddTail(pcNewPenality);
			}
		if( pos==NULL ) break;
		}

	return *this;

}

void CPenalityList::Purge()
	{

	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(1)
		{
		CPenality* pcPenality=GetNext(pos);
		if( pcPenality ) delete pcPenality;
		if( pos==NULL ) break;
		}
	RemoveAll();
	}

CPenality* CPenalityList::GetNext(POSITION& pos)
{
	return (CPenality*)CObList::GetNext(pos);
}

void CPenalityList::AddToList(CPenality* pcAddPenality)
	{
	//  If a penality with this date and code is there already, remove it
	CPenality* pcNewPenality=new CPenality(pcAddPenality);

	/*  commented out to allow for duplicates

	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CPenality* pcPenality=(CPenality*)GetNext(pos);
			if( pcPenality->m_cContestNo==pcAddPenality->m_cContestNo	&&
				pcPenality->m_ePenality	==pcAddPenality->m_ePenality	&&
				pcPenality->m_cDate		==pcAddPenality->m_cDate		)
				{
				delete pcPenality;
				SetAt( thispos, (CPenality*)pcNewPenality );
				return;
				}
			}
		}
		*/

//  If we got here, it must be a new one
	AddTail( (CPenality*)pcNewPenality );
	}

#define NUM_COLUMNS 6

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("ID"), _T("Date"), _T("Type"), _T("Penalty"), _T("Code"), _T("Explaination")};


static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	40, 90, 90, 120, 50, 200
};

void CPenalityList::CreateControlColumns(CListCtrl& ListCtrl)
{
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(  int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gszColumnLabel[i];
		lvc.cx =  _gnColumnWidth[i];
		lvc.fmt =_gnColumnFmt[i];
		ListCtrl.InsertColumn(i,&lvc);
	}

}

void CPenalityList::LoadPenalityList(CListCtrl& ListCtrl)
{
//	char buff[50];

	CPenality* pcPenality;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcPenality=GetNext(pos);
		if( pcPenality )
			{
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcPenality;
			ListCtrl.InsertItem(&lvi);
			ListCtrl.SetItemText(iItem,0,pcPenality->m_cContestNo );
			ListCtrl.SetItemText(iItem,1,pcPenality->GetDateText() );
			ListCtrl.SetItemText(iItem,2,pcPenality->GetTypeText() );
			ListCtrl.SetItemText(iItem,3,pcPenality->GetPenaltityText());
			ListCtrl.SetItemText(iItem,4,pcPenality->m_cPenalityCode);
			ListCtrl.SetItemText(iItem,5,pcPenality->m_cPenalityReason);
			ListCtrl.SetItemData(iItem, (LPARAM)pcPenality);
			iItem++;
			}
		}
	}

void CPenalityList::RemoveByPointer(CPenality * pcRemovePenality)
{
	if( IsEmpty() !=0  )  return;
	POSITION thispos, pos = GetHeadPosition();
	while(pos)
		{
		thispos=pos;
		CPenality* pcPenality=GetNext(pos);
		if( pcPenality==pcRemovePenality ) 
			{
			delete pcPenality;
			RemoveAt(thispos);
			return;
			}
		}
}

CPenality* CPenalityList::Get(	CString& cContestNo, 
								CTime& cDate, 
								EPenality ePenality)
	{

	m_pos = GetHeadPosition();
	while( m_pos!=NULL)
		{
		CPenality* pcPenality=GetNext(m_pos);
		ASSERT(pcPenality);
		if( pcPenality->m_cContestNo==cContestNo	&&
			DatesEqual(pcPenality->m_cDate,cDate)	&&
			pcPenality->m_ePenality	==ePenality			)
			{
			return pcPenality;
			}
		}
	return NULL;
	}

int CPenalityList::GetContestTotal(	CString& cContestNo )
	{
	double dTotal=0.0;
	m_pos = GetHeadPosition();
	while( m_pos!=NULL)
		{
		CPenality* pcPenality=GetNext(m_pos);
		ASSERT(pcPenality);
		if( pcPenality->m_cContestNo==cContestNo		&&
			pcPenality->m_ePenality	==eContestPenalty			)
			{
			dTotal+=pcPenality->m_dPenality;
			}
		}
	return (int)Roundoff(dTotal);
	}

CPenality* CPenalityList::GetNext(	CString&	cContestNo, 
									CTime&		cDate, 
									EPenality	ePenality,
									bool		bRewind)
	{

	if( bRewind ) 
		m_pos = GetHeadPosition();

	while( m_pos!=NULL)
		{
		CPenality* pcPenality=GetNext(m_pos);
		ASSERT(pcPenality);
		if( pcPenality->m_cContestNo==cContestNo	&&
			DatesEqual(pcPenality->m_cDate,cDate)	&&
			pcPenality->m_ePenality	==ePenality			)
			{
			return pcPenality;
			}
		}
	return NULL;
	}

CString CPenalityList::DayText(CString & cContestNo, CTime & cDate)
{
CPenality* pcPenality;
EPenality  ePen[4] = {	eDayPoint, 
						eDayPercent, 
						eComment,
						eContestPenalty };
BOOL fAppend=FALSE;

CString cText;

for (int i=0; i<4; i++ )
	{
	bool bFirst=true;
	while(1)
		{
		pcPenality=GetNext( cContestNo, cDate, ePen[i],bFirst);
		if(pcPenality) 
			{
			if( fAppend ) cText+=" & ";
			cText+=pcPenality->m_cPenalityCode+"-"+pcPenality->m_cPenalityReason;
			fAppend=TRUE;
			}
		else
			break;
		bFirst=false;
		}
	}
return cText;

}


void CPenalityList::AddCodes(CString& cContestNo, CTime &cDate, int *pnCodes, CStringArray& cCodes, CStringArray& cReasons)
{
#define  nCodes 4

CPenality* pcPenality;
EPenality  ePen[nCodes] = {	eDayPoint, 
							eDayPercent, 
							eComment,	
							eContestPenalty };
CString cText;

for (int i=0; i<nCodes; i++ )
	{
	bool bFirst=true;
	while(1)
		{
		pcPenality=GetNext( cContestNo, cDate, ePen[i],bFirst);
		if(pcPenality) 
			{
			(*pnCodes)++;
			cCodes.Add(pcPenality->m_cPenalityCode);
			cReasons.Add(pcPenality->m_cPenalityReason);
			}
		else
			break;
		bFirst=false;
		}
	}
}


void CPenalityList::Merge(CPenalityList &cInputPenalityList)
{

	CPenality *pcNewPenality, *pcPenality;

	if( cInputPenalityList.IsEmpty() !=0  )  return ;

	POSITION pos = cInputPenalityList.GetHeadPosition();

	while(1)
		{
		pcPenality=(CPenality*)cInputPenalityList.GetNext(pos);
		if( pcPenality  ) 
			{
			Remove(*pcPenality);
			pcNewPenality=new CPenality(pcPenality);
			(CPenality*)AddTail(pcNewPenality);
			}
		if( pos==NULL ) break;
		}

	return;

}

void CPenalityList::Remove(CPenality &cPenality)
{
	POSITION thispos, pos = GetHeadPosition();
	while(pos)
		{
		thispos=pos;
		CPenality* pcPenality=GetNext(pos);
		if( pcPenality->m_cDate			==cPenality.m_cDate			&&
			pcPenality->m_cContestNo	==cPenality.m_cContestNo	&&
			pcPenality->m_ePenality		==cPenality.m_ePenality		&&
			pcPenality->m_cPenalityCode	==cPenality.m_cPenalityCode &&
			pcPenality->m_cPenalityReason==cPenality.m_cPenalityReason &&
			pcPenality->m_dPenality		==cPenality.m_dPenality		) 
			{
			delete pcPenality;
			CObList::RemoveAt( thispos );
			}
		}

	return;
}


void CPenalityList::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
{
	if( IsEmpty() !=0  )  return;

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pParentNode, pIDOMChildNode, _T("Penalties"));
	
	POSITION pos = GetHeadPosition();
	while(1)
		{
		CPenality* pcPenality=GetNext(pos);
		if( pcPenality ) pcPenality->GetXML( cMgr, pIDOMChildNode );
		if( pos==NULL ) break;
		}
}


int CPenalityList::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pWinscore)
	{
	int nImported=0;
	long length=0;

	MSXML2::IXMLDOMNodePtr pPenalitys;
	HRESULT HR= cMgr.SelectChild(_T("Penalties"),pWinscore, pPenalitys);
	if( pPenalitys==NULL ) return 0;

	MSXML2::IXMLDOMNodeListPtr pPenalityList=cMgr.SelectNodeList(_T("Penalty"), pPenalitys);
	if FAILED(HR) return 0;

	HR=pPenalityList->get_length(&length);
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pPenality;
		HR=pPenalityList->get_item(i,&pPenality);
		if FAILED(HR) return 0;
		CPenality* pcPenality=NULL;
		try
			{
			pcPenality=new CPenality( cMgr, pPenality );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcPenality) delete pcPenality;
			continue;
			}
		AddTail(pcPenality);
		nImported++;
		}

	return nImported;
	}
