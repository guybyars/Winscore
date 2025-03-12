//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Position.cpp: implementation of the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Winscore.h"
#include "location.h"
#include "Position.h"

#include "gate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosition::CPosition()
{
	m_iCorrectedAltitude=0;
	m_iPressureAltitude	=0;
	m_iGPSAltitude		=0;
	for( int i=0; i<ALLOCTASKTPS; i++) 
		{
		m_adDistanceFromControlPoint[i]=0.0;
		m_asTurnpoint[i]= -1;
		}
	m_dDistanceFromFinish=0.0;
	m_dDistanceFromStart=0.0;
	m_fVSpeed=0.;
	m_bBFI=false;
	m_bPEV=false;
}

CPosition::~CPosition()
{

}

CPosition::CPosition(int iYear, int iMonth, int iDay, int iTimeZone, CString strRecord, int iAccuracyStartPos, int iAccuracyEndPos, int iENLStartPos, int iENLEndPos, int iMOPStartPos, int iMOPEndPos, bool bBFI,  bool bPEV) : CLocation()
{
		int iAccuracy=30; // Default accuracy of 30m for those without accuracy in trace

		m_dSpeed=0.0;
		m_dAccuracy=0.;
		m_iEngineNoiseLevel	=0;
		m_iMOPLevel			=0;
		m_iPressureAltitude	=0;
		m_iCorrectedAltitude=0;
		m_iGPSAltitude		=0;
		m_fVSpeed			=0.;

		for( int i=0; i<ALLOCTASKTPS; i++) 
			{
			m_adDistanceFromControlPoint[i]=0.0;
			m_asTurnpoint[i]= -1;
			}		
		m_dDistanceFromFinish=0.0;
		m_dDistanceFromStart=0.0;

		m_iStatus	=  0;

		int iHours	=atoi( strRecord.Mid(1,2) );
		int iMin	=atoi( strRecord.Mid(3,2) );
		int iSec	=atoi( strRecord.Mid(5,2) );

		if( iHours>23 || iMin>59 || iSec>59 ) throw 1;

		m_cTime=CTime( iYear, iMonth, iDay, iHours, iMin, iSec )-CTimeSpan(iTimeZone*60*60);
		int iLatDeg	=atoi( strRecord.Mid(7,2) );
		int iLatMin	=atoi( strRecord.Mid(9,5) );
		CString cLat=strRecord.Mid(14,1);
		double dLatMin= (double)iLatMin/1000.;


		int iLonDeg	=atoi( strRecord.Mid(15,3) );
		int iLonMin	=atoi( strRecord.Mid(18,5) );
		CString cLon=strRecord.Mid(23,1);
		double dLonMin= (double)iLonMin/1000.;

		m_iPressureAltitude	=atoi( strRecord.Mid(25,5) );
		m_iGPSAltitude		=atoi( strRecord.Mid(30,5) );


		// Convert altitude from meters to feet
		m_iPressureAltitude=(int)((double)m_iPressureAltitude*3.280848049511);
		m_iGPSAltitude=(int)((double)m_iGPSAltitude*3.280848049511);

//		CLocation(	iLatDeg, dLatMin, 
//					iLonDeg, dLonMin, 
//					(cLat=='N')?(eNORTH):(eSOUTH), 
//					(cLat=='E')?(eEAST):(eWEST) );

		// Initialize CLocation members
		m_iLatDegrees=iLatDeg;
		m_dLatMinutes=dLatMin;
		m_iLongDegrees=iLonDeg;
		m_dLongMinutes=dLonMin;
		m_eLatDirection=(cLat=='N')?(eNORTH):(eSOUTH);
		m_eLongDirection=(cLon=='E')?(eEAST):(eWEST);

		if( m_iLatDegrees==0 && m_dLatMinutes==0 ) throw 1;
		if( m_iLongDegrees==0 && m_dLongMinutes==0 ) throw 1;

		// Read Accuracy
		if( iAccuracyStartPos>0 && iAccuracyEndPos>0 && iAccuracyStartPos<iAccuracyEndPos )
			{
			int iSpan=iAccuracyEndPos-iAccuracyStartPos+1;
			CString strAcc=strRecord.Mid(iAccuracyStartPos-1,iSpan);
			iAccuracy	=atoi( strAcc );
			}
		// Read engine noise level
		if( iMOPStartPos>0 && iMOPEndPos>0 && iMOPStartPos<iMOPEndPos )
			{
			int iSpan=iMOPEndPos-iMOPStartPos+1;
			CString strMOP	=strRecord.Mid(iMOPStartPos-1,iSpan);
			m_iMOPLevel		=atoi( strMOP );
			}

		if( iENLStartPos>0 && iENLEndPos>0 && iENLStartPos<iENLEndPos )
			{
			int iSpan=iENLEndPos-iENLStartPos+1;
			CString strENL=strRecord.Mid(iENLStartPos-1,iSpan);
			m_iEngineNoiseLevel	=atoi( strENL );
			}

		m_dAccuracy=(double)iAccuracy;
		m_dAccuracy/=1000; // Convert m to km.
		m_dAccuracy*=NMPERDEGREE/KMPERDEGREE; // convert km to nm
		m_bBFI=bBFI;
		m_bPEV=bPEV;


	/*
B H H M M S S D D M M M M M N D D D M M M M M E V P P P P P G G G G G CR LF 
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
   
     Time UTC
                      6 bytes
                                 HHMMSS
                                                       Valid characters 0-9
     Latitude
                      8 bytes
                                 DDMMmmmN
                                                       Valid characters N, S, 0-9
     Longitude
                      9 bytes
                                 DDDMMmmmE
                                                       Valid characters E,W, 0-9
     Fix valid
                      1 byte
                                 V
                                                       A: valid, V:nav warning
     Press Alt.
                      5 bytes
                                 PPPPP
                                                       Valid characters -, 0-9
     GNSS Alt.
                      5 bytes
                                 GGGGG
                                                       Valid characters -, 0-9

    The "fix valid" byte should be the equivalent of the "Nav receiver status flag" from the
    NMEA GPRMC sentence. 
*/
}

