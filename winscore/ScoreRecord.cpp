//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ScoreRecord.cpp: implementation of the CScoreRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"

#include "Penality.h"
#include "PenalityList.h"
#include "Location.h"
#include "ScoreRecord.h"
#include <math.h>
#include <Atlbase.h>
#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CScoreRecord, CScoreRecord, VERSIONABLE_SCHEMA|5)
CScoreRecord::CScoreRecord()
{
	m_dCumPoints=0;
	m_iDayRank=0;
	m_iCumRank=0;
	m_nAcheivedTurnpoints=0;
	m_cCumRankText="";
	m_dScoredSpeed=0;
	m_dScoredHandicappedSpeed=0;
	m_dWDSA=0;

	m_cContestNo="";
	m_cDate=0;

	m_eClass=eStandard;
	m_bGuest=0;
	m_dPoints=0;
	m_dWDSA=0;
	m_dSpeed=0;
	m_dScoredSpeed=0;
	m_dDistance=0;
	m_dHandicappedSpeed=0;
	m_dScoredHandicappedSpeed=0;
	m_dHandicappedDistance=0;
	m_dCumPoints=0;
	m_dAppliedPenalityPoints=0;
	m_dHandicap=0;
	m_nAcheivedTurnpoints=0;
	m_cSTOC=0;

	m_cStartTime=0;
	m_cFinishTime=0;
	m_fFinisher=0;
	m_fContestant=0;
}

CScoreRecord::~CScoreRecord()
{

}

CScoreRecord::CScoreRecord(CString cContestNo, 
						   EClass eClass, 
						   CTime cDate,
						   bool  bGuest )
{
	m_fFinisher=FALSE;
	m_fContestant=FALSE;
	m_cContestNo=cContestNo;
	m_cDate=cDate;
	m_eClass=eClass;
	m_bGuest=bGuest;

	m_dAppliedPenalityPoints=0.0;
	m_dPoints=0.0;
	m_dSpeed=0.;
	m_dScoredSpeed=0.;
	m_dDistance=0.;
	m_dHandicap=1.0;
	m_dHandicappedSpeed=0.;
	m_dScoredHandicappedSpeed=0.;
	m_dHandicappedDistance=0.;
	m_cStartTime=0;
	m_cFinishTime=0;
	m_fContestant=	FALSE;
	m_fFinisher=	FALSE;
	m_dCumPoints=	0;
	m_iDayRank=0;
	m_iCumRank=0;
	m_nAcheivedTurnpoints=0;
	m_uPenalityMask=0;
	m_dWDSA=0;

}

CScoreRecord::CScoreRecord(		
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
					int			uPenalityMask)
{

	m_fFinisher=FALSE;
	m_fContestant=FALSE;
	m_cContestNo=cContestNo;
	m_cDate=cDate;
	m_eClass=eClass;
	m_bGuest=bGuest;
	m_dCumPoints=	0;
	m_iDayRank=0;
	m_iCumRank=0;
	m_dWDSA=0;
	m_dHandicap=dHandicap;

	m_dAppliedPenalityPoints=0.0;
	m_dPoints=		0.0;
	m_dSpeed=		dSpeed;
	m_dScoredSpeed=	dScoredSpeed; 
	m_dDistance=	dDistance;
	m_dHandicappedSpeed=		dHdcapSpeed;
	m_dScoredHandicappedSpeed=	dScoredHdcapSpeed;
	m_dHandicappedDistance=	dHdcapDist;
	m_cStartTime=			cStartTime;
	m_cFinishTime=			cFinishTime;
	m_fContestant=	fContestant;
	m_fFinisher=	fFinisher;
	m_nAcheivedTurnpoints=nAcheivedTurnpoints;
	m_uPenalityMask=uPenalityMask;

}

