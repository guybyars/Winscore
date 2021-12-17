//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TraceGroup.h: interface for the CTraceGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "trace.h"
#include "winscoredefs.h"
#include "hitlist.h"

#if !defined(AFX_TRACEGROUP_H__B8BD8D81_584E_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_TRACEGROUP_H__B8BD8D81_584E_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTraceGroup  
{
public:
	Winscore::CTrace* GetTrace(CString strContestNo);
	Winscore::CTrace* GetTrace(int i);
	BOOL AnyoneHighlighted(bool bClear=true);
	int  GetHighlightedContestant();
	int GetClosestPilot( CPoint &point, short sRadius);
	void Highlight(CString strContestID);
	void Highlight(int iPID, CDC* pDC);
	void SetDisplayLength(long lLength);
	int GetNumFixes(CString strCID);
	void Purge();
	long m_lTraceDisplayLength;
	bool m_bPointDisplay;
	bool IsPilotVisible(CString strCID);
	void SetPilotVisible(CString strCID, bool bVisible);
	void SetPilotColor(CString strContestNo, long lColor);
	short GetPilotIndex(CString cContestNo);
	void AddTimes(short  sPilotID, CTime &cStartTime, CTime &cFinishTime);
	void Invalidate();
	void Draw(CDC* pDC, C3DTransformation &c3DTransformation, CTime &cStart, CTime &cEnd, EClass eActiveClass, short sAltMax, short sAltMin, CHitList &cHitList );
	void Draw(CDC* pDC, C3DTransformation &c3DTransformation, short sTimeIndex1,  short sTimeIndex2, short sAltMax, short sAltMin, CHitList &cHitList );
	bool AddFix( short sPilotIndex, CFix &cFix);
	int AddTrace( Winscore::CTrace &cTrace);

	CTraceGroup();
	virtual ~CTraceGroup();

	CPtrArray m_cPtrArray;

};

#endif // !defined(AFX_TRACEGROUP_H__B8BD8D81_584E_11D3_BA96_0010A4043281__INCLUDED_)