void CPosition::AddStatus(int iStatus)
	{
	m_iStatus|=iStatus;
	}

void CPosition::RemoveStatus(int iStatus)
	{
	m_iStatus^=iStatus;
	}

void CPosition::SetTurnpoint(int iID, int iPos)
	{
	if( iPos>=ALLOCTASKTPS || iPos<0 ) return;
	m_asTurnpoint[iPos]=iID;
	}

bool CPosition::CheckStatus(int iStatus)
{
	return (iStatus & m_iStatus)?(true):(false);
}

void CPosition::SetDistanceFromStart(double dDist)
{
	m_dDistanceFromStart=dDist;

}

void CPosition::SetDistanceFromFinish(double dDist)
{
	m_dDistanceFromFinish=dDist;

}

void CPosition::SetDistanceFromControlPoint(int iPnt, double dDist)
{
	if( iPnt<ALLOCTASKTPS && iPnt>=0 ) m_adDistanceFromControlPoint[iPnt]=dDist;
}


double CPosition::GetDistanceFromControlPoint(int iPnt)
	{
	if( iPnt>ALLOCTASKTPS || iPnt<0 ) 
		return 0.0;
	else
		return m_adDistanceFromControlPoint[iPnt];
	
	}

CString CPosition::GetStatusText(bool bStartGateCyl, bool bFinishGateCyl)
{
	CString strOut="";
	if( CheckStatus( FAN_ROLLING		 ) 	) strOut+="Flying, ";
	if( CheckStatus( FAN_LANDED			 ) 	) strOut+="Landed, ";
	if( CheckStatus( FAN_IN_START_CYL	 )	) 
		{
		if( bStartGateCyl )	strOut+="InStartCyl, ";    
		if( CheckStatus( FAN_ABOVE_START_CYL )	) strOut+="AboveStart, ";   
		}
	if( CheckStatus( FAN_IN_FINISH_CYL	 )  ) 
		{
		if( bFinishGateCyl ) strOut+="InFinishCyl, ";
		if( CheckStatus( FAN_BELOW_FINISH_CYL)	) strOut+="BelowFinish, ";
		}
	if( CheckStatus( FAN_MOTOR_ON		 ) 	)			strOut+="MotorON, ";
	if( CheckStatus( FAN_TURN_AREA_TURNPOINT   ) 	)	strOut+="Turn Area Turnpoint, ";
	else if( CheckStatus( FAN_AT_TURNPOINT    ) 	)	strOut+="AtTurnpoint, ";
	else if( CheckStatus( FAN_OUTER_TURNPOINT   ) 	)	strOut+="OuterTurnpointRadius, ";
	if( CheckStatus( FAN_START			 ) 	) 
		{
		strOut+=bStartGateCyl?"Started, ":"Crossed Start, ";
		}

	if( CheckStatus( FAN_FINISH			 ) 	) 
		{
		strOut+=bFinishGateCyl?"Finished, ":"Crossed Finish, ";;
		}
	if( CheckStatus( FAN_LATEST_START	 ) 	) strOut+="LatestStart, ";
	if( CheckStatus( FAN_GAP			 ) 	) strOut+="GapInTrace, ";
	if( CheckStatus( FAN_FURTHEST_PROGRESS)	) strOut+="FurthestProgress, ";


	int iComma= strOut.ReverseFind( ',' );
	if( iComma>0 ) strOut.SetAt(iComma, ' ');

	return strOut;
}

