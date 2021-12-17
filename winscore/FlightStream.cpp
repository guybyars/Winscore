//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightStream.cpp: implementation of the CFlightStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "fix.h"
#include "FlightStream.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CFlightStream, CFlightStream, VERSIONABLE_SCHEMA|2)

CFlightStream::CFlightStream(CTime &cStart, CTime &cEnd, int iIncrement)
	{

//  Perform check on start/end times.  Make sure it isn't more than 12 hours
	CTimeSpan cSpan=cEnd-cStart;
	if( cSpan.GetTotalHours()>12 )
		m_cEndhTime=cStart+CTimeSpan(0,12,0,0);
	else
		m_cEndhTime=cEnd;

	m_iIncrement=iIncrement;
	m_MaxPoints=(long)cSpan.GetTotalSeconds()/iIncrement;	
	m_cStartTime=cStart;
	m_iPointsAvailable=0;
	m_sMinAltitude=32000;
	m_sMaxAltitude=0;
	if( m_MaxPoints>0 )
		m_pcData=new CFixList[m_MaxPoints];
	else
		m_pcData=NULL;

	}

CFlightStream::CFlightStream()
	{
	m_sMinAltitude=32000;
	m_sMaxAltitude=0;
	m_iPointsAvailable=0;
	m_pcData=NULL;
	}

CFlightStream::~CFlightStream()
	{
	if( m_pcData ) delete[] m_pcData;
	}

void CFlightStream::AddFlight(CFlight *pcFlight, int iHomeElevation)
	{

	CTimeSpan		cPrevSpan(0);
	int iStart=0;

	for( int iList=0; iList<m_MaxPoints; iList++ )
		{
		CTime cListTime=m_cStartTime+iList*m_iIncrement;
		CPosition* pcPrevPos=NULL;

		int iAltCorrection=0;

		CPosition* pcFirstPos=pcFlight->GetPosition(0);
		if( pcFirstPos )
			iAltCorrection=pcFirstPos->m_iPressureAltitude-iHomeElevation;


		for(int iPos=iStart; iPos<pcFlight->GetNumPoints(); iPos++ )
			{
			CPosition* pcPos=pcFlight->GetPosition(iPos);
			iStart=iPos;
				
			int iError=abs( pcPos->m_iPressureAltitude-pcPos->m_iGPSAltitude );

			pcPos->m_iCorrectedAltitude=pcPos->m_iPressureAltitude-iAltCorrection;

			if( iError<500 ) // If the GPS and Pressure differ by too much, ignore this point in the min/max
				{
				m_sMinAltitude=min(m_sMinAltitude, pcPos->m_iCorrectedAltitude);
				m_sMinAltitude=max(m_sMinAltitude,0);

				if( pcPos->m_iCorrectedAltitude>m_sMaxAltitude )
					{
					// In a crude attempt to filter out stray points in the stratosphere, 
					// only increment larget jumps
					if(pcPos->m_iCorrectedAltitude-m_sMaxAltitude<2000 )
						m_sMaxAltitude= pcPos->m_iCorrectedAltitude;
					else
						m_sMaxAltitude+=200;
					}
				}
			if( pcPos->m_cTime == cListTime ) 
				{
				m_pcData[iList].AddTail( new CFix( pcFlight->m_strCID, pcPos ) );
				break;
				}

			if( pcPrevPos )
				{
				if( pcPrevPos->m_cTime	< cListTime &&
					pcPos->m_cTime		> cListTime		)
					{
					m_pcData[iList].AddTail( new CFix( pcFlight->m_strCID, cListTime, pcPrevPos, pcPos )  );
					break;
					}				 
				}

			if( pcPos->m_cTime > cListTime ) break;


			pcPrevPos=pcPos;
			}
		}
	}


void CFlightStream::Serialize(CArchive & ar)
{

	if (ar.IsStoring())
		{
		ar << m_iIncrement;
		ar << m_MaxPoints;
		SerializeTime(ar,m_cStartTime);
		SerializeTime(ar,m_cEndhTime);
		ar << m_sMinAltitude;
		ar <<  m_sMaxAltitude;

		for( int i=0; i<m_MaxPoints; i++ )
			m_pcData[i].Serialize(ar);
		}
	else
		{

		m_iPointsAvailable=0;
		ar >> m_iIncrement;
		ar >> m_MaxPoints;
		SerializeTime(ar, m_cStartTime);
		SerializeTime(ar, m_cEndhTime);
		ar >> m_sMinAltitude;
		ar >>  m_sMaxAltitude;

		m_pcData=new CFixList[m_MaxPoints];

		for( int i=0; i<m_MaxPoints; i++ )
			{
			m_pcData[i].Serialize(ar);
			m_iPointsAvailable++;
			}

		}

}

