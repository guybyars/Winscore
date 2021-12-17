//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// LandingCardList.h: interface for the CLandingCardList class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_LANDINGCARDLIST_H__8CBF6E82_542D_11D1_ACEA_000000000000__INCLUDED_)
//#define AFX_LANDINGCARDLIST_H__8CBF6E82_542D_11D1_ACEA_000000000000__INCLUDED_

#include "LandingCard.h"
#include "ContestantList.h"

//#include "WinscoreDoc.h"	// Added by ClassView

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CLandingCardList : public CObList  
{
public:
	void RemoveByPointer(CLandingCard* pcRemoveLandingCard);
	void Purge(CTime cDate);

	CLandingCard* GetAt(int iPosition, CTime cDate);
	void Merge(CLandingCardList &cInputLandingCardList );
	CLandingCardList();
	virtual ~CLandingCardList();

	void CreateControlColumns( CListCtrl& );
	void LoadTaskList(CListCtrl&);
	void LoadLandingCardList(CListCtrl& ListCtrl, CTime cDate);
	void LoadLandingCardList(CListCtrl& ListCtrl, CTime cDate, CContestantList& cContestantList, EClass eClass, CTurnpointArray& cTurnpointArray,EUnits eUnits, CTask *pcTask );
	void RemoveCard(CTime cDate, CString cContestNo);

	CLandingCard* GetNext( POSITION& pos);
	CLandingCard* GetNext( POSITION& pos, CTime cDate);

	void Purge();
	CLandingCardList& operator =(CLandingCardList&);
	void AddToList(CLandingCard*);
	CLandingCard* Get(CTime cDate, CString m_ContestNO);
};

//#endif // !defined(AFX_LANDINGCARDLIST_H__8CBF6E82_542D_11D1_ACEA_000000000000__INCLUDED_)

