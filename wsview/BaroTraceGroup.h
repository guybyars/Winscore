//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroTraceGroup.h: interface for the BaroTraceGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAROTRACEGROUP_H__4913434F_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)
#define AFX_BAROTRACEGROUP_H__4913434F_1CE2_11D4_B1D2_54F85E000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "barotrace.h"
#include "2dtransform.h"

class CBaroTraceGroup  
{
public:
	CBaroTrace* IsSinglePilotVisible();
	short GetPilotIndex(CString cContestNo);
	CBaroTrace*	GetTrace(short sIndex);

	void DrawTraces( CDC* pdc, C2DTransform &c2DTransform, bool bRefresh, EUnits eUnits, EAltUnits eAltUnits, short sTrace=(ENLTRACE|GPSTRACE|PRESSURETRACE) );

	void GetDataMinMax( long lStart, long lEnd, short uDisplayMask, short &sMaxAltitude, short &sMinAltitude );
	int AddTrace(CBaroTrace* pcTrace);
	CBaroTraceGroup();
	virtual ~CBaroTraceGroup();

	CPtrArray	m_cPtrArray;

	void		Purge();

};

#endif // !defined(AFX_BAROTRACEGROUP_H__4913434F_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)

