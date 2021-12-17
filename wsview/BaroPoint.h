//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroPoint.h: interface for the CBaroPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAROPOINT_H__4913434A_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)
#define AFX_BAROPOINT_H__4913434A_1CE2_11D4_B1D2_54F85E000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaroPoint  
{
public:
	short GetAltitude(bool bPressure);
	float m_fLat, m_fLong, m_fSpeed, m_fVSpeed;
	short m_sENL;
	short m_sGPSAltitude;
	short m_sPressure;
	CTime m_cTime;

	CBaroPoint( CTime cTime, short sGPSAltitude, short sPressureAltitude);
	CBaroPoint::CBaroPoint( CTime	cTime, 
						short	sGPSAltitude, 
						short	sPressureAltitude,
						float	fLat,
						float	fLong,
						float	fSpeed,
						float	fVSpeed,
						short	sENL	);
	CBaroPoint( CBaroPoint &cPoint );
	virtual ~CBaroPoint();

};

#endif // !defined(AFX_BAROPOINT_H__4913434A_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)

