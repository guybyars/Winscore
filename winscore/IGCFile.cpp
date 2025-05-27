//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// IGCFile.cpp: implementation of the CIGCFile class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "winscore.h"

#include "position.h"
#include "Averager.h"
#include "IGCFile.h"
#include "IGCDirectories.h"
#include <fstream>
#include <math.h>
#include <atlbase.h>

#include "global_prototypes.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXLINELEN	256

#define   FATAL_ERROR(strRecType)   { \
			     m_strErrorText.Format(_T("ERROR occured on line : %d; RecordType : %s "), 	m_nLinesRead, strRecType ); \
				 m_strErrorText+=_T("\n");\
				 m_strErrorText+=strRecord;\
				 m_bValid=false;}  \

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGCFile::CIGCFile()
	{
	m_bCONVCAM=false;
	m_bValid=false;
	m_nPositionPoints=0;
	m_nLinesRead=0;
	m_iTimeZone=0;
	m_nInvalidFixes=0;
	m_iMaxPresAltitude=0;
	m_iMaxGPSAltitude=0;
	m_iENLMin=0;
	m_iENLMax=0;
	m_iMOPMin=0;
	m_iMOPMax=0;
	}

CIGCFile::~CIGCFile()
	{
	FreePositionData();
	}


CIGCFile::CIGCFile(CString strIGCFileName)
{
	m_iAccuracyStartPos= -1;
	m_iAccuracyEndPos= -1;
	m_iENLStartPos= -1;
	m_iENLEndPos= -1;
	m_iMOPStartPos= -1;
	m_iMOPEndPos= -1;
	m_strFilePath=GetRelativePath(strIGCFileName);
	m_bValid=true;
	m_iDay	=0;
	m_iMonth=0;
	m_iYear	=0;
	m_nPositionPoints=0;
	m_nLinesRead=0;
	m_bFileMissing=false;
	m_iTimeZone=0;
	m_nInvalidFixes=0;
	m_iMaxPresAltitude=0;
	m_iMaxGPSAltitude=0;
	m_bCONVCAM=false;
	m_iENLMin=0;
	m_iENLMax=0;
	m_iMOPMin=0;
	m_iMOPMax=0;

	char  cRecord[MAXLINELEN] ;
	CTime	cCurrentDate;	

	ifstream	cIfstream;

//  Save last modified date
	CFile cfile(strIGCFileName, CFile::modeRead|CFile::shareDenyNone );
	CFileStatus rStatus;
	cfile.GetStatus( rStatus );
	m_strFileName=cfile.GetFileName();
	cfile.Close();
	m_mtime=rStatus.m_mtime;
	

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(strIGCFileName, ios::binary  );
	if (cIfstream.is_open()==0)
		{
		return;
		}

	while( !cIfstream.eof() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		m_nLinesRead++;
		if( cIfstream.fail() ) break;
		CString strRecord(cRecord);
		strRecord.TrimLeft();
		if( strRecord.GetLength()<=0 ) continue;

		if(		 strRecord[0]=='A' )
			{
			if( !ProcessARecord(strRecord) ) 
				{
				FATAL_ERROR(_T("A"));
				cIfstream.close();
				return;
				}
			}
		else if( strRecord[0]=='H' )
			{
			if( !ProcessHRecord(strRecord) ) 
				{
				FATAL_ERROR(_T("H"));
				cIfstream.close();
				return;
				}
			}
		else if( strRecord[0]=='I' )
			{
			if( !ProcessIRecord(strRecord) ) 
				{
				FATAL_ERROR(_T("I"));
				cIfstream.close();
				return;
				}
			}
		else if( strRecord[0]=='B' )
			{
			//m_nPositionPoints++;
			break;
			}

		}// end loop on records

	cIfstream.close();

//	m_bValid=m_nPositionPoints>0;
	m_bValid=true;

}

bool CIGCFile::ProcessARecord(CString strRecord)
	{
	if( strRecord.GetLength()<7 ) 
		{
		return false;
		}
	m_strManufacturer=strRecord.Mid(1,3);
	m_strFDRID=strRecord.Mid(4,6);
	m_strFDRID.TrimLeft();
	m_strFDRID.TrimRight();

	if( m_strFDRID[3]=='_' || m_strFDRID[3]==',' )
		{
		m_strFDRID=m_strFDRID.Left(3);
		}
	else if( strRecord.Mid(7,6)=="FLIGHT" )
		{
		m_strFDRID=m_strFDRID.Left(3);
		}

	return true;
	}

