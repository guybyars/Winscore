//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#include "stdafx.h"
#include "winscore.h"
#include "wsversion.h"
#include <process.h>
#include "task.h"
#include "igcfile.h"
#include <atlbase.h>
#include "Global_Prototypes.h"
#include "WinscoreFonts.h"
#include "LocalMachineRegistry.h"

#include <math.h>
#include <map>
#include <string>

static CTime cDefaultDay;
static BOOL fRemind=FALSE;
static EClass cDefaultClass=eStandard;
static CString strWSCPath;

static CSemaphore cSem_uploadfile;

static std::map<string,string> mapWinscoreProfileString;
static std::map<string,int> mapWinscoreProfileInt;

typedef struct
{
	bool  bRet;
	CString strDllName;
	CString strIGCName;
	CString strResult;
} TValDAta;

/*
 string original = "super flying monkeys are aweseome, aren't they?";
 cout << "Original data = " << original;
 string encrypted = "";
 string unencrypt = "";


 for (int temp = 0; temp < original.size(); temp++){
  encrypted += original[temp] ^ (int(key) + temp) % 255;
 }
 cout << "nEncrypted data = " << encrypted;

 for (int temp = 0; temp < original.size(); temp++){
  unencrypted += encrypted[temp] ^ (int(key) + temp) % 255;
 }
 */

CString GetWSCPath()
	{
	return strWSCPath;
	}

void SetWSCPath(CString &strPath)
	{
	strWSCPath=strPath;
	}

CString GetNonRelativePath(CString &strFileIn)
	{
	char szDrive[3];
	memset(szDrive,0,3);
	_splitpath_s(strFileIn, szDrive, 3, NULL, 0, NULL, 0, NULL, 0);
	if( szDrive[1]==':' ) 
		return strFileIn; // Already full
	else
		return GetWSCPath()+"\\"+strFileIn;
	}


CString GetRelativePath(CString &strFull,bool bNonRelativeFull)
	{

	// If the relative hasn't been defined, exit at once
    if( GetWSCPath().GetLength()==0 ) return strFull;

	char szDrive[3];
	memset(szDrive,0,3);
	char szFolder[256];
	memset(szFolder,0,256);
	char szFile[256];
	memset(szFile,0,256);
	char szExt[256];
	memset(szExt,0,256);
	_splitpath_s(strFull, szDrive, 3, szFolder, 256, szFile, 256, szExt, 256);
	if( szDrive[1]==':' )
		{
		// A full path was entered, get the relative

		int iPos=strFull.Find(GetWSCPath());
		if( iPos==0 )
			{
			CString cRelative=strFull.Right(strFull.GetLength()-GetWSCPath().GetLength()-1);
			return cRelative;
			}
		else
			if( bNonRelativeFull ) 
				return strFull;
			else
				{
				CString strRet=szFile;
				strRet+=szExt;
				return strRet;
				}
		}
	return strFull;
	}

BOOL	WriteWinscoreStringEncrypt(LPCTSTR lpszEntry,	LPCTSTR lpszValue  )
	{
	// Convert the string to a series of 2 digit ASCII codes in HEX
	CString strOutput;
	CString strTemp;

	for (int temp = 0; temp < (int)strlen(lpszValue); temp++)
		{
		strTemp.Format("%02X",int(lpszValue[temp]));
		strOutput+=strTemp;
		}
	return WriteWinscoreString( lpszEntry,	strOutput  );
	}

 CString GetWinscoreStringDecrypt( LPCTSTR lpKeyName,	LPCTSTR lpDefault)
	{
	CString strOutput;
	char  cASCII[3];
	CString strInput=GetWinscoreString( lpKeyName,	lpDefault);
	if( strInput==lpDefault ) return lpDefault;

	ZeroMemory(cASCII,3);
	for (int temp = 0; temp < strInput.GetLength(); temp+=2)
		{// Convert the 2 digit HEX back to a char.
		cASCII[0]=strInput[temp+0];
		cASCII[1]=strInput[temp+1];
		int iChar=strtol(cASCII, NULL, 16);
		char cChar=(char)iChar;
		strOutput+=cChar;
		}
	return strOutput;
	}



CString GetWinscoreString( LPCTSTR lpKeyName,	LPCTSTR lpDefault)
	{
	std::map<string,string>::iterator it = mapWinscoreProfileString.find(lpKeyName); 
	if (it == mapWinscoreProfileString.end())
			{
			//Not here, return default
			return lpDefault;
			}
	   else
			{//Already here, return it
			return CString(it->second.c_str());
			}
	}


int		GetWinscoreInt( LPCTSTR lpKeyName,		int nDefault )
	{
	std::map<string,int>::iterator it = mapWinscoreProfileInt.find(lpKeyName); 
	if (it == mapWinscoreProfileInt.end())
			{
			//Not here, return default
			return nDefault;
			}
	   else
			{//Already here, return it
			return it->second;
			}
	}

void  WriteWinscoreDouble( LPCTSTR lpszEntry, double dVal )
	{
	CString strDouble;
	strDouble.Format(_T("%20.15f"), dVal);
	WriteWinscoreString( lpszEntry, strDouble);
	}


double  GetWinscoreDouble( LPCTSTR lpszEntry, double dDefault )
	{
	double dVal=0.0;
	CString strDouble=GetWinscoreString( lpszEntry,"" );
	if( strDouble.GetLength()==0 ) return dDefault;
	if( sscanf_s(strDouble, _T("%lf"), &dVal)!=1 ) return dDefault;
	return dVal;
	}


BOOL    RemoveWinscoreEntry(LPCTSTR lpszEntry)
	{

	std::map<string,string>::iterator it = mapWinscoreProfileString.find(lpszEntry); 
	if (it == mapWinscoreProfileString.end())
			{
			//Not here, try an Int
			std::map<string,int>::iterator it2 = mapWinscoreProfileInt.find(lpszEntry); 
			if (it2 != mapWinscoreProfileInt.end())
				{//Found, Remove it
				mapWinscoreProfileInt.erase(it2);
				return true;
				}
			}
	   else
			{//Found, Remove it
			mapWinscoreProfileString.erase(it);
			return true;
			}
	return false;
	}



BOOL	WriteWinscoreInt(LPCTSTR lpszEntry,	int nValue  )
	{

	std::map<string,int>::iterator it = mapWinscoreProfileInt.find(lpszEntry); 
	if (it == mapWinscoreProfileInt.end())
			{
			//Not here, add it
			mapWinscoreProfileInt.insert(std::make_pair(lpszEntry, nValue));
			}
	   else
			{//Already here, replace it
			it->second = nValue;
			}
		return true;
	}

BOOL	WriteWinscoreString(LPCTSTR lpszEntry,	LPCTSTR lpszValue  )
	{

	std::map<string,string>::iterator it = mapWinscoreProfileString.find(lpszEntry); 
	if (it == mapWinscoreProfileString.end())
			{
			//Not here, add it
			mapWinscoreProfileString.insert(std::make_pair(lpszEntry, lpszValue));
			}
	   else
			{//Already here, replace it
			it->second = lpszValue;
			}
		return true;
	}

void GetRegistryXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode )
	{
	MSXML2::IXMLDOMNodePtr pIDOMChildNodeRegistry=NULL;
	cMgr.CreateChild( pIDOMNode, pIDOMChildNodeRegistry, LPCSTR("Registry"));

	MSXML2::IXMLDOMNodePtr pIDOMChildNodeStrings=NULL;
	cMgr.CreateChild( pIDOMChildNodeRegistry, pIDOMChildNodeStrings, LPCSTR("Strings"));

	std::map<string,string>::iterator it;
	for (it = mapWinscoreProfileString.begin(); it != mapWinscoreProfileString.end(); it++)
		{
		MSXML2::IXMLDOMNodePtr pIDOMChildNodeString=NULL;
		cMgr.CreateChild( pIDOMChildNodeStrings, pIDOMChildNodeString, LPCSTR("String"));
		cMgr.CreateElement( pIDOMChildNodeString, _T("Key"),	it->first.c_str() );
		cMgr.CreateElement( pIDOMChildNodeString, _T("Value"),	it->second.c_str() );
		}   

	MSXML2::IXMLDOMNodePtr pIDOMChildNodeInts=NULL;
	cMgr.CreateChild( pIDOMChildNodeRegistry, pIDOMChildNodeInts, LPCSTR("Ints"));

    std::map<string,int>::iterator it2;
    for (it2 = mapWinscoreProfileInt.begin(); it2 != mapWinscoreProfileInt.end(); it2++)
		{
		MSXML2::IXMLDOMNodePtr pIDOMChildNodeInt=NULL;
		cMgr.CreateChild( pIDOMChildNodeInts, pIDOMChildNodeInt, LPCSTR("Int"));
		cMgr.CreateElement(		pIDOMChildNodeInt, _T("Key"),	it2->first.c_str() );
		cMgr.CreateElementInt(	pIDOMChildNodeInt, _T("Value"),	it2->second );
		}
	}

void ImportRegistryXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode )
	{
	MSXML2::IXMLDOMNodePtr pIDOMChildNodeRegistry;
	TESTHR(cMgr.SelectChild(_T("Registry"), pIDOMNode, pIDOMChildNodeRegistry));
	if( pIDOMChildNodeRegistry ) 
			{
			MSXML2::IXMLDOMNodePtr pIDOMChildNodeStrings;
			TESTHR(cMgr.SelectChild(_T("Strings"), pIDOMChildNodeRegistry, pIDOMChildNodeStrings));
			if( pIDOMChildNodeStrings ) 
				{

				MSXML2::IXMLDOMNodeListPtr pStringList=cMgr.GetChildList(pIDOMChildNodeStrings);
				if( pStringList ) 
					{
					long length=0;
					TESTHR(pStringList->get_length(&length));
					for( int i=0; i<length; i++ )
						{
						MSXML2::IXMLDOMNodePtr pString;
						TESTHR(pStringList->get_item(i,&pString));
						CString strKey, strValue;
						GET_XML_STR( cMgr, pString, "Key", strKey);
						GET_XML_STR( cMgr, pString, "Value",strValue);
						if( strKey.GetLength()>0 )
							{
							string strKeys = strKey.GetBuffer();
							string strValues = strValue.GetBuffer();
							std::pair<string, string> lpair = std::make_pair(strKeys, strValues);
							mapWinscoreProfileString.insert(lpair);
							}
						}
					}
				}
			MSXML2::IXMLDOMNodePtr pIDOMChildNodeInts;
			TESTHR(cMgr.SelectChild(_T("Ints"), pIDOMChildNodeRegistry, pIDOMChildNodeInts));
			if( pIDOMChildNodeInts ) 
				{

				MSXML2::IXMLDOMNodeListPtr pIntList=cMgr.GetChildList(pIDOMChildNodeInts);
				if( pIntList ) 
					{
					long length=0;
					TESTHR(pIntList->get_length(&length));
					for( int i=0; i<length; i++ )
						{
						MSXML2::IXMLDOMNodePtr pInt;
						TESTHR(pIntList->get_item(i,&pInt));
						CString strKey;
						int iValue;
						GET_XML_STR( cMgr, pInt, "Key", strKey);
						GET_XML_INT( cMgr, pInt, "Value", int, iValue, 0);
						if( strKey.GetLength()>0 )
							{
							string strKeys = strKey.GetBuffer();
							std::pair<string, int> lpair = std::make_pair(strKeys, iValue);
							mapWinscoreProfileInt.insert(lpair);
							}
						}
					}
				}
			}
	}
void PurgeWinscoreRegistry()
	{
	mapWinscoreProfileInt.clear();
	mapWinscoreProfileString.clear();
	}
/*
<Registry>
	<Strings>
		<String>
		  <Key>strKey</Key><Value>strValue</Value>
		 </String>
	</Strings>
	<Ints>
		<Int>
		<Key>strKey</Key><Value>76</Value>
		</Int>
	</Ints>
</Registry>
*/





void strClean( const char *pInput, CString &strOut)
	{
// Clean the following ascii codes as they are not permitted in XML
//#x0 - #x8 (ASCII 0 - 8)
//#xB - #xC (ASCII 11 - 12)
//#xE - #x1F (ASCII 14 - 31)}
	
	int iLen=strlen(pInput);
	if( iLen==0 ) return;

	char *pNew=new char[iLen+1];

	for( int ichr=0; ichr<iLen; ichr++ )
		{
		if( (pInput[ichr]>= 0 && pInput[ichr]<= 8)	||
			(pInput[ichr]>=11 && pInput[ichr]<=12)  ||
			(pInput[ichr]>=14 && pInput[ichr]<=31)    )
			pNew[ichr]=32;
		else
			pNew[ichr]=pInput[ichr];
		}
	pNew[iLen]=0;
	strOut=pNew;
	strOut.TrimRight();
	strOut.TrimLeft();
	delete pNew;
	}


void ImportXMLTime(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName, CTime &cTime, int iDYear, int iDMonth, int iDDay)
	{
	cTime=0;
	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	TESTHR(cMgr.SelectChild(strChildName,pIDOMNode,pIDOMChildNode) );
	if( pIDOMChildNode==NULL ) return ;

	int iYear=0, iMonth=0, iDay=0, iHour=0, iMin=0, iSec=0;
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("yr"), int, iYear, iDYear);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("mon"),int, iMonth,iDMonth);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("day"),  int, iDay,  iDDay);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("hr"),	int,	iHour,	0);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("min"),	int,	iMin,	0);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("sec"), int,	iSec,	0);

	if( iYear==0 || iMonth==0 || iDay==0 ) return;

	cTime=CTime( iYear, iMonth, iDay, iHour, iMin, iSec);

	}

void ImportXMLDate(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName, CTime &cDate)
	{
	cDate=0;
	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	TESTHR(cMgr.SelectChild(strChildName,pIDOMNode,pIDOMChildNode));
	if( pIDOMChildNode==NULL ) return ;

	int iYear=0, iMonth=0, iDay=0;
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("yr"), int, iYear, 0);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("mon"),int, iMonth,0);
	GET_XML_INT	( cMgr, pIDOMChildNode, _T("day"),  int, iDay,  0);
	cDate=CTime( iYear, iMonth, iDay, 0, 0, 0 );

	}

void GetXMLDate(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode, CString strChildName, CTime cDate)
	{
	if( cDate.GetTime()<=0 || cDate.GetYear()<1980  ) return;

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pIDOMNode, pIDOMChildNode, LPCSTR(strChildName));

	cMgr.CreateElementInt	( pIDOMChildNode, _T("yr"), cDate.GetYear());
	cMgr.CreateElementInt	( pIDOMChildNode, _T("mon"), cDate.GetMonth());
	cMgr.CreateElementInt	( pIDOMChildNode, _T("day"), cDate.GetDay());
	}

void GetXMLDateFull(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode, CString strChildName, CTime cDate)
	{
	if( cDate.GetTime()<=0 || cDate.GetYear()<1980  ) return;

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pIDOMNode, pIDOMChildNode, LPCSTR(strChildName));

	cMgr.CreateElementInt	( pIDOMChildNode, _T("yr"),   cDate.GetYear());
	cMgr.CreateElementInt	( pIDOMChildNode, _T("mon"),  cDate.GetMonth());
	cMgr.CreateElementInt	( pIDOMChildNode, _T("day"),  cDate.GetDay());
	cMgr.CreateElement ( pIDOMChildNode, _T("full"), LPCSTR(cDate.Format( _T("%A, %B %d, %Y") )) );
	cMgr.CreateElement ( pIDOMChildNode, _T("Mon"), LPCSTR(cDate.Format( _T("%B") )) );	
	}


void GetXMLTime(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMNode, CString strChildName, CTime cTime, bool bDate)
	{
	if( cTime.GetTime()==0 || cTime.GetYear()<1980) return;

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pIDOMNode, pIDOMChildNode, LPCSTR(strChildName));
	if( bDate )
		{
		cMgr.CreateElementInt	( pIDOMChildNode, _T("yr"),	cTime.GetYear());
		cMgr.CreateElementInt	( pIDOMChildNode, _T("mon"),	cTime.GetMonth());
		cMgr.CreateElementInt	( pIDOMChildNode, _T("day"),	cTime.GetDay());
		}
	cMgr.CreateElementInt	( pIDOMChildNode, _T("hr"),	cTime.GetHour());
	cMgr.CreateElementIntC( pIDOMChildNode, _T("min"), cTime.GetMinute());
	cMgr.CreateElementIntC( pIDOMChildNode, _T("sec"), cTime.GetSecond());
	}

CTime	LocalToGMT(CTime cLocalTime )
	{
	long lTime;
    _get_timezone(&lTime);
	return cLocalTime+CTimeSpan(lTime);
	}
CTime	GMTToLocal(CTime cGMTTime )
	{
	long lTime;
    _get_timezone(&lTime);
	return cGMTTime-CTimeSpan(lTime);
	}

bool DatesEqual( CTime &time1, CTime &time2 )
	{
	return (	time1.GetDay()==time2.GetDay() &&
				time1.GetMonth()==time2.GetMonth() &&
				time1.GetYear()==time2.GetYear()		);

	}

//BOOL ValidateUser(CString &strName, int iRegNo, int iYear )
//	{
//	if( iRegNo==0 || strName.GetLength()==0 ) return false;
//
//	return iRegNo==GetUserRegNo(strName, iYear);
//	}


CString UnitsText(EUnits eUnits)
{
	if( eUnits==eNautical )
		return _T("Nm");
	else if( eUnits==eStatute )
		return _T("Miles");
	else if( eUnits==eKilometers )
		return _T("Km");

	return _T(" ");
}

CString UnitsTextShort(EUnits eUnits)
{
	if( eUnits==eNautical )
		return _T("Nm");
	else if( eUnits==eStatute )
		return _T("Mi");
	else if( eUnits==eKilometers )
		return _T("Km");

	return _T(" ");
}
CString UnitsSpeedText(EUnits eUnits)
{
	if( eUnits==eNautical )
		return _T("(Knots)");
	else if( eUnits==eStatute )
		return _T("(Mph)");
	else if( eUnits==eKilometers )
		return _T("(Kmph)");

	return _T(" ");
}
CString UnitsSpeedTextShort(EUnits eUnits)
{
	if( eUnits==eNautical )
		return _T("(kt)");
	else if( eUnits==eStatute )
		return _T("(mph)");
	else if( eUnits==eKilometers )
		return _T("(kph)");

	return _T(" ");
}


CString AltitudeUnitsText(EAltUnits eUnits )
	{
	if( eUnits==eMeters ) 
		return _T("m");
	else
		return _T("ft");
	}

CTime   GetDefaultDay()
{
return cDefaultDay;
}


void    SetDefaultDay(CTime cDate)
{
	// Save the default date to the registry
	cDefaultDay=cDate;
	int iDay=cDate.GetDay();
	int iMonth=cDate.GetMonth();
	AfxGetApp()->WriteProfileInt(REGSECTION, _T("DefaultDay"), iDay );
	AfxGetApp()->WriteProfileInt(REGSECTION, _T("DefaultMonth"), iMonth );

};


void    SetDefaultDayToday()
	{

	CTime cToday=CTime::GetCurrentTime();
	int iMonth=cToday.GetMonth();
	int iDay=cToday.GetDay();

	int iRegDay		=AfxGetApp()->GetProfileInt(REGSECTION, _T("DefaultDay"), iDay);
	int iRegMonth	=AfxGetApp()->GetProfileInt(REGSECTION, _T("DefaultMonth"), iMonth);

	cDefaultDay=CTime(	CURRENTYEAR,
						iRegMonth,
						iRegDay,	
						0,0,0,0);
	};


