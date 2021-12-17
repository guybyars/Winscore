// BaroZoom.cpp: implementation of the CBaroZoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsview.h"
#include "BaroZoom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaroZoom::CBaroZoom(CRect cWindow) :
	m_cGridWindow(cWindow)
	{
	m_lStartPoint	=0;
	m_lEndPoint		=0;
	}

CBaroZoom::~CBaroZoom()
	{

	}


void CBaroZoom::Draw(CDC &dc)
    {
	CPen cPen;
	cPen.CreatePen(PS_DASH | PS_COSMETIC, 1,RGB( 0xFF,0xFF,0xFF ));
	CPen* pOldPen = dc.SelectObject( &cPen );
	int iOld= dc.SetROP2( R2_XORPEN );

	CRect cBorder	=m_cGridWindow;
	cBorder.DeflateRect( 0,1 );
	cBorder.left	=m_lStartPoint;
	cBorder.right	=m_lEndPoint;


	dc.DrawFocusRect(cBorder);
/*
	dc.MoveTo( m_lStartPoint,		m_cGridWindow.bottom );
	dc.LineTo( m_lEndPoint,			m_cGridWindow.top );

	dc.LineTo( m_lEndPoint,			m_cGridWindow.bottom );
	dc.LineTo( m_lStartPoint,		m_cGridWindow.top );
	dc.LineTo( m_lStartPoint,		m_cGridWindow.bottom );
*/
	dc.MoveTo( m_lStartPoint,		m_cGridWindow.bottom-2 );
	dc.LineTo( m_lEndPoint,			m_cGridWindow.bottom-2 );

	dc.MoveTo( m_lStartPoint,		m_cGridWindow.top+2 );
	dc.LineTo( m_lEndPoint,			m_cGridWindow.top+2 );

//	dc.LineTo( m_cGridWindow.bottom,m_cGridWindow.top );
//	dc.LineTo( m_cGridWindow.top,	m_lEndPoint );
//	dc.LineTo( m_lEndPoint, m_cGridWindow.bottom );

	dc.SetROP2( iOld );
	dc.SelectObject( pOldPen );
    }
   
    

void CBaroZoom::SetBeginning(long sPt)
	{
	m_lStartPoint=sPt;
	}



void CBaroZoom::Cancel(CDC &dc)
	{
	Draw(dc);
	}

void CBaroZoom::OnMove(CDC &dc, CPoint &cPoint)
	{
	if( cPoint.x>m_cGridWindow.right ||
		cPoint.x<m_cGridWindow.left		) return;

	if( m_lEndPoint!=0 ) Draw(dc);

	if( cPoint.x<m_lStartPoint )
		m_lStartPoint=cPoint.x-2;
	else
		m_lEndPoint=cPoint.x;


	Draw(dc);
	}