bool CIGCFile::ProcessIRecord(CString strRecord)
	{
	int iFXAPos= strRecord.Find(_T("FXA"));
	if( iFXAPos>=0 )
		{
		m_iAccuracyStartPos	=atoi( strRecord.Mid(iFXAPos-4,2) );
		m_iAccuracyEndPos	=atoi( strRecord.Mid(iFXAPos-2,2) );
		}
	int iENLPos= strRecord.Find(_T("ENL"));
	if( iENLPos>=0 )
		{
		m_iENLStartPos=atoi( strRecord.Mid(iENLPos-4,2) );
		m_iENLEndPos	=atoi( strRecord.Mid(iENLPos-2,2) );
		}
	int iMOPPos= strRecord.Find(_T("MOP"));
	if( iMOPPos>=0 )
		{
		m_iMOPStartPos	=atoi( strRecord.Mid(iMOPPos-4,2) );
		m_iMOPEndPos	=atoi( strRecord.Mid(iMOPPos-2,2) );
		}
	

	return true;
	}

bool CIGCFile::ProcessHRecord(CString strRecord)
{

	CString strSubType=strRecord.Mid(2,3);
	int iLen=strRecord.GetLength();
	int iColon=strRecord.Find(':',0);
	if( strSubType==_T("DTE") )
		{
// We have two possible formats
		if( iColon>0 ) 
			{
//HFDTEDATE:080319
			if( iLen<16 ) return false;
			m_iDay	=atoi( strRecord.Mid(10,2) );
			m_iMonth=atoi( strRecord.Mid(12,2) );
			m_iYear	=atoi( strRecord.Mid(14,2) );
			}
		else
			{
//HFDTE080319
			if( iLen<11 ) return false;
			m_iDay	=atoi( strRecord.Mid(5,2) );
			m_iMonth=atoi( strRecord.Mid(7,2) );
			m_iYear	=atoi( strRecord.Mid(9,2) );
			}

		// Paranoia Checks
		if( m_iDay<1	|| m_iDay>31	||
			m_iMonth<1	|| m_iMonth>12	||
			m_iYear<0	|| m_iYear>99	   ) return false;

		//  I'll be 122 years old when this code breaks.  No sweat!!!!
		if( m_iYear<80 ) 
			m_iYear+=2000;
		else
			m_iYear+=1900;
		}
	else if( strSubType==_T("PLT") )
		{
		if( iColon<0 ) iColon=12;
		m_strPilot=strRecord.Right(iLen-iColon-1);
		m_strPilot.TrimLeft();
		m_strPilot.TrimRight();
		}
	else if( strSubType==_T("GTY") )
		{
		if( iColon<0 ) iColon=17;
		m_strGliderType=strRecord.Right(iLen-iColon-1);
		m_strGliderType.TrimLeft();
		m_strGliderType.TrimRight();
		}
	else if( strSubType==_T("GID") )
		{
		if( strRecord.Find(_T("GliderType"))>=0 || strRecord.Find(_T("Glider Type"))>=0)
			{
			if( iColon<0 ) iColon=17;
			m_strGliderType=strRecord.Right(iLen-iColon-1);
			m_strGliderType=strRecord.Right(iLen-16);
			m_strGliderType.TrimLeft();
			m_strGliderType.TrimRight();
			}
		else
			{
			if( iColon<0 ) iColon=15;
			m_strGliderID=strRecord.Right(iLen-iColon-1);
			m_strGliderID.TrimLeft();
			m_strGliderID.TrimRight();
			}
		}
	else if( strSubType==_T("CID") )
		{
		if( iColon<0 ) iColon=20;
		m_strCompetitionID=strRecord.Right(iLen-iColon-1);
		m_strCompetitionID.TrimLeft();
		m_strCompetitionID.TrimRight();
		m_strCompetitionID.MakeUpper();
		}
	else if( strSubType==_T("SIT") )
		{
		if( iColon<0 ) iColon=11;
		m_strSite=strRecord.Right(iLen-iColon-1);
		m_strSite.TrimLeft();
		m_strSite.TrimRight();
		}
	else if( strSubType==_T("UNT") )
		{
		if( iColon<0 ) iColon=12;
		m_strUnits=strRecord.Right(iLen-iColon-1);
		m_strUnits.TrimLeft();
		m_strUnits.TrimRight();
		}
	else if( strSubType==_T("RHW") )
		{
	//HFRHWHARDWAREVERSION:300 Series Version 2
		if( iColon<0 ) iColon=20;
		m_strHardwareVersion=strRecord.Right(iLen-iColon-1);
		m_strHardwareVersion.TrimLeft();
		m_strHardwareVersion.TrimRight();
		}

	else if( strSubType==_T("TZO") )
		{
		if( iColon>0 )
			{
			CString strTimeZone=strRecord.Right(iLen-iColon-1);
			strTimeZone.Replace(_T("UTC"),_T(""));
			strTimeZone.Replace(_T(":00"),_T(""));
			int nRead=sscanf_s(strTimeZone, _T("%d"), &m_iTimeZone);
			if ( nRead!=1 )	m_iTimeZone=0;
			}
		}
	else if( strSubType==_T("TZN") )
		{
//HFTZNTIMEZONE:UTC-04:00
		if( iColon>0 )
			{
			CString strTimeZone=strRecord.Right(iLen-iColon-1);
			strTimeZone.Replace(_T("UTC"),_T(""));
			strTimeZone.Replace(_T(":00"),_T(""));
			int nRead=sscanf_s(strTimeZone, _T("%d"), &m_iTimeZone);
			if ( nRead!=1 )	m_iTimeZone=0;
			}
		}


/*
ACAM00074
HFDTE111098
HFFXA050
HFPLTPilot: Guy Byars
HFGIDGliderType: N49EH    
HFGIDGliderID: 11
HFDTM100Datum: 
HFRFWFIRMWAREVERSION: 
HFRHWHARDWAREVERSION: 0
HFSITSite: Caesar Creek, Ohio              
HFTZOTimezone: 0
HFUNTUnits: mi,ft,kt


H F DTE DATE:  DD MM YY,  NN  CR

H S D T E D D M M Y Y CR LF 
H S F X A A A A CR LF 
H S P L T P I L O T : T E X T S T R I N G CR LF 
H S G T Y G L I D E R T Y P E : T E X T S T R I N G CR LF 
H S G I D G L I D E R I D : T E X T S T R I N G CR LF 
H S D T M N N N G P S D A T U M : T E X T S T R I N G CR LF 
H S R F W F I R M W A R E V E R S I O N: T E X T S T R I N G CR LF 
H S R H W H A R D W A R E V E R S I O N: T E X T S T R I N G CR LF 
H S C I D C O M P E T I T I O N I D : T E X T S T R I N G CR LF 
H S C C L C O M P E T I T I O N C L A S S : T E X T S T R I N G CR LF 
*/

return true;
}