double  Roundoff( double dPoints)
	{
	double dLeft;

	double  dRight=(modf( dPoints, &dLeft) );

	if( dPoints >=0.0 ) 
		{
		if( dRight>=.5 ) dLeft+=1.0;
		}
	else
		{
		if( abs(dRight)>=.5 ) dLeft-=1.0;
		}
	return dLeft;
	}


void	Shuffle20Percent(CStringArray& cInputArray)
	{
	CStringArray cTempArray;
	int i;
	int nSize=cInputArray.GetSize();

	int i20pct=(int)(((double)nSize)*.2);
	if( i20pct<=0 ) i20pct=1;

	int j=0;
	for( i=i20pct; i<nSize; i++ )
		{
		cTempArray.SetAtGrow( j++, cInputArray.GetAt(i) );
		}

	for( i=0; i<i20pct; i++ )
		{
		cTempArray.SetAtGrow( j++, cInputArray.GetAt(i) );
		}
	cInputArray.RemoveAll();

	cInputArray.Copy(cTempArray);
	}


void GetSpeed(double dDist, CTimeSpan cTOC, double* pdSpeed)
{
	int iSeconds=(int)cTOC.GetTotalSeconds();
	if( iSeconds==0 ) 
		{
		*pdSpeed=0.0;
		return;
		}
	double dHours=((double)iSeconds)/(60.*60.);
	*pdSpeed=dDist/dHours;
}

#define PAGE_WIDTH 800

void DrawStandardTrailer(CDC *pDC)
	{
	CString cText;
	CTime cToday = CTime::GetCurrentTime();
	cText=cToday.Format(_T("Printed on %A %B %d, %Y - %H:%M by Winscore") );
	pDC->SetTextAlign( TA_TOP| TA_CENTER );
	SelectFont( pDC, 13, FW_NORMAL, FALSE); 

	DRAW_HORIZONTAL( -1030 );
	pDC->TextOut( PAGE_WIDTH/2, -1035 , cText, cText.GetLength() );
	}

int GetPageWidth(CDC *pDC)
	{
//	CSize cWin=pDC->GetWindowExt( );
//	return cWin.cx-1;
	return 790;
	}

void RemindAboutScores()
	{
	if( !fRemind ) return;
	CString cError;
	cError.Format(
		_T("REMINDER: Have you emailed your scores to \"scores@ssa.org\" today?"));
	AfxMessageBox( cError, MB_OK | MB_ICONQUESTION, 0 );
	}


void ArmReminder()
	{
	fRemind=TRUE;
	}

int strctok( char *pOutput,  size_t nCharOut, char **ppIn, char cToken )
	{

	char *pChar=strchr( *ppIn,  cToken );
	if(!pChar ) return 0;

	int nChar=pChar-*ppIn;
	strncpy_s(pOutput, nCharOut, *ppIn, nChar);
	pOutput[nChar]='\0';

	*ppIn+=nChar+1; //Advance the input pointer past the token

	return nChar;
	}

void SetDefaultClass(EClass eClass)
{
cDefaultClass=eClass;
}

EClass GetDefaultClass()
{
return cDefaultClass;
}

void  WriteProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dVal )
	{
	CString strDouble;
	strDouble.Format(_T("%20.15f"), dVal);
	AfxGetApp()->WriteProfileString( lpszSection, lpszEntry, strDouble);
	}


double  GetProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault )
	{
	double dVal=0.0;
	CString strDouble=AfxGetApp()->GetProfileString( lpszSection, lpszEntry );
	if( strDouble.GetLength()==0 ) return dDefault;
	if( sscanf_s(strDouble, _T("%lf"), &dVal)!=1 ) return dDefault;
	return dVal;
	}


double GetDistance	(	ETaskType	    eTaskType,
	                    CGate			&cStartGate,
					    CLocation       *pcStartFix,
						CTurnpointArray &cTurnpointArray,
						CGate			&cFinishGate,
						int				nTurnpoints,
						CLocation		acTurnpointLocations[],
						double 			adTurnpointRadii[],
						int				fCompleted,
						CLocation		&cTurnpointSought,
						CLocation		&cLandoutLocation)
{
	double dDist=0.0;
	bool   bSubtractStart=false;

	if( cStartGate.GetPointID()==0 ) return 0.;
	if( cFinishGate.GetPointID()==0 ) return 0.;
	CLocation* pcStartPoint=NULL;
	
	if( eTaskType==eFAIAssignedArea || eTaskType==eFAIRacing )
		{
		pcStartPoint=cTurnpointArray.GetAt(cStartGate.GetPointID()-1);
		if(  eTaskType==eFAIRacing ) bSubtractStart=true;
		}
	else
		{
		if( cStartGate.IsFirstLegAtExit() && pcStartFix )
			pcStartPoint=pcStartFix;
		else
			{
			pcStartPoint=cTurnpointArray.GetAt(cStartGate.GetPointID()-1);
			bSubtractStart=true;
			}
		}

	if( !pcStartPoint ) return 0.0;
	CTurnpoint* pcFinishPoint=cTurnpointArray.GetAt(cFinishGate.GetPointID()-1);
	if( !pcFinishPoint) return 0.0;

	if( fCompleted )
		{ // Easy, no landout, just sum the legs.

		if( nTurnpoints==0 ) return 0.0;

		// Calculate from start to first turn
		dDist+=pcStartPoint->DistanceTo(acTurnpointLocations[0], SYSTEMUNITS );
		if( cStartGate.IsGPSCylinder() && (!cStartGate.IsFirstLegAtExit() || bSubtractStart) ) 
				dDist-=cStartGate.GetRadius();

//---------------
// Check removed for 2018 season, restored in 2020
		
		if( cStartGate.IsGPSCylinder() && 
			cStartGate.IsFirstLegAtExit() &&
			cTurnpointArray.GetAt(cStartGate.GetPointID()-1) ) 
			{
//			Check to make sure the 1st leg is not greater than the distance from the center of the
//			start cylinder to the 1st turnpoint
			double dCheckDistance=cTurnpointArray.GetAt(cStartGate.GetPointID()-1)->DistanceTo(acTurnpointLocations[0], SYSTEMUNITS);
			dDist=min(dCheckDistance,dDist);
			}
//---------------

		// Sum the intermediate legs
		for( int i=1; i<nTurnpoints; i++ )
			dDist+=acTurnpointLocations[i].DistanceTo(acTurnpointLocations[i-1], SYSTEMUNITS );

		// Calculate from last turn to finish
		dDist+=acTurnpointLocations[nTurnpoints-1].DistanceTo(pcFinishPoint, SYSTEMUNITS );
		if( cFinishGate.IsGPSCylinder() ) 
				dDist-=cFinishGate.GetRadius();
	
		}
	else
		{ // Landed out somewheres along the way

		// Process the first leg
		if( nTurnpoints==0 )
			{ // Landed out before the first turn
			if( eTaskType==eFAIAssignedArea )
				{
				//Find the location of the closest point to the furthese progress to be the sought point.
				double dCourseTo=cTurnpointSought.CourseTo(cLandoutLocation);
				double dCorr=GetLatCorrection(cLandoutLocation.GetLat());
				CLocation cFAISought=cTurnpointSought;
				double dMoveDir = ((double)(dCourseTo))*TORAD;
				cFAISought.Move(  adTurnpointRadii[0], dMoveDir, dCorr);
				dDist=pcStartPoint->DistanceTo(cFAISought, SYSTEMUNITS );

				double dRemaining = cLandoutLocation.DistanceTo(cFAISought, SYSTEMUNITS );

				if( dRemaining>0 )
					dDist-=dRemaining;

				}
			else
				{
				double dLegTotal=pcStartPoint->DistanceTo(cTurnpointSought, SYSTEMUNITS );
				double dFromLandoutToSought=(cLandoutLocation.DistanceTo(cTurnpointSought, SYSTEMUNITS ));
				dDist+=dLegTotal-dFromLandoutToSought;
				}
			}
		else
			{
			dDist+=pcStartPoint->DistanceTo(acTurnpointLocations[0], SYSTEMUNITS );
			}

		// Subtract the start radius if appropriate.
		if( eTaskType==eFAIRacing )
				if( cStartGate.IsGPSCylinder()) 
						dDist-=cStartGate.GetRadius();
		else
			{
				if( cStartGate.IsGPSCylinder() && !cStartGate.IsFirstLegAtExit() ) 
						dDist-=cStartGate.GetRadius();
			}

		if( nTurnpoints==0 ) 
			{
			return max(dDist,0.0);
			}

//  Check removed for 2018 season restored in 2020
		
		if( cStartGate.IsGPSCylinder() && 
			cStartGate.IsFirstLegAtExit() &&
			cTurnpointArray.GetAt(cStartGate.GetPointID()-1) ) 
			{
//			Check to make sure the 1st leg is not greater than the distance from the center of the
//			start cylinder to the 1st turnpoint
			double dCheckDistance=cTurnpointArray.GetAt(cStartGate.GetPointID()-1)->DistanceTo(acTurnpointLocations[0], SYSTEMUNITS);
			dDist=min(dCheckDistance,dDist);
			}


		// Sum the full legs
		for( int i=1; i<nTurnpoints; i++ )
			{
			double dLeg=acTurnpointLocations[i-1].DistanceTo(acTurnpointLocations[i], SYSTEMUNITS );
			dDist+=dLeg;
			}

		// Now process from the last turnpoint to the landout location
		// Note, if he lands out within the finish cylinder, or non-finish at the home airport,
		// Make sure he is scored for the full distance, but no more.

		//Subtract the cylinder distance (if appliciable), 
		double dGateDistance=0.0;
		if( cFinishGate.IsGPSCylinder() && cTurnpointSought==*pcFinishPoint )
				dGateDistance=cFinishGate.GetRadius();

		double dLegTotal=0.0;
		if( eTaskType==eFAIAssignedArea )
			{
			//Find the location of the closest point to the furthese progress to be the sought point.
			double dCourseTo=cTurnpointSought.CourseTo(cLandoutLocation);
			double dCorr=GetLatCorrection(cLandoutLocation.GetLat());
			CLocation cFAISought=cTurnpointSought;
			double dMoveDir = ((double)(dCourseTo))*TORAD;
			cFAISought.Move(  adTurnpointRadii[nTurnpoints], dMoveDir, dCorr);
			dLegTotal=acTurnpointLocations[nTurnpoints-1].DistanceTo(cFAISought, SYSTEMUNITS );

			dLegTotal-=dGateDistance;

			dDist+=dLegTotal;

			double dRemaining = cLandoutLocation.DistanceTo(cFAISought, SYSTEMUNITS );

		//	if( eTaskType!=eFAIRacing && eTaskType!=eFAIAssignedArea )   SeeYou did this by mistake.
				{
				// Subtract the gate distance from the remaining, but not for FAI
				dRemaining-=dGateDistance;
				}

			if( dRemaining>0 )
				dDist-=dRemaining;

			}
		else
			{
			dLegTotal=acTurnpointLocations[nTurnpoints-1].DistanceTo(cTurnpointSought, SYSTEMUNITS );

			dLegTotal-=dGateDistance;

			dDist+=dLegTotal;

			double dRemaining = cLandoutLocation.DistanceTo(cTurnpointSought, SYSTEMUNITS );
		//	if( eTaskType!=eFAIRacing && eTaskType!=eFAIAssignedArea )   SeeYou did this by mistake.
				{
				// Subtract the gate distance from the remaining, but not for FAI
				dRemaining-=dGateDistance;
				}

			if( dRemaining>0 )
				dDist-=dRemaining;
			}
		}
	return max(dDist,0.0);
}

