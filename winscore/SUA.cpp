//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// SUA.cpp: implementation of the CSUASection class.
//
//////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdafx.h"
#include "SUA.h"

#include <iostream>
//#include <strstrea>
#include <sstream>
#include <fstream>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static bool readlocation(CString &cLine, int iStart, CLocation &cLocation);
static int readAltitude(CString &cLine, int iStart);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSUASection::CSUASection()
{

}

CSUASection::~CSUASection()
	{
	if( m_aSubSections.GetSize()>0 )
		{
		for( int i=0; i<m_aSubSections.GetSize(); i++ )
			delete (CSUASubSection*)m_aSubSections[i];
		}
	}

CSUASection::CSUASection(const TCHAR *pstr)
	{
#define MAXLINELEN		256
	int			iTop=0, iBase=0;
	double		dRadius=10;
	double		dWidth=10;
	int			iCharLen=strlen(pstr);

	m_strOriginalText=pstr;

	CSUASection::ESUAType	eType=eCTACTR;
	char  cBuff[MAXLINELEN];

	std::istringstream strInp(pstr);

	while( !strInp.eof() )
		{
		strInp.getline( cBuff, MAXLINELEN );
		if( strInp.fail() ) break;

		CString cLine=cBuff;
		if( cLine.GetLength()==0 ) continue;

		if( cLine.Find(_T("TITLE="))>=0 )
			{
			cLine.Replace(_T("TITLE="),_T(""));
			m_strTitle=cLine;
			}

		else if( cLine.Find(_T("BASE"))==0 )
			{
			iBase=readAltitude(cLine, 5);
			}

		else if( cLine.Find(_T("TOPS"))==0 )
			{
			iTop=readAltitude(cLine, 5);
			}

		else if( cLine.Find(_T("CIRCLE"))>=0 )
			{
			try {
				CSUASubSection *pSSect=new CSUASubSection();
				m_aSubSections.Add(pSSect);

				//CIRCLE RADIUS=10 CENTRE=N555700 W0032227
				int iPos=cLine.Find(_T("RADIUS"));
				iPos=cLine.Find(_T("="),iPos);
				int icPos=cLine.Find(_T("CENTRE"));
				CString cTest(cLine.Mid(iPos+1,icPos-iPos-1));
				
				double dRadius=atof(cLine.Mid(iPos+1,icPos-iPos-1));

				readlocation(cLine, icPos+6, pSSect->m_cCenter);
				pSSect->m_eType=CSUASubSection::eCIRCLE;
				pSSect->m_dRadius=dRadius;
				pSSect->m_iBase=iBase;
				pSSect->m_iTop=iTop;
				pSSect->m_cLocation=pSSect->m_cCenter;
				}
			catch(...)
				{
				AfxMessageBox(_T("Unhandled exception in CSUASection::CSUASection processing CIRCLE."));
				throw;
				}
			}
		else if( cLine.Find(_T("CLOCKWISE"))==0 ||
				 cLine.Find(_T("ANTI-CLOCKWISE"))==0 )
			{
			try { 
				CSUASubSection *pSSect=new CSUASubSection();
				m_aSubSections.Add(pSSect);

				//CLOCKWISE RADIUS=8 CENTRE=N522734 W0014494 TO=N521948 W0014754

				int iPos=cLine.Find(_T("RADIUS"));
				iPos=cLine.Find(_T("="),iPos);
				int icPos=cLine.Find(_T("CENTRE"));

				double dRadius=atof(cLine.Mid(iPos+1,icPos-(iPos+1)));

				readlocation(cLine, icPos+6, pSSect->m_cCenter);

				iPos=cLine.Find(_T("TO="));
				readlocation(cLine, iPos+3, pSSect->m_cLocation);

				if( cLine.Find(_T("CLOCKWISE"))==0 )
					pSSect->m_eType=CSUASubSection::eCLOCKWISE;
				else
					pSSect->m_eType=CSUASubSection::eANTICLOCKWISE;
				pSSect->m_dRadius=dRadius;
				pSSect->m_iBase=iBase;
				pSSect->m_iTop=iTop;
				}
			catch(...)
				{
				AfxMessageBox(_T("Unhandled exception in CSUASection::CSUASection processing CLOCKWISE."));
				throw;
				}
			}

		else if( cLine.Find(_T("POINT="))>=0 )
			{
			CSUASubSection *pSSect=NULL;
			try {
				pSSect=new CSUASubSection();
				m_aSubSections.Add(pSSect);
				readlocation(cLine, 5, pSSect->m_cLocation);
				pSSect->m_eType=CSUASubSection::ePOINT;
				pSSect->m_iBase=iBase;
				pSSect->m_iTop=iTop;
				}
			catch(...)
				{
				AfxMessageBox(_T("Unhandled exception in CSUASection::CSUASection processing POINT."));
				throw;
				}
			}
		}

	}


