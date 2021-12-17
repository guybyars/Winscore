//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ControlPoint.cpp: implementation of the CControlPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSView.h"
#include "ControlPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlPoint::CControlPoint( 	int iID, float fLat, float fLong, CString strName, int iAttributes, int iAltitude )
	{
	m_iID		=iID;
	m_fLat		=fLat;
	m_fLong		=fLong;
	m_strName	=strName;
	m_iAttributes=iAttributes;
	m_iAltitude	=iAltitude;
	m_strAttributeText="";
	}


CControlPoint::~CControlPoint()
{

}


void CControlPoint::UpdateAttributeText(int iTurnpoint,
										int iAirport,
										int iLandable, 
										int iStart,
										int iFinish, 
										int iHome)
{
	m_strAttributeText="";
	if( m_iAttributes & iTurnpoint	) m_strAttributeText+="T";
	if( m_iAttributes & iAirport	) m_strAttributeText+="A";
	if( m_iAttributes & iLandable	) m_strAttributeText+="L";
	if( m_iAttributes & iStart		) m_strAttributeText+="S";
	if( m_iAttributes & iFinish		) m_strAttributeText+="F";
	if( m_iAttributes & iHome		) m_strAttributeText+="H";
}