double GetHandicappedDistance(	ETaskType	    eTaskType,
	                            CGate			&cStartGate,
							    CLocation       *pcStartFix,
								CTurnpointArray& cTurnpointArray,
								CGate			&cFinishGate,
								double			HCP,
								int				nTurnpoints,
								int				aiTurnpoints[],
						        double			adTurnpointRadii[],
								int				fCompleted,
								int				iTurnpointSought,
								CLocation		&cLandoutLocation)
	{
	CLocation cTurnpointSought;
	CLocation cLocations[MAXTASKTPS];
	for( int i=0; i<nTurnpoints; i++)
		{
		CTurnpoint *pcTurnpoint=cTurnpointArray.GetAt(aiTurnpoints[i]-1);
		if( !pcTurnpoint ) return 0.0;
		cLocations[i]= *pcTurnpoint;
		}
	CTurnpoint *pcTurnpoint=cTurnpointArray.GetAt(iTurnpointSought-1);
	if( !pcTurnpoint ) 
		pcTurnpoint = cTurnpointArray.GetAt(cFinishGate.GetPointID()-1);

	if( pcTurnpoint ) 
		cTurnpointSought= *pcTurnpoint;

	return  HCP*GetDistance	(	eTaskType,
								cStartGate,
								pcStartFix,
								cTurnpointArray,
								cFinishGate,
								nTurnpoints,
								cLocations,
								adTurnpointRadii,
								fCompleted,
								cTurnpointSought,
								cLandoutLocation);
	}


/*



double GetHandicappedDistance(	CGate			&cStartGate,
								CTurnpointArray& cTurnpointArray,
								CGate			&cFinishGate,
								double			HCP,
								int				nTurnpoints,
								int				aiTurnpoints[],
								int				fCompleted,
								int				iTurnpointSought,
								CLocation		&cLandoutLocation)
{

	CTurnpoint* pcTurnpoint=NULL;
	CTurnpoint* pcNextTurnpoint=NULL;


//(3) Index speeds
//If at least one finisher exists: IS = largest value of (HCP x DIST / TOC) for 
//any finisher


	double TLSDIST=0.;
	double dDist=0.0;

	if( cStartGate.GetPointID()==0 ) return 0;

	if( fCompleted )
		{ // Easy, no landout, just sum the legs.

		// Calculate from start to first turn
		pcTurnpoint= cTurnpointArray.GetAt(cStartGate.GetPointID()-1);
		pcNextTurnpoint=cTurnpointArray.GetAt(aiTurnpoints[0]-1);
		if( pcTurnpoint && pcNextTurnpoint )
			{
			TLSDIST=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
			if( cStartGate.IsGPSCylinder() ) 
					TLSDIST-=cStartGate.GetRadius();
			}
		else
			{
			TLSDIST=0.0;
			}

		dDist+=TLSDIST*HCP;

		// Sum the intermediate legs
		for( int i=1; i<nTurnpoints; i++ )
			{
			pcTurnpoint= cTurnpointArray.GetAt(aiTurnpoints[i-1]-1);
			pcNextTurnpoint=cTurnpointArray.GetAt(aiTurnpoints[i]-1);
			if( pcTurnpoint && pcNextTurnpoint )
				{
				TLSDIST=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
				dDist+=TLSDIST*HCP;
				}
			else
				{
				TLSDIST=0.0;
				}
			}

		// Calculate from last turn to finish
		pcTurnpoint= cTurnpointArray.GetAt(aiTurnpoints[nTurnpoints-1]-1);
		pcNextTurnpoint=cTurnpointArray.GetAt(cFinishGate.GetPointID()-1);
		if( pcTurnpoint && pcNextTurnpoint )
			{
			TLSDIST=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
			if( cFinishGate.IsGPSCylinder() ) 
					TLSDIST-=cFinishGate.GetRadius();
			}
		else
			{
			TLSDIST=0.0;
			}
		dDist+=TLSDIST*HCP;
		}
	else
		{ // Landed out somewhere along the way


		// Process the first leg
		pcTurnpoint= cTurnpointArray.GetAt(cStartGate.GetPointID()-1);
		if( nTurnpoints==0 )
			{ // Landed out before the first turn
			pcNextTurnpoint=cTurnpointArray.GetAt(iTurnpointSought-1);
			if( pcTurnpoint && pcNextTurnpoint )
				{
				double dLegTotal=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
				double dFromLandoutToSought=(cLandoutLocation.DistanceTo(pcNextTurnpoint, SYSTEMUNITS ));
				TLSDIST=dLegTotal-dFromLandoutToSought;
				TLSDIST=max(0.0,TLSDIST);
				}
			else
				{
				TLSDIST=0.0;
				}
			}
		else
			{
			pcNextTurnpoint=cTurnpointArray.GetAt(aiTurnpoints[0]-1);
			if( pcTurnpoint && pcNextTurnpoint )
				{
				TLSDIST=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
				}
			else
				{
				TLSDIST=0.0;
				}			
			}

		if( cStartGate.IsGPSCylinder() ) 
				TLSDIST-=cStartGate.GetRadius();

		dDist+=TLSDIST*HCP;
		if( nTurnpoints==0 ) return max(dDist,0.0);


		// Sum the full legs

		for( int i=1; i<nTurnpoints; i++ )
				{
				pcTurnpoint= cTurnpointArray.GetAt(aiTurnpoints[i-1]-1);
				pcNextTurnpoint=cTurnpointArray.GetAt(aiTurnpoints[i]-1);
				if( pcTurnpoint && pcNextTurnpoint )
					{
					TLSDIST=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS );
					}
				else
					{
					TLSDIST=0.0;
					}	

				dDist+=TLSDIST*HCP;
				}


	// Now process from the last turnpoint to the landout location

		double dGateDistance=0.0;
		if( cFinishGate.IsGPSCylinder() && iTurnpointSought==cFinishGate.GetPointID() ) 
				dGateDistance=cFinishGate.GetRadius();
		pcTurnpoint= cTurnpointArray.GetAt(aiTurnpoints[nTurnpoints-1]-1);
		pcNextTurnpoint=cTurnpointArray.GetAt(iTurnpointSought-1);
		if( pcTurnpoint && pcNextTurnpoint )
				{
				double dLegTotal=pcTurnpoint->DistanceTo(pcNextTurnpoint, SYSTEMUNITS )-dGateDistance;
				TLSDIST=dLegTotal-(cLandoutLocation.DistanceTo(pcNextTurnpoint, SYSTEMUNITS )-dGateDistance);
				TLSDIST=max(0.0,TLSDIST);
				}
			else
				{
				TLSDIST=0.0;
				}	
		dDist+=TLSDIST*HCP;
		}

	return max(dDist,0.0);
	}

*/
void SerializeTime(CArchive & ar, CTime &cTime)
	{
	if (ar.IsStoring())
		{
		if( cTime.GetTime()>=0 )
			{
			ar << (short)cTime.GetYear();
			ar << (unsigned char )cTime.GetMonth();
			ar << (unsigned char )cTime.GetDay();
			ar << (unsigned char )cTime.GetHour();
			ar << (unsigned char )cTime.GetMinute();
			ar << (unsigned char )cTime.GetSecond();
			}
		else
			{
			// Handle an illegal time gracefully
			ar << (short)1970;
			ar << (unsigned char )12;
			ar << (unsigned char )31;
			ar << (unsigned char )19;
			ar << (unsigned char )0;
			ar << (unsigned char )0;
			}
		}
	else
		{
		short sYear;
		unsigned char sMonth, sDay, sHour, sMinute, sSecond;
		ar >> sYear;
		ar >> sMonth;
		ar >> sDay;
		ar >> sHour;
		ar >> sMinute;
		ar >> sSecond;
		cTime= CTime( max(sYear,1970), sMonth, sDay, sHour, sMinute, sSecond );
		// Check for invalid date.
		if( cTime.GetTime()<0 ) cTime=CTime(0);
		}
	}

double GetLatCorrection(double dLatitude)
	{
	double dAng=(dLatitude/180)*PI;
	dAng=min(dAng, 80);
	return 1./cos(dAng);
	}


