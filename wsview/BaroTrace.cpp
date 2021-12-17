//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroTrace.cpp: implementation of the CBaroTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSView.h"
#include "BaroPoint.h"
#include "BaroTrace.h"
#include "WSBaroCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define TRACERECTPOS( cPoint, iSize )  CRect( cPoint.x-iSize/2,cPoint.y-iSize/2, cPoint.x+iSize/2,cPoint.y+iSize/2) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaroTrace::CBaroTrace()
{
	m_sMinAltitude=32000;
	m_sMaxAltitude=0;
	m_bVisible=true;
	m_pPen=NULL;
	m_LastDrawnPosition=NULL;
//	m_p2ndToLastPoint=NULL;
	m_pLastPoint=NULL;
}

CBaroTrace::CBaroTrace(  CString strContestNo, CString strLastName, CString strFirstName, short sClass, long lColorREF ):
	m_strContestNo(strContestNo),
	m_strFirstName(strFirstName),
	m_strLastName(strLastName),
	m_sClass(sClass)
	{
	m_R=GetRValue(lColorREF);
	m_G=GetGValue(lColorREF);
	m_B=GetBValue(lColorREF);

	m_sMinAltitude=32000;
	m_sMaxAltitude=0;
	m_bVisible=true;
	m_pPen=NULL;
	m_LastDrawnPosition=NULL;
//	m_p2ndToLastPoint=NULL;
	m_pLastPoint=NULL;

	}

CBaroTrace::~CBaroTrace()
{
	if( m_cPtrList.IsEmpty() !=0  )  return;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CBaroPoint* pcPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcPoint ) delete pcPoint;
		}
	m_cPtrList.RemoveAll();

	if( m_pPen )
		{
		m_pPen->DeleteObject();
		delete m_pPen;
		}
}

void CBaroTrace::SetColor(COLORREF lColor)
	{
	m_R=GetRValue(lColor);
	m_G=GetGValue(lColor);
	m_B=GetBValue(lColor);
	}

void CBaroTrace::AddPoint(CBaroPoint &cPoint)
	{
	// Some systems blow up if they get too many points.  So limit points to a short int 32767
	if( m_cPtrList.GetCount()>32700) return;

	CBaroPoint *pPoint=new CBaroPoint(cPoint);
	m_cPtrList.AddTail(pPoint);

//	m_p2ndToLastPoint=m_pLastPoint;
//	m_pLastPoint=pPoint;

	m_sMinAltitude=min(m_sMinAltitude,cPoint.m_sGPSAltitude);
	m_sMinAltitude=min(m_sMinAltitude,cPoint.m_sPressure);
	m_sMaxAltitude=max(m_sMaxAltitude,cPoint.m_sGPSAltitude);
	m_sMaxAltitude=max(m_sMaxAltitude,cPoint.m_sPressure);

	}