CString CIGCFile::SummaryText()
{
	CString str, str1;

	//str1.Format("Date : %d/%d/%d", m_iDay, m_iMonth, m_iYear );
	str+=CTime( m_iYear, m_iMonth, m_iDay, 0,0,0).Format( _T("\r\nDate: %B %d, %Y") );
	str+=str1+_T("\r\n");

	str1.Format(_T("Pilot : %s"), m_strPilot );
	str+=str1+_T("\r\n");
	str1.Format(_T("Glider ID : %s"), m_strGliderID );
	str+=str1+_T("\r\n");
	str1.Format(_T("Competition ID : %s"), m_strCompetitionID );
	str+=str1+_T("\r\n");
	str1.Format(_T("Units : %s"), m_strUnits );
	str+=str1+_T("\r\n");
	str1.Format(_T("Manufacturer : %s"), m_strManufacturer );
	str+=str1+_T("\r\n");
	str1.Format(_T("Recorder ID : %s"), m_strFDRID );
	str+=str1+_T("\r\n");
	str1.Format(_T("Number of lines read : %d"), 	m_nLinesRead );
	str+=str1+_T("\r\n");
	str1.Format(_T("Number of Position Fixes : %d"), 	m_nPositionPoints );
	str+=str1+_T("\r\n");

	return str;
}

bool CIGCFile::GetARecord(CString strIGCFile,  CString &strLongName )
{
	ifstream	cIfstream;
	char		cRecord[MAXLINELEN];
	CString		strDTERecord,strARecord;
	CString strManufacturer, strSN;
	int iYear, iMonth, iDay, iVer=1;

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(strIGCFile, ios::binary    );
	if (cIfstream.is_open()==0) return false;

	bool bFoundA=false, bFoundHDTE=false;
	while( !cIfstream.eof() && (!bFoundA || !bFoundHDTE) )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		CString strRecord(cRecord);
		strRecord.TrimLeft();
		int iLen=strRecord.GetLength();
		if( iLen<=0 ) continue;

		if( strRecord[0]=='A' )
			{
			strARecord=strRecord;
			strManufacturer=strRecord.Mid(1,3);
			int iFlight=strRecord.Find("FLIGHT:");
			if( iFlight>6 ) 
				{
				iVer=atoi(strRecord.Mid(iFlight+7,(iLen-iFlight+7)) );
				iVer=min(iVer,9);
				iVer=max(iVer,1);
				strSN=strRecord.Mid(4,3);
				}
			else
				{
				if(iLen==11)
				  strSN=strRecord.Mid(4,6);
				else
				  strSN=strRecord.Mid(4,3);
				}

			bFoundA=true;
			}
		if( strRecord.Find("HFDTE")==0 && !bFoundHDTE ) 
			{
			strDTERecord=strRecord;
			int iColon=strRecord.Find(':',0);
// We have two possible formats
			if( iColon>0 ) 
				{
//HFDTEDATE:080319
				iYear	=atoi(strRecord.Mid(14,2))+2000;
				iMonth	=atoi(strRecord.Mid(12,2));
				iDay	=atoi(strRecord.Mid(10,2));
				bFoundHDTE=true;
				}
			else
				{
//HFDTE080319
				iYear	=atoi(strRecord.Mid(9,2))+2000;
				iMonth	=atoi(strRecord.Mid(7,2));
				iDay	=atoi(strRecord.Mid(5,2));
				bFoundHDTE=true;
				}
			}
		}

	cIfstream.close();
	if( bFoundA && bFoundHDTE )
		{
        strLongName.Format("%4d-%02d-%02d-%s-%s-%02d.IGC",iYear,iMonth,iDay,strManufacturer,strSN,iVer);
		return true;
		}
	else
		return false;


}