CScoreRecord::CScoreRecord(CScoreRecord* pcRecord)
{

	m_fFinisher	=pcRecord->m_fFinisher;
	m_fContestant=pcRecord->m_fContestant;
	m_cContestNo=pcRecord->m_cContestNo;
	m_cDate		=pcRecord->m_cDate;
	m_eClass	=pcRecord->m_eClass;
	m_bGuest	=pcRecord->m_bGuest;
	m_dHandicap =pcRecord->m_dHandicap;
	m_dCumPoints=pcRecord->m_dCumPoints;
	m_iDayRank	=pcRecord->m_iDayRank;
	m_iCumRank	=m_iCumRank;
	m_dAppliedPenalityPoints=pcRecord->m_dAppliedPenalityPoints;

	m_dPoints					=pcRecord->m_dPoints;
	m_dSpeed					=pcRecord->m_dSpeed;
	m_dScoredSpeed				=pcRecord->m_dScoredSpeed;
	m_dDistance					=pcRecord->m_dDistance;
	m_dHandicappedSpeed			=pcRecord->m_dHandicappedSpeed;
	m_dScoredHandicappedSpeed	=pcRecord->m_dScoredHandicappedSpeed;
	m_dHandicappedDistance		=pcRecord->m_dHandicappedDistance;
	m_cStartTime				=pcRecord->m_cStartTime;
	m_cFinishTime				=pcRecord->m_cFinishTime;
	m_fContestant				=pcRecord->m_fContestant;
	m_fFinisher					=pcRecord->m_fFinisher;
	m_nAcheivedTurnpoints		=pcRecord->m_nAcheivedTurnpoints;
	m_uPenalityMask				=pcRecord->m_uPenalityMask;
	m_cSTOC						=pcRecord->m_cSTOC;
	m_dWDSA						=pcRecord->m_dWDSA;

}

CString CScoreRecord::STOCText()
{
if( m_uPenalityMask & WSP_NOSTART )
	return _T(" ");

if( !m_fFinisher)
	return _T(" ");
else 
	{
	double dHours=((double)m_cSTOC.GetTotalSeconds())/(60*60);
	CString str;
	str.Format(_T(" %s (%6.5lf)"),m_cSTOC.Format(_T(" %H:%M:%S ") ),dHours);
	return str;
	}
}




CString CScoreRecord::TOCText()
{
if( m_uPenalityMask & WSP_NOSTART )
	return _T(" ");

if( !m_fFinisher)
	return _T(" ");
else 
	{
	CString strTemp;
	CTimeSpan  cTOC=m_cFinishTime-m_cStartTime;
	int nDays=(int)cTOC.GetDays();
	if( nDays>0 ) 	
		return cTOC.Format(_T("%D days %H:%M:%S ") );
	else
		return cTOC.Format(_T(" %H:%M:%S ") );


	}
};


CString CScoreRecord::StartTimeText()
{
	if( m_uPenalityMask & WSP_NOSTART )
		return _T(" ");

	if( m_fFinisher || m_dDistance>0.0 )
		return m_cStartTime.Format( _T("%H : %M : %S") );
	else
		return _T(" ");
};


CString CScoreRecord::FinishTimeText()
	{
	if( m_uPenalityMask & WSP_NOSTART )
		return _T(" ");

	if( !m_fFinisher)
		return _T(" ");
	else 
		return m_cFinishTime.Format( _T("%H : %M : %S") );
	};


CString CScoreRecord::STITimeText()
{
		return _T(" ");
};

CString CScoreRecord::PointsText()
{
CString cTemp;
cTemp.Format(_T("%4.0lf"), m_dPoints);
return cTemp;
}

CString CScoreRecord::StatusText()
{
	CString strStatus=_T("");

	if( m_uPenalityMask & WSP_NOLAUNCH	)
		return _T("No Information");

	if( m_fFinisher)
		strStatus=_T("Finished");
	else
		strStatus+=_T("Incomplete");

	if( m_uPenalityMask & WSP_NOFINISH	)
		strStatus+=_T(", No Finish Time");
	if( m_uPenalityMask & WSP_NOSTART	)
		strStatus+=_T(", No Start Time");
	if( m_uPenalityMask & WSP_NOLANDINGCARD 	)
		strStatus+=_T(". No Landing Card");
	return strStatus;
}

CString CScoreRecord::DistanceText(EUnits eUnits)
{
CString cTemp;
cTemp.Format(_T("%6.2lf "), ConvertDistance(m_dDistance, SYSTEMUNITS, eUnits ) );
return cTemp;
};

