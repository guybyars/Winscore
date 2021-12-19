//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Fix.cpp: implementation of the CFix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "WSView.h"
#include "Winscore.h"
#include <time.h>
#include "Fix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CFix, CFix, DB_SCHEMA_ID)


CFix::CFix( CString& cContestNo, CPosition *pcPos )
	{
	m_strContestNo=cContestNo;
	m_cTime		=pcPos->m_cTime;
	m_fLatitude	=(float)pcPos->GetLat();
	m_fLongitude=(float)pcPos->GetLong();
//	m_iAltitude	=pcPos->m_iPressureAltitude;
	m_iAltitude	=pcPos->m_iCorrectedAltitude;
	m_sMotor	=pcPos->m_iEngineNoiseLevel;
	}

CFix::CFix( CString& cContestNo, CTime& cInterpolationTime, CPosition *pcPos1, CPosition *pcPos2)
	{
	if( cInterpolationTime<=pcPos1->m_cTime		)
		CFix(cContestNo, pcPos1);
	else if( cInterpolationTime>=pcPos2->m_cTime)
		CFix(cContestNo, pcPos2);
	else
		{
		// Interpolate
		m_strContestNo	=cContestNo;
		m_cTime			=cInterpolationTime;
		double dInterval=(double)(pcPos2->m_cTime-pcPos1->m_cTime).GetTotalSeconds();
		double dStep	=(double)(cInterpolationTime-pcPos1->m_cTime).GetTotalSeconds();
		float fPercent	=(float)(dStep/dInterval);
		m_fLatitude		=(float)pcPos1->GetLat() +(float)(pcPos2->GetLat() -pcPos1->GetLat() )*fPercent;
		m_fLongitude	=(float)pcPos1->GetLong()+(float)(pcPos2->GetLong()-pcPos1->GetLong())*fPercent;
		//m_iAltitude		=pcPos1->m_iPressureAltitude+int((pcPos2->m_iPressureAltitude-pcPos1->m_iPressureAltitude)*fPercent);
		m_iAltitude		=pcPos1->m_iCorrectedAltitude+int((pcPos2->m_iCorrectedAltitude-pcPos1->m_iCorrectedAltitude)*fPercent);
		m_sMotor		=pcPos1->m_iEngineNoiseLevel;
		}
	}


CFix::CFix()
	{
	// This empty constructor should be used by serialization only
	}


CFix::CFix(	CTime	&cTime,
			float	fLatitude,
			float	fLongitude,
			int		iAltitude,
			short	sMotor )
			{
			m_strContestNo="";
			m_cTime		=cTime;
			m_fLatitude	=fLatitude;
			m_fLongitude=fLongitude;
			m_iAltitude	=iAltitude;
			m_sMotor	=sMotor;
			}

CFix::~CFix()
{

}

CFix::CFix(CFix &cFix)
	{
	m_strContestNo=cFix.m_strContestNo;
	m_cTime		=cFix.m_cTime;
	m_fLatitude	=cFix.m_fLatitude;
	m_fLongitude=cFix.m_fLongitude;
	m_iAltitude	=cFix.m_iAltitude;
	m_sMotor	=cFix.m_sMotor;
	}

CFix::CFix( int iYear, int iMonth, int iDay, CString strRecord )
	{
//          1         2         3
//01234567890123456789012345678901
//KS 1753503928853N08406695WA00443
	    struct tm newtime;        
		time_t long_time;
		time( &long_time );               
		localtime_s( &newtime, &long_time ); 

		long lTimeZone;
		_get_timezone(&lTimeZone);
		CTimeSpan GMTCorrection(lTimeZone);
		if( newtime.tm_isdst!=0 ) GMTCorrection-=CTimeSpan(3600); 
		int iTimeZone=GMTCorrection.GetHours();

		m_strContestNo=strRecord.Mid( 0, 3 );
		m_strContestNo.TrimRight();

		int iHours	=atoi( strRecord.Mid(3,2) );
		int iMin	=atoi( strRecord.Mid(5,2) );
		int iSec	=atoi( strRecord.Mid(7,2) );

		m_cTime=CTime( iYear, iMonth, iDay, iHours, iMin, iSec )-CTimeSpan(iTimeZone*60*60);
		int iLatDeg	=atoi( strRecord.Mid(9,2) );
		int iLatMin	=atoi( strRecord.Mid(11,5) );
		CString cLat=strRecord.Mid(16,1);
		m_fLatitude= (float)iLatDeg + ((float)iLatMin/1000.f)/60.f;

		int iLonDeg	=atoi( strRecord.Mid(17,3) );
		int iLonMin	=atoi( strRecord.Mid(20,5) );
		CString cLon=strRecord.Mid(25,1);
		m_fLongitude= iLonDeg + ((float)iLonMin/1000.f)/60.f;

		m_iAltitude	=atoi( strRecord.Mid(27,5) );

		if (cLat=='S') m_fLatitude  = -m_fLatitude;
		if (cLon=='W') m_fLongitude = -m_fLongitude;
}






