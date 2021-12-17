//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// HitList.cpp: implementation of the CHitList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsview.h"
#include "HitList.h"
#include "controlpoint.h"
#include "fix.h"
#include <math.h>
#include <stdlib.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHitPoint::CHitPoint(CHitPoint* ptr)
	{
	m_sX = ptr->m_sX;
	m_sY = ptr->m_sY;
	m_ptr= ptr->m_ptr;
	m_ePointType= ptr->m_ePointType;
	}

CHitPoint::CHitPoint(CPoint cPoint, void* ptr, EPointType ePointType)
	{
	m_sX = (short)cPoint.x;
	m_sY = (short)cPoint.y;
	m_ptr= ptr;
	m_ePointType = ePointType;
	}

CHitPoint::~CHitPoint()
{

}

//+++============================



CHitList::CHitList()
	{
	m_iXMax=0;
	m_iYMax=0;
	}

CHitList::~CHitList()
	{
	Initialize(0,0);
	}

void CHitList::Initialize(int xMax, int yMax)
	{
	m_iXMax=xMax;
	m_iYMax=yMax;
	if( m_cPtrList.IsEmpty() !=0  )  return;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CHitPoint* pcHit=(CHitPoint*)m_cPtrList.GetNext(pos);
		if( pcHit ) delete pcHit;
		}
	m_cPtrList.RemoveAll();
	}

void CHitList::AddHitPoint(CHitPoint *pcHit)
	{
	if( pcHit->m_sX<0		||
		pcHit->m_sY<0		|| 
		pcHit->m_sX>m_iXMax ||
		pcHit->m_sY>m_iYMax 
		)
		// Not visible, ignore it
		delete pcHit;
	else
		m_cPtrList.AddTail(pcHit);
	}

CHitPoint* CHitList::Get(CPoint &point, double dToler,  CHitPoint::EPointType ePointType)
	{
	double dClosest=1e30;

	CHitPoint* pcHitClosest=NULL;

	if( m_cPtrList.IsEmpty() !=0  )  return NULL;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CHitPoint* pcHit=(CHitPoint*)m_cPtrList.GetNext(pos);

		if( ePointType!=CHitPoint::eEither )
			{
			if( pcHit->m_ePointType!= ePointType ) continue;
			}

			double dDist=sqrt( double((pcHit->m_sY-point.y)*(pcHit->m_sY-point.y)) + double((pcHit->m_sX-point.x)*(pcHit->m_sX-point.x)) );
		if( dDist<dToler && dDist<dClosest ) 
			{
			dClosest=dDist;
			pcHitClosest=pcHit;
			}
		}

	return pcHitClosest;
	}

void CHitPoint::Highlight(CDC* pDC)
	{
	ASSERT( pDC!=NULL );

	if( !pDC ) return;

	CPen cPen;
	cPen.CreatePen(PS_SOLID | PS_COSMETIC, 1,RGB( 0xFF,0xFF,0xFF ));
	CPen* pOldPen = pDC->SelectObject( &cPen );

	pDC->SetROP2( R2_XORPEN );

	int iVertexSize=5;
	CPoint upperLeft, lowerRight, cHitPoint(m_sX,m_sY);
	upperLeft=cHitPoint;
	upperLeft.Offset(-iVertexSize, iVertexSize);
	lowerRight=cHitPoint;
	lowerRight.Offset(iVertexSize, -iVertexSize);
	CRect cRect(upperLeft, lowerRight);
	pDC->Arc( cRect, cHitPoint, cHitPoint );
	pDC->SetROP2( R2_COPYPEN );

	pDC->SelectObject( pOldPen );
	cPen.DeleteObject();
	}

CPoint CHitPoint::GetPoint()
{
	CPoint cLocal=CPoint(m_sX,m_sY);
	return cLocal;
}

void CHitPoint::GetLocation(CLocation &cLoc)
	{
	if (m_ePointType==eNavPoint )
		{
		CControlPoint *pPt=(CControlPoint*)m_ptr;
		cLoc.SetLat(pPt->m_fLat);
		cLoc.SetLong(pPt->m_fLong);
		}
	else
		{
		CFix *pPt=(CFix*)m_ptr;
		cLoc.SetLat(pPt->m_fLatitude);
		cLoc.SetLong(pPt->m_fLongitude);
		}
	}

long CHitPoint::GetTime()
	{
	if (m_ePointType!=eLogPoint )
		return 0;
	else
		{
		CFix *pPt=(CFix*)m_ptr;
		return (long)pPt->m_cTime.GetTime();
		}

	}

long CHitPoint::GetAltitude()
	{
	if (m_ePointType!=eLogPoint )
		return -1;
	else
		{
		CFix *pPt=(CFix*)m_ptr;
		return pPt->m_iAltitude;
		}

	}

CString CHitPoint::GetPointName()
{
	if (m_ePointType==eNavPoint )
		{
		CControlPoint *pPt=(CControlPoint*)m_ptr;
		CString str;
		str.Format("%d - %s",pPt->m_iID, pPt->m_strName);
		return str;
		}
	return "";
}

