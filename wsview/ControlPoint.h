//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ControlPoint.h: interface for the CControlPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLPOINT_H__667CBCA1_5AA9_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_CONTROLPOINT_H__667CBCA1_5AA9_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CControlPoint  
{
public:
	void UpdateAttributeText(	int iTurnpoint,
								int iAirport,
								int iLandable, 
								int iStart,
								int iFinish, 
								int iHome);

	CString m_strAttributeText;
	CControlPoint( 	int m_iID, float m_fLat, float m_fLong, CString m_strName, int m_iAttributes, int m_iAltitude );
	virtual ~CControlPoint();

	int m_iID;
	float m_fLat;
	float m_fLong;
	CString m_strName;
	int m_iAttributes;
	int m_iAltitude;

};

#endif // !defined(AFX_CONTROLPOINT_H__667CBCA1_5AA9_11D3_BA96_0010A4043281__INCLUDED_)