CString CScoreRecord::HdcapDistanceText(EUnits eUnits)
{
CString cTemp;
cTemp.Format(_T("%6.2lf "), ConvertDistance(m_dHandicappedDistance, SYSTEMUNITS, eUnits ) );
return cTemp;
};

CString CScoreRecord::SpeedText(EUnits eUnits)
	{
	if( m_uPenalityMask & WSP_NOSTART )
		return _T(" ");

	if( m_fFinisher)
		{
		CString cTemp;
		cTemp.Format(_T("%6.2lf "), ConvertDistance(m_dSpeed, SYSTEMUNITS, eUnits ) );
		return cTemp;
		}
	else
		return _T(" ");
	};

CString CScoreRecord::HdcapSpeedText(EUnits eUnits)
{
if( m_fFinisher)
	{
	CString cTemp;
	cTemp.Format(_T("%6.2lf "), ConvertDistance(m_dHandicappedSpeed, SYSTEMUNITS, eUnits ) );
	return cTemp;
	}
else
	return _T(" ");
};

CString CScoreRecord::PenalityText()
{
	CString cText;
	int i=0;

	if( m_uPenalityMask & WSP_MINDIST) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Minimum Distance");
		i++;
		}
	if( m_uPenalityMask & WSP_MINTIME) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Minimum Time");
		i++;
		}
	if( m_uPenalityMask & WSP_MINTIME15) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Time > 15 Min. Under Task Time");
		i++;
		}
	if( m_uPenalityMask & WSP_AIRFIELDBONUS) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Airfield Bonus Applied");
		i++;
		}
	if( m_uPenalityMask & WSP_WITHDREW) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Withdrew from contest");
		i++;
		}
	if( m_uPenalityMask & WSP_FOREIGN) 
		{
		if( i>0 ) cText+= _T(" & ");
		cText+=_T("Foreign Pilot");
		i++;
		}


	cText+=_T("  ");
	return cText;
};


CString CScoreRecord::CumPointsText()
	{
	CString cTemp;
	cTemp.Format(_T("%4.0lf"), max( m_dCumPoints, 0) );
	return cTemp;
	}

CString CScoreRecord::DNCText(EStatus eStatus)
{
	if( m_uPenalityMask & WSP_NOLAUNCH )
		{
		if( eStatus==ePreliminary )
			return _T("   ");
		else
			return _T("DNC");
		}
	else if( m_uPenalityMask & WSP_NOSTART )
		{
		return _T("DNS");
		}
	else
		return _T("0.0");
}



CString CScoreRecord::AppliedPenalityText(CPenalityList& cPenalityList)

	{
	CString cTemp;
	double dPenality=m_dAppliedPenalityPoints;

	CPenality *pcContestPenality=cPenalityList.Get(  m_cContestNo, 
													 m_cDate,
													 eContestPenalty);
	if( pcContestPenality )
		{
		// Poor bastard has a contest penality, apply it!
		dPenality+=pcContestPenality->m_dPenality;
		}

	if(  dPenality==0 ) return _T("");
	cTemp.Format(_T("%4.0lf"), max( dPenality, 0) );
	return cTemp;
	}

