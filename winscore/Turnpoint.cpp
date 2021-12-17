//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Turnpoint.cpp: implementation of the CTurnpoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "Location.h"
#include "Turnpoint.h"
#include "global_prototypes.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CTurnpoint, CLocation, VERSIONABLE_SCHEMA|2)
CTurnpoint::CTurnpoint()
{
	// This empty constructor should be used by serialization only
}


CTurnpoint::CTurnpoint(int iID, 
					   CString strName, 
					   CLocation* pcLocation, 
					   int iAttribute, int iElevation ) : CLocation(pcLocation)
{
	m_iID=iID;
	m_strName=strName;
	m_iAttribute=iAttribute;
	m_iElevation=iElevation;
}

CTurnpoint::CTurnpoint(CString strLCAMField )
	{
//123456789012345678911234567892123456789312345678941
//LCAMN000012711270N08150400W000180TA      01 Arcadia  
//          HHMMMMM HHHMMMMM

	int iLatDeg	=atoi( strLCAMField.Mid(10,2) );
	int iLatMin	=atoi( strLCAMField.Mid(12,5) );
	CString cLat=strLCAMField.Mid(17,1);
	double dLatMin= (double)iLatMin/1000.;

	int iLonDeg	=atoi( strLCAMField.Mid(18,3) );
	int iLonMin	=atoi( strLCAMField.Mid(21,5) );
	CString cLon=strLCAMField.Mid(26,1);
	double dLonMin= (double)iLonMin/1000.;

	m_iID=atoi( strLCAMField.Mid(7,3) );

	m_iElevation=0;
	m_iElevation	=atoi( strLCAMField.Mid(27,5) );

	CString cAttributes=strLCAMField.Mid(32,8);
	
	m_iAttribute=0;
	if( cAttributes.Find(_T("T"),0)>=0 ) m_iAttribute|=TPT_CONTROL;
	if( cAttributes.Find(_T("A"),0)>=0 || cAttributes.Find(_T("L"),0)>=0 ) m_iAttribute|=TPT_AIRFIELD;
	if( cAttributes.Find(_T("F"),0)>=0 ) m_iAttribute|=TPT_FINISH;
	if( cAttributes.Find(_T("H"),0)>=0 ) m_iAttribute|=TPT_HOME;
	if( cAttributes.Find(_T("S"),0)>=0 ) m_iAttribute|=TPT_START;

	m_strName=strLCAMField.Mid(41,10);


	// Initialize CLocation members
	m_iLatDegrees=iLatDeg;
	m_dLatMinutes=dLatMin;
	m_iLongDegrees=iLonDeg;
	m_dLongMinutes=dLonMin;
	m_eLatDirection=(cLat=='N')?(eNORTH):(eSOUTH);
	m_eLongDirection=(cLat=='E')?(eEAST):(eWEST);
	}



CTurnpoint::~CTurnpoint()
{

}

CString CTurnpoint::GetFormattedInfo(ECoordFormat eCoordFormat )
	{
	CString cTemp1, strLat, strLon;
	(m_eLatDirection==eNORTH ) ? (strLat=_T("N")) : (strLat=_T("S"));
	(m_eLongDirection==eWEST ) ? (strLon=_T("W")) : (strLon=_T("E"));

	if( eCoordFormat==eHHMMMM )
	{   
	cTemp1.Format(_T(" %3d  %-30s \t|   %2d  %-7.4lf %s \t|   %2d  %-7.4lf %s"), 
		m_iID, 
		m_strName,
		m_iLatDegrees,
		m_dLatMinutes, 
		strLat, 
		m_iLongDegrees, 
		m_dLongMinutes,
		strLon);
	}
	else
	{
	cTemp1.Format(_T(" %3d  %-30s \t|   %2d  %2d  %2d %s \t|  %2d  %2d  %2d %s"),  
		 m_iID, 
		 m_strName,
		 m_iLatDegrees,
		 this->GetLatMinutes(),
		 this->GetLatSeconds(),
		 strLat, 
		 m_iLongDegrees, 
		 this->GetLongMinutes(),
		 this->GetLongSeconds(),
		 strLon);
	}

	return cTemp1;
}




CString CTurnpoint::IdText()
	{
	CString cTemp;
	cTemp.Format( _T("%d"), m_iID );
	return cTemp;
	}

CString CTurnpoint::HHMMSSLatText()
	{
	CString cTemp1, strLat;
	(m_eLatDirection==eNORTH ) ? (strLat=_T("N")) : (strLat=_T("S"));
	cTemp1.Format(_T("%2d  %2d  %2d %s"),  
		 m_iLatDegrees, 
		 GetLatMinutes(),
		 GetLatSeconds(),
		 strLat);
	return cTemp1;
	}

