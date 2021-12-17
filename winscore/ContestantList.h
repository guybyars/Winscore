//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ContestantList.h: interface for the CContestantList class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "Contestant.h"
#include "wsclass.h"
#include <iostream>
#include "GliderInfoList.h"

#if !defined(AFX_CONTESTANTLIST_H__23C6BD63_40EF_11D1_ACD0_000000000000__INCLUDED_)
#define AFX_CONTESTANTLIST_H__23C6BD63_40EF_11D1_ACD0_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CContestantList : public CObList  
{
public:
	int CheckClasses(EContest);
	void UpdateHandicaps(EContest eContest, CGliderInfoList &cGIList);
	int ImportXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode, int &nSkipped);
	int ImportSSA(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pWinscore, CContestantList &cExistingContestantList, int &nSkipped);

	int GetXML	 (CXMLMgr &cMgr, IDispatch *pIDOMNode, bool bHandicappedOnly=false);
	CContestant* GetByPilotName(CString cName);

	CString GetText();
	void Sort(int iSortColumn );
	void GetContestants( EClass eClass, CStringArray& cArray,bool bGuestsOnly=false);
	void RemoveByPosition( int iPos);
	void CreateControlColumns( CListCtrl& );
	void LoadContestantList(CListCtrl&);
	void RemoveByPointer(CContestant* pcContestant);
	void SetContestantItem(CListCtrl& ListCtrl, int iItem, CContestant *pcContestant);


	CContestant* GetNext( POSITION& pos);

	void Purge();
	CContestantList& operator =(CContestantList&);
	bool AddToList(CContestant* pcContestant);
	CContestant* GetByContestNo(CString strContestNo);
	CContestantList();
	double GetLowestPerformanceHandicap();
	virtual ~CContestantList();

	int NumberSummaryPages();

	void DrawContestantList(	CDC		*pDC,
								CString &cContestName,
								CString &cContestLocation,
								CTime	&cDate, 
								int		iPage );


	int GetCountClass(EClass eClass);
};

ostream& operator<<( ostream& os, CContestantList& cList );
istream& operator>>( istream& is, CContestantList& cList );


#endif // !defined(AFX_CONTESTANTLIST_H__23C6BD63_40EF_11D1_ACD0_000000000000__INCLUDED_)

