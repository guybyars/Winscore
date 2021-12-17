// CAIWaypoint.h: interface for the CCAIWaypoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAIWAYPOINT_H__7420EAA6_175F_11D4_B1CE_20CADD000000__INCLUDED_)
#define AFX_CAIWAYPOINT_H__7420EAA6_175F_11D4_B1CE_20CADD000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>


// Constants
#define   TORAD			3.1415926535/180.
#define   TODEG			180./3.1415926535
#define	  MIPERDEGREE	69.09332474
#define   KMPERDEGREE   111.1949267
#define   NMPERDEGREE   60.

// Attribute bits
#define CWP_TURNPOINT	0x01
#define CWP_AIRPORT		0x02
#define CWP_LANDABLE	0x08
#define CWP_START		0x10
#define CWP_FINISH		0x20
#define CWP_HOME		0x40

// Length of Remarks and waypoint name
#define NAMELEN	13



class CCAIWaypoint  
{
public:
	bool IsControlPoint();
	CString GetFormattedInfo();
	double GetLong();
	double GetLat();
	void	SetLong(double dLong) {mdLongitude=dLong;};
	void	SetLat( double dLat)  {mdLatitude=dLat;};
	int GetElevation();
	CCAIWaypoint(CString stIGCLine,bool bCONVCAM=false);

	CString GetNoNameText();

// Members

	double  mdLatitude;    // negatives are in the southern hem.
	double  mdLongitude;   // negatives are in the western hem.
	float	mufElevation;
	UINT	muiAttribute;
	UINT	muiNPID;
	TCHAR	mlptstrName[NAMELEN];
	TCHAR   mlptstrRemark[NAMELEN];


// Constructors

	CCAIWaypoint() {};
	CCAIWaypoint(CCAIWaypoint &cWaypoint);
	CCAIWaypoint(	int		ID, 
					CString	strName, 
					int		iLatDegrees,
					float	fLatMinutes,
					int		iNorthSouth,
					int		iLongDegrees,
					float	fLongMinutes,
					int		iEastWest,
					int		iAttribute, 
					float	fElevation,
					CString	strRemark);

	CCAIWaypoint::CCAIWaypoint(	int		ID, 
								CString	strName, 
								float	fLat,
								float	fLong,
								int		iAttribute, 
								float	fElevation,
								CString	strRemark);

	virtual ~CCAIWaypoint();


// Methods

	int			GetID();
	CString		IdText();
	CString		LongText();
	CString		LatText();
	CString		ElevationText(float fElevationUnits);
	CString		AttributeText();
	void		LoadListItem(CListCtrl &ListCtrl, int iItem, bool bAddItem, float fElevationUnits );
	void		DistanceTo(CCAIWaypoint &cToWaypoint, double *pdDistanceInMiles, double *pdDistanceInKM, double *pdDistanceInNM );
	double		DistanceTo(CCAIWaypoint &cWay );

	BOOL		CheckAttribute(int iAtt){return muiAttribute & iAtt;}
	void		SetAttribute(int iAtt)	{muiAttribute|=iAtt;}
	BOOL		ToDat(LPBYTE lpbDatStr);

	CCAIWaypoint& operator =(CCAIWaypoint&);
	BOOL operator==(CCAIWaypoint& cWaypoint);


};

ostream &operator <<(ostream &os, CCAIWaypoint& cWay);
istream &operator >>(istream &is, CCAIWaypoint& cWay);

#endif // !defined(AFX_CAIWAYPOINT_H__7420EAA6_175F_11D4_B1CE_20CADD000000__INCLUDED_)