CString CTurnpoint::HHMMSSLongText()
	{
	CString cTemp1,  strLon;
	(m_eLongDirection==eWEST ) ? (strLon=_T("W")) : (strLon=_T("E"));

	cTemp1.Format(_T("%2d  %2d  %2d %s"),  
		 m_iLongDegrees, 
		 GetLongMinutes(),
		 GetLongSeconds(),
		 strLon);
	return cTemp1;
	}

CString CTurnpoint::HHHMMMLatText()
	{
	CString cTemp1, strLat;
	(m_eLatDirection==eNORTH ) ? (strLat=_T("N")) : (strLat=_T("S"));
	cTemp1.Format(_T(" %2d  %06.3lf %s"), 
		m_iLatDegrees,
		m_dLatMinutes, 
		strLat);

	return cTemp1;
	}


CString CTurnpoint::HHHMMMLongText()
	{
	CString cTemp1, strLon;
	(m_eLongDirection==eWEST ) ? (strLon=_T("W")) : (strLon=_T("E"));
	cTemp1.Format(_T("%2d  %06.3lf %s"), 
		m_iLongDegrees,
		m_dLongMinutes, 
		strLon);

	return cTemp1;
	}


void CTurnpoint::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		ar << m_iID;
		ar << m_strName;
		ar << m_iAttribute;
		ar << m_iElevation;
	}
	else
	{
		UINT iSchema=ar.GetObjectSchema( );
		ar >> m_iID;
		ar >> m_strName;
		ar >> m_iAttribute;
		if( iSchema==1 )
			m_iElevation=0;
		else
			ar >> m_iElevation;

	}
	CLocation::Serialize(ar);
}


int CALLBACK CompareTurnpoint(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	double dTemp1, dTemp2;
	int iR=0;
	switch( abs(iColumn)-1 )
		{
	case 0:
		if(	 ((CTurnpoint*)lParam1)->m_iID < 
			 ((CTurnpoint*)lParam2)->m_iID   ) iR= -1;
		else if( ((CTurnpoint*)lParam1)->m_iID > 
			 ((CTurnpoint*)lParam2)->m_iID   ) iR=  1;
		break;
	case 1:
		iR=		strcmp( ((CTurnpoint*)lParam1)->m_strName,
						((CTurnpoint*)lParam2)->m_strName  );
		break;
	case 2:
	    dTemp1= ((CTurnpoint*)lParam1)->m_iLatDegrees +
				((CTurnpoint*)lParam1)->m_dLatMinutes/60.;
		if (((CTurnpoint*)lParam1)->m_eLatDirection==eSOUTH) dTemp1=(-dTemp1);


	    dTemp2= ((CTurnpoint*)lParam2)->m_iLatDegrees +
				((CTurnpoint*)lParam2)->m_dLatMinutes/60;
		if (((CTurnpoint*)lParam2)->m_eLatDirection==eSOUTH) dTemp2=(-dTemp2); 

		if( dTemp1>dTemp2 ) iR= -1;
		if( dTemp1<dTemp2 ) iR=   1;
		break;
	case 3:
	    dTemp1= ((CTurnpoint*)lParam1)->m_iLongDegrees +
				((CTurnpoint*)lParam1)->m_dLongMinutes/60.;
		if (((CTurnpoint*)lParam1)->m_eLongDirection==eEAST) dTemp1=(-dTemp1);

	    dTemp2= ((CTurnpoint*)lParam2)->m_iLongDegrees +
				((CTurnpoint*)lParam2)->m_dLongMinutes/60.;
		if (((CTurnpoint*)lParam2)->m_eLongDirection==eEAST) dTemp2=(-dTemp2);

		if( dTemp1>dTemp2 ) iR=  -1;
		if( dTemp1<dTemp2 ) iR=   1;
		break;
	case 4:
		if(	 ((CTurnpoint*)lParam1)->m_iAttribute < 
			 ((CTurnpoint*)lParam2)->m_iAttribute   ) iR=  -1;
		else if( ((CTurnpoint*)lParam1)->m_iAttribute > 
			 ((CTurnpoint*)lParam2)->m_iAttribute   ) iR=   1;
		break;
	case 5:
		if(	 ((CTurnpoint*)lParam1)->m_iElevation < 
			 ((CTurnpoint*)lParam2)->m_iElevation   ) iR=  -1;
		else if( ((CTurnpoint*)lParam1)->m_iElevation > 
			 ((CTurnpoint*)lParam2)->m_iElevation   ) iR=   1;
		break;


	}
	return (iColumn<0)?(-iR):(iR);

}

