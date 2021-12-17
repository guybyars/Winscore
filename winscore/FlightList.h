//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightList.h: interface for the CFlightList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLIGHTLIST_H__CF2AAA62_4D12_11D1_ACE2_000000000000__INCLUDED_)
#define AFX_FLIGHTLIST_H__CF2AAA62_4D12_11D1_ACE2_000000000000__INCLUDED_

#include <afx.h>
#include "Flight.h"
#include "flightstream.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFlightList : public CObList  
{
public:
	int ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pWinscore, CTaskList &cTaskList);
	int GetFileList(EClass eClass, CTime cDate, CStringArray &strarray, CContestantList &contestantList);
	int GetFileList2(EClass eClass, CTime cDate, CStringArray &strarray, CStringArray &strarrayName, CContestantList &contestantList, CScoreRecordList &scoreRecordList, int iMinRank);
	void GetXML(CXMLMgr &cMgr, EClass eClass, CTime cDate, MSXML2::IXMLDOMNodePtr &pParentNode, CContestantList &contestantList);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);

	int GetNumberFlights( CTime &cDate, int nClasses, EClass aeClasses[] );
	//CFlightStream* CreateFlightStream(CTurnpointArray &turnpointArray, CContestantList &contestantList, CTime &cDate, int iInterval, int nClasses, EClass aeClasses[], int &nFlights, CProgressCtrl &m_cProgress);
	CFlight* Get(CTime &cDate, CString &strContestNo);
	bool LoadListFromIGC(CString strPath, CTime cDate, CContestantList &contestantList, bool bSilent=false, CListCtrl *pList=NULL );
	void RemoveByPointer(CFlight*);

	CFlightList();
	virtual ~CFlightList();

	void CFlightList::CreateControlColumns(CListCtrl& ListCtrl);
	void CFlightList::LoadFlightList(CListCtrl& ListCtrl, CTime cDate, EClass eClass, CContestantList &contestantList, EUnits, int iSortColumn);
	void CFlightList::AddToList(CFlight * pcAddFlight);
	void CFlightList::Purge();
	void CFlightList::Purge(CTime cDate);
	CFlight*  CFlightList::LoadIGC(CString &strIGCFileName, CContestantList &contestantList );


	CFlight* CFlightList::FindByIGCFileName(CString cIGCFileName);
	CFlight* CFlightList::GetNext(POSITION& pos);
	CFlight* CFlightList::GetNext(POSITION& pos, CTime cDate);
	BOOL CFlightList::Remove(CFlight &cFlight);
	CString CFlightList::GetText(CTime &cDate );

	CSemaphore m_cSem_FindByIGCFileName, m_cSem_RemoveByPointer, m_cSem_LoadIGC;

	int	GetClassDateCount(CTime cDate, EClass eClass);

	void CFlightList::AnalyzeAll(	void* pFrame,
									HWND hwndParent,
									CTime			cDate,
									EClass			eClass,
									CTurnpointArray &turnpointArray,
									CContestantList &contestantList,
									CTaskList		&taskList,
									BOOL			bNewOnly,
									EUnits			eUnits);

};

#endif // !defined(AFX_FLIGHTLIST_H__CF2AAA62_4D12_11D1_ACE2_000000000000__INCLUDED_)