bool CIGCFile::ReadFlight(bool bReadWaypoints)
{
	CSingleLock cLock(&m_cSem_ReadFlight);

	if( !m_bValid ) return false;

	if( m_iDay==0 && m_iMonth==0 && m_iYear==0 ) return false;

	int iNWaypoints=0;
	int iTimeZone=0;
	bool bIncrementedDay=false;

	CFileStatus rStatus;

	// Get the current time to figure out daylight savings time.
    struct tm newtime;        
    time_t long_time;
    time( &long_time );               
    localtime_s(&newtime, &long_time);


	ELogTime	eTimeOption;
	eTimeOption = (ELogTime)GetWinscoreInt( LOGTIMEOPTION, eSystemTime ); 

	if( eTimeOption==eSystemTime )
		{
		long iSeconds;
		_get_timezone(&iSeconds);
		CTimeSpan GMTCorrection(iSeconds);
		if( newtime.tm_isdst!=0 ) GMTCorrection-=CTimeSpan(3600); 
		iTimeZone=GMTCorrection.GetHours();
		}
	else if( eTimeOption==eUserTime )
		{
		iTimeZone = -1 * (GetWinscoreInt( LOGTIMEUSERVALUE, 0 )); 
		}
	else if( eTimeOption==eLogTime )

		{
		iTimeZone=-m_iTimeZone;
		}

	char  cRecord[MAXLINELEN] ;
	int nLinesRead=0;
	m_nPositionPoints=0;
	FreePositionData();
	CPosition* pcPrevPos=NULL;
	ifstream	cIfstream;
	int iDay	=m_iDay;
	int iMonth	=m_iMonth;
	int iYear	=m_iYear;
	int nBadFixes=0;
	bool bBFI=false;
	bool bPEV=false;


	// Correct the full file name if the flight logs were moved.
	if( !CFile::GetStatus(GetNonRelativePath(m_strFilePath),  rStatus) )
		{
		CIGCDirectories cIGCDIR;
		if( !cIGCDIR.FindIGCFileName( m_strFileName, m_strFilePath ) ) return false;
		}

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(GetNonRelativePath(m_strFilePath), ios::binary    );
	if (cIfstream.is_open()==0)
		{
		return false;
		}

	while( !cIfstream.eof() &&  !cIfstream.fail() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		nLinesRead++;

		// paranoia check to prevent infinite loop.  A million lines should be enough.
		if( nLinesRead>1000000) break;

		CString strRecord(cRecord);
		strRecord.TrimLeft();
		int iLen=strRecord.GetLength();
		if( iLen<=0 ) continue;


		if(		 strRecord[0]=='L' && bReadWaypoints )
			{
			if( strRecord.Find(_T("LCAMN00"),0)==0 )
				{
				CTurnpoint* pTurnpoint=new CTurnpoint(strRecord);
				m_cTurnpointArray.Add(pTurnpoint);
				}
			}

		 if( strRecord[0]=='I' ) ProcessIRecord(strRecord);


		if(		 strRecord[0]=='B' && (iLen<25 || strRecord[24]!='A') )
			m_nInvalidFixes++;


		if(		 strRecord[0]=='B' && (iLen>25 && strRecord[24]=='A') )
			{
			CPosition *pPos=NULL;
			try
				{
				pPos=new CPosition( iYear, iMonth, iDay, 0,  strRecord, m_iAccuracyStartPos, m_iAccuracyEndPos, m_iENLStartPos, m_iENLEndPos, m_iMOPStartPos, m_iMOPEndPos, bBFI, bPEV);
				bPEV=false;
				}
			catch(...)
				{
				nBadFixes++;
				continue;
				}

			// Kludge the date.  If we see that the hours of a fix is =0, but 
			// the previous fix is =23, that means that it has just passed
			// midnight GMT.  Correct for this. (but only do it once.)
			if( pPos && pcPrevPos && pPos->m_cTime>0 && pcPrevPos->m_cTime>0 )
				{
				if(	   (pPos->m_cTime.GetHour()>=0			&& 
						pPos->m_cTime.GetHour()<=12			&& 
						pcPrevPos->m_cTime.GetHour()<=23	&&
						pcPrevPos->m_cTime.GetHour()>=13)	&& !bIncrementedDay )
					{
					// Refresh the day month (by crossing over, it is a day behind)
					pPos->m_cTime+=CTimeSpan(60*60*24);
					iDay=pPos->m_cTime.GetDay();
					iMonth=pPos->m_cTime.GetMonth();
					iYear=pPos->m_cTime.GetYear();
					bIncrementedDay=true;
					}
				}



			if( pcPrevPos!=NULL && pcPrevPos->m_cTime==pPos->m_cTime ) // Filter out duplicate times
				{
				delete pPos;
				}
			else
				{
				m_PositionArray.Add(pPos);
				m_nPositionPoints++;
				pcPrevPos=pPos;

				m_iMaxPresAltitude=max(m_iMaxPresAltitude,pPos->m_iPressureAltitude);
				m_iMaxGPSAltitude=max(m_iMaxGPSAltitude, pPos->m_iGPSAltitude);

				}
			}

		if(		 strRecord[0]=='E' )
			{
			// Check for Artificial horizon on/off
			//E195039BFION AH
			//E195052BFIOFF AH
			if( strRecord.Find("BFION AH")>0 )	
				bBFI = true;
			else if( strRecord.Find("BFIOFF AH")>0 )	
				bBFI = false;
			else if( strRecord.Find("PEV")>0 )	
				bPEV = true;
			}

		}// end loop on records

	cIfstream.close();

	if( nBadFixes>0 ) 
		{
		CString strBad;
		strBad.Format("%d invalid fixes were skipped in %s", nBadFixes,m_strFileName);
		AfxMessageBox(strBad);
		}


	// Now adjust all the times for itime offset, and update the ENL/MOP min/maxes
	for( int iPos=0; iPos<m_PositionArray.GetSize(); iPos++)
		{
		((CPosition*)m_PositionArray[iPos])->m_cTime-=CTimeSpan(iTimeZone*60*60);

		m_iENLMax=max(m_iENLMax,((CPosition*)m_PositionArray[iPos])->m_iEngineNoiseLevel);
		m_iMOPMax=max(m_iMOPMax,((CPosition*)m_PositionArray[iPos])->m_iMOPLevel);
		if( iPos==0 )
			{
			m_iENLMin=1000;
			m_iMOPMin=1000;
			}
		else
			{
			m_iENLMin=min(m_iENLMin,((CPosition*)m_PositionArray[iPos])->m_iEngineNoiseLevel);
			m_iMOPMin=min(m_iMOPMin,((CPosition*)m_PositionArray[iPos])->m_iMOPLevel);
			}
		}
	
	// If we just read it, it can't be missing.
	SetIGCFileMissing(false);

	return true;

}