CString CTurnpoint::GetNoNameText()
	{
	CString cTemp1;

	cTemp1.Format(_T("%-3d - %-30s"), m_iID, m_strName);
	cTemp1.TrimRight();
	return cTemp1;
}

CString CTurnpoint::AttributeText()
{
	CString cTemp=_T(" ");
	if( m_iAttribute & TPT_HOME ) cTemp+=_T("H ");
	if( m_iAttribute & TPT_START ) cTemp+=_T("S ");
	if( m_iAttribute & TPT_FINISH ) cTemp+=_T("F ");
	if( m_iAttribute & TPT_AIRFIELD ) cTemp+=_T("A ");
	if( m_iAttribute & TPT_CONTROL ) cTemp+=_T("T ");
	return cTemp;
}

CString CTurnpoint::ElevationText()
{
	CString cTemp;
	cTemp.Format(_T("%d"),m_iElevation);
	return cTemp;
}

bool CTurnpoint::IsAirfield()
{
	return (m_iAttribute & TPT_AIRFIELD)?(true):(false);
}

bool CTurnpoint::IsControlPoint()
{
if	 ((m_iAttribute & TPT_HOME)	  ||
	  (m_iAttribute & TPT_START)  ||
	  (m_iAttribute & TPT_FINISH) ||
	  (m_iAttribute & TPT_CONTROL)   )
	return true;
else
	return false;

}

bool CTurnpoint::IsTurnPoint()
{
if	 (m_iAttribute & TPT_CONTROL )
	return true;
else
	return false;
}

void CTurnpoint::LoadListItem(CListCtrl &ListCtrl, int iItem,  ECoordFormat eCoordFormat)
{
		ListCtrl.SetItemText(iItem,0,IdText());
		ListCtrl.SetItemText(iItem,1,m_strName);
		if( eCoordFormat==eHHMMSS ) 
			{
			ListCtrl.SetItemText(iItem,2,HHMMSSLatText());
			ListCtrl.SetItemText(iItem,3,HHMMSSLongText());
			}
		else
			{
			ListCtrl.SetItemText(iItem,2,HHHMMMLatText());
			ListCtrl.SetItemText(iItem,3,HHHMMMLongText());
			}
		ListCtrl.SetItemText(iItem,4,AttributeText());
		ListCtrl.SetItemText(iItem,5,ElevationText());
		ListCtrl.SetItemData(iItem, (LPARAM)this );

}

BOOL CTurnpoint::HasAttribute(int iAtt)
{
if	 (m_iAttribute & iAtt)
	return true;
else
	return false;
}

int CTurnpoint::GetID()
{
return m_iID;
}

int CTurnpoint::GetElevation()
{
return m_iElevation;
}

bool CTurnpoint::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pParentNode )
{
	MSXML2::IXMLDOMNodePtr pWaypoint;
	TESTHR(cMgr.CreateChild(pParentNode, pWaypoint, _T("Waypoint") ));

    TESTHR(cMgr.CreateElementInt(	pWaypoint, _T("ID"), GetID()));
    TESTHR(cMgr.CreateElement(		pWaypoint, _T("Name"), m_strName));

	CLocation::GetXML(cMgr, pWaypoint);

	CString strAtt=AttributeText();
    TESTHR(cMgr.CreateElement(		pWaypoint, _T("Attributes"), strAtt));
    TESTHR(cMgr.CreateElementInt(	pWaypoint, _T("Elevation"), m_iElevation));


	return true;
}

CTurnpoint::CTurnpoint(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWaypoint)
	{
	m_iID= -1;
	MSXML2::IXMLDOMNodePtr pNode;
		
    GET_XML_INT( cMgr, pWaypoint, _T("ID"), int, m_iID, 0);
	if( m_iID==0 ) throw NULL;

    GET_XML_STR( cMgr, pWaypoint, _T("Name"), m_strName);
    GET_XML_INT( cMgr, pWaypoint, _T("Elevation"), int, m_iElevation, 0);

	CString strAtt;
    GET_XML_STR( cMgr, pWaypoint, _T("Attributes"), strAtt);

	m_iAttribute=0;
	if( strAtt.Find('A')>=0 ) 
		m_iAttribute |=TPT_AIRFIELD;
	if( strAtt.Find('S')>=0 ) 
		m_iAttribute |=TPT_START;
	if( strAtt.Find('F')>=0 ) 
		m_iAttribute |=TPT_FINISH;
	if( strAtt.Find('H')>=0 ) 
		m_iAttribute |=TPT_HOME;
	if( strAtt.Find('T')>=0 ) 
		m_iAttribute |=TPT_CONTROL;

	CLocation::FromXML(cMgr, pWaypoint);
	}
