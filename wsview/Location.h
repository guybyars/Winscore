//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Location.h: interface for the CLocation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCATION_H__7CBA71A2_3B0C_11D1_ACC4_000000000000__INCLUDED_)
#define AFX_LOCATION_H__7CBA71A2_3B0C_11D1_ACC4_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "winscoredefs.h"

#ifndef _WINDLL
#include "xmlmgr.h"
#endif

enum ELatDirection { eNORTH, eSOUTH } ;
enum ELongDirection { eEAST, eWEST } ;

// 3958.7559 miles or 6371.0 Kilometers

#define	  MIPERDEGREE	69.09332474
#define   KMPERDEGREE   111.1949267
#define   NMPERDEGREE   60.


class CLocation : public CObject
{

protected:

#ifndef _CAIEXPLR
	DECLARE_SERIAL(CLocation)
#endif

public:
#ifndef _WINDLL
	void ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pFlightNode, CString strChild);
	void FromXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pLatLong);
	void GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode);
#endif
	bool IsNull();
    CLocation();
    CLocation(double, double);
	CLocation& operator =(CLocation&);
	double DistanceTo(CLocation cLoc, EUnits eUnits);

	CString GetFormattedLong();
	CString GetFormattedLat();
	double CourseTo(CLocation& cLoc );
	CString GetFormattedInfo();
	bool CLocation::operator ==(CLocation&);

#ifndef _CAIEXPLR
	virtual void Serialize(CArchive& ar);
#endif

	double m_dLatMinutes, m_dLongMinutes;
	int m_iLatDegrees, m_iLongDegrees;
	ELatDirection m_eLatDirection;
	ELongDirection m_eLongDirection;

	double GetLat();
	double GetLong();
	void SetLat(double);
	void SetLong(double);
	void Move( double dDist, double dBearing, double dCorr=1.0);

	static bool GetCrossingLocation(CLocation &cEnd1, CLocation &cEnd2, CLocation &cP1, CLocation &cP2, CLocation &cLoc);

	CLocation( CString strloc);
    CLocation(CLocation*);
    CLocation(CLocation&);
	CLocation(	int iLatDeg, double dLatMin, int iLatSec,
				int iLongDeg, double dLongMin, int iLongSec,
				ELatDirection eLatDirection, 
				ELongDirection eLongDirection);

	CLocation(	int iLatDeg, double dLatMin, 
				int iLongDeg, double dLongMin, 
				ELatDirection eLatDirection, 
				ELongDirection eLongDirection);
	virtual ~CLocation();

	int  CLocation::GetLatSeconds();
	int  CLocation::GetLongSeconds();
	int  CLocation::GetLatMinutes();
	int  CLocation::GetLongMinutes();
};

double ConvertDistance(double dDist, EUnits eUnitsFrom, EUnits eUnitsTo );

#endif // !defined(AFX_LOCATION_H__7CBA71A2_3B0C_11D1_ACC4_000000000000__INCLUDED_)


