//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Cursor.cpp: implementation of the CCursor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsview.h"
#include "Cursor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCursor::CCursor( CRect &cPlotWindow, C2DTransform &c2DTransform ):
	m_c2DTransform(c2DTransform),
	m_cPlotWindow(cPlotWindow)

{
	m_pcCursorPen=new CPen();
	m_pcCursorPen->CreatePen( PS_SOLID, 2, RGB(255, 255, 0) );
	m_lMaxXLimit=0;
	m_lMinXLimit=0;
}

CCursor::~CCursor()
{
	m_pcCursorPen->DeleteObject();
	delete m_pcCursorPen;

}

void CCursor::DrawCursor(CDC *pdc)
{
	CPen* pOldPen = pdc->SelectObject( m_pcCursorPen );

	pdc->SetROP2( R2_XORPEN );

	short sX=m_c2DTransform.WorldXToScreen(m_lLocation);
	sX=(short)min(sX,m_cPlotWindow.right-2 );
	sX=(short)max(sX,m_cPlotWindow.left+2 );

	pdc->MoveTo( sX, m_cPlotWindow.bottom );
	pdc->LineTo( sX, m_cPlotWindow.top );

	pdc->SelectObject( pOldPen );
}

void CCursor::SetPlotWindow(CRect &cRect)
	{
	m_cPlotWindow=cRect;
	}

short CCursor::MoveCursorTo(CDC *pdc, short sScreenLocation)
	{

	DrawCursor(pdc);
	short sNewScreenLocation=sScreenLocation;
	sNewScreenLocation=max(sNewScreenLocation, (short)m_cPlotWindow.left);
	sNewScreenLocation=min(sNewScreenLocation, (short)m_cPlotWindow.right);
	m_lLocation=m_c2DTransform.ScreenXToWorld(sNewScreenLocation);
	if( m_lMaxXLimit>0 )
		m_lLocation=min(m_lLocation, m_lMaxXLimit);
	DrawCursor(pdc);
	return m_c2DTransform.WorldXToScreen(m_lLocation);
	
	}

long  CCursor::MoveCursorInc(CDC *pdc, long iInc)
{
	DrawCursor(pdc);
	m_lLocation+=iInc;
	if( m_lMaxXLimit>0 )
		m_lLocation=min(m_lLocation, m_lMaxXLimit);
	if( m_lMinXLimit>0 )
		m_lLocation=max(m_lLocation, m_lMinXLimit);
	DrawCursor(pdc);
	return m_lLocation;
}


void CCursor::IncreaseMaxXLimit(long lTime)
	{
	m_lMaxXLimit=max(m_lMaxXLimit,lTime);
	}

long CCursor::MoveCursorTo(CDC* pdc, CTime &cTime)
{
	DrawCursor(pdc);
	m_lLocation=(long)cTime.GetTime();
	if( m_lMaxXLimit>0 )
		m_lLocation=min(m_lLocation, m_lMaxXLimit);
	if( m_lMinXLimit>0 )
		m_lLocation=max(m_lLocation, m_lMinXLimit);

	DrawCursor(pdc);
	return m_lLocation;
}

bool CCursor::IsCursorAtEnd()
	{
	return m_lLocation>=m_lMaxXLimit;
	}

void CCursor::SetTimeRange(long lStartTime, long lEndTime)
	{
//	m_lMaxXLimit=lEndTime;
	m_lMinXLimit=lStartTime;
	}

