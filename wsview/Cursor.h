//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Cursor.h: interface for the CCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURSOR_H__6DB0EDC7_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_)
#define AFX_CURSOR_H__6DB0EDC7_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2dtransform.h"

class CCursor  
{
public:
	void SetTimeRange( long lStartTime, long lEndTime);
	bool IsCursorAtEnd();
	long MoveCursorTo(CDC* pdc, CTime &cTime);
	void IncreaseMaxXLimit(long lTime);
	void SetInfoWindow(CRect &cRect);
	long MoveCursorInc(CDC *pdc, long lInc);
	short MoveCursorTo( CDC *pdc, short sNewLocation );
	void SetPlotWindow(CRect &cRect);

	CRect		 &m_cPlotWindow;
	C2DTransform &m_c2DTransform;	

	long m_lLocation;
	void DrawCursor(CDC *pdc);
	long m_lMaxXLimit;
	long m_lMinXLimit;

	CPen* m_pcCursorPen;
	CCursor( CRect &cPlotWindow, C2DTransform &c2DTransform );
	virtual ~CCursor();

};

#endif // !defined(AFX_CURSOR_H__6DB0EDC7_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_)