int CIGCFile::GetNumPoints()
{
return m_PositionArray.GetSize();
}

CPosition* CIGCFile::GetPosition(int i)
	{
	ASSERT(i<m_PositionArray.GetSize() && i>=0);

	if( i>=m_PositionArray.GetSize() || i<0 ) return NULL;
	return (CPosition*)m_PositionArray[i];
	}

void CIGCFile::FreePositionData()
	{
	CSingleLock cLock(&m_cSem_FreePositionData);
	for( int i=0; i<m_PositionArray.GetSize(); i++)
		delete (CPosition*)m_PositionArray[i];
	m_PositionArray.RemoveAll();
	}

CString CIGCFile::GetContestID()
{
return m_strCompetitionID;
}

void CIGCFile::SetIGCFileMissing(bool bMissing)
{
m_bFileMissing=bMissing;
}

CString CIGCFile::GetIGCFileNameText()
{
	CString strOut;
	if( m_bFileMissing ) strOut+=_T("missing ");
	strOut+=m_strFilePath;
	return strOut;
}

bool CIGCFile::ContainsFixAccuracy()
{
 return  m_iAccuracyStartPos>0 && m_iAccuracyEndPos>0;
}

bool CIGCFile::GetTimeRange(CTime &cTimeBegin, CTime &cTimeEnd)
	{
	int nPoints=m_PositionArray.GetSize();
	if( nPoints==0 ) return false;

	CPosition* pPos=GetPosition(0);
	if( !pPos ) return false;
	cTimeBegin=pPos->m_cTime;
	
	pPos=GetPosition(nPoints-1);
	if( !pPos ) return false;
	cTimeEnd=pPos->m_cTime;
	return true;

	}

