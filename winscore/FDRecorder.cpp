//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRecorder.cpp: implementation of the CFDRecorder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreDefs.h"
#include "global_prototypes.h"
#include "FDRecorder.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFDRecorder::CFDRecorder()
	{
	m_iENLMin=0;
	m_iENLMax=0;
	m_iMOPMin=0;
	m_iMOPMax=0;

	m_iYear=0;
	m_iMonth=0;
	m_iDay=0;
	}

CFDRecorder::~CFDRecorder()
{

}

CFDRecorder::CFDRecorder(CIGCFile *pcFlight)
	{
	m_strManufacturer	=pcFlight->m_strManufacturer;
	m_strFDRID			=pcFlight->m_strFDRID;
	m_strHardwareVersion=pcFlight->m_strHardwareVersion;
	m_strFileName		=pcFlight->m_strFileName;
	m_strCompetitionID	=pcFlight->m_strCompetitionID;

	m_iENLMin=pcFlight->m_iENLMin;
	m_iENLMax=pcFlight->m_iENLMax;
	m_iMOPMin=pcFlight->m_iMOPMin;
	m_iMOPMax=pcFlight->m_iMOPMax;

	m_iYear=	pcFlight->m_iYear;
	m_iMonth=	pcFlight->m_iMonth;
	m_iDay=		pcFlight->m_iDay;

	}
	
CString CFDRecorder::GetDateText()
{
	CString strDate=CTime( m_iYear, m_iMonth, m_iDay, 0,0,0).Format( _T("%B %d, %Y") );
	return strDate;
}


CString CFDRecorder::GetFDRENLMOPText()
	{
	CString strInfo;
	CString strENL, strMOP;
	if( m_iENLMax>0 )
		strENL.Format("ENL: %i/%i ",m_iENLMin, m_iENLMax);
	if( m_iMOPMax>0 ) 
		strMOP.Format("MOP: %i/%i",m_iMOPMin, m_iMOPMax);
	strInfo=strENL+strMOP;
	return strInfo;
	}

CString CFDRecorder::GetFDRID(bool bIncludeENL)
	{
	if(!bIncludeENL ) return m_strFDRID;

	CString strOut=m_strFDRID;
	strOut+="   ";
	strOut+=GetFDRENLMOPText();
	return strOut;
	}

void  CFDRecorder::GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	MSXML2::IXMLDOMNodePtr pFDRRecorder=NULL;
	cMgr.CreateChild( pParentNode, pFDRRecorder, _T("FDRRecorder") );

	cMgr.CreateElement		( pFDRRecorder, _T("FDRID"),		LPCSTR(m_strFDRID) );
	cMgr.CreateElement		( pFDRRecorder, _T("MANUFACTURER"), LPCSTR(m_strManufacturer) );
	cMgr.CreateElement		( pFDRRecorder, _T("HARDWAREVERS"), LPCSTR(m_strHardwareVersion) );
	cMgr.CreateElement		( pFDRRecorder, _T("FILENAME"),		LPCSTR(m_strFileName) );
	cMgr.CreateElement		( pFDRRecorder, _T("CID"),			LPCSTR(m_strCompetitionID) );

	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("MinENL"),			m_iENLMin));
	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("MaxENL"),			m_iENLMax));
	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("MinMOP"),			m_iMOPMin));
	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("MaxMOP"),			m_iMOPMax));

	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("Year"),			m_iYear));
	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("Month"),			m_iMonth));
	TESTHR(cMgr.CreateElementIntC( pFDRRecorder, _T("Day"),				m_iDay));
	}

CFDRecorder::CFDRecorder(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pRecorder)
	{
	cMgr.SelectChildSTR( pRecorder, _T("FDRID"),		m_strFDRID);
	cMgr.SelectChildSTR( pRecorder, _T("MANUFACTURER"), m_strManufacturer);
	cMgr.SelectChildSTR( pRecorder, _T("HARDWAREVERS"),	m_strHardwareVersion);
	cMgr.SelectChildSTR( pRecorder, _T("FILENAME"),		m_strFileName);
	cMgr.SelectChildSTR( pRecorder, _T("CID"),			m_strCompetitionID);

	GET_XML_INT( cMgr, pRecorder, _T("MinENL"),			int, m_iENLMin,0);
	GET_XML_INT( cMgr, pRecorder, _T("MaxENL"),			int, m_iENLMax,0);
	GET_XML_INT( cMgr, pRecorder, _T("MinMOP"),			int, m_iMOPMin,0);
	GET_XML_INT( cMgr, pRecorder, _T("MaxMOP"),			int, m_iMOPMax,0);

	GET_XML_INT( cMgr, pRecorder, _T("Year"),			int, m_iYear,0);
	GET_XML_INT( cMgr, pRecorder, _T("Month"),			int, m_iMonth,0);
	GET_XML_INT( cMgr, pRecorder, _T("Day"),			int, m_iDay,0);

	}

	CString CFDRecorder::GetENLText()
		{
		CString strOut;
		strOut.Format("%i/%i",m_iENLMin,m_iENLMax);
		return strOut;
		}
	CString CFDRecorder::GetMOPText()
		{
		CString strOut;
		strOut.Format("%i/%i",m_iMOPMin,m_iMOPMax);
		return strOut;
		}

CTime CFDRecorder::GetTime()
	{
	return CTime(	m_iYear, m_iMonth, m_iDay, 0, 0, 0 );
	}


//	_T("FDR ID"), _T("Date"), _T("CID"), _T("Manufacturer"), _T("Hardware Version"), _T("File Name"),  _T("ENL (min/max)"), _T("MOP (min/max)") 
int CALLBACK CompareRecorders(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	int iR=0;
	switch( abs(iColumn)-1 )
		{
	case 0:
		iR=strcmp(  ((CFDRecorder*)lParam1)->GetFDRID(),
					((CFDRecorder*)lParam2)->GetFDRID()  );
		break;
	case 1:
		{
		if( ((CFDRecorder*)lParam1)->GetTime() <
			((CFDRecorder*)lParam2)->GetTime()  ) iR= -1;
		if( ((CFDRecorder*)lParam1)->GetTime() >
			((CFDRecorder*)lParam2)->GetTime()  ) iR=  1;
		break;
//		iR	= strcmp( ((CFDRecorder*)lParam1)->GetDateText(),
//					  ((CFDRecorder*)lParam2)->GetDateText() );
		}
	case 2:
		iR	= strcmp( ((CFDRecorder*)lParam1)->GetCID(),
					  ((CFDRecorder*)lParam2)->GetCID()  );
		break;
	case 3:
		iR=strcmp(  ((CFDRecorder*)lParam1)->GetManufacturer(),
					((CFDRecorder*)lParam2)->GetManufacturer()  );
		break;
	case 4:
		iR=strcmp(  ((CFDRecorder*)lParam1)->GetHardwareVersion(),
					((CFDRecorder*)lParam2)->GetHardwareVersion() );
		break;
	case 5:
		iR=strcmp(   ((CFDRecorder*)lParam1)->GetFileName(),
				     ((CFDRecorder*)lParam2)->GetFileName()  );
		break;
	case 6:
		iR=strcmp(  ((CFDRecorder*)lParam1)->GetENLText(),
					((CFDRecorder*)lParam2)->GetENLText() );
		break;
	case 7:
		iR=strcmp(  ((CFDRecorder*)lParam1)->GetMOPText(),
					((CFDRecorder*)lParam2)->GetMOPText() );
		break;
	}
	return (iColumn<0)?(-iR):(iR);
}
