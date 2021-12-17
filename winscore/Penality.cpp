//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Penality.cpp: implementation of the CPenality class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "Penality.h"
#include "Global_Prototypes.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPenality::CPenality(CPenality* pcPenality)
	{
	m_cContestNo=pcPenality->m_cContestNo;
	m_cDate=pcPenality->m_cDate;
	m_ePenality=pcPenality->m_ePenality;
	m_cPenalityCode=pcPenality->m_cPenalityCode;
	m_cPenalityReason=pcPenality->m_cPenalityReason;
	m_dPenality=pcPenality->m_dPenality;
	}

IMPLEMENT_SERIAL( CPenality, CPenality, VERSIONABLE_SCHEMA | 3)

CPenality::CPenality(CString &cContestNo, CTime &cDate, EPenality ePenality, CString &cCode, CString &cReason, double  dPenality)
{
	m_cContestNo=cContestNo;
	m_cDate=cDate;
	m_ePenality=ePenality;
	m_cPenalityCode=cCode;
	m_cPenalityReason=cReason;
	m_dPenality=dPenality;
}

CPenality::CPenality()
{
}

CPenality::~CPenality()
{
}

void CPenality::Serialize(CArchive & ar)
{

	WORD w;
	if (ar.IsStoring())
		{
		ar << m_cContestNo;
		SerializeTime( ar, m_cDate );
		ar << (WORD)m_ePenality;
		ar << m_cPenalityCode;
		ar << m_cPenalityReason;
		ar << m_dPenality;
		ar << (WORD)0;
		}
	else
		{
		UINT iSchema=ar.GetObjectSchema( );
		ar >> m_cContestNo;

		if( iSchema<=2 )
			ar >> m_cDate;
		else
			SerializeTime( ar, m_cDate );

		ar >> w;
		m_ePenality=(EPenality)w;
		ar >> m_cPenalityCode;
		ar >> m_cPenalityReason;
		if( iSchema==1 )
			{
			int i;
			ar >> i;
			m_dPenality=i;
			}
		else if( iSchema>=2 )
			ar >> m_dPenality;

		ar >> w;
		}

}
CString CPenality::GetDateText()
{
CString cTemp = m_cDate.Format(_T("%b %d, %y") );
return cTemp;
}

CString CPenality::GetTypeText()
{
if( m_ePenality==eDayPoint )
	return _T("Task Point Penalty");

else if( m_ePenality==eDayPercent )
	return _T("Task Percent Penalty");

else if( m_ePenality==eContestPenalty )
	return _T("Contest Penalty");

else if( m_ePenality==eComment )
	return _T("Comment Only");

else
	return _T(" ");
}


CString CPenality::GetPenaltityText()
	{
	CString cTemp=_T(" ");

	if( m_ePenality==eDayPoint )
		cTemp.Format(_T("%4.0f Points"), m_dPenality);

	else if( m_ePenality==eDayPercent )
		cTemp.Format(_T("%4.2f %% Task's Score"), m_dPenality);

	else if( m_ePenality==eContestPenalty )
		cTemp.Format(_T("%4.0f Points"), m_dPenality);

	return cTemp;
	}

double CPenality::ApplyDailyPenality(double dPoints, double dMaxDayPoints )
{
	double dNewPoints;
	if(	m_ePenality==eDayPoint )
		{
		//dNewPoints= dPoints-(dMaxDayPoints/1000)*m_dPenality;
		dNewPoints= dPoints-m_dPenality;
		return max( dNewPoints, 0 );
		}
	else if(m_ePenality==eDayPercent)
		{
		dNewPoints= Roundoff(dPoints-(dMaxDayPoints*(m_dPenality/100.)));
		return max( dNewPoints, 0 );
		}

	return 0.0;
}