bool GetIGCPathName( CStringArray &strArray, CString &strIGCFilename, CString &strIGCPathName )
	{
	// Given an IGC file name, search the array of valid IGC file names and return the 
	// full path.
	for( int iFile=0; iFile<strArray.GetSize(); iFile++ )
		{
		CString str1=strIGCFilename;
		str1.MakeLower();
		CString str2=strArray[iFile];
		str2.MakeLower();
		if( str2.Find(str1)>=0 )
			{
			strIGCPathName=strArray[iFile];
			return true;
			}
		}
	return false;
	} 

#define DIRECTORY	0x10
#define HIDDEN		0x02
#define SYSTEM		0x04

bool FindIGCFilesInDirectory2( CString strPath, CString strType, CTime cDate,  CStringArray &strArrayPath, CStringArray &strArray, int nLevels )
	{
	CFileFind cFindFile;
	CString strSearchName;
	bool bFound=false;

	if( cDate==0 )
		strSearchName.Format(_T("*.%s"),  strType  );  //Find all .igc files
	else
		strSearchName.Format(_T("%s*.%s"), CIGCFile::GetDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() ), strType  );

	CString strTemp=strPath+_T("//");
	strTemp+=strSearchName;
	BOOL fMore=true;

	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();
			CString strName=cFindFile.GetFilePath();
			strName.MakeLower();
			strArrayPath.Add(strName);
			strName=cFindFile.GetFileName();
			strName.MakeLower();
			strArray.Add(strName);
			bFound=true;
			}
		}

	if( nLevels<=0 ) return bFound;

	strTemp=strPath+_T("//*");
	fMore=true;
	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();
			CString strFileName=cFindFile.GetFilePath();

			if( cFindFile.IsDots()	) continue;

			if(cFindFile.IsDirectory() )
				{
				if( FindIGCFilesInDirectory2( strFileName, strType, cDate, strArrayPath, strArray, nLevels-1 ) ) bFound=true;
				}
			}
		}

	return bFound;
	}

void FindIGCFilesInDirectory( CTime cDate,  CStringArray &strArray )
	{
	CFileStatus rStatus;
	CFileFind cFindFile;

	CString strSearchName;
	CString strSearchLongName;

	CStringArray strFolders;

	strSearchName.Format	(_T("%s*.igc"), CIGCFile::GetDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() )  );
	strSearchLongName.Format(_T("%s*.igc"), CIGCFile::GetLongDatePrefix( cDate.GetYear(), cDate.GetMonth(), cDate.GetDay() )  );

	GetFlightSearchFolders( strFolders );

	for( int iPath=0; iPath<strFolders.GetSize(); iPath++)
		{

		CString strTemp=strFolders[iPath]+_T("//");
		strTemp+=strSearchName;
		BOOL fMore=true;

		if( cFindFile.FindFile(strTemp) )
			{
			while( 1 )
				{
				if( !fMore ) break;
				fMore=cFindFile.FindNextFile();
				CString strFileName=cFindFile.GetFilePath();
				CFile::GetStatus(strFileName, rStatus);

				strArray.Add(strFileName);
				}
			}

//    Now search for files with the new long format

		strTemp=strFolders[iPath]+_T("//");
		strTemp+=strSearchLongName;
		fMore=true;

		if( cFindFile.FindFile(strTemp) )
			{
			while( 1 )
				{
				if( !fMore ) break;
				fMore=cFindFile.FindNextFile();
				CString strFileName=cFindFile.GetFilePath();
				CFile::GetStatus(strFileName, rStatus);

				strArray.Add(strFileName);
				}
			}

		}

	//Now remove duplicates
	for( int i=0; i<strArray.GetSize(); i++ )
		{
		CString str1=strArray[i];
		str1.MakeLower();
		int iFind=str1.ReverseFind('\\');
		if( iFind>0 ) str1=str1.Right(str1.GetLength()-iFind-1);
		for( int j=i+1; j<strArray.GetSize(); j++ )
			{
			CString str2=strArray[j];
			str2.MakeLower();
			int iFind=str2.ReverseFind('\\');
			if( iFind>0 ) str2=str2.Right(str2.GetLength()-iFind-1);
			if( str1==str2 ) 
				strArray.RemoveAt(j);
			}
		}

	return;
	}

bool CheckIGCSecurity( CString strIGCFile,  CString &strStatus )
	{
	bool bSecure=false;

	CIGCFile cIGCFile(strIGCFile);

	if( cIGCFile.IsVolkslogger() )
		{
		CString strReason;
		if( ValidateIGCThread( _T("igc-gcs.dll"), GetNonRelativePath(strIGCFile), strReason ) )
			bSecure=true;
		else
			{
			strStatus+=_T("SECURITY WARNING - ");
			strStatus+=_T(" security failure for:  ");
			strStatus+=strIGCFile;
			strStatus+=_T("\r\n");
			strStatus+=strReason;
			strStatus+=_T("\r\n");
			}
		}
	else if( cIGCFile.IsLXNav() )
		{
		CString strReason;
		if( ValidateIGCThread( _T("igc-lxn.dll"), GetNonRelativePath(strIGCFile), strReason ) )
			bSecure=true;
		else
			{
			strStatus+=_T("WARNING - ");
			strStatus+=_T(" security failure for:  ");
			strStatus+=strIGCFile;
			strStatus+=_T("\r\n");
			strStatus+=strReason;
			strStatus+=_T("\r\n");
			}
		}
	else
		{
		// Try to find generic security dll based on the manufacturer's ID.
		CString strDLL;
	    HMODULE mod = NULL;

		//First try the later version of the file, the 2 version.  If there, use it.
		strDLL.Format(_T("igc-%s2.dll"),cIGCFile.m_strManufacturer);
	    mod = LoadLibrary(strDLL);
		if( mod == NULL )
			{
			strDLL.Format(_T("igc-%s.dll"),cIGCFile.m_strManufacturer);
			mod = LoadLibrary(strDLL);
			}
		if( mod!= NULL )
			{
			FreeLibrary(mod);
			CString strReason;
			if( ValidateIGCThread( strDLL, GetNonRelativePath(strIGCFile), strReason ) )
				bSecure=true;
			else
				{
				strStatus+=_T("WARNING - ");
				strStatus+=_T(" security failure for:  ");
				strStatus+=strIGCFile;
				strStatus+=_T("\r\n");
				strStatus+=strReason;
				strStatus+=_T("\r\n");
				}
			}
		else
			{
			strStatus+=_T("WARNING - ");
			strStatus+=_T(" security failure for:  ");
			strStatus+=strIGCFile;
			strStatus+=_T("\r\n");
			strStatus+=_T("Could not find the manufacturer's security dll: .");
			strStatus+=strDLL;
			strStatus+=_T("\r\n");
			}
		}

	return bSecure;

	}


bool ImportIGC( CString strInputFilePath, CString strInputFileName, CString strLogPath, CString &strOutputFileName, CString &strStatus )
	{

	CString strLongName;
	if( CIGCFile::GetARecord(strInputFilePath, strLongName) )
		{
		strOutputFileName=strLogPath;
		strOutputFileName+=_T("\\");
		strOutputFileName+=strLongName;
		}
	else
		{
		strStatus+=strInputFileName+_T(" does not contain a valid date record.\r\n");
		return false;
		}

	CFileStatus rStatus;
	if( CFile::GetStatus(strOutputFileName, rStatus) )
		{
		strStatus+=strInputFilePath+_T(" has already been imported\r\n");
		return false;
		}

	if( CopyFile(strInputFilePath, strOutputFileName, FALSE )==0 )
		{
		// copy failed
		TCHAR cBuff[512];
		GetShortPathName(strInputFilePath, cBuff, 512);
		if( strOutputFileName.CompareNoCase(cBuff)==0 )
			{
			strStatus+=strInputFilePath+_T(" has already been imported\r\n");
			return false;
			}
		else
			{
			CString cError;
			cError.Format(_T("System error %d while attempting to copy IGC file."),GetLastError());
			AfxMessageBox(cError);
			return false;
			}
		}

	CIGCFile igcFile(strOutputFileName);
	if( igcFile.m_bValid )
		strStatus+=_T("IGC file ")+strInputFileName+_T(" for ")+igcFile.m_strGliderID+", "+igcFile.m_strCompetitionID+" - "+igcFile.m_strPilot+_T(" sucessfully imported.\r\n");
	else
		{
		strStatus+=_T("ERROR Reading - ")+strOutputFileName+_T("\r\n");
		return false;
		}

	return true;
	}


bool FindIGCFileInDirectory( CString strPath, CString strIGCFileName, CString& strFoundIGCFileName )
	{
	CFileStatus rStatus;

	CStringArray strFolders;

	GetFlightSearchFolders( strFolders );

	for( int iPath=0; iPath<strFolders.GetSize(); iPath++)
		{
	
		CFileFind cFindFile;
		BOOL fMore=true;

		CString strTemp=strFolders[iPath]+_T("\\");
		strTemp+=strIGCFileName;

		if( cFindFile.FindFile(strTemp) )
			{
			cFindFile.FindNextFile();
			strFoundIGCFileName=cFindFile.GetFilePath();
			return true;
			}
		}
	return false;

/*

	strTemp=strPath+"\\*");

	if( cFindFile.FindFile(strTemp) )
		{
		while( 1 )
			{
			if( !fMore ) break;
			fMore=cFindFile.FindNextFile();
			CString strFileName=cFindFile.GetFilePath();
			CFile::GetStatus(strFileName, rStatus);

			if( (rStatus.m_attribute & HIDDEN)	||
				(rStatus.m_attribute & SYSTEM)		) continue;

			if( cFindFile.GetFileName( )=="." ||
				cFindFile.GetFileName( )==".."	) continue;

			if( rStatus.m_attribute & DIRECTORY ) 
				{
				if( FindIGCFileInDirectory( strFileName,  strIGCFileName, strFoundIGCFileName ) )
					{
					return true;
					}
				}
			}
		}
	return false;
	*/
	}

