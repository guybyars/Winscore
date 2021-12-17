//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Location.cpp: implementation of the CLocation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifndef _CAIEXPLR
#include "Winscore.h"
#endif
#include "Winscoredefs.h"

#ifndef _WINDLL
#include "global_prototypes.h"
#endif

#include "Location.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static EDatum eDistDatum=DEFAULTDATUM;

static double Vincenty_Distance(const double& latitude_01, const double& longitude_01,
	const double& latitude_02, const double& longitude_02,
	const double& a,
	const double& b);

static   double mod(double y,double x);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifndef _CAIEXPLR
IMPLEMENT_SERIAL(CLocation, CObject, DB_SCHEMA_ID)
#endif

CLocation::CLocation()
{
	m_dLatMinutes=0.00;
	m_dLongMinutes=0.00;
	m_iLatDegrees=0;
	m_iLongDegrees=0;
	m_eLatDirection=eNORTH;
	m_eLongDirection=eWEST;	
}

void CLocation::Move( double dDist, double dBearing, double dCorr)
	{
	double dLatOld=GetLat();
	double dLongOld=GetLong();
	double dLa=(dDist/60.)*cos(dBearing);
	double dLo=(dDist/60.)*sin(dBearing);
	double dLatNew=dLatOld+dLa;
	double dLonNew=dLongOld+(dLo*dCorr);
	SetLat(dLatNew);
	SetLong(dLonNew);
	}

void CLocation::SetDatum(EDatum eDatum)
	{
	eDistDatum=eDatum;
	}

void CLocation::SetLat(double dLatNew)
	{
	double dDegrees=0;
	m_dLatMinutes=fabs(modf( dLatNew, &dDegrees)*60.);
	m_iLatDegrees=(int)fabs(dDegrees);
	m_eLatDirection=(dLatNew>0)?(eNORTH):(eSOUTH);
	}

void CLocation::SetLong(double dLongNew)
	{
	double dDegrees=0;
	m_dLongMinutes=fabs(modf( dLongNew, &dDegrees)*60.);
	m_iLongDegrees=(int)fabs(dDegrees);
	m_eLongDirection=(dLongNew>0)?(eEAST):(eWEST);
	}

double CLocation::GetLat()
	{
	double dLat=m_iLatDegrees + m_dLatMinutes/60.;
	if( m_eLatDirection==eSOUTH )
		return -dLat;
	else
		return dLat;
	}

double CLocation::GetLong()
	{
	double dLong= m_iLongDegrees + m_dLongMinutes/60.;
	if( m_eLongDirection==eWEST )
		return -dLong;
	else
		return dLong;
	}


CLocation::CLocation(double dLat, double dLong)
	{
	SetLat(dLat);
	SetLong(dLong);
	}


CLocation::CLocation(CLocation* pcLocationItem)
	{
	m_dLatMinutes=pcLocationItem->m_dLatMinutes;
	m_dLongMinutes=pcLocationItem->m_dLongMinutes;
	m_iLatDegrees=pcLocationItem->m_iLatDegrees;
	m_iLongDegrees=pcLocationItem->m_iLongDegrees;
	
	m_eLatDirection=pcLocationItem->m_eLatDirection;
	m_eLongDirection=pcLocationItem->m_eLongDirection;
	}

CLocation::CLocation(CLocation &cLocationItem)
	{
	m_dLatMinutes=cLocationItem.m_dLatMinutes;
	m_dLongMinutes=cLocationItem.m_dLongMinutes;
	m_iLatDegrees=cLocationItem.m_iLatDegrees;
	m_iLongDegrees=cLocationItem.m_iLongDegrees;
	
	m_eLatDirection=cLocationItem.m_eLatDirection;
	m_eLongDirection=cLocationItem.m_eLongDirection;
	}