void CBaroTrace::DrawTrace( CDC* pdc, C2DTransform &c2DTransform, bool bRefresh, short iDrawMask, EUnits eUnits,  EAltUnits eAltUnits)
	{
	POSITION pos, iniPos;

	if( !m_bVisible ) return;

	if( m_cPtrList.IsEmpty() !=0  )  return;

	CSingleLock cLock(&m_cSemaphore);
	cLock.IsLocked();
	cLock.Lock();

	int nPoints=m_cPtrList.GetCount();

	if( !m_pPen )
		{
		m_pPen=new CPen();
		m_pPen->CreatePen( PS_SOLID, 1, RGB(m_R, m_G, m_B)  );
		}
	CPen* pOldPen=pdc->SelectObject( m_pPen );

	CPoint *pcGPSPoints=new CPoint[nPoints];
	CPoint *pcPresPoints=new CPoint[nPoints];
	CPoint *pcENLPoints=new CPoint[nPoints];

	if( bRefresh && m_LastDrawnPosition )
		{
		iniPos=m_LastDrawnPosition;
		}
	else
		iniPos=m_cPtrList.GetHeadPosition();

	int i=0;
	int nToDraw=0;
	pos=iniPos;
	int iMaxENL=0;
	int iMaxAlt=0;
	while(pos!=NULL)
		{
		m_LastDrawnPosition=pos;
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( !pcBaroPoint ||
			pcBaroPoint->m_cTime<c2DTransform.m_cDataWindow.left	||
			pcBaroPoint->m_cTime>c2DTransform.m_cDataWindow.right		) continue;	

		pcGPSPoints[i].x=c2DTransform.WorldXToScreen((int)pcBaroPoint->m_cTime.GetTime());
		pcGPSPoints[i].y=c2DTransform.WorldYToScreen((int)ConvertAltitude(pcBaroPoint->m_sGPSAltitude, eAltUnits));
		
		pcPresPoints[i].x=c2DTransform.WorldXToScreen((int)pcBaroPoint->m_cTime.GetTime());
		pcPresPoints[i].y=c2DTransform.WorldYToScreen((int)ConvertAltitude(pcBaroPoint->m_sPressure, eAltUnits));

		iMaxENL=max(iMaxENL,pcBaroPoint->m_sENL);
		iMaxAlt=max(iMaxAlt,(int)ConvertAltitude(pcBaroPoint->m_sGPSAltitude, eAltUnits));
		iMaxAlt=max(iMaxAlt,(int)ConvertAltitude(pcBaroPoint->m_sPressure, eAltUnits));
		i++;
		nToDraw++;
		}

	int iPointSize=3;
	if( nToDraw<150 )
		iPointSize=5;

	i=0;
	pos=iniPos;
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( !pcBaroPoint ||
			pcBaroPoint->m_cTime<c2DTransform.m_cDataWindow.left	||
			pcBaroPoint->m_cTime>c2DTransform.m_cDataWindow.right		) continue;	
		
		pcENLPoints[i].x=c2DTransform.WorldXToScreen((int)pcBaroPoint->m_cTime.GetTime());
		if( iMaxENL>0 )
//			pcENLPoints[i].y=c2DTransform.WorldYToScreen(pcBaroPoint->m_sENL*iMaxAlt/iMaxENL);
			pcENLPoints[i].y=(int)c2DTransform.WorldYToScreen((int)min(150,(int)pcBaroPoint->m_sENL)*(int)((float)iMaxAlt/150.0));
		else
			pcENLPoints[i].y=c2DTransform.WorldYToScreen(0);

		i++;
		}

