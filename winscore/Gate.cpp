//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Gate.cpp: implementation of the CGate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifndef _CAIEXPLR
#include "winscore.h"
#include "Global_Prototypes.h"
#else
#include "caiexplr.h"
#include "utility.h"
#endif
#include "position.h"
#include "TurnpointArray.h"
#include "Gate.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef _CAIEXPLR
IMPLEMENT_SERIAL( CGate, CGate, 1)
#endif

CGate::CGate( ) : 
	m_iGatePointID(0),
	m_dOrientation(0.),
	m_dRadius(5),
	m_eGateType(eCylinder),
	m_iBase(0),
	m_iHeight(5000),
	m_dHeading(0),
	m_bFirstLegAtExit(false),
	m_bPerpToCourse(false),
	m_bScoreOutTheTop(true)
	{
	}

void CGate::Initialize( CLocation &cCenterLoc )
	{
	double dCorr=GetLatCorrection(cCenterLoc.GetLat());

	m_cCenter=cCenterLoc;
	m_dOrientation=((double)(m_dHeading-90))*TORAD;

	m_cEnd1=m_cCenter;
	m_cEnd1.Move( m_dRadius, m_dOrientation, dCorr);
	m_cEnd2=m_cCenter;
	m_cEnd2.Move( m_dRadius, ((double)(m_dHeading+90))*TORAD, dCorr);

	m_cIP=m_cCenter;
	m_cIP.Move( m_dRadius, ((double)(m_dHeading+180))*TORAD, dCorr );

	}

void CGate::SetHeading(double d)
	{
	m_dHeading=d;
    }


BOOL CGate::CrossedLine(CLocation &cP1, CLocation &cP2, BOOL bAllowEitherDirection)
{
	if( m_eGateType!=eLine ) return false;

	CLocation cLoc;
	if (GetCrossingLocation(cP1, cP2, cLoc) )
		{
		// He crossed the gate, now lets see if he went the right direction.
		if (bAllowEitherDirection) return true;

		//Find a vector from the 1st fix to the 2nd
		double dV1x=cP2.GetLong()-cP1.GetLong();
		double dV1y=cP2.GetLat() -cP1.GetLat();

		// Find a vector from the IP to the center of the gate.
		double dVCenterx=m_cCenter.GetLong()-m_cIP.GetLong();
		double dVCentery=m_cCenter.GetLat() -m_cIP.GetLat();

		// Dot the two vectors, if Negative, he went the wrong way.
		double dDot1=dV1x*dVCenterx+dV1y*dVCentery;
		if( dDot1<0.0 ) return false;

		return true;
		}
/*
	double dV1x=cP1.GetLong()-m_cCenter.GetLong();
	double dV1y=cP1.GetLat() -m_cCenter.GetLat();
	double dV2x=cP2.GetLong()-m_cCenter.GetLong();
	double dV2y=cP2.GetLat() -m_cCenter.GetLat();

	double dVCenterx=m_cIP.GetLong()-m_cCenter.GetLong();
	double dVCentery=m_cIP.GetLat() -m_cCenter.GetLat();
	
	double dDot1=dV1x*dVCenterx+dV1y*dVCentery;
	double dDot2=dV2x*dVCenterx+dV2y*dVCentery;

	if( (dDot1>0.0 && dDot2<0.0) || (bAllowEitherDirection && (dDot1<0.0 && dDot2>0.0)) ) 
		{
		return true;
		}
*/
	return false;
}

BOOL CGate::CrossingTime(CPosition &cP1, CPosition &cP2, CTime &cTime)
	{

	CLocation cCrossingPoint;

	if( !GetCrossingLocation(cP1, cP2, cCrossingPoint) ) return false;

	double dTotalDistance	=cP1.DistanceTo(&cP2,SYSTEMUNITS);
	double dP1Distance		=cP1.DistanceTo(&cCrossingPoint,SYSTEMUNITS);

	// Compute the percent interpolate the time.
	double dPercent=dP1Distance/dTotalDistance;
	if( dPercent<0.0 || dPercent>1.0 ) return false;

	CTimeSpan cIncrement=cP2.m_cTime-cP1.m_cTime;
	int iIncrementSec=(int)cIncrement.GetTotalSeconds();
	int	   nSeconds=(int)Roundoff((((double)iIncrementSec)*dPercent));
	cTime=cP1.m_cTime+CTimeSpan(nSeconds);

	return true;

	}