CLocation::CLocation(CString strloc)
	{
	// This string is of the format
	// Specified in the form of decimal degrees.  A decimal point is 
	// mandatory and must be preceded and followed by at least one digit; 
	// otherwise, leading and trailing zeroes are optional.  A leading "-" 
	// character indicates a negative value (denoting south latitude or west longitude).

	int iCommaLoc=strloc.Find(_T(","));
	double dLat, dLong, dDegrees;

	CString strLat=strloc.Left( iCommaLoc );
	CString strLong=strloc.Right( strloc.GetLength()-iCommaLoc-1 );

	if(  sscanf_s(LPCTSTR(strLat), _T("%lf"), &dLat)!=1 )
		{
		// error;
		}
	m_dLatMinutes=fabs(modf( dLat, &dDegrees)*60.);
	m_iLatDegrees=(int)fabs(dDegrees);
	m_dLatMinutes=(double)fabs(m_dLatMinutes);
	m_eLatDirection=(dDegrees>0)?(eNORTH):(eSOUTH);

	if(  sscanf_s(LPCTSTR(strLong), _T("%lf"), &dLong)!=1 )
		{
		// error;
		}
	m_dLongMinutes=fabs(modf( dLong, &dDegrees)*60.);
	m_iLongDegrees=(int)fabs(dDegrees);
	m_dLongMinutes=(double)fabs(m_dLongMinutes);

	m_eLongDirection=(dDegrees>0)?(eEAST):(eWEST);

	}

CLocation::CLocation(int iLatDeg, double dLatMin, int iLatSec,
					 int iLongDeg, double dLongMin, int iLongSec,
					 ELatDirection eLatDirection, 
					 ELongDirection eLongDirection)
	{
	m_dLatMinutes=dLatMin + (double)iLatSec/60.;
	m_dLongMinutes=dLongMin +(double)iLongSec/60.;
	m_iLatDegrees=iLatDeg;
	m_iLongDegrees=iLongDeg;
	
	m_eLatDirection=eLatDirection;
	m_eLongDirection=eLongDirection;
	};

CLocation::CLocation(int iLatDeg, double dLatMin, 
					 int iLongDeg, double dLongMin, 
					 ELatDirection eLatDirection, 
					 ELongDirection eLongDirection)
	{
	m_dLatMinutes=dLatMin;
	m_dLongMinutes=dLongMin;
	m_iLatDegrees=iLatDeg;
	m_iLongDegrees=iLongDeg;
	
	m_eLatDirection=eLatDirection;
	m_eLongDirection=eLongDirection;
	};

CLocation::~CLocation()
{
}

int  CLocation::GetLatSeconds()
	{
	double dMinutes;
	int iSecLat=(int)((modf( m_dLatMinutes, &dMinutes) )*60.000001);
	return iSecLat;
	}

int  CLocation::GetLongSeconds()
	{
	double dMinutes;
	int iSecLong=(int)(( modf( m_dLongMinutes, &dMinutes) )*60.000001);
	return iSecLong;
	}

int CLocation::GetLatMinutes()
	{
	double dMinutes;
	modf( m_dLatMinutes, &dMinutes);
	return (int)dMinutes;
	}

int  CLocation::GetLongMinutes()
	{
	double dMinutes;
	modf( m_dLongMinutes, &dMinutes);
	return (int)dMinutes;
	}


CLocation& CLocation::operator =(CLocation& cLocationItem)
{
	m_dLatMinutes=cLocationItem.m_dLatMinutes;
	m_dLongMinutes=cLocationItem.m_dLongMinutes;
	m_iLatDegrees=cLocationItem.m_iLatDegrees;
	m_iLongDegrees=cLocationItem.m_iLongDegrees;
	
	m_eLatDirection=cLocationItem.m_eLatDirection;
	m_eLongDirection=cLocationItem.m_eLongDirection;
	return *this;
}
double CLocation::DistanceTo(CLocation cLocA, EUnits eUnits )
{
const double a = 6378137.0;
const double b = 6356752.314245;


double LatA	=TORAD*(cLocA.m_iLatDegrees +(cLocA.m_dLatMinutes/60.));
if( cLocA.m_eLatDirection==eSOUTH ) LatA= -LatA;

double LongA=TORAD*(cLocA.m_iLongDegrees+(cLocA.m_dLongMinutes/60.));
if( cLocA.m_eLongDirection==eWEST ) LongA= -LongA;

double LatB	=TORAD*(m_iLatDegrees +(m_dLatMinutes/60.));
if( m_eLatDirection==eSOUTH ) LatB= -LatB;

double LongB=TORAD*(m_iLongDegrees+(m_dLongMinutes/60.));
if( m_eLongDirection==eWEST ) LongB= -LongB;

if( eDistDatum==eWGS84 ) 
	{
	double dDist=Vincenty_Distance(LatA, LongA,	LatB, LongB, a, b);

	dDist=dDist/1000.; //convert from m to KM.

	return ConvertDistance(dDist, eKilometers, eUnits);
	}
else
	{

// *** METHOD I FROM THE SPORTING CODE
//Cos Alpha = (Cos LatA x Cos LatB x Cos (LongA - LongB)) + (Sin LatA x Sin LatB)
//double Alpha =  cos(LatA) * cos(LatB) * cos(LongA-LongB)+(sin(LatA) * sin(LatB));
//double dDist=TODEG*(acos(Alpha));

// *** METHOD II FROM THE SPORTING CODE
//Sin Alpha/2 =   Square root of the following expression:
//Cos LatA x Cos LatB x Sin**2((LongA - LongB)/2) + Sin**2((LatA - LatB)/2)

double Alpha=2.*asin(sqrt( cos(LatA)*cos(LatB) * pow(sin((LongA-LongB)/2.),2) + pow( sin((LatA-LatB)/2.) ,2) ));
double dDist=TODEG*Alpha;

if (eUnits==eStatute)
	dDist*=(MIPERDEGREE);
else if( eUnits==eKilometers )
	dDist*=(KMPERDEGREE);
else if( eUnits==eNautical )
	dDist*=(NMPERDEGREE);

return dDist;
}

return 0.0;

}