void	CIGCFile::AssignSpeedROC()
	{
	double	dSpeed=0.0, dAveSpeed;
	CArray<double,double> dSpeedArray;

	CAverager cAveAltDiff(5);

	CPosition* pcPos	=GetPosition(0);
	CPosition* pcPrevPos=NULL;

	int iPrevAltitude=0;
	int	iAltitude=0;

	int nPoints=GetNumPoints();

	double dLastSpeed=0.0;
	int iSec=0;
	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;

		// Compute speeds and store in array
		if( iPoint>0 )
			{
			CTimeSpan cInc=pcPos->m_cTime-pcPrevPos->m_cTime;
			double dDist=pcPos->DistanceTo( pcPrevPos, SYSTEMUNITS );
			iSec=(int)cInc.GetTotalSeconds();
			if( iSec>0 )
				{
				dSpeed=dDist/iSec;
				dSpeed*=3600;
				}

			// Now filter the speed by limiting the acceleration to 4 knots per second.
			double dAccel=4*iSec;
			dAccel=min(40.,dAccel);  // but keep it reasonable between fixes.

			if ( dSpeed>dLastSpeed )
				{
				if( dSpeed-dLastSpeed > dAccel )
					dSpeed=dLastSpeed + dAccel;
				}
			else
				{
				if( dLastSpeed-dSpeed > dAccel )
					dSpeed=dLastSpeed - dAccel;
				}
			}

		dSpeedArray.Add( dSpeed );
		dLastSpeed=dSpeed;
		}
	

	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{

		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;


		iAltitude=pcPos->m_iPressureAltitude;
		if( iPoint==0 )
			iPrevAltitude=iAltitude;

		dAveSpeed=GetAveSpeed(dSpeedArray, iPoint,5);
		pcPos->m_dSpeed=dAveSpeed;

		cAveAltDiff.SetValue(iAltitude-iPrevAltitude);
		int iGrossAltChange = abs((int)cAveAltDiff.Average());

		if( iPoint>0 )
			{
			CTimeSpan cInc=pcPos->m_cTime-pcPrevPos->m_cTime;
			int iSec=(int)cInc.GetTotalSeconds();

			pcPos->m_fVSpeed=(float)cAveAltDiff.Average()/(float)iSec;
			}

		iPrevAltitude=iAltitude;

		}

	}// end method


double CIGCFile::GetAveSpeed( CArray<double,double> &dSpeedArray, int iPoint, int nPoints)
	{
	CAverager cAveSpeed(nPoints);
	for( int i=0; i<nPoints; i++ )
		{
		int iPos=iPoint-(nPoints/2)+i;
		if( iPos<0 || iPos>=dSpeedArray.GetSize())
			cAveSpeed.SetValue( 0.00 );
		else
			cAveSpeed.SetValue( dSpeedArray[iPos] );		
		}
	return cAveSpeed.Average();
	}

BOOL CIGCFile::ValidateFileName()
	{
	CString strPrefix=GetDatePrefix(m_iYear, m_iMonth, m_iDay);
	CString strFileName=m_strFileName;
	strPrefix.MakeLower();
	strFileName.MakeLower();
    if( strFileName.Find(strPrefix)==0 ) return true;

	strPrefix=GetLongDatePrefix(m_iYear, m_iMonth, m_iDay);
	strFileName=m_strFileName;
	strPrefix.MakeLower();
	strFileName.MakeLower();
    if( strFileName.Find(strPrefix)==0 ) return true;

	return false;
	}

CString CIGCFile::GetValidFileName()
	{
	return GetDatePrefix(m_iYear, m_iMonth, m_iDay)+m_strFileName.Right(m_strFileName.GetLength()-3);
	}


CString	CIGCFile::GetDatePrefix(int iYear, int iMonth, int iDay)
	{
	CString strAlphabet=_T("123456789abcdefghijklmnopqrstuv");

	double dYear=((double)iYear)*.1;
	double dSDYear;
	double dInt=modf( dYear, &dSDYear );
	dSDYear=dInt*10.000001;

	CString cMonth=strAlphabet[iMonth-1];
	CString cDay  =strAlphabet[iDay-1];
	CString str;
	str.Format(_T("%d%s%s"), (int)dSDYear, cMonth, cDay );
	return str;
	}

CString	CIGCFile::GetLongDatePrefix(int iYear, int iMonth, int iDay)
	{
	CString str;
	str.Format(_T("%.4d-%.2d-%.2d-"), iYear, iMonth, iDay );
	return str;
	}