bool GetFlightSearchFolders( CStringArray& strFolders, bool bResolveRelativePaths )
	{
	char buff[10];
	CString strLogPath	=GetFlightLogPath(bResolveRelativePaths);

	if( strLogPath.GetLength()>0 ) 
		strFolders.Add(strLogPath);

	int iNumFolders		=GetWinscoreInt( LOGPATHNUMFOLDERS, 0);

	for( int i=0; i<iNumFolders; i++ )
		{
		CString strSection=LOGPATH;
		_itoa_s( i+1, buff, 10, 10);
		strSection+=buff;
		CString strLogPath	=GetWinscoreString( strSection, _T("")  );
		if( strLogPath.GetLength()>0 )
			{
			if( !bResolveRelativePaths )
				strFolders.Add(strLogPath);
			else
				strFolders.Add(GetNonRelativePath(strLogPath));
			}
		}

	return strFolders.GetSize()>0;
	}


void WriteFlightSearchFolders( CStringArray& strFolders )
	{
	char buff[10];
	if( strFolders.GetSize()==0 ) 
		{
		//Nothing here, remove all
		RemoveWinscoreEntry(LOGPATH);
		RemoveWinscoreEntry(LOGPATHNUMFOLDERS);
		int i=1;
		while(i<100)
			{
			CString strSection=LOGPATH;
			_itoa_s(i, buff, 10, 10);
			strSection+=buff;
			if( !RemoveWinscoreEntry( strSection ) ) break;
			i++;
			}
		return;
		}

	WriteWinscoreInt( LOGPATHNUMFOLDERS, strFolders.GetSize()-1);

	for( int i=0; i<strFolders.GetSize(); i++ )
		{
		CString strSection=LOGPATH;
		if( i>0 ) 
			{
			_itoa_s(i, buff, 10, 10);
			strSection+=buff;
			}

		WriteWinscoreString( strSection, strFolders[i]  );
		}
	}

typedef BOOL (*ProcValidateLog)(LPCTSTR filename);
typedef VOID (*ProcInitializeDLL)(HWND windowHandle, BOOL quietMode);

bool ValidateIGC( const TCHAR* strDLLName, const TCHAR* strIGCFile, CString &strReason )
	{
	bool  bReturnVal= false;
	HMODULE mod = LoadLibrary(strDLLName);
	ProcValidateLog lpfnDllFunc1=NULL;

	if( mod!= NULL )
		{
	   lpfnDllFunc1 = (ProcValidateLog)GetProcAddress(mod, "ValidateLog" );
	   if (!lpfnDllFunc1)
			{
	   		strReason.Format(_T("Invalid manufacturer security dll: %s"),strDLLName);
		    bReturnVal=false;
			}
		else
			{
			try
				{
				int iReturnVal = lpfnDllFunc1(strIGCFile);
				if( iReturnVal==1 ) 
					bReturnVal=true;
				else
					{
					bReturnVal=false;
			   		strReason.Format(_T("Flight log failed manufacturer's security check."));
					}
				}
			catch(...)
				{
				bReturnVal=false;
		   		strReason.Format(_T("Severe error encountered during manufacturer's security check."));
				}
			}
	   FreeLibrary(mod);
	   }
	else
		{
		strReason.Format(_T("Could not find/load manufacturer's dll: %s"),strDLLName);
		bReturnVal=false;
		}

	return bReturnVal;
	}

bool WriteColumns(CString strName, CListCtrl &cList )
	{
//HKEY_CURRENT_USER\Software\Vold Solutions, Inc.\VSIRotate2\Settings

	CLocalMachineRegistry cReg(HKEY_CURRENT_USER);
	CString strKey=_T("Winscore App");
	strKey+=_T("\\Winscore");
	cReg.SetRegistryKey(strKey,_T("Settings") );

	int nColumns=cList.GetHeaderCtrl()->GetItemCount();

	for( int i=0; i<nColumns; i++ )
		{
		CString strCOLID;
		strCOLID.Format(_T("Column%d"),i);

		if (!cReg.WriteProfileInt(		strName,
										strCOLID,
										cList.GetColumnWidth(i))) return false;

		}


	return true;
	}

bool ReadColumns(CString strName, CListCtrl &cList )
	{
//HKEY_CURRENT_USER\Software\Vold Solutions, Inc.\VSIRotate2\Settings

	CLocalMachineRegistry cReg(HKEY_CURRENT_USER);
	CString strKey=_T("Winscore App");
	strKey+=_T("\\Winscore");
	cReg.SetRegistryKey(strKey,_T("Settings"));

	int nColumns=cList.GetHeaderCtrl()->GetItemCount();

	for( int i=0; i<nColumns; i++ )
		{
		CString strCOLID;
		strCOLID.Format(_T("Column%d"),i);
		int iWidth = cReg.GetProfileInt(	strName,
											strCOLID,
											-1);
		if( iWidth>=0 ) 
			{
			iWidth=max(iWidth,10);
			cList.SetColumnWidth(i,iWidth);
			}
		}


	return true;
	}


void	LoadViewControl( CWSView &cWSView, CWSBaro* pWSBaro, CWinscoreDoc *pDoc, CFlight *pcFlight, CTask *pcTask )
	{
	float fTurnpointRadius		= (float)GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	float fTurnpointOuterRadius	= (float)GetWinscoreDouble( OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );
	cWSView.SetDisplayOptionD(WSO_INNER_RADIUS, fTurnpointRadius);
	cWSView.SetDisplayOptionD(WSO_OUTER_RADIUS, fTurnpointOuterRadius);
	cWSView.SetDisplayOption(WSC_UNITS, pDoc->m_eUnits);

	CTurnpoint *pcTurnpoint=NULL;

	CContestant *pcContestant=pDoc->m_contestantList.GetByContestNo(pcFlight->m_strCID);
	if( pWSBaro
		&& pcContestant && pcContestant->IsMotorized() )
		pWSBaro->SetDisplayOption(WSC_ENL_DISPLAY, 1);
	if( pWSBaro )
		pWSBaro->SetDisplayOption(WSC_GPS_ALT_DISPLAY, 0);

	// Add the control points
	pDoc->m_turnpointArray.AddToControl(cWSView);

	CGate &cStartGate=pcFlight->m_cStartGate;

	if( pcTask )
		pcTask->AddToControl(pDoc->m_turnpointArray, cWSView, 3, &cStartGate, pcTask->m_eType==eModifiedAssigned );

	if( pcTask && ( pcTask->m_eType!=eModifiedAssigned ) )
		{
		int iPt=0;

		if( cStartGate.IsFirstLegAtExit() )
			{
			cWSView.AddTATLocation(	iPt++, 
									(float)pcFlight->m_cStartFix.GetLat(),
									(float)pcFlight->m_cStartFix.GetLong() );

			}
		else
			{
			pcTurnpoint = (CTurnpoint*)pDoc->m_turnpointArray.GetAt(cStartGate.GetPointID()-1);
			if( pcTurnpoint )
				{
				cWSView.AddTATLocation(	iPt++, 
										(float)pcTurnpoint->GetLat(),
										(float)pcTurnpoint->GetLong() );

				}
			}

		for ( int i=0; i<pcFlight->GetNumAcheivedTurnpoints(); i++ )
			{
			cWSView.AddTATLocation(	iPt++, 
									(float)pcFlight->m_acTurnAreaTurnpointLocations[i].GetLat(),
									(float)pcFlight->m_acTurnAreaTurnpointLocations[i].GetLong() );
			}

		pcTurnpoint = (CTurnpoint*)pDoc->m_turnpointArray.GetAt(pcTask->m_cFinishGate.GetPointID()-1);
		if( pcTurnpoint )
			{
			cWSView.AddTATLocation(	iPt++, 
									(float)pcTurnpoint->GetLat(),
									(float)pcTurnpoint->GetLong() );

			}
		}

	if( pcTask && (/*m_pcTask->m_eType==ePost ||*/ pcTask->m_eType==eModifiedAssigned) )
		{
		int iPt=0;

		cWSView.AddTaskPoint(cStartGate.GetPointID());

		if( cStartGate.IsFirstLegAtExit() )
			{
			cWSView.AddTATLocation(	iPt++, 
									(float)pcFlight->m_cStartFix.GetLat(),
									(float)pcFlight->m_cStartFix.GetLong() );

			}
		else
			{
			pcTurnpoint = (CTurnpoint*)pDoc->m_turnpointArray.GetAt(cStartGate.GetPointID()-1);
			if( pcTurnpoint )
				{
				cWSView.AddTATLocation(	iPt++, 
										(float)pcTurnpoint->GetLat(),
										(float)pcTurnpoint->GetLong() );

				}
			}


		// Add the task points
		for( int i=0; i<pcFlight->GetNumAcheivedTurnpoints(); i++)
			{
			cWSView.AddTaskPoint(pcFlight->GetAcheivedTurnpoint(i));

			cWSView.AddTATLocation(	iPt++, 
									(float)pcFlight->m_acTurnAreaTurnpointLocations[i].GetLat(),
									(float)pcFlight->m_acTurnAreaTurnpointLocations[i].GetLong() );

			}

		if( !pcFlight->IsTaskComplete() )
			cWSView.AddTaskPoint(pcFlight->GetSoughtTurnpoint());

		cWSView.AddTaskPoint(pcTask->m_cFinishGate.GetPointID());

		pcTurnpoint = (CTurnpoint*)pDoc->m_turnpointArray.GetAt(pcTask->m_cFinishGate.GetPointID()-1);
		if( pcTurnpoint )
			{
			cWSView.AddTATLocation(	iPt++, 
									(float)pcTurnpoint->GetLat(),
									(float)pcTurnpoint->GetLong() );

			}
		}

	// Add the pilot info
	int iPilotID=cWSView.AddPilot(	pcFlight->m_strCID, 
										pcFlight->m_strPilot, 
										_T(""), 
										pcFlight->m_eClass );
	COLORREF lColor=cWSView.GetPilotColor(iPilotID);

	if( pWSBaro )
		pWSBaro->AddPilot(	pcFlight->m_strCID,  
							pcFlight->m_strPilot, 
							_T(""), 
							pcFlight->m_eClass,
							lColor); 
	
	// Set the start and finish times.
	CPosition* pcPosStart=pcFlight->GetPosition(0);
	CPosition* pcPosEnd=pcFlight->GetPosition(pcFlight->GetNumPoints()-1);
	if( pWSBaro && pcPosStart && pcPosEnd)
		pWSBaro->SetTimeRange((long)pcPosStart->m_cTime.GetTime(), (long)pcPosEnd->m_cTime.GetTime() );

	int iStartPos=0, iFinishPos=0;
	bool bLanded=false;
	CTime cProgressTime(0);

	// At the pilot's fixes
	for ( int iPoint=0; iPoint<pcFlight->GetNumPoints(); iPoint++)
		{
		CPosition* pcPos=pcFlight->GetPosition(iPoint);

		if( !pcPos ) continue;

		if( pcPos->m_cTime < pcFlight->GetStartTime() )
				iStartPos=iPoint;
	
		if( pcPos->CheckStatus( FAN_FURTHEST_PROGRESS ) )
				{
				bLanded=true;
				iFinishPos=iPoint;
				cProgressTime=pcPos->m_cTime;
				}
		if( !bLanded && pcPos->m_cTime < pcFlight->GetFinishTime() )
				iFinishPos=iPoint;

		cWSView.AddPosition(	iPilotID, 
								(long)pcPos->m_cTime.GetTime(), 
								(float)pcPos->GetLat(), 
								(float)pcPos->GetLong(), 
								pcPos->m_iCorrectedAltitude, 
								pcPos->m_iEngineNoiseLevel );
	if( pWSBaro )
		pWSBaro->AddFullPoint(	iPilotID,  
								(long)pcPos->m_cTime.GetTime(),
								0, 
								pcPos->m_iCorrectedAltitude, 
								(float)pcPos->GetLat(), 
								(float)pcPos->GetLong(), 
								0, 
								0, 
								pcPos->m_iEngineNoiseLevel);
		}


	cWSView.SetCurrentTimeIndex(  pcFlight->GetNumPoints()-1 );


	// Add the pilot's time
	if( pcFlight->GetStatus()==eIncompleteTask || 	pcFlight->GetStatus()==eBelowFinishCylinder)
		{
		cWSView.SetPilotTimes(iPilotID, 
								(long)pcFlight->GetStartTime().GetTime(), 
								(long)cProgressTime.GetTime());
		if( pWSBaro )
			{
			pWSBaro->SetTaskOpenTime((long)pcFlight->GetStartTime().GetTime());
			pWSBaro->SetTaskCloseTime((long)cProgressTime.GetTime());
			}
		}

	if( pcFlight->GetStatus()==eCompletedTask	)
		{
		cWSView.SetPilotTimes(	iPilotID, 
									(long)pcFlight->GetStartTime().GetTime(),
									(long)pcFlight->GetFinishTime().GetTime());
		if( pWSBaro )
			{
			pWSBaro->SetTaskOpenTime((long)pcFlight->GetStartTime().GetTime());
			pWSBaro->SetTaskCloseTime((long)pcFlight->GetFinishTime().GetTime());
			}
		}

	// Add the SUA Stuff
	BOOL bSUADisplay = GetWinscoreInt(  SUADISPLAY, DEFAULT_SUADISPLAY );
	if( bSUADisplay )
		{
		int nSections=LoadSUA(pDoc->m_turnpointArray);
		if( nSections>0  ) 
			{
			double dCheckDist=pDoc->m_turnpointArray.GetMaxDistanceFromHome()*2;
			cWSView.SetColor(WSC_AIRSPACE_COLOR, RGB(255,16,16));
			for( int i=0; i<GetSUAArray()->GetSize(); i++ )
				{
				cWSView.AddSUAirspace( GetSUAArray()->GetAt(i)->m_strOriginalText );
				}
			}
		}

	}