// If pressure altitude, draw in the stored trace color

	if( iDrawMask == PRESSURETRACE )
		{
		if( !m_pPen )
			{
			m_pPen=new CPen();
			m_pPen->CreatePen( PS_SOLID, 1, RGB(m_R, m_G, m_B)  );
			}
		CPen* pOldPen=pdc->SelectObject( m_pPen );

		pdc->MoveTo( pcPresPoints[0] );
		pdc->Polyline( pcPresPoints, nToDraw );
		
		pdc->SelectObject( pOldPen );
		}
	else
		{
		if( iDrawMask & PRESSURETRACE )
			{
			CPen* pPressPen=new CPen();
			pPressPen->CreatePen( PS_SOLID, 1, PRESSTRACECOLOR  );
			pOldPen=pdc->SelectObject( pPressPen );

			CBrush* pBrush=new CBrush();
			pBrush->CreateSolidBrush( PRESSTRACECOLOR );
			CBrush *pcOldBrush=pdc->SelectObject( pBrush );

			pdc->MoveTo( pcPresPoints[0] );
			for (int i=1; i<nToDraw; i++ )
				{
				if( pcPresPoints[i-1]!=pcPresPoints[i]	)
					pdc->LineTo(pcPresPoints[i]);
				}
			//pdc->Polyline( pcPresPoints, min(nToDraw,16000) );

			if( nToDraw <200 )
				{
				for( int i=0; i<nToDraw; i++ )
					pdc->Rectangle(TRACERECTPOS(pcPresPoints[i],iPointSize));
				}

			pdc->SelectObject( pcOldBrush );
			pBrush->DeleteObject();
			delete pBrush;

			pPressPen->DeleteObject();
			pdc->SelectObject( pOldPen );
			delete pPressPen;
			}

		if( iDrawMask & GPSTRACE )
			{
			CPen* pGPSPen=new CPen();
			pGPSPen->CreatePen( PS_SOLID, 1, GPSTRACECOLOR  );
			pOldPen=pdc->SelectObject( pGPSPen );
			CBrush* pBrush=new CBrush();
			pBrush->CreateSolidBrush( GPSTRACECOLOR );
			CBrush *pcOldBrush=pdc->SelectObject( pBrush );


			pdc->MoveTo( pcGPSPoints[0] );
			for (int i=1; i<nToDraw; i++ )
				if( pcGPSPoints[i-1]!=pcGPSPoints[i]	)
					pdc->LineTo(pcGPSPoints[i]);

//			pdc->Polyline( pcGPSPoints, min(nToDraw,16000) );

			if( nToDraw <200 )
				{
				for( int i=0; i<nToDraw; i++ )
					pdc->Rectangle(TRACERECTPOS(pcGPSPoints[i],iPointSize));
				}

			pdc->SelectObject( pcOldBrush );
			pBrush->DeleteObject();
			delete pBrush;


			pdc->SelectObject( pOldPen );
			pGPSPen->DeleteObject();
			delete pGPSPen;
			}


		if( iDrawMask & ENLTRACE )
			{
			CPen* pENLPen=new CPen();
			pENLPen->CreatePen( PS_SOLID, 1, ENLTRACECOLOR  );
			pOldPen=pdc->SelectObject( pENLPen );
			CBrush* pBrush=new CBrush();
			pBrush->CreateSolidBrush( GPSTRACECOLOR );
			CBrush *pcOldBrush=pdc->SelectObject( pBrush );

//			pdc->MoveTo( pcENLPoints[0] );
//			pdc->Polyline( pcENLPoints, min(nToDraw,16000) );
			pdc->MoveTo( pcENLPoints[0] );
			for (int i=1; i<nToDraw; i++ )
				{
				if( pcENLPoints[i-1]!=pcENLPoints[i]	)
				pdc->LineTo(pcENLPoints[i]);
				}

			if( nToDraw <200 )
				{
				for( int i=0; i<nToDraw; i++ )
					pdc->Rectangle(TRACERECTPOS(pcENLPoints[i],iPointSize));
				}

			pdc->SelectObject( pcOldBrush );
			pBrush->DeleteObject();
			delete pBrush;

			pdc->SelectObject( pOldPen );
			pENLPen->DeleteObject();
			delete pENLPen;
			}
		}

	delete[] pcGPSPoints;
	delete[] pcPresPoints;
	delete[] pcENLPoints;

	cLock.Unlock(1);

	}
/*
void CBaroTrace::GetAltitudes(CTime cTime, short &sGPSAlt, short &sPressAlt )
{
	sGPSAlt=0;
	sPressAlt=0;

	if( m_cPtrList.IsEmpty() !=0  )  return;

	int nPoints=m_cPtrList.GetCount();

	POSITION pos=m_cPtrList.GetHeadPosition();

	CBaroPoint* pcPrevBaroPoint=NULL;
	int i=0;
	int nToDraw=0;
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint && pcPrevBaroPoint ) 
			{
			if( pcPrevBaroPoint->m_cTime<=cTime &&
				pcBaroPoint->m_cTime>cTime			)
				{
				sGPSAlt=pcBaroPoint->m_sGPSAltitude;
				sPressAlt=pcBaroPoint->m_sPressure;;
				return;
				}
			}
		pcPrevBaroPoint=pcBaroPoint;
		}

	if( pcPrevBaroPoint )
		{
		sGPSAlt		=pcPrevBaroPoint->m_sGPSAltitude;
		sPressAlt	=pcPrevBaroPoint->m_sPressure;
		return;
		}

}
*/
long CBaroTrace::GetNextPoint(CTime cTime)
	{
	if( m_cPtrList.IsEmpty() !=0  )  return 0;

	POSITION pos=m_cPtrList.GetHeadPosition();

	CBaroPoint* pcPrevBaroPoint=NULL;
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint && pcPrevBaroPoint ) 
			{
			if( pcPrevBaroPoint->m_cTime<=cTime &&
				pcBaroPoint->m_cTime>cTime			)
				{
				return (long)pcBaroPoint->m_cTime.GetTime();
				}
			}
		pcPrevBaroPoint=pcBaroPoint;
		}

	return (long)pcPrevBaroPoint->m_cTime.GetTime();

}