double CPenality::CalculateDailyPenality( double dMaxDayPoints )
{
	double dPenPoints;
	if(	m_ePenality==eDayPoint )
		{
//		dPenPoints= (dMaxDayPoints/1000)*m_dPenality;
		dPenPoints= m_dPenality;
		return max( dPenPoints, 0 );
		}
	else if(m_ePenality==eDayPercent)
		{
		dPenPoints= Roundoff(dMaxDayPoints*(m_dPenality/100.));
		return max( dPenPoints, 0 );
		}

	return 0.0;
}
//		0			1			2			3				4			5
//	_T("ID"), _T("Date"), _T("Type"), _T("Penalty"), _T("Code"), _T("Explaination")};

//	CString		m_cContestNo;
//	CTime		m_cDate;
//	EPenality	m_ePenality;
//	CString		m_cPenalityCode;
//	CString		m_cPenalityReason;
//	double		m_dPenality;

int CALLBACK ComparePenalty(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	int iR=0;
	switch( abs(iColumn) )
		{
	case 0:
		iR=		strcmp( ((CPenality*)lParam1)->m_cContestNo,
						((CPenality*)lParam2)->m_cContestNo  );

		break;
	case 1:
		if(	 ((CPenality*)lParam1)->m_cDate < 
			 ((CPenality*)lParam2)->m_cDate   ) iR=  -1;
		else if( ((CPenality*)lParam1)->m_cDate > 
			 ((CPenality*)lParam2)->m_cDate   ) iR=   1;		break;
	case 2:
		if(	 ((CPenality*)lParam1)->m_ePenality < 
			 ((CPenality*)lParam2)->m_ePenality   ) iR=  -1;
		else if( ((CPenality*)lParam1)->m_ePenality > 
			 ((CPenality*)lParam2)->m_ePenality   ) iR=   1;
		break;
	case 3:
		if(	 ((CPenality*)lParam1)->m_dPenality < 
			 ((CPenality*)lParam2)->m_dPenality   ) iR=  -1;
		else if( ((CPenality*)lParam1)->m_dPenality > 
			 ((CPenality*)lParam2)->m_dPenality   ) iR=   1;
		break;
	case 4:
		iR=		strcmp( ((CPenality*)lParam1)->m_cPenalityCode,
						((CPenality*)lParam2)->m_cPenalityCode  );
		break;
	case 5:
		iR=		strcmp( ((CPenality*)lParam1)->m_cPenalityReason,
						((CPenality*)lParam2)->m_cPenalityReason  );
		break;
	}
	return (iColumn<0)?(-iR):(iR);

}


void CPenality::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	MSXML2::IXMLDOMNodePtr pChildNode=NULL;
	cMgr.CreateChild( pParentNode, pChildNode, _T("Penalty"));

	cMgr.CreateElement		( pChildNode, _T("ContestNo"),		LPCSTR(m_cContestNo));
	GetXMLDate				( cMgr, pChildNode, _T("Date"), m_cDate);
	cMgr.CreateElementInt	( pChildNode, _T("PenaltyType"),	(WORD)m_ePenality);
	cMgr.CreateElement		( pChildNode, _T("ReasonCode"),		LPCSTR(m_cPenalityCode));
	cMgr.CreateElement		( pChildNode, _T("Reason"),			LPCSTR(m_cPenalityReason));
	cMgr.CreateElementInt	( pChildNode, _T("Points"),			(int)m_dPenality);
	}

CPenality::CPenality(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pPenalty)
	{
	GET_XML_STR	( cMgr, pPenalty, _T("ContestNo"),		m_cContestNo);
	ImportXMLDate(cMgr, pPenalty, _T("Date"), m_cDate);
	GET_XML_INT	( cMgr, pPenalty, _T("PenaltyType"),		EPenality,	m_ePenality, eDayPoint);
	GET_XML_STR	( cMgr, pPenalty, _T("ReasonCode"),		m_cPenalityCode);
	GET_XML_STR	( cMgr, pPenalty, _T("Reason"),			m_cPenalityReason);
	GET_XML_INT	( cMgr, pPenalty, _T("Points"),		int,m_dPenality, 0);
	}