int CGate::CrossingAltitude(CPosition &cP1, CPosition &cP2 )
	{
	CLocation cCrossingPoint;

	if( !GetCrossingLocation(cP1, cP2, cCrossingPoint) ) return cP1.m_iPressureAltitude;

	double dTotalDistance	=cP1.DistanceTo(&cP2,SYSTEMUNITS);
	double dP1Distance		=cP1.DistanceTo(&cCrossingPoint,SYSTEMUNITS);

	// Compute the percent interpolate the time.
	double dPercent=dP1Distance/dTotalDistance;
	if( dPercent<=0.0 ) return cP1.m_iPressureAltitude;
    if( dPercent>=1.0 ) return cP2.m_iPressureAltitude;

	int iDelta=cP2.m_iPressureAltitude-cP1.m_iPressureAltitude;
	int	nFeet=(int)Roundoff((((double)iDelta)*dPercent));

	return cP1.m_iPressureAltitude+nFeet;
	}



CGate::~CGate()
{

}



void CGate::GetEnds(float &fLat1, float &fLong1, float &fLat2, float &fLong2, double dCorr)
	{
	CLocation cEnd1(&m_cCenter);
	cEnd1.Move(m_dRadius, ((double)(m_dHeading-90))*TORAD, dCorr );
	fLat1=(float)cEnd1.GetLat();
	fLong1=(float)cEnd1.GetLong();

	CLocation cEnd2(&m_cCenter);
	cEnd2.Move( m_dRadius, ((double)(m_dHeading+90))*TORAD, dCorr );
	fLat2=(float)cEnd2.GetLat();
	fLong2=(float)cEnd2.GetLong();
	}


void CGate::Serialize(CArchive & ar)
{
	short s;
	if (ar.IsStoring())
		{
		ar << (short)3;// Current Version
		ar << (short)m_bFirstLegAtExit;
		ar << m_iGatePointID;
		ar << m_dOrientation;
		ar << m_dRadius;
		ar << (short)m_eGateType;
		ar << m_iBase;
		ar << m_iHeight;
		ar << m_dHeading;
		ar << (short)m_bPerpToCourse;
		}
	else
		{
		ar >> s;
		int nVersion=s;
		int iTemp;

		if( nVersion<=1 )
			{
			ar >> s;
			ar >> m_iGatePointID;
			ar >> m_dOrientation;
			ar >> m_dRadius;
			ar >> s;
			m_eGateType=(EGateType)s;
			ar >> m_iBase;
			ar >> m_iHeight;
			ar >> iTemp;
			m_dHeading=(double)iTemp;
			m_bFirstLegAtExit=false;
			}
		else if( nVersion==2 )
			{
			ar >> s;
			ar >> m_iGatePointID;
			ar >> m_dOrientation;
			ar >> m_dRadius;
			ar >> s;
			m_eGateType=(EGateType)s;
			ar >> m_iBase;
			ar >> m_iHeight;
			ar >> m_dHeading;
			ar >> s;
			m_bPerpToCourse=(s==0)?(false):(true);
			m_bFirstLegAtExit=false;
			}
		else
			{
			ar >> s;
			m_bFirstLegAtExit=(s==0)?(false):(true);
			ar >> m_iGatePointID;
			ar >> m_dOrientation;
			ar >> m_dRadius;
			ar >> s;
			m_eGateType=(EGateType)s;
			ar >> m_iBase;
			ar >> m_iHeight;
			ar >> m_dHeading;
			ar >> s;
			m_bPerpToCourse=(s==0)?(false):(true);
			}
		}
}

CGate& CGate::operator =(CGate &cGate)
	{
	m_iGatePointID	 =cGate.m_iGatePointID;
	m_dOrientation	 =cGate.m_dOrientation;
	m_dRadius		 =cGate.m_dRadius;
	m_eGateType		 =cGate.m_eGateType;
	m_iBase			 =cGate.m_iBase;
	m_iHeight		 =cGate.m_iHeight;
	m_dHeading		 =cGate.m_dHeading;
	m_bFirstLegAtExit=cGate.m_bFirstLegAtExit;
	m_bPerpToCourse	 =cGate.m_bPerpToCourse;
	m_bScoreOutTheTop=cGate.m_bScoreOutTheTop;
	return *this;
	}

CString CGate::GetText(EUnits eUnits)
{
	CString cFDRText=_T("");
	if(IsGPSCylinder())
    	cFDRText.Format(_T("(-%4.1lf Gate Radius)"), ConvertDistance(GetRadius(),SYSTEMUNITS, eUnits));

	return cFDRText;

}

CString CGate::GetDescription(EUnits eUnits)
{
	CString str;

	if( GetPointID()<0 ) 
		return _T("Gate not defined");

	double dDist=ConvertDistance(GetRadius(),SYSTEMUNITS, eUnits);
	if( m_eGateType==eLine )
		dDist*=2.;

   	str.Format(_T("%s, %3.1lf %s %s, Center at point %d"), 
				(m_eGateType==eCylinder)?(_T("Cylinder")):(_T("Line")),
				dDist,
				UnitsText(eUnits),
				(m_eGateType==eCylinder)?(_T("Radius")):(_T("Wide")),
				GetPointID() );

	return str;
}