bool CPosition::CheckTurnpoint(int iPos, int iTPID)
{
	if( iPos<0 || iPos>=ALLOCTASKTPS ) return false;

	return m_asTurnpoint[iPos]==iTPID;;
}

bool CPosition::CheckTurnpoint(int iTPID, bool bOnlyOne)
{
	if( bOnlyOne )
		{
		int iNum=0;
		for( int i=0; i<ALLOCTASKTPS; i++) 
			if( m_asTurnpoint[i]>0 ) iNum++;
		if( iNum!=1 ) return false;
		}

	for( int i=0; i<ALLOCTASKTPS; i++) 
		{
		if( m_asTurnpoint[i]==iTPID ) return true;
		}
	return false;
}

CString CPosition::GetTurnpointText()
	{
	CString cText=_T("");
	int iNum=GetNumTurnpoints();
	if( iNum==0 ) return _T("");

	if( CheckStatus(FAN_TURN_AREA_TURNPOINT) )
		cText=_T("Assigned Area Point ");

	for( int i=0; i<MAXTASKTPS; i++) 
		{
		if( m_asTurnpoint[i]<0 ) continue;
		CString cInt;
		cInt.Format(_T("%d  "), m_asTurnpoint[i] );

		if( CheckStatus(FAN_AT_TURNPOINT) && iNum==1 )
			cInt.Format(_T("At %d "), m_asTurnpoint[i] );
		else
			cInt.Format(_T("%d "), m_asTurnpoint[i] );
		
		if( cText.GetLength()>0 ) cText+=_T(", ");

		cText+=cInt;
		}
	return cText;
	}


CString CPosition::GetTurnpointDistText(EUnits eUnits)
{
	int iNum=GetNumTurnpoints();
	if( iNum==0 ) return _T("");

	CString cText;
	for( int i=0; i<MAXTASKTPS; i++) 
		{
		if( m_asTurnpoint[i]<0 ) continue;

		CString cInt;
		cInt.Format(_T("%6.3lf"), ConvertDistance(m_adDistanceFromControlPoint[i], SYSTEMUNITS, eUnits) );

		if( cText.GetLength()>0 ) cText+=_T(",  ");

		cText +=cInt;
		}

	return cText;
}

int CPosition::GetNumTurnpoints()
{
	int iNum=0;
	for( int i=0; i<ALLOCTASKTPS; i++) if( m_asTurnpoint[i]>=0 ) iNum++;
	return iNum;
}

int CPosition::GetTurnpoint(int iPos)
	{
	if( iPos<0 ) 
		return 0;
	else
		return m_asTurnpoint[iPos];
	}


CString CPosition::GetTimeString()
{
return m_cTime.Format( _T("%H:%M:%S") );
}