CSUASubSection::CSUASubSection()
	{

	}

CSUASubSection::~CSUASubSection()
	{
	}


static bool readlocation(CString &cLine, int iStart, CLocation &cLocation)
	{
	int iPos=0;

	iPos=cLine.Find(_T("N"), iStart);
	if( iPos<0 )
		{
		iPos=cLine.Find(_T("S"), iStart);
		}

	if( iPos<0 ) return false;

	TCHAR cLat  =cLine[iPos];
	int iLatDeg=atoi(cLine.Mid(iPos+1,2));
	int iLatMin=atoi(cLine.Mid(iPos+3,2));
	int iLatSec=atoi(cLine.Mid(iPos+5,2));

	TCHAR cLon  =cLine[iPos+8];
	int iLonDeg=atoi(cLine.Mid(iPos+9,3));
	int iLonMin=atoi(cLine.Mid(iPos+12,2));
	int iLonSec=atoi(cLine.Mid(iPos+14,2));

	CLocation cLoc(	iLatDeg, iLatMin, iLatSec,
					iLonDeg, iLonMin, iLonSec,
					(cLat=='N')?(eNORTH):(eSOUTH), 
					(cLon=='E')?(eEAST):(eWEST) );
	cLocation=cLoc;
	return true;
	};


static int readAltitude(CString &cLine, int iStart)
	{
	if( cLine.Find(_T("SFC"))>0 )
		return 0;
	else if( cLine.Find(_T("UNLTD"))>0 )
		return 30000;
	else
		{
		cLine.Replace(_T("BASE="),_T(""));
		cLine.Replace(_T("TOPS="),_T(""));
		if( cLine.Find(_T("ALT"))>0 )
			{
			cLine.Replace(_T("ALT"),_T(""));
			return atoi(cLine);
			}
		else if( cLine.Find(_T("FL"))>=0 )
			{
			cLine.Replace(_T("FL"),_T(""));
			return atoi(cLine)*100;
			}
		else
			return atoi(cLine);
		}
	return 0;
	}

bool CSUASection::FarFrom(CLocation &cLocation, double dDist)
{
		if( m_aSubSections.GetSize()>0 )
			{
			for( int i=0; i<m_aSubSections.GetSize(); i++ )
				{
				CSUASubSection* pSection=(CSUASubSection*)m_aSubSections[i];
				if( pSection->m_cLocation.DistanceTo(cLocation,eNautical)<dDist ) return false;
				}
			}
		return true;
}

//////////////////////////////////////////////////////////////////////
// CSUASectionArray Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSUASectionArray::CSUASectionArray()
{

}

CSUASectionArray::~CSUASectionArray()
	{
	Purge();
	}

CSUASection* CSUASectionArray::GetAt(int i)
	{
	return (CSUASection*)CPtrArray::GetAt(i);
	}


void CSUASectionArray::Purge()
{
	m_strFileName=_T("");
	for ( int i=0; i<GetSize(); i++)  
		{
		CSUASection* ptr=GetAt(i);
		delete ptr;
		}
	RemoveAll( );
}