CIGCFile::CIGCFile(CIGCFile *pcIGCFile)
	{
	m_strFilePath		=pcIGCFile->m_strFilePath;
	m_bFileMissing		=pcIGCFile->m_bFileMissing;
	m_bValid			=pcIGCFile->m_bValid;
	m_mtime				=pcIGCFile->m_mtime;
	m_strErrorText		=pcIGCFile->m_strErrorText;
	m_strFileName		=pcIGCFile->m_strFileName;
	m_strGliderType		=pcIGCFile->m_strGliderType;
	m_strGliderID		=pcIGCFile->m_strGliderID;
	m_strCompetitionID	=pcIGCFile->m_strCompetitionID;
	m_strPilot			=pcIGCFile->m_strPilot;
	m_strSite			=pcIGCFile->m_strSite;
	m_strUnits			=pcIGCFile->m_strUnits;
	m_strManufacturer	=pcIGCFile->m_strManufacturer;
	m_strFDRID			=pcIGCFile->m_strFDRID;
	m_nPositionPoints	=pcIGCFile->m_nPositionPoints;
	m_nLinesRead		=pcIGCFile->m_nLinesRead;
	m_iDay				=pcIGCFile->m_iDay;
	m_iMonth			=pcIGCFile->m_iMonth;
	m_iYear				=pcIGCFile->m_iYear;
	m_iAccuracyStartPos	=pcIGCFile->m_iAccuracyStartPos;
	m_iAccuracyEndPos	=pcIGCFile->m_iAccuracyEndPos;
	m_iENLStartPos		=pcIGCFile->m_iENLStartPos;
	m_iENLEndPos		=pcIGCFile->m_iENLEndPos;
	m_iMOPStartPos		=pcIGCFile->m_iMOPStartPos;
	m_iMOPEndPos		=pcIGCFile->m_iMOPEndPos;
	m_iTimeZone			=pcIGCFile->m_iTimeZone;
	m_iENLMin			=pcIGCFile->m_iENLMin;
	m_iENLMax			=pcIGCFile->m_iENLMax;
	m_iMOPMin			=pcIGCFile->m_iMOPMin;
	m_iMOPMax			=pcIGCFile->m_iMOPMax;
	}


bool CIGCFile::Smash(CString strIGCFileName, CString strOutputFileName, int nSec)
{
	int iTime=0;
	int iPrevTime=0;
	bool bFirstB=false;
	char  cRecord[MAXLINELEN] ;

	ifstream	cIfstream;
	ofstream	cOfstream;

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(strIGCFileName  ); 
	if (cIfstream.is_open()==0)
		{
		return false;
		}

	cOfstream.open(strOutputFileName, ios::out  );
	if (cOfstream.is_open()==0)
		{
		return false;
		}

	while( !cIfstream.eof() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		if( cIfstream.fail() ) break;
		CString strRecord(cRecord);
		strRecord.TrimLeft();
		strRecord.TrimRight();
		if( strRecord.GetLength()<=0 ) continue;

		if(		 strRecord[0]=='H' || strRecord[0]=='A' )
			{
			cOfstream << strRecord << endl;
			}
		else if( strRecord[0]=='B' )
			{
			if( !bFirstB ) 
				{
				cOfstream << _T("LXXX") << endl;
				cOfstream << _T("LXXX  Note: This IGC file has been condensed by Winscore for Internet spectators.")	<< endl;
				cOfstream << _T("LXXX        Contact the contest organizers for the complete version of this file.")	<< endl;
				cOfstream << _T("LXXX        (The complete version was used for contest scoring calculations.)")		<< endl;
				cOfstream << _T("LXXX") << endl;
				bFirstB=true;
				}

			strRecord=strRecord.Left(30);
			int iHours	=atoi( strRecord.Mid(1,2) );
			int iMin	=atoi( strRecord.Mid(3,2) );
			int iSec	=atoi( strRecord.Mid(5,2) );
			iTime=(60*(iHours*60+iMin))+iSec;
			if( iPrevTime>iTime || iTime>(iPrevTime+nSec) )
				{
				cOfstream << strRecord << endl;
				iPrevTime=iTime;
				}
			}

		}// end loop on records

	cIfstream.close();
	cOfstream.close();

	return true;
}


