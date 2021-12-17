//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// LandingCardList.cpp: implementation of the CLandingCardList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "LandingCardList.h"
#include "Global_Prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLandingCardList::CLandingCardList()
{

}

CLandingCardList::~CLandingCardList()
{
	//Purge();
	return;
}
/*

CLandingCard* CLandingCardList::Get(CTime cDate, CString cContestNo)
	{
	CLandingCard* pcLandingCard;

	if( IsEmpty() !=0  )  return NULL;

	POSITION pos = GetHeadPosition();

	while(pos!=NULL)
		{
		pcLandingCard=GetNext(pos);
		if( DatesEqual(pcLandingCard->m_cDate,cDate)  &&
			pcLandingCard->m_cContestNo==cContestNo ) return pcLandingCard;
		}
	return NULL;
	}


void CLandingCardList::RemoveCard(CTime cDate, CString cContestNo)
	{
	CLandingCard* pcLandingCard;

	if( IsEmpty() !=0  )  return;

	POSITION thispos, pos = GetHeadPosition();

	while(pos!=NULL)
		{
		thispos=pos;
		pcLandingCard=GetNext(pos);
		if( DatesEqual(pcLandingCard->m_cDate, cDate)  &&
			pcLandingCard->m_cContestNo==cContestNo ) 
			{
			delete pcLandingCard;
			RemoveAt(thispos);
			return;
			}
		}
	return;
	}


void CLandingCardList::Purge()
	{
	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CLandingCard* pcLandingCard=GetNext(pos);
		if( pcLandingCard ) delete pcLandingCard;
		}
	RemoveAll();
	}

void CLandingCardList::Purge(CTime cDate)
	{
	if( IsEmpty() !=0  )  return;

	POSITION thispos, pos = GetHeadPosition();
	while(pos!=NULL)
		{
		thispos=pos;
		CLandingCard* pcLandingCard=GetNext(pos);
		if( !pcLandingCard ) continue;
			

		if( DatesEqual(cDate, pcLandingCard->m_cDate) 	)
			{
			RemoveAt(thispos);
			delete pcLandingCard;
			}						
		}
	}


CLandingCard* CLandingCardList::GetNext(POSITION& pos)
{
	return (CLandingCard*)CObList::GetNext(pos);
}

CLandingCard* CLandingCardList::GetNext(POSITION& pos, CTime cDate)
{

	while(pos!=NULL)
		{
		CLandingCard* pcCard=GetNext(pos);
		if( !pcCard ) return NULL;
		if( DatesEqual( cDate,pcCard->m_cDate )		)
			{
			return pcCard;
			}
		}
	return NULL;



}


#define NUM_COLUMNS 8

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
		_T("No"), 
		_T("Status"), 
		_T("Distance"),
		_T("Start Gate"), 
		_T("Turnpoints Acheived"), 
		_T("Airfield Bonus"), 
		_T("Landing Point"), 
		_T("Seeking") };

static int _gnColumnFmt[NUM_COLUMNS] = 
{
	LVCFMT_LEFT, LVCFMT_LEFT, 
	LVCFMT_LEFT, LVCFMT_LEFT,
	LVCFMT_LEFT, LVCFMT_LEFT,
	LVCFMT_LEFT, LVCFMT_LEFT
};

static int _gnColumnWidth[NUM_COLUMNS] = 
{
  50, 100, 130, 100, 150, 100, 170, 140 };


CLandingCard* CLandingCardList::GetAt(int iPosition, CTime cDate)
	{
	int nFound=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CLandingCard* pcLandingCard=GetNext(pos,cDate);
		if( nFound==iPosition ) return pcLandingCard;
		nFound++;
		}
	return NULL;
	}

void CLandingCardList::RemoveByPointer(CLandingCard *pcRemoveLandingCard)
	{
	CLandingCard* pcLandingCard;

	if( !pcRemoveLandingCard ) return;

	if( IsEmpty() !=0  )  return;

	POSITION thispos, pos = GetHeadPosition();

	while(pos!=NULL)
		{
		thispos=pos;
		pcLandingCard=GetNext(pos);
		if( pcLandingCard==pcRemoveLandingCard ) 
			{
			delete pcLandingCard;
			RemoveAt(thispos);
			return;
			}
		}
	return;
	}
	*/