double ConvertDistance(double dDist, EUnits eUnitsFrom, EUnits eUnitsTo )
	{
	if( eUnitsFrom==eUnitsTo ) return dDist;

//  Convert to Nautical(degrees), then convert to desired

	if( eUnitsFrom==eKilometers ) dDist*=(NMPERDEGREE/KMPERDEGREE);
	else if( eUnitsFrom==eStatute ) dDist*=(NMPERDEGREE/MIPERDEGREE);

	if( eUnitsTo==eKilometers ) dDist*=KMPERDEGREE/NMPERDEGREE;
	else if( eUnitsTo==eStatute ) dDist*=MIPERDEGREE/NMPERDEGREE;

	return dDist;

}


#ifndef _CAIEXPLR

void CLocation::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_dLatMinutes;
		ar << m_dLongMinutes;
		ar << m_iLatDegrees;
		ar << m_iLongDegrees;
		ar << (WORD)m_eLatDirection;
		ar << (WORD)m_eLongDirection;
	}
	else
	{
		WORD w;
		ar >> m_dLatMinutes;
		ar >> m_dLongMinutes;
		ar >> m_iLatDegrees;
		ar >> m_iLongDegrees;
		ar >> w;
		m_eLatDirection=(ELatDirection)w;
		ar >> w;
		m_eLongDirection=(ELongDirection)w;
	}

}

#endif

CString CLocation::GetFormattedInfo()
	{
	CString cTemp1, strLat, strLon;
	(m_eLatDirection==eNORTH ) ? (strLat=_T("N")) : (strLat=_T("S"));
	(m_eLongDirection==eWEST ) ? (strLon=_T("W")) : (strLon=_T("E"));

	cTemp1.Format(_T("%2d  %06.3lf %s  %2d  %06.3lf %s"), 
		m_iLatDegrees,
		m_dLatMinutes, 
		strLat, 
		m_iLongDegrees, 
		m_dLongMinutes,
		strLon);
	return cTemp1;
	}


double CLocation::CourseTo(CLocation& cLoc)
{

// Compute course from this location to location cLoc.

double lat1	=TORAD*(m_iLatDegrees +(m_dLatMinutes/60.));
if( m_eLatDirection==eSOUTH ) lat1= -lat1;

double lon1=TORAD*(m_iLongDegrees+(m_dLongMinutes/60.));
if( m_eLongDirection==eEAST ) lon1= -lon1;

double lat2	=TORAD*(cLoc.m_iLatDegrees +(cLoc.m_dLatMinutes/60.));
if( cLoc.m_eLatDirection==eSOUTH ) lat2= -lat2;

double lon2=TORAD*(cLoc.m_iLongDegrees+(cLoc.m_dLongMinutes/60.));
if( cLoc.m_eLongDirection==eEAST ) lon2= -lon2;

double tc1;

// http://williams.best.vwh.net/avform.htm#Crs

//The initial course, tc1, (at point 1) from point 1 to point 2 is given by:

// tc1=mod(atan2(sin(lon1-lon2)*cos(lat2),
//        cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2*pi)

tc1=mod(atan2(sin(lon1-lon2)*cos(lat2),
        cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon1-lon2)), 2*PI);

tc1*=TODEG;

return tc1;
}


