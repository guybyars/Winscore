//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// 2DTransform.cpp: implementation of the C2DTransform class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsview.h"
#include "2DTransform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DTransform::C2DTransform()
	{
	m_fYScale=1;
	m_fXScale=1;

	m_cPlotWindow=CRect(0,0,0,0);
	m_cDataWindow=CRect(0,0,0,0);
	}

C2DTransform::~C2DTransform()
	{

	}

short C2DTransform::WorldXToScreen(int X)
	{
	int sX=m_cPlotWindow.left+(int)(m_fXScale*(float)(X-m_cDataWindow.left));
	sX=min(sX,(short)m_cPlotWindow.right);
	sX=max(sX,(short)m_cPlotWindow.left);
	return sX;
	}

short C2DTransform::WorldYToScreen(int Y)
	{
	short sY=(short)(m_cPlotWindow.bottom+(int)(m_fYScale*(float)(Y-m_cDataWindow.bottom)));
	sY=max(sY,(short)m_cPlotWindow.top);
	sY=min(sY,(short)m_cPlotWindow.bottom);
	return sY;
	}

int	C2DTransform::ScreenXToWorld(int X)
	{
	return int((float)(X-m_cPlotWindow.left)/m_fXScale)+m_cDataWindow.left;
	}

int	C2DTransform::ScreenYToWorld(int Y)
	{
	return 0;
	}

void C2DTransform::Initialize(CRect &cPlotWindow, CRect &cDataWindow)
	{
	m_cPlotWindow=cPlotWindow;
	m_cDataWindow=cDataWindow;

	m_fYScale	=(float)cPlotWindow.Height()/(float)cDataWindow.Height();
	m_fXScale	=(float)cPlotWindow.Width()/(float)cDataWindow.Width();
	}

