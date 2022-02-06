//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ScoreRecord.h: interface for the CScoreRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCORERECORD_H__E1D1ABD1_5870_11D1_ACF1_000000000000__INCLUDED_)
#define AFX_SCORERECORD_H__E1D1ABD1_5870_11D1_ACF1_000000000000__INCLUDED_

#define MAXCOLUMNS   21

#include "PenalityList.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CScoreRecord : public CObject  
{

protected:

	CScoreRecord();
	DECLARE_SERIAL(CScoreRecord)

public:

	CScoreRecord(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);

	bool GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode);
	CString DNCText(EStatus eStatus);
	CString AppliedPenalityText(CPenalityList& cPenalityList);

	CString CumPointsText();

	int m_uPenalityMask;

	CString m_acColumnText[MAXCOLUMNS];
	CString StatusText();
	CString PointsText();
	CString TOCText();
	CString StartTimeText();
	CString STOCText();
	CString FinishTimeText();
	CString STITimeText();
	CString DistanceText(EUnits eUnits);
	CString HdcapDistanceText(EUnits eUnits);
	CString SpeedText(EUnits eUnits);
	CString HdcapSpeedText(EUnits eUnits);
	CString PenalityText();
	CString m_cCumRankText;

	int     m_iDayRank;
	int	    m_iCumRank;

	CString m_cContestNo;
	CTime	m_cDate;

	EClass	m_eClass;
	bool	m_bGuest;
	double	m_dPoints;
	double	m_dWDSA;
	double  m_dSpeed;
	double  m_dScoredSpeed;
	double  m_dDistance;
	double  m_dHandicappedSpeed;
	double  m_dScoredHandicappedSpeed;
	double  m_dHandicappedDistance;
	double  m_dCumPoints;
	double  m_dAppliedPenalityPoints;
	double  m_dHandicap;
	int		m_nAcheivedTurnpoints;
	CTimeSpan	m_cSTOC;

	CTime	m_cStartTime;
	CTime	m_cFinishTime;
	BOOL m_fFinisher;
	BOOL m_fContestant;

	CScoreRecord(CScoreRecord* pcRecord);
	CScoreRecord(CString, EClass, CTime, bool);
	CScoreRecord(	CString		cContestNo,
					CTime		cDate,
					EClass		eClass,
					bool		bGuest,
					double 		dHandicap,
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
					int			uPenalityMask);

	virtual ~CScoreRecord();

};

#endif // !defined(AFX_SCORERECORD_H__E1D1ABD1_5870_11D1_ACF1_000000000000__INCLUDED_)