static   double mod(double y,double x)
{
	double mod;
  if( y>=0.)
    mod=y- x*(int)(y/x);
  else
    mod=y+ x*((int)(-y/x)+1);
  return mod;
}

bool CLocation::operator ==(CLocation &cLocation)
{
	return( 	m_dLatMinutes		==cLocation.m_dLatMinutes	&&
				m_dLongMinutes		==cLocation.m_dLongMinutes	&&
				m_iLatDegrees		==cLocation.m_iLatDegrees	&&
				m_iLongDegrees		==cLocation.m_iLongDegrees	&&
				m_eLatDirection		==cLocation.m_eLatDirection	&&
				m_eLongDirection	==cLocation.m_eLongDirection );
}

CString CLocation::GetFormattedLat()
{
	CString cTemp1, strLat;
	(m_eLatDirection==eNORTH ) ? (strLat=_T("N")) : (strLat=_T("S"));

	cTemp1.Format(_T("%2d  %06.3lf %s"), 
		m_iLatDegrees,
		m_dLatMinutes, 
		strLat);
	return cTemp1;
}

CString CLocation::GetFormattedLong()
{
	CString cTemp1, strLon;
	(m_eLongDirection==eWEST ) ? (strLon=_T("W") ) : (strLon=_T("E"));

	cTemp1.Format(_T("%2d  %06.3lf %s"), 
		m_iLongDegrees, 
		m_dLongMinutes,
		strLon);
	return cTemp1;
}


bool CLocation::GetCrossingLocation(CLocation &cEnd1, CLocation &cEnd2, CLocation &cP1, CLocation &cP2, CLocation &cLoc)
	{

	//http://www.ncgia.ucsb.edu/~good/176b/p08.html#SEC32.3.3

	double x1=cEnd1.GetLong();
	double y1=cEnd1.GetLat(); 
	double x2=cEnd2.GetLong();
	double y2=cEnd2.GetLat();  
	double u1=cP1.GetLong();
	double v1=cP1.GetLat();
	double u2=cP2.GetLong();
	double v2=cP2.GetLat();
	double xi=0.0,yi=0.0,b1=0,b2=0,a1=0,a2=0;

	if (fabs(x1-x2)>.00000001)
		{ 
		b1 = (y2-y1)/(x2-x1); 
		if (u1!=u2)
			{
			b2 = (v2-v1)/(u2-u1); 
			a1 = y1-b1*x1; 
			a2 = v1-b2*u1; 
			if (b1==b2)
				{
				return false;
				}
			else
				{
				xi = - (a1-a2)/(b1-b2); 
				yi = a1+b1*xi; 
				}
			} 
		else
			{
			xi = u1; 
			a1 = y1-b1*x1; 
			yi = a1+b1*xi; 
			}
		}
	else
		{ 
		xi = x1; 
		if (u1!=u2) 
			{
			b2 = (v2-v1)/(u2-u1); 
			a2 = v1-b2*u1; 
			yi = a2+b2*xi; 
			}
		else
			{
			return false;
			} 
		} 
double dToler= -1.e-20;

	if(	(x1-xi)*(xi-x2)>=dToler &&
		(u1-xi)*(xi-u2)>=dToler && 
		(y1-yi)*(yi-y2)>=dToler && 
		(v1-yi)*(yi-v2)>=dToler )
		{
		CLocation cCrossingPoint(yi,xi);
		cLoc=cCrossingPoint;
		return true;
		}
	else 
		return false; // lines intersect somewhere, but don't cross in the range of interest.

	}

#ifndef _WINDLL
void CLocation::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	if( IsNull() ) return;
	TESTHR(cMgr.CreateElement(pParentNode, _T("Lat"), LPCSTR(GetFormattedLat())));
	TESTHR(cMgr.CreateElement(pParentNode, _T("Long"), LPCSTR(GetFormattedLong())));
	}

void CLocation::FromXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pLatLong)
	{
	int iRead=0;

	CString strLat;
	GET_XML_STR( cMgr, pLatLong, _T("Lat"), strLat);
	if( strLat.GetLength()>0 )
		{
		float fLat;
		iRead=sscanf_s(strLat, _T("%d %f"), &m_iLatDegrees, &fLat);
		m_dLatMinutes=fLat;
		if( iRead!=2 ) throw NULL;
		m_eLatDirection=(strLat.Find(_T("N"))>0)?(eNORTH):(eSOUTH);
		}

	CString strLong;
	GET_XML_STR( cMgr, pLatLong, _T("Long"), strLong);
	if( strLong.GetLength()>0 )
		{
		float fLong;
		iRead=sscanf_s(strLong, _T("%d %f"), &m_iLongDegrees, &fLong);
		m_dLongMinutes=fLong;
		if( iRead!=2 ) throw NULL;
		m_eLongDirection=(strLong.Find(_T("E"))>0)?(eEAST):(eWEST);
		}
	}