CBaroPoint* CBaroTrace::GetBaroPoint(CTime &cTime)
{
	if( m_cPtrList.IsEmpty() !=0  )  return NULL;

	POSITION pos=m_cPtrList.GetHeadPosition();

	CBaroPoint* pcPrevBaroPoint=NULL;
	int i=0;
	int nToDraw=0;
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint && pcPrevBaroPoint ) 
			{
			if( pcPrevBaroPoint->m_cTime<=cTime &&
				pcBaroPoint->m_cTime>cTime			)
				{
				return pcPrevBaroPoint;
				}
			}
		pcPrevBaroPoint=pcBaroPoint;
		}
	return pcPrevBaroPoint;
}


CBaroPoint* CBaroTrace::GetMinAltitudeInRange(long lStart, long lEnd, short sType)
{
	if( m_cPtrList.IsEmpty() !=0  )  return NULL;
	short sMinAlt=32000;
	CBaroPoint* pcMinBaroPoint=NULL;

	POSITION pos=m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint->m_cTime>=lStart && pcBaroPoint->m_cTime<=lEnd ) 
			{
			if( sType==PRESSURETRACE )
				{	
				if( pcBaroPoint->m_sPressure<sMinAlt )
					{
					sMinAlt=pcBaroPoint->m_sPressure;
					pcMinBaroPoint=pcBaroPoint;
					}
				}
			else
				{
				if( pcBaroPoint->m_sGPSAltitude<sMinAlt )
					{
					sMinAlt=pcBaroPoint->m_sGPSAltitude;
					pcMinBaroPoint=pcBaroPoint;
					}
				}
			}
		}
	return pcMinBaroPoint;

}

CBaroPoint* CBaroTrace::GetMaxAltitudeInRange(long lStart, long lEnd, short sType)
{
	if( m_cPtrList.IsEmpty() !=0  )  return NULL;
	short sMaxAlt=0;
	CBaroPoint* pcMaxBaroPoint=NULL;

	POSITION pos=m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint->m_cTime>=lStart && pcBaroPoint->m_cTime<=lEnd ) 
			{
			if( sType==PRESSURETRACE )
				{	
				if( pcBaroPoint->m_sPressure>sMaxAlt )
					{
					sMaxAlt=pcBaroPoint->m_sPressure;
					pcMaxBaroPoint=pcBaroPoint;
					}
				}
			else
				{
				if( pcBaroPoint->m_sGPSAltitude>sMaxAlt )
					{
					sMaxAlt=pcBaroPoint->m_sGPSAltitude;
					pcMaxBaroPoint=pcBaroPoint;
					}
				}
			}
		}
	return pcMaxBaroPoint;

}

long CBaroTrace::GetPrevPoint(CTime cTime)
	{
	if( m_cPtrList.IsEmpty() !=0  )  return 0;

	POSITION pos=m_cPtrList.GetHeadPosition();

	CBaroPoint* pcPrevBaroPoint=NULL;
	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint && pcPrevBaroPoint ) 
			{
			if( pcPrevBaroPoint->m_cTime<cTime &&
				pcBaroPoint->m_cTime>=cTime			)
				{
				return (long)pcPrevBaroPoint->m_cTime.GetTime();
				}
			}
		pcPrevBaroPoint=pcBaroPoint;
		}

	return (long)pcPrevBaroPoint->m_cTime.GetTime();
	}

long CBaroTrace::GetClosestPoint(CTime &cTime)
{
	if( m_cPtrList.IsEmpty() !=0  )  return (long)cTime.GetTime();

	POSITION pos=m_cPtrList.GetHeadPosition();

	CBaroPoint* pcClosestPoint=NULL;
	long lClosest=INT_MAX;

	while(pos!=NULL)
		{
		CBaroPoint* pcBaroPoint=(CBaroPoint*)m_cPtrList.GetNext(pos);
		if( pcBaroPoint ) 
			{
			CTimeSpan cSpan=pcBaroPoint->m_cTime-cTime;
			if( abs(int(cSpan.GetTotalSeconds()))<lClosest )
				{
				lClosest=abs(int(cSpan.GetTotalSeconds()));
				pcClosestPoint=pcBaroPoint;
				}
			}
		}

	return (long)pcClosestPoint==NULL?(long)cTime.GetTime():(long)pcClosestPoint->m_cTime.GetTime();
}