int LoadContestsFromSSA( CComboBox	*pCombo, CListCtrl *pList ) 
{
 int nAdded=0;
	CWaitCursor cWait;

	try
		{
		CXMLMgr cMgr;
		cMgr.Initlize();

		bool b=cMgr.Load(_T("https://members.ssa.org/cde/contest_xml.asp"));
		if( !b ) 			{
			AfxMessageBox( _T("Could not load: https://members.ssa.org/cde/contest_xml.asp."),  MB_ICONSTOP   );
			return 0;
			}


		CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
		if(strRoot!=_T("SSAContest") )
			{
			AfxMessageBox( _T("Contest information at ssa.org is not valid."),  MB_ICONSTOP   );
			return 0;
			}

		MSXML2::IXMLDOMNodePtr pContests;
		TESTHR(cMgr.SelectChild(_T("contests"), cMgr.GetRoot(), pContests));
		if( pContests==NULL )
			{
			AfxMessageBox( _T("Contest information at ssa.org is empty."),  MB_ICONSTOP   );
			return 0;
			}

		MSXML2::IXMLDOMNodeListPtr pContestList=cMgr.GetChildList(pContests);
		if( pContestList==NULL ) return 0;

		long length=0;
		TESTHR(pContestList->get_length(&length));
		if( length==0 ) return 0;

		for( int i=0; i<length; i++ )
			{
			MSXML2::IXMLDOMNodePtr pContest;
			TESTHR(pContestList->get_item(i,&pContest));
			if( pContest==NULL ) break;

			CString strName;
			int		iID;
			cMgr.SelectChildSTR( pContest, _T("ContestName"), strName);
			GET_XML_INT( cMgr, pContest, _T("ContestID"), int, iID, 0);
			if( iID<=0 ) continue;

			if( pCombo )
				{
				int iIndex=pCombo->AddString (strName);
				if(iIndex<0 ) continue;
				pCombo->SetItemData(iIndex,iID);
				}
			if( pList )
				{
				int iIndex=pList->InsertItem(i,strName);
				if(iIndex<0 ) continue;
				pList->SetItemData(iIndex,iID);
				nAdded++;
				}

			nAdded++;
			}
		}
	catch (...)
		{
		AfxMessageBox( _T("Could not read contest information from ssa.org."),  MB_ICONSTOP   );
		return 0;
		}

	return nAdded;
	}	
	


CString FindExePath(CString strFileName)
{
	CFileStatus rStatus;
    TCHAR lpname[1024];

	// First try the "default" directory
	if( CFile::GetStatus( strFileName, rStatus ) ) return rStatus.m_szFullName;

	//No?  Now try in the .exe folder
      GetModuleFileName(NULL,lpname,1024);
      CString strTmp=lpname;
      int iPtr=strTmp.ReverseFind('\\');
      CString strExeFolder=strTmp.Left(iPtr);
	  CString strExeName=strExeFolder+'\\'+strFileName;

	  if( CFile::GetStatus( strExeName, rStatus ) ) return strExeName;

	//Still No?  Now try one up in the the .exe folder
      iPtr=strExeFolder.ReverseFind('\\');
      CString strExeFolder2=strExeFolder.Left(iPtr);
	  strExeName=strExeFolder2+'\\'+strFileName;

	  if( CFile::GetStatus( strExeName, rStatus ) ) return strExeName;

	  //Still No, return null.
	  return _T("");

}



CString FindMyDocumentsPath()
{
 CHAR my_documents[MAX_PATH];
 HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
 if( result==S_OK )
   return my_documents;
 else
   return "";
}

CString GetTaskLibraryPath()
	{
	CString strReg=GetWinscoreString( TASKPATH,_T("")  );
	if( strReg.GetLength() >0 ) return strReg;

	CString strMyDocuments=FindMyDocumentsPath();
	CString strTaskLibrary=strMyDocuments+"\\Winscore\\Tasks";
	CreateDirectory(strTaskLibrary, NULL);
	return strTaskLibrary;
}


CString GetFlightLogPath(bool bResolveRelativePaths)
	{
	CString strReg=GetWinscoreString( LOGPATH, _T("")  );
	if( strReg.GetLength() >0 ) 
		{
		if( !bResolveRelativePaths ) 
			return strReg; //Check for full path, return if so.
		else  
			return GetNonRelativePath(strReg);
		}

	CString strMyDocuments=FindMyDocumentsPath();
	CString strDocumentLogs=strMyDocuments+"\\Winscore\\FlightLogs";
	CreateDirectory(strDocumentLogs, NULL);
	return strDocumentLogs;
}




CString GetHandicapFileName(bool bCheckExistance)
{
	CFileStatus rStatus;

	// First check if it is in MyDocuments\Winscore
	CString strMyDocuments=FindMyDocumentsPath();
	CString strHCFile=strMyDocuments+"\\Winscore\\";
	CreateDirectory(strHCFile, NULL);
	strHCFile+=HCFILE;

	if(!bCheckExistance)
		{
		//We don't care if it is there or not, we are probably going to create it shortly
		return strHCFile;
		}

	// If it exists, return it
	if( CFile::GetStatus( strHCFile, rStatus ) ) return rStatus.m_szFullName;	
//
//  User updated copy not there, get the one in the EXE path.
	return FindExePath(HCFILE);
}



//-------------------------------------------
// A function that uses threads to Validate IGCs
//-------------------------------------------
DWORD WINAPI ValidateIGCThreadWorker( LPVOID lpParam ) 
{
	// Worker thread, call Validate IGC from here.
	TValDAta *pValData = ((TValDAta*)lpParam); 
	pValData->bRet = ValidateIGC(pValData->strDllName,pValData->strIGCName,pValData->strResult );

    return 0; 
} 


