//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// InvalidLegList.cpp: implementation of the CInvalidLegList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "InvalidLegList.h"
#include "WinscoreFonts.h"
#include "global_prototypes.h"


#define PAGE_WIDTH    800
#define NUM_LEGS_PER_PAGE	40

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvalidLegList::CInvalidLegList()
{

}

CInvalidLegList::~CInvalidLegList()
{
	//if( IsEmpty() !=0  )  return;
	//Purge();
	//return;
}
#if 0

CInvalidLegList& CInvalidLegList::operator =(CInvalidLegList& cInputInvalidLegList)
{
	CInvalidLeg *pcNewInvalidLeg, *pcInvalidLeg;

	//  Clean out the destination  
	Purge();

	if( cInputInvalidLegList.IsEmpty() !=0  )  return *this;

	POSITION pos = cInputInvalidLegList.GetHeadPosition();

	while(1)
		{
		pcInvalidLeg=(CInvalidLeg*)cInputInvalidLegList.GetNext(pos);
		if( pcInvalidLeg ) 
			{
			pcNewInvalidLeg=new CInvalidLeg(pcInvalidLeg);
			(CInvalidLeg*)AddTail(pcNewInvalidLeg);
			}
		if( pos==NULL ) break;
		}

	return *this;

}

void CInvalidLegList::Purge()
	{

	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(1)
		{
		CInvalidLeg* pcInvalidLeg=GetNext(pos);
		if( pcInvalidLeg ) delete pcInvalidLeg;
		if( pos==NULL ) break;
		}
	RemoveAll();
	}


CInvalidLeg* CInvalidLegList::GetNext(POSITION& pos)
{
	return (CInvalidLeg*)CObList::GetNext(pos);
}

void CInvalidLegList::AddToList(CInvalidLeg* pcAddInvalidLeg)
	{
	CInvalidLeg* pcNewInvalidLeg=new CInvalidLeg(pcAddInvalidLeg);
	AddTail( (CInvalidLeg*)pcNewInvalidLeg );
	}



#define NUM_COLUMNS 2

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
	_T("From"), _T("To")
};


static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
	150, 150
};

void CInvalidLegList::CreateControlColumns(CListCtrl& ListCtrl)
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

void CInvalidLegList::LoadInvalidLegList(CListCtrl& ListCtrl, CTurnpointArray& cTurnpointArray)
{
//	char buff[50];

	CInvalidLeg* pcInvalidLeg;

	LV_ITEM lvi;

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcInvalidLeg=GetNext(pos);
		if( pcInvalidLeg )
			{
			lvi.iItem = iItem;
			lvi.lParam = (LPARAM)pcInvalidLeg;
            CTurnpoint* pcFrom=cTurnpointArray[pcInvalidLeg->m_iFirstTurnpoint-1];
            CTurnpoint* pcTo=cTurnpointArray[pcInvalidLeg->m_iSecondTurnpoint-1];
			ASSERT(pcFrom && pcTo);
			if( pcTo==NULL || pcFrom==NULL ) continue;
			ListCtrl.InsertItem(&lvi);
			ListCtrl.SetItemText(iItem,0,pcFrom->GetNoNameText() );
			ListCtrl.SetItemText(iItem,1,pcTo->GetNoNameText() );
			iItem++;
			}
		}
	}

void CInvalidLegList::RemoveByPointer(CInvalidLeg * pcRemoveInvalidLeg)
{
	if( IsEmpty() !=0  )  return;
	POSITION thispos, pos = GetHeadPosition();
	while(pos)
		{
		thispos=pos;
		CInvalidLeg* pcInvalidLeg=GetNext(pos);
		if( pcInvalidLeg==pcRemoveInvalidLeg ) 
			{
			delete pcInvalidLeg;
			RemoveAt(thispos);
			return;
			}
		}
}

BOOL CInvalidLegList::InvalidLeg(int iFirst, int iSecond)
{
	if( IsEmpty() !=0  )  return FALSE;
	POSITION  pos = GetHeadPosition();
	while(pos)
		{
		CInvalidLeg* pcInvalidLeg=GetNext(pos);
		ASSERT(pcInvalidLeg);
		if( (pcInvalidLeg->m_iFirstTurnpoint == iFirst &&
			 pcInvalidLeg->m_iSecondTurnpoint == iSecond  )  ||
			(pcInvalidLeg->m_iFirstTurnpoint == iSecond &&
			 pcInvalidLeg->m_iSecondTurnpoint == iFirst  ) 		) 
			{
			return TRUE;
			}
		}
	return FALSE;
}

int CInvalidLegList::NumberSummaryPages()
	{
	if( IsEmpty() )  return 1;

	int nLegs=GetCount();

	int nPages=nLegs/NUM_LEGS_PER_PAGE;
	if( nPages*NUM_LEGS_PER_PAGE < nLegs ) nPages++;

	return nPages;

	}

