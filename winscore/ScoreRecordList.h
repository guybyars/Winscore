//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ScoreRecordList.h: interface for the CScoreRecordList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCORERECORDLIST_H__E1D1ABD2_5870_11D1_ACF1_000000000000__INCLUDED_)
#define AFX_SCORERECORDLIST_H__E1D1ABD2_5870_11D1_ACF1_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScoreRecord.h"
#include "ContestantList.h"
#include "TaskList.h"

#ifndef WSVIEWER
#include "PenalityList.h"
#include "summary.h"
#endif

#include "wsclass.h"

class CScoreRecordList : public CObList  
{
public:

	int ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParent);

	void CheckMinimumTaskDistance( CWSClass &cClass, CTime &cDate );
	double GetMaxDayPoints( CTime &cDate, EClass eClass);
	double GetMedianDayPoints( CTime &cDate, EClass eClass);

	void GetXML(CXMLMgr &cMgr, EClass eClass, CTime cDate, MSXML2::IXMLDOMNodePtr &pParentNode);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);

#ifndef WSVIEWER

	int CalculateNumAssignedTurnpointsAcheived( CTask*	 pcTask,
   	  											CTime		 &cDate, 
    											CWSClass   &cClass );

	void CalculatePoints(	CTime		cDate,  
    						CWSClass   &cClass,
    						CTask	   *pcTask,
							CSummary	&cSummary );
	void ApplyPenalties( CTime &cDate, EClass eClass, CPenalityList &cPenalityList, double dMaxDayPoints );
	void CalculateSpeeds(	CTask*		pcTask, 
    						CTime		cDate, 
    						CWSClass	&cClass, 
							int			nMaxAcheivedAssignedTurnpoints,
    						CWinscoreListCtrl& cStatus,
							CTurnpointArray &turnpointArray);

	void DNC(	CString cContestNo, 
				EClass eClass, 
				CTime cDate, 						  
				bool  bGuest,
				int uPenalityMask=0 );

	void LoadScoreRecordList(	CListCtrl&	ListCtrl, 
								CTime		cDate, 
								EClass		eClass, 
								CTask		*pcTask, 
								CContestantList& cContestantList,
								CPenalityList& cPenalityList,
								EUnits		eUnits);

	void GetSummary(CTime		cDate,
					EClass		eClass,
					CTask		*pcTask,
					double		dMinDistance,
					CSummary	&cSummary	);

	void GetSSASummary(	CTime		cDate,
						EClass		eClass,
						CTask		*pcTask,
						double		dMinDistance,
						CSummary	&cSummary	);

	void GetFAISummary(	CTime		cDate,
						EClass		eClass,
						CTask		*pcTask,
						double		dMinDistance,
						CSummary	&cSummary	);

#endif

	int GetCount(EClass eClass, CTime cDate );
	CScoreRecord* Get( CString& cContestNo, CTime& cDate);
	CScoreRecord* CScoreRecordList::Get(CString & cContestNo );
	CScoreRecordList& operator <<(CScoreRecordList& cScoreRecordList);

	void Purge(bool bKeep=false);


	void CScoreRecordList::Purge(EClass eClass, CTime cDate);

	void AddScore(CScoreRecord* pcScoreRecord);
	int CopySort(CTime cDate, EClass eClass, CScoreRecordList& cOutputScoreRecordList, ESortBy eSortBy);
	int CopyByDate(CTime &cDate, CScoreRecordList & cInputScoreRecordList);


	CScoreRecordList();
	virtual ~CScoreRecordList();
 
	void CreateControlColumns(CListCtrl& ListCtrl, EClass eClass, EUnits eUnits);

	CScoreRecord* GetNext(POSITION& pos, CTime cDate );
	CScoreRecord* GetNext(POSITION& pos, CTime cDate, EClass eClass);
	CScoreRecord* GetNext(POSITION& pos, CTime cDate, CString cContestNo);
	CScoreRecord* GetNext(POSITION& pos, CString cContestNo);



void StoreData(
					CString		cContestNo,
					CTime		cDate,
					EClass		eClass,
					bool		bGuest,
					double		dHandicap,
					BOOL		fContestant,
					BOOL		fFinisher,
					CTime		cStartTime,
					CTime		cFinishTime,
					CTimeSpan	cSTI,
					double		dDistance,
					double 		dSpeed,
					double 		dScoredSpeed,
					double 		dHdcapDist,
					double 		dHdcapSpeed,
					double 		dScoredHdcapSpeed,
					int			nAcheivedTurnpoints,
					int			uPenalityMask
					);

};

int CALLBACK CompareScores(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn);

int compare_records( const void *arg1, const void *arg2 );



#endif // !defined(AFX_SCORERECORDLIST_H__E1D1ABD2_5870_11D1_ACF1_000000000000__INCLUDED_)

