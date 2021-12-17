//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroTrace.h: interface for the CBaroTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAROTRACE_H__4913434B_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)
#define AFX_BAROTRACE_H__4913434B_1CE2_11D4_B1D2_54F85E000000__INCLUDED_

#include "baropoint.h"
#include "2dtransform.h"
#include "winscoredefs.h"
#include <afxmt.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GPSTRACECOLOR RGB(0,0,255)
#define PRESSTRACECOLOR RGB(255,0,0)
#define ENLTRACECOLOR RGB(0,155,0)

#define PRESSURETRACE	0x01
#define GPSTRACE		0x02
#define ENLTRACE		0x04

class CBaroTrace  
{
public:
	long GetClosestPoint(CTime &cTime);
	long GetPrevPoint(CTime cTime);
	CBaroPoint* GetMinAltitudeInRange(long lStart, long lEnd, short sType);
	CBaroPoint* GetMaxAltitudeInRange(long lStart, long lEnd, short sType);
	CBaroPoint* GetBaroPoint(CTime &cTime);
	long GetNextPoint(CTime cTime);

//	void GetAltitudes(CTime cTime, short &sGPSAlt, short &sPressAlt );

	CPen* m_pPen;

	CSemaphore m_cSemaphore;

	void DrawTrace( CDC* pdc, C2DTransform &c2DTransform, bool bRefresh, short iDrawMask, EUnits eUnits, EAltUnits eAltUnits);

	void AddPoint( CBaroPoint& cPoint);
	CBaroTrace(  CString strContestNo, CString strLastName, CString strFirstName, short sClass, long lColorREF );
	CBaroTrace();

	virtual ~CBaroTrace();

	void SetColor(COLORREF lColor);

	CString		m_strLastName;
	CString		m_strFirstName;
	CString		m_strContestNo;

	short		m_sMinAltitude;
	short		m_sMaxAltitude;

	bool		m_bVisible;
	short		m_sClass;

	CBaroPoint*	m_pLastPoint;
	POSITION	m_LastDrawnPosition;

	unsigned char		m_R, m_G, m_B;

private:

	CPtrList m_cPtrList;

};

#endif // !defined(AFX_BAROTRACE_H__4913434B_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)