void CLocation::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pNode, CString strChild)
	{
	MSXML2::IXMLDOMNodePtr pChile;
	TESTHR(cMgr.SelectChild(strChild, pNode, pChile));
	if( pChile==NULL ) return;

	FromXML(cMgr, pChile);
	}
#endif

bool CLocation::IsNull()
	{
	return 	m_dLatMinutes==0 && m_iLatDegrees==0 && m_dLongMinutes==0 && m_iLongDegrees==0;
	}


/**
* Vincenty Distance
*/
double Vincenty_Distance(const double& latitude_01, const double& longitude_01,
	const double& latitude_02, const double& longitude_02,
	const double& a,
	const double& b)
{
	if( latitude_01==latitude_02 && longitude_01==longitude_02 ) return 0.0;

	// Flattening
	const double f = (a - b) / a;

	// tan U1
	const double tan_U1 = (1 - f) * std::tan(latitude_01);
	const double tan_U2 = (1 - f) * std::tan(latitude_02);

	// Longitudinal Distance
	const double cos_U1 = 1 / std::sqrt(1 + tan_U1 * tan_U1);
	const double cos_U2 = 1 / std::sqrt(1 + tan_U2 * tan_U2);
	const double sin_U1 = tan_U1 * cos_U1;
	const double sin_U2 = tan_U2 * cos_U2;

	// Iterate until complete
	const double L = longitude_02 - longitude_01;
	double lambda = L;
	double diff, sigma;
	double cos_alpha_sq, cos_2sigma_m;
	double cos_sigma, sin_sigma;
	int    ntries=0;

	while (ntries<10) {

		// 
		double sin_lambda = std::sin(lambda);
		double cos_lambda = std::cos(lambda);

		double c1 = (cos_U2 * sin_lambda)*(cos_U2 * sin_lambda);
		double c2 = (cos_U1 * sin_U2);
		double c3 = (sin_U1 * cos_U2 * cos_lambda);


		//  sin sigma
		sin_sigma = std::sqrt(c1 + (c2 - c3)*(c2 - c3));

		// cos sigma
		cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lambda;

		// sigma
		sigma = std::atan2(sin_sigma, cos_sigma);

		// sin alpha
		double sin_alpha = (cos_U1 * cos_U2 * sin_lambda) / (sin_sigma);

		// cos^2 alpha
		cos_alpha_sq = 1 - (sin_alpha*sin_alpha);

		// cos^2 2sigmam
		cos_2sigma_m = cos_sigma - (2 * sin_U1 * sin_U2) / (cos_alpha_sq);

		// C
		double C = (f / 16.0) * cos_alpha_sq * (4 + f * (4 - 3 * cos_alpha_sq));

		// Update Lambda
		diff = lambda;
		lambda = L + (1 - C) * f * sin_alpha * (sigma + C * sin_sigma * (cos_2sigma_m + C * cos_sigma * (-1 + 2 * cos_2sigma_m*cos_2sigma_m)));
		diff = lambda - diff;
		if (std::fabs(diff) < 0.00001) { break; }
		ntries++;
	}

	// U2
	double u_sq = cos_alpha_sq  * (a*a - b*b) / (b*b);

	// Compute A, B
	double A = 1 + (u_sq / 16384) * (4096 + u_sq * (-768 + u_sq * (320 - 175 * u_sq)));

	double B = (u_sq / 1024) * (256 + u_sq * (-128 + u_sq * (-128 + u_sq * (74 - 47 * u_sq))));

	// Sigma
	double cos_2sigma_m_sq = cos_2sigma_m * cos_2sigma_m;
	double delta_sigma = B * sin_sigma * (cos_2sigma_m + (B / 4.0) * (cos_sigma * (-1 * 2 * cos_2sigma_m_sq) - (B / 6.0) * cos_2sigma_m * (-3 + 4 * sin_sigma*sin_sigma) * (-3 + 4 * cos_2sigma_m_sq)));

	// Distance
	double s = b * A * (sigma - delta_sigma);
	return s;
}