void CIGCFile::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	MSXML2::IXMLDOMNodePtr pChildNode=NULL;
	cMgr.CreateChild( pParentNode, pChildNode, _T("IGCFile"));

	cMgr.CreateElement	( pChildNode, _T("FilePath"), LPCSTR(m_strFilePath) );
	cMgr.CreateElementIntC( pChildNode, _T("FileMissing"), (WORD)m_bFileMissing );
	cMgr.CreateElementIntC( pChildNode, _T("Valid"), (WORD)m_bValid );

	GetXMLTime  ( cMgr, pChildNode, _T("mtime"), m_mtime) ;

	cMgr.CreateElement	( pChildNode, _T("ErrorText"),		LPCSTR(m_strErrorText));
	cMgr.CreateElement	( pChildNode, _T("FileName"),		LPCSTR(m_strFileName));
	cMgr.CreateElement	( pChildNode, _T("GliderType"),	LPCSTR(m_strGliderType));
	cMgr.CreateElement	( pChildNode, _T("GliderID"),		LPCSTR(m_strGliderID));
	cMgr.CreateElement	( pChildNode, _T("CompetitionID"), LPCSTR(m_strCompetitionID));
	cMgr.CreateElement	( pChildNode, _T("Pilot"),			LPCSTR(m_strPilot));
	cMgr.CreateElement	( pChildNode, _T("Site"),			LPCSTR(m_strSite));
	cMgr.CreateElement	( pChildNode, _T("Units"),			LPCSTR(m_strUnits));
	cMgr.CreateElement	( pChildNode, _T("Manufacturer"),	LPCSTR(m_strManufacturer));
	cMgr.CreateElement	( pChildNode, _T("FDRID"),			LPCSTR(m_strFDRID));
	cMgr.CreateElement	( pChildNode, _T("HardwareVersion"),LPCSTR(m_strHardwareVersion));


	cMgr.CreateElementIntC( pChildNode, _T("NumPositionPoints"), m_nPositionPoints);
	cMgr.CreateElementIntC( pChildNode, _T("Day"),				m_iDay);
	cMgr.CreateElementIntC( pChildNode, _T("Month"),			m_iMonth);
	cMgr.CreateElementIntC( pChildNode, _T("Year"),				m_iYear);
	cMgr.CreateElementIntC( pChildNode, _T("TimeZone"),			m_iTimeZone);

	cMgr.CreateElementIntC( pChildNode, _T("MinENL"),			m_iENLMin);
	cMgr.CreateElementIntC( pChildNode, _T("MaxENL"),			m_iENLMax);
	cMgr.CreateElementIntC( pChildNode, _T("MinMOP"),			m_iMOPMin);
	cMgr.CreateElementIntC( pChildNode, _T("MaxMOP"),			m_iMOPMax);
	}

void CIGCFile::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIGCNode)
	{
	MSXML2::IXMLDOMNodePtr pChildNode;
	TESTHR(cMgr.SelectChild(_T("IGCFile"),pIGCNode, pChildNode));
	if( pChildNode==NULL ) return;

	GET_XML_STR	( cMgr, pChildNode, _T("FilePath"), m_strFilePath );
	GET_XML_BOOL( cMgr, pChildNode, _T("FileMissing"), m_bFileMissing, false );
	GET_XML_BOOL( cMgr, pChildNode, _T("Valid"), m_bValid, false );

	ImportXMLTime  ( cMgr, pChildNode, _T("mtime"), m_mtime) ;

	GET_XML_STR	( cMgr, pChildNode, _T("ErrorText"),		m_strErrorText);
	GET_XML_STR	( cMgr, pChildNode, _T("FileName"),		(m_strFileName));
	GET_XML_STR	( cMgr, pChildNode, _T("GliderType"),	(m_strGliderType));
	GET_XML_STR	( cMgr, pChildNode, _T("GliderID"),		(m_strGliderID));
	GET_XML_STR	( cMgr, pChildNode, _T("CompetitionID"), (m_strCompetitionID));
	GET_XML_STR	( cMgr, pChildNode, _T("Pilot"),			(m_strPilot));
	GET_XML_STR	( cMgr, pChildNode, _T("Site"),			(m_strSite));
	GET_XML_STR	( cMgr, pChildNode, _T("Units"),			(m_strUnits));
	GET_XML_STR	( cMgr, pChildNode, _T("Manufacturer"),	(m_strManufacturer));
	GET_XML_STR	( cMgr, pChildNode, _T("FDRID"),			(m_strFDRID));
	GET_XML_STR	( cMgr, pChildNode, _T("HardwareVersion"),	(m_strHardwareVersion));


	GET_XML_INT( cMgr, pChildNode, _T("NumPositionPoints"),	int, m_nPositionPoints, 0);
	GET_XML_INT( cMgr, pChildNode, _T("Day"),				int, m_iDay, 0);
	GET_XML_INT( cMgr, pChildNode, _T("Month"),				int, m_iMonth, 0);
	GET_XML_INT( cMgr, pChildNode, _T("Year"),				int, m_iYear, 0);
	GET_XML_INT( cMgr, pChildNode, _T("TimeZone"),			int, m_iTimeZone, 0);

	GET_XML_INT( cMgr, pChildNode, _T("MinENL"),			int, m_iENLMin,0);
	GET_XML_INT( cMgr, pChildNode, _T("MaxENL"),			int, m_iENLMax,0);
	GET_XML_INT( cMgr, pChildNode, _T("MinMOP"),			int, m_iMOPMin,0);
	GET_XML_INT( cMgr,pChildNode,  _T("MaxMOP"),			int, m_iMOPMax,0);

	}

bool CIGCFile::IsVolkslogger()
{
	return ( m_strManufacturer.Find(_T("GCS"))>=0 );
}

bool CIGCFile::IsLXNav()
{
	return ( m_strManufacturer.Find(_T("LXN"))>=0 );
}