void CInvalidLegList::DrawInvalidLegList(	CDC		*pDC,
											CString &cContestName,
											CString &cContestLocation,
											CTime	&cDate, 
											int		iPage,
											CTurnpointArray& cTurnpointArray)
	{
	CSize	 cTextSize;
	int iYpos=0;
	CString  cTitle="Invalid Task Legs", cText, cPage;
	int iStart=((iPage-1)*NUM_LEGS_PER_PAGE)+1;
	int iEnd=iPage*NUM_LEGS_PER_PAGE;

	SelectFont( pDC, 30, FW_BOLD, FALSE);

    pDC->SetTextAlign( TA_TOP| TA_CENTER );
    pDC->TextOut( PAGE_WIDTH/2, 0, cTitle, cTitle.GetLength() );
	cTextSize=pDC->GetTextExtent(cTitle);
	iYpos-=cTextSize.cy;
	iYpos-=5;

	SelectFont( pDC, 20, FW_BOLD, FALSE);
	cPage.Format("Page %d/%d", iPage, NumberSummaryPages());
    pDC->SetTextAlign( TA_TOP| TA_RIGHT );
    pDC->TextOut( PAGE_WIDTH, 0, cPage, cPage.GetLength() );

	SelectFont( pDC, 25, FW_BOLD, FALSE);
    pDC->SetTextAlign( TA_TOP| TA_LEFT );

	pDC->TextOut( 0, iYpos, cContestName, cContestName.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestName);
	iYpos-=cTextSize.cy;

	pDC->TextOut( 0, iYpos, cContestLocation, cContestLocation.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestLocation);
	iYpos-=cTextSize.cy;

	CString strDate = cDate.Format( "%B %Y" );
	pDC->TextOut( 0, iYpos, strDate, strDate.GetLength() );
	cTextSize=pDC->GetTextExtent(strDate);
	iYpos-=cTextSize.cy;
	iYpos-=8;

//  Output Column Headers

	SelectFont( pDC, 20, FW_BOLD, FALSE); 
	cText="FROM";
	pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );
	cText="TO";
	pDC->TextOut( 400 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);
	iYpos-=cTextSize.cy;

	SelectFont( pDC, 20, FW_BOLD, TRUE); 
	cText="ID";
	pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );

	cText="Name";
	pDC->TextOut( 30 , iYpos, cText, cText.GetLength() );

	cText="ID";
	pDC->TextOut( 400, iYpos, cText, cText.GetLength() );

	cText="Name";
	pDC->TextOut( 430 , iYpos, cText, cText.GetLength() );
	cTextSize=pDC->GetTextExtent(cText);
	iYpos-=cTextSize.cy;


	SelectFont( pDC, 15, FW_NORMAL, FALSE); 

	int nLegs=0;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CInvalidLeg *pcInvalidLeg=GetNext(pos);
		if( pcInvalidLeg )
			{
			nLegs++;
			if( nLegs< iStart || nLegs> iEnd ) continue;

            CTurnpoint* pcFrom=cTurnpointArray[pcInvalidLeg->m_iFirstTurnpoint-1];
            CTurnpoint* pcTo=cTurnpointArray[pcInvalidLeg->m_iSecondTurnpoint-1];
			ASSERT(pcFrom && pcTo);
			if( pcTo==NULL || pcFrom==NULL ) continue;

			cText.Format("%d",pcFrom->m_iID);
			pDC->TextOut( 0 , iYpos, cText, cText.GetLength() );

			cText=pcFrom->m_strName;
			pDC->TextOut( 30 , iYpos, cText, cText.GetLength() );

			cText.Format("%d",pcTo->m_iID);
			pDC->TextOut( 400 , iYpos, cText, cText.GetLength() );

			cText=pcTo->m_strName;
			pDC->TextOut( 430 , iYpos, cText, cText.GetLength() );
			cTextSize=pDC->GetTextExtent(cText);
			iYpos-=cTextSize.cy;

			}
		}

	CTime cToday = CTime::GetCurrentTime();
	cText=cToday.Format("Printed on %A %B %d, %Y - %H:%M by Winscore" );
	pDC->SetTextAlign( TA_TOP| TA_CENTER );
	SelectFont( pDC, 13, FW_NORMAL, FALSE); 

	DRAW_HORIZONTAL( -1030 );
	pDC->TextOut( PAGE_WIDTH/2, -1035 , cText, cText.GetLength() );

	return;
	}


bool CInvalidLegList::IsLegAlreadyInList(CInvalidLeg &cLeg)
{
	if( IsEmpty() !=0  )  return FALSE;
	POSITION  pos = GetHeadPosition();
	while(pos)
		{
		CInvalidLeg* pcInvalidLeg=GetNext(pos);
		ASSERT(pcInvalidLeg);
		if( (pcInvalidLeg->m_iFirstTurnpoint == cLeg.m_iFirstTurnpoint &&
			 pcInvalidLeg->m_iSecondTurnpoint == cLeg.m_iSecondTurnpoint  )  ||
			(pcInvalidLeg->m_iFirstTurnpoint == cLeg.m_iSecondTurnpoint &&
			 pcInvalidLeg->m_iSecondTurnpoint == cLeg.m_iFirstTurnpoint  ) 		) 
			{
			return TRUE;
			}
		}
	return FALSE;
}

CString	CInvalidLegList::GetText()
{

	CString str;
	if( !IsEmpty() )
		{
		POSITION pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CInvalidLeg* pcInvalidLeg=GetNext(pos);
			if( !pcInvalidLeg ) continue;
			CString cTemp;
			cTemp.Format("%d \t %d", pcInvalidLeg->m_iFirstTurnpoint, pcInvalidLeg->m_iSecondTurnpoint);
			str+=cTemp;
			str+="\r\n";
			}
		}
	return str;
}

#endif