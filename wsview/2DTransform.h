//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// 2DTransform.h: interface for the C2DTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DTRANSFORM_H__6DB0EDC8_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_)
#define AFX_2DTRANSFORM_H__6DB0EDC8_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C2DTransform  
{
public:
	void	Initialize( CRect &cPlotWindow, CRect &cDataWindow);
	short	WorldYToScreen(int Y);
	short	WorldXToScreen(int X);
	int		ScreenXToWorld(int X);
	int		ScreenYToWorld(int Y);

	C2DTransform();
	virtual ~C2DTransform();

	float	m_fYScale,	m_fXScale;

	CRect	m_cPlotWindow, m_cDataWindow;

};

#endif // !defined(AFX_2DTRANSFORM_H__6DB0EDC8_21E4_11D4_B1D4_A6F1B0000000__INCLUDED_)

