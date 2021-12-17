//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroPoint.cpp: implementation of the CBaroPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSView.h"
#include "BaroPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaroPoint::CBaroPoint( CTime cTime, short sGPSAltitude, short sPressureAltitude) :
	m_sGPSAltitude(sGPSAltitude),
	m_sPressure(sPressureAltitude),
	m_cTime(cTime)
{
	m_fLat			= 0;
	m_fLong			= 0;
	m_fSpeed		= 0;
	m_fVSpeed		= 0;
	m_sENL			= 0;
}


CBaroPoint::CBaroPoint( CTime	cTime, 
						short	sGPSAltitude, 
						short	sPressureAltitude,
						float	fLat,
						float	fLong,
						float	fSpeed,
						float	fVSpeed,
						short	sENL						
						) :
	m_sGPSAltitude(sGPSAltitude),
	m_sPressure(sPressureAltitude),
	m_cTime(cTime),
	m_fLat(fLat),
	m_fLong(fLong),
	m_fSpeed(fSpeed),
	m_fVSpeed(fVSpeed),
	m_sENL(sENL)	
{
}


CBaroPoint::CBaroPoint( CBaroPoint &cPoint )
	{
	m_sGPSAltitude	= cPoint.m_sGPSAltitude;
	m_sPressure		= cPoint.m_sPressure;
	m_cTime			= cPoint.m_cTime;
	m_fLat			= cPoint.m_fLat;
	m_fLong			= cPoint.m_fLong;
	m_fSpeed		= cPoint.m_fSpeed;
	m_fVSpeed		= cPoint.m_fVSpeed;
	m_sENL			= cPoint.m_sENL;
	}


CBaroPoint::~CBaroPoint()
{
}


short CBaroPoint::GetAltitude(bool bPressure)
{
	if( bPressure ) 
		return m_sPressure;
	else
		return m_sGPSAltitude;
}