bool ValidateIGCThread ( const TCHAR* strDLLName, const TCHAR* strIGCFile,  CString &strReason )
	{
	// This function does the security checking in a separate thread, then checks if that thread is hung.
	// This was needed because of the poor quality of the Volkslogger security code.

//    CWaitCursor cWait;

	TValDAta ValData;
	ValData.strDllName=strDLLName;
	ValData.strIGCName=strIGCFile;
	ValData.bRet=false;

    // Create thread to handle the validation.
    HANDLE Handle_Of_Thread = CreateThread( NULL, 0, ValidateIGCThreadWorker, &ValData, 0, NULL);  
    if ( Handle_Of_Thread == NULL)
		{
		strReason="Internal system error while checking security.";
		return false;
		}

    // Wait 5 seconds for thread thread to terminate.
    DWORD dRet=WaitForSingleObject( Handle_Of_Thread, 5000 );

	// Thread is no longer needed.  It might be "busy" displaying a useless dialog, 
	// so terminate now it no matter what it is doing.
	TerminateThread(Handle_Of_Thread,0);

    // Close thread handle.
    CloseHandle(Handle_Of_Thread);

	if( dRet==0 ) 
		{
		if( ValData.bRet ) 
			{
			return true;
			}
		else
			{
			strReason=ValData.strResult;
			return false;
			}
		}
	else if( dRet==WAIT_TIMEOUT )
		{
		strReason=_T("Time out occured while attempting security validation.");
		return false;
		}
	else
		{
		strReason="Internal system error while checking security.";
		return false;
		}

	return false;
}

bool CheckForHandicapUpdate()
	{
	CGliderInfoList cLocalList, cSSAList;

	if( cLocalList.ImportXML(GetHandicapFileName())==0 ) return true;
	if( cSSAList.ImportXML(HC_URL)==0 ) return true;

	if( cLocalList==cSSAList ) 
		return false; // Lists are equal, we already have the latest
	else
		return true;  // Lists not equal, update available.
	}

bool UpdateHandicapListFromSSA(CGliderInfoList &cGIList)
	{
	CString strHCFile;
	CWaitCursor  cWait;
	bool bSavedOK=false;

	// Read from the SSA website
	int iRead=cGIList.ImportXML(HC_URL);
	if( iRead==0 ) 
		{
		CString strError;
		strError.Format("Could not read handicap list from the SSA website at: %s",HC_URL);
		AfxMessageBox(strError,MB_ICONSTOP);
		}
	else
		{
		//SSA Read OK, Refresh the local handicap list with that from the SSA.
		//We write this copy to MyDocuments/Winscore and will search for this file first whenever reading the list
		CString strHCFile=GetHandicapFileName(false);
		bSavedOK=cGIList.ExportXML(strHCFile);
		if( bSavedOK ) 
			{
			CString strInfo;
			strInfo.Format("%i handicap glider definitions successfully downloaded from www.SSA.org.\n\nTitle: %s\n\nDefinitions were saved to:\n\n %s\n\nThis file will be used for all future handicap lookups.",iRead,cGIList.m_strTitle,strHCFile);
			AfxMessageBox(strInfo,MB_ICONINFORMATION);
			}
		else
			{
			CString strError;
			strError.Format("Error writing handicap list : %s",strHCFile);
			AfxMessageBox(strError,MB_ICONSTOP);
			}
		}
	return bSavedOK;
	}

double GetAdjustedHandicap(CGliderInfo *pcInfo, double dWeight, double dSpan, int bRootFairing, int bTurbulation, int bWinglets )
	{
	double dWeightFactor=1.0f;
	double dSpanFactor=1.0f;
	double dHandicap=0.0;

	if( dWeight>.1 && pcInfo->m_fWeight>.001 )
		{

//      Hcap =HCref*(1.3-.4*(W/Wref)+.1*(w/wref)^2)	// Original 2019 rule publication.
//      Hcap =HCref*(1.3-.04*WR  +.01*WR*WR)	  	// Original 2019 rule publication.
//      Hcap =HCref*(1.3+ (-.04*WR+.01*WR*WR))	    // Original 2019 rule publication.
//      Hcap =HCref*(1.3-(0.4*WR-.01*WR*WR))	    // Original 2019 rule publication.
//      Hcap =HCref*(1.3-WR*(0.4-.01*WR))	        // Original 2019 rule publication.

//      Hcap =HCref*(1.3-WR*(0.4-0.1*WR))			// Corrected Mar 09, 19 (but it was correct all along)
//
//     Original 2019 implementation. 
//      dWeightFactor=1.3 - .4* (dWeight/pcInfo->m_fWeight) + .1*pow((dWeight/pcInfo->m_fWeight),2);
//
//     Updated 2019 implementation (actually the same as above)
		double dWR=dWeight/pcInfo->m_fWeight;
		dWeightFactor=1.3 - dWR*(.4 - .1*dWR);

		// Pre 2019:  dWeightFactor=1.0 - ((dWeight) - (pcInfo->m_fWeight)) * 0.0002 ;
		}
	else
		dWeightFactor=1.0;

	if( dSpan>.1)
		dSpanFactor= 1.0 - ((dSpan) - (pcInfo->m_fSpan)) / (5.0 * (pcInfo->m_fSpan));
	else
		dSpanFactor= 1.0;

	dHandicap=pcInfo->m_dHandicap*dSpanFactor*dWeightFactor;

	if( bRootFairing )	dHandicap*=.99;
//	if( bTurbulation)	dHandicap*=.99;
	if( bWinglets )		dHandicap*=.99;

	return dHandicap;
	}



bool uploadfile(CString strFile, CString &strUser, CString &strPass, bool bSilent, CString &strError )
{
	CSingleLock cLock(&cSem_uploadfile);
   // C++ example to HTTP upload any file to an HTTP server.
    CkString html;
	CkHttp http;
    CkHttpRequest req;
	bool bRetCode=false;

    // Unlock once at the beginning of your program.
     bool success=http.UnlockComponent(("CURRENHttp_QQhPraefOU7G"));
    if (success != true) 
		{
		strError=_T("Internal Error - Could not initialize CkHttp");
        if(!bSilent) AfxMessageBox(strError);
        return false;
		}
	 // Create an HTTP request for file upload.
    req.UseUpload();

    // The first argument to AddFileForUpload is an arbitrary name,
    // the second argument is a filepath for a file that exists on your local
    // filesystem.
    success=req.AddFileForUpload(("filename"),strFile);
    if (success != true) {
        strError=_T("Upload file not found");
        if(!bSilent) AfxMessageBox(strError);
        return false;
    }
	CString strPath;
	strPath.Format("/cde/TestUploadAction.asp?user=%s&pass=%s",strUser,strPass );

    req.put_Path(strPath);

    //  Send the HTTP POST and get the response.  Note: This is a blocking call.
    //  The method does not return until the full HTTP response is received.
    CkString domain;
    int port;
    bool ssl;
    domain = _T("members.ssa.org");
    port = 443;//80;
    ssl = true;//false;
    CkHttpResponse *resp = 0;
    resp = http.SynchronousRequest(domain,port,ssl,req);
    if (resp == 0 ) {
		strError=_T("Could not connect to the SSA server. Check you Internet connection.");
		if(!bSilent) AfxMessageBox(strError,MB_ICONSTOP);
		return false;
    }
    else {
        int responseStatus;
        responseStatus = resp->get_StatusCode();
        if (responseStatus == 200) 
			{
			CkString html;
			html = resp->bodyStr();
			CString str((const char *)html);
			CString strMess("The SSA Server returned the following:\n\n");
			strMess+=str;
			UINT uStyle;
			if( str.Find(_T("authorised"))>=0 )
				{
				uStyle=MB_ICONSTOP;
				strMess+=+_T("\n\n Check that your username and password are correct");
				}
			else
				{
				if( str.Find(_T("success"))>=0 || str.Find(_T("bad file type"))>=0) 
					{
					bRetCode=true;
					uStyle=MB_ICONINFORMATION;
					}
				else if( str.Find(_T("already exists"))>=0 ) 
					{
					bRetCode=false;
					uStyle=MB_ICONSTOP;
					strMess=_T("This scoresheet already exists on the server.  Please try the transfer again in 1 minute.");
					}
				else
					{	
					uStyle=MB_ICONSTOP;
					}
				}
			strError=strMess;
			if(!bSilent) AfxMessageBox(strMess,uStyle);
			}
		else
			{
			strError.Format(_T("SSA Server returned error code = %d"),responseStatus);
			if(!bSilent) AfxMessageBox(strError,MB_ICONSTOP);
			return false;
			}
	}

return bRetCode;
}

CString	GetScoresheetPathName(int iSSA_ID)
			{
			 CString cFileName;
			 CString strPathName;
			 CTime tnow = CTime::GetCurrentTime();
			 cFileName.Format(	_T("Scoresheet_%d_%s.xml"), iSSA_ID, tnow.Format(_T("%d-%b-%y_%H-%M")) );
			size_t bSize=256;
			TCHAR *pTemp=new TCHAR[bSize];
			_dupenv_s(&pTemp, &bSize, _T("TEMP") );
			if( pTemp==NULL ) 
				   _dupenv_s(&pTemp, &bSize, _T("TMP") );
			strPathName=pTemp;
			strPathName+=_T("\\");
			strPathName+=cFileName;
			delete pTemp;
			return strPathName;
			}

CString  GetTempPath()
	{
	size_t bSize=256;
	TCHAR *pTemp=new TCHAR[bSize];
	_dupenv_s(&pTemp, &bSize, _T("TEMP") );
	if( pTemp==NULL ) 
		   _dupenv_s(&pTemp, &bSize, _T("TMP") );
	CString strRet(pTemp);
	delete pTemp;
	return strRet;
	}

CString getNoLogKey(CString cCID, CTime cDate)
	{
	CString strKey;
	strKey.Format("NOLOG_%s_%d_%d",cCID,cDate.GetMonth(),cDate.GetDay());
	return strKey;
	}

void  FormatFlightLogZipName( CString &strZipFileName, int iSSA_ID, CTime *pcTime) 
	{
	strZipFileName.Format(_T("FlightLogs_%d_%s.zip"), 
	 					  iSSA_ID,
			  			  pcTime->Format(_T("%d-%b-%Y")));
	}