bool CScoreRecord::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode,pIDOMChildNode, _T("ScoreRecord"));

	bool bHdcp = GetClass(m_eClass).IsHandicapped();

	cMgr.CreateElement(	pIDOMChildNode, _T("CID"),			LPCSTR(m_cContestNo) );
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("DayRank"),		m_iDayRank);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("CumRank"),		m_iCumRank);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("AcheivedTps"),	m_nAcheivedTurnpoints);
   	GetXMLDate(	cMgr,	pIDOMChildNode, _T("Date2"),		m_cDate);
	cMgr.CreateElementInt(	pIDOMChildNode, _T("ClassID"),		m_eClass);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("Contestant"),	m_fContestant);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("Points"),		(int)m_dPoints);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("CPoints"),		(int)m_dCumPoints);
	if( m_fContestant )
		{
   		GetXMLTime(	cMgr,	pIDOMChildNode, _T("StartTime2"),	m_cStartTime, false);
		if( m_fFinisher )
			{
			if( bHdcp )
				{
				cMgr.CreateElementDblC(	pIDOMChildNode, _T("HSpeed"),		m_dHandicappedSpeed);
				cMgr.CreateElementDblC(	pIDOMChildNode, _T("ScoredHSpeed"), m_dScoredHandicappedSpeed);
				}
			cMgr.CreateElementDblC(	pIDOMChildNode, _T("Speed"),		m_dSpeed);
			cMgr.CreateElementDblC(	pIDOMChildNode, _T("ScoredSpeed"),	m_dScoredSpeed);
			cMgr.CreateElementInt(	pIDOMChildNode, _T("TOC"),			(int)m_cSTOC.GetTotalSeconds());
   			GetXMLTime(	cMgr,	pIDOMChildNode, _T("FinishTime2"),	m_cFinishTime, false);
			}
		cMgr.CreateElementIntC(	pIDOMChildNode, _T("Finisher"),		m_fFinisher);
		cMgr.CreateElementDblC(	pIDOMChildNode, _T("Distance"),		m_dDistance);
		if( bHdcp )
			cMgr.CreateElementDblC(	pIDOMChildNode, _T("HDistance"),	m_dHandicappedDistance);

		}
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("Guest"),		m_bGuest);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("PenaltyMask"),  m_uPenalityMask);
	cMgr.CreateElementDblC(	pIDOMChildNode, _T("WDSA"),			m_dWDSA);
	cMgr.CreateElementIntC(	pIDOMChildNode, _T("Penalty"),	(int)m_dAppliedPenalityPoints);

	return true;
	}

CScoreRecord::CScoreRecord(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMChildNode)
	{
	CScoreRecord();

	GET_XML_STR(	cMgr, pIDOMChildNode, _T("CID"),	m_cContestNo );

	GET_XML_INT(	cMgr, pIDOMChildNode, _T("DayRank"),	int, m_iDayRank, 0);
	GET_XML_INT(	cMgr, pIDOMChildNode, _T("CumRank"),	int, m_iCumRank, 0);
	GET_XML_INT(	cMgr, pIDOMChildNode, _T("AcheivedTps"),int, 	m_nAcheivedTurnpoints, 0);
	GET_XML_INT(	cMgr, pIDOMChildNode, _T("Date"),		int, m_cDate, 0);
	ImportXMLDate(  cMgr, pIDOMChildNode, _T("Date2"), m_cDate);
	GET_XML_INT(	cMgr, pIDOMChildNode, _T("ClassID"),	EClass, m_eClass, eStandard);
	GET_XML_BOOL(	cMgr, pIDOMChildNode, _T("Contestant"),  m_fContestant, false);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("Points"),	double,	m_dPoints, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("CPoints"),	double, m_dCumPoints, 0);

	ImportXMLTime(  cMgr, pIDOMChildNode, _T("StartTime2"), m_cStartTime, m_cDate.GetYear(), m_cDate.GetMonth(), m_cDate.GetDay());

	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("Speed"),	double, m_dSpeed, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("HSpeed"),	double, m_dHandicappedSpeed, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("ScoredSpeed"),double,  m_dScoredSpeed, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("ScoredHSpeed"),double,  m_dScoredHandicappedSpeed, 0);

	GET_XML_INT(	cMgr, pIDOMChildNode, _T("TOC"),		CTimeSpan, m_cSTOC, 0);
	ImportXMLTime(  cMgr, pIDOMChildNode, _T("FinishTime2"), m_cFinishTime, m_cDate.GetYear(), m_cDate.GetMonth(), m_cDate.GetDay());
	GET_XML_BOOL(	cMgr, pIDOMChildNode, _T("Finisher"), m_fFinisher, false);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("Distance"), double, m_dDistance, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("HDistance"),double, m_dHandicappedDistance, 0);

	GET_XML_BOOL(	cMgr, pIDOMChildNode, _T("Guest"),	m_bGuest, false);
	GET_XML_INT(	cMgr, pIDOMChildNode, _T("PenaltyMask"),int,  m_uPenalityMask, 0);

	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("WDSA"),		double, m_dWDSA, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("Penalty"),	double, m_dAppliedPenalityPoints, 0);
	GET_XML_DBL(	cMgr, pIDOMChildNode, _T("Handicap"), double, m_dHandicap, 1);
	}