bool CGate::ScoreOutTheTop()
	{
	return m_bScoreOutTheTop;
	}

bool CGate::IsPerpToCourse()
	{
	return m_bPerpToCourse && m_eGateType==eLine;
	}


bool CGate::GetCrossingLocation(CLocation &cP1, CLocation &cP2, CLocation &cLoc)
	{
	return CLocation::GetCrossingLocation(m_cEnd1, m_cEnd2, cP1, cP2, cLoc);
	}

CString CGate::GetIDString()
{
	CString cStr;
	cStr.Format(_T("%d"),GetPointID());
	return cStr;
}

bool CGate::GetSSAXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode, EUnits eUnits, void* pTPArray)
{
    MSXML2::IXMLDOMNodePtr DOMContestantPtr;
    VARIANT vtTemp;
    vtTemp.vt=VT_I2;
    vtTemp.iVal = MSXML2::NODE_ELEMENT;


	cMgr.CreateElementInt( pParentNode, _T("ID"), m_iGatePointID);
	CTurnpoint* pTP=((CTurnpointArray*)pTPArray)->GetAt(m_iGatePointID-1);

	cMgr.CreateElement( pParentNode, _T("Name"), pTP->m_strName);
	cMgr.CreateElement( pParentNode, _T("Type"), GetGateType()==eLine?_T("Line"):_T("Cylinder"));

	cMgr.CreateElementIntC( pParentNode, _T("Base"),	m_iBase);
	cMgr.CreateElementIntC( pParentNode, _T("Height"), m_iHeight);

	if( GetGateType()==eLine )
		{
		cMgr.CreateElementInt( pParentNode, _T("Heading"), (int)m_dHeading);
		}
	else
		{
		CString strRadius;
   		strRadius.Format(_T("%4.1lf"), ConvertDistance(GetRadius(),SYSTEMUNITS, eUnits));
		cMgr.CreateElement( pParentNode,  _T("Radius"), strRadius);
		}

	return true;
	}

bool CGate::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
{

	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode,pIDOMChildNode,_T("Gate") );

	cMgr.CreateElementInt( pIDOMChildNode, _T("PointID"), m_iGatePointID);
	cMgr.CreateElementInt( pIDOMChildNode, _T("Type"), m_eGateType);
	cMgr.CreateElementInt( pIDOMChildNode, _T("FirstLegAtExit"),  m_bFirstLegAtExit);
	cMgr.CreateElementInt( pIDOMChildNode, _T("PerpToCourse"),  m_bPerpToCourse);
	cMgr.CreateElementInt( pIDOMChildNode, _T("ScoreOutTheTop"),  m_bScoreOutTheTop);

	cMgr.CreateElementIntC( pIDOMChildNode, _T("Base"),	m_iBase);
	cMgr.CreateElementIntC( pIDOMChildNode, _T("Height"), m_iHeight);

	cMgr.CreateElementDbl ( pIDOMChildNode, _T("Radius"), m_dRadius);
	cMgr.CreateElementDblC( pIDOMChildNode, _T("Heading"), m_dHeading);

	return true;
	}

bool CGate::FromXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParent)
	{
	MSXML2::IXMLDOMNodePtr pGate;
	TESTHR(cMgr.SelectChild(_T("Gate"),pParent,pGate));
	if( pGate==NULL ) return false;


	GET_XML_INT( cMgr, pGate, _T("PointID"), int, m_iGatePointID, -1);
	GET_XML_INT( cMgr, pGate, _T("Type"), EGateType, m_eGateType, eCylinder);
	GET_XML_BOOL( cMgr, pGate, _T("FirstLegAtExit"), m_bFirstLegAtExit, false);
	GET_XML_BOOL( cMgr, pGate, _T("PerpToCourse"), m_bPerpToCourse, false);
	GET_XML_BOOL( cMgr, pGate, _T("ScoreOutTheTop"), m_bScoreOutTheTop, true);

	GET_XML_INT( cMgr, pGate, _T("Base"), int, m_iBase, 0);
	GET_XML_INT( cMgr, pGate, _T("Height"), int, m_iHeight, 5000);


	GET_XML_DBL ( cMgr, pGate, _T("Radius"), double, m_dRadius, ConvertDistance(5,eStatute,SYSTEMUNITS));
	GET_XML_DBL ( cMgr, pGate, _T("Heading"), double, m_dHeading, 0);

	return true;
	}
