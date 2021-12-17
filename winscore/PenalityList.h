//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PenalityList.h: interface for the CPenalityList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PENALITYLIST_H__69D749C2_5EC0_11D1_ACFB_000000000000__INCLUDED_)
#define AFX_PENALITYLIST_H__69D749C2_5EC0_11D1_ACFB_000000000000__INCLUDED_

#include "Penality.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPenalityList : public CObList  
{
public:
	int ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pWinscore);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);

	POSITION m_pos;

	void Merge(CPenalityList &cInputList);
	CString DayText( CString& cContestNo, CTime& cDate );
	void RemoveByPointer(CPenality* pcPenality);
	CPenalityList();
	virtual ~CPenalityList();

	void CPenalityList::AddCodes(CString& cContestNo, CTime &cDate, int *pnCodes, CStringArray& cCodes, CStringArray& cReasons);
	void CPenalityList::Remove(CPenality &cPenality);

	void CPenalityList::CreateControlColumns(CListCtrl& ListCtrl);
	void CPenalityList::LoadPenalityList(CListCtrl& ListCtrl);
	void CPenalityList::AddToList(CPenality * pcAddPenality);
	int CPenalityList::GetContestTotal(	CString& cContestNo );
	CPenalityList& CPenalityList::operator =(CPenalityList& cInputPenalityList);
	void CPenalityList::Purge();
	CPenality* GetNext(POSITION& pos);
    CPenality* Get(	CString& cContestNo, 
					CTime& cDate, 
					EPenality ePenality);
   CPenality* GetNext(	CString& cContestNo, 
						CTime& cDate, 
						EPenality ePenality,
						bool		bRewind);
};

#endif // !defined(AFX_PENALITYLIST_H__69D749C2_5EC0_11D1_ACFB_000000000000__INCLUDED_)

