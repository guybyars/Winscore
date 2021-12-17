//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// InvalidLegList.h: interface for the CInvalidLegList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVALIDLEGLIST_H__52978673_6839_11D1_AD0C_000000000000__INCLUDED_)
#define AFX_INVALIDLEGLIST_H__52978673_6839_11D1_AD0C_000000000000__INCLUDED_


#include "InvalidLeg.h"
#include "TurnpointArray.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CInvalidLegList : public CObList  
{
public:
	CString GetText();
	bool IsLegAlreadyInList( CInvalidLeg &cLeg);
	BOOL InvalidLeg(int iFirst, int iSecond);
	CInvalidLegList();
	virtual ~CInvalidLegList();

	CInvalidLegList& operator =(CInvalidLegList& cInputInvalidLegList);
	void Purge();
	CInvalidLeg* GetNext(POSITION& pos);
	void CreateControlColumns(CListCtrl& ListCtrl);
	void LoadInvalidLegList(CListCtrl& ListCtrl, CTurnpointArray& cTurnpointArray);
	void RemoveByPointer(CInvalidLeg * pcRemoveInvalidLeg);
	void AddToList(CInvalidLeg* pcAddInvalidLeg);

	int NumberSummaryPages();

	void DrawInvalidLegList(	CDC		*pDC,
								CString &cContestName,
								CString &cContestLocation,
								CTime	&cDate, 
								int		iPage,
								CTurnpointArray& cTurnpointArray);
};

#endif // !defined(AFX_INVALIDLEGLIST_H__52978673_6839_11D1_AD0C_000000000000__INCLUDED_)

