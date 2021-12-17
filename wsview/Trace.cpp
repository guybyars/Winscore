//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Trace.cpp: implementation of the CTrace class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WSView.h"
#include "fix.h"
#include "Trace.h"
#include <math.h>
#include "WSViewctl.h"
#include "hitlist.h"
#include "3DTransformation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Winscore::CTrace::CTrace( CString strContestNo, CString strLastName, CString strFirstName, EClass eClass, long lColor, void* pCtrl  )
{
	m_strLastName	=strLastName;
	m_strFirstName	=strFirstName;
	m_strContestNo	=strContestNo;
	m_eClass		=eClass;
	m_cFinishTime	=CTime(0);
	m_cStartTime	=CTime(0);
	m_sLastIndexPainted=0;
	m_R=GetRValue(lColor);
	m_G=GetGValue(lColor);
	m_B=GetBValue(lColor);

	m_dLabelPositionX=0;
	m_dLabelPositionY=0;
	m_sAltitude=0;
	m_bHighlighted=false;
	m_pWSViewCtl=pCtrl;

	m_pPen=NULL;


}

Winscore::CTrace::CTrace( CTrace &cTrace  )
{
	m_cFinishTime	=cTrace.m_cFinishTime;
	m_cStartTime	=cTrace.m_cStartTime;
	m_strLastName	=cTrace.m_strLastName;
	m_strFirstName	=cTrace.m_strFirstName;
	m_strContestNo	=cTrace.m_strContestNo;
	m_eClass		=cTrace.m_eClass;
	m_sLastIndexPainted=0;
	m_bVisible=TRUE;

	m_R=cTrace.m_R;
	m_G=cTrace.m_G;
	m_B=cTrace.m_B;
	m_dLabelPositionX=0;
	m_dLabelPositionY=0;
	m_sAltitude=0;
	m_bHighlighted=false;
	m_pWSViewCtl=cTrace.m_pWSViewCtl;

	m_pPen=NULL;
	m_bAltitudeMeters=false;
}

Winscore::CTrace::~CTrace()
	{
	if( m_cPtrList.IsEmpty() !=0  )  return;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);
		if( pcFix ) delete pcFix;
		}
	m_cPtrList.RemoveAll();

	if( m_pPen )
		{
		m_pPen->DeleteObject();
		delete m_pPen;
		}

	}


void Winscore::CTrace::AddFix(CFix &cFix)
	{
	m_cPtrList.AddTail(new CFix(cFix));
	m_bGLUpToDate=false;

//	m_sMaxAltitude	=max( m_sMaxAltitude, cFix.m_iAltitude) ;
//	if( m_cPtrList.GetCount()==1 )
//		m_sMinAltitude	=cFix.m_iAltitude;
//	else
//		m_sMinAltitude	=min( m_sMinAltitude, cFix.m_iAltitude) ;

	}


CFix* Winscore::CTrace::Get(CTime &cTime)
	{
	CFix* pcPrevFix=NULL;

	if( m_cPtrList.IsEmpty() !=0  )  return NULL;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);

		if( pcFix->m_cTime==cTime )
			return pcFix;
		else if( pcPrevFix && pcPrevFix->m_cTime<cTime && pcFix->m_cTime>cTime )
			return pcPrevFix;

		pcPrevFix=pcFix;		
		}

	return pcPrevFix;

	}


void Winscore::CTrace::Draw(CDC* pDC, C3DTransformation &c3DTransformation, short sTimeIndexStart, short sTimeIndexStop, short sMaxAlt, short sMinAlt, CHitList &cHitList)
	{
	if( !m_bVisible ) return;
	if( m_cPtrList.IsEmpty() !=0  )  return;

	COLORREF crClimbColor		=((CWSViewCtrl*)m_pWSViewCtl)->GetColor(WSC_CLIMB_COLOR);
	BOOL bDifferentClimbColor	=((CWSViewCtrl*)m_pWSViewCtl)->GetDisplayOption(WSO_DIFFERENT_CLIMB_COLOR);
	BOOL bFullTraceLength		=((CWSViewCtrl*)m_pWSViewCtl)->GetDisplayOption(WSO_FULL_TRACE_DISPLAY);
	BOOL bScenery				=((CWSViewCtrl*)m_pWSViewCtl)->m_bTexturePresent;

	CPen cGreyPen;
	CPen cRedPen;
	cGreyPen.CreatePen( PS_SOLID,				  1,	RGB(155, 155, 155)  );
	cRedPen.CreatePen ( PS_SOLID, (bScenery)?(3):(1),	RGB(255,   0,   0)  );

	CBrush cGreyBrush;
	CBrush cRedBrush;
	cGreyBrush.CreateSolidBrush(	RGB(155, 155, 155)  );
	cRedBrush.CreateSolidBrush(		RGB(255,   0,   0)  );


	int nPoints=m_cPtrList.GetCount();
	CPoint *pcPoints=new CPoint[nPoints];


//==============  Draw the shadow trace  =======================================

	CPen *oldPen=pDC->SelectObject( &cGreyPen ); 
	CBrush *oldBrush=pDC->SelectObject( &cGreyBrush ); 

	CFix *pcLastFix=NULL;

	int i=0;
	int nToDraw=0;
	CPoint cPoint;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);
		if( !pcFix ) continue;	
		i++;

		if( i< sTimeIndexStart ) continue;
		
		c3DTransformation.WorldToLogical( pcPoints[nToDraw], pcFix->m_fLongitude, pcFix->m_fLatitude, 0 );
		pcPoints[nToDraw]=SafePoint(pcPoints[nToDraw]);
		nToDraw++;

		if( i-1 >= sTimeIndexStop ) break;
		}

	if( nToDraw>0 ) 
		{
		pDC->Polyline( pcPoints, nToDraw );
		}

//==============  Draw the main trace  =======================================

	i=0;
	pos = m_cPtrList.GetHeadPosition();
	nToDraw=0;
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);
		if( !pcFix ) continue;
		i++;
		
		if( i< sTimeIndexStart ) continue;

		if( m_cStartTime.GetTime()>0	&&
			pcFix->m_cTime< m_cStartTime	)
			{
			pDC->SelectObject( &cGreyPen );
			pDC->SelectObject( &cGreyBrush ); 
			}
		else if( m_cFinishTime.GetTime()>0	&&
				 pcFix->m_cTime> m_cFinishTime	)
			{
			pDC->SelectObject( &cGreyPen );
			pDC->SelectObject( &cGreyBrush ); 
			}
		else
			{
			pDC->SelectObject( &cRedPen );
			pDC->SelectObject( &cRedBrush ); 
			}

		if( i > sTimeIndexStop+1 ) break;

		pcLastFix=pcFix;
		c3DTransformation.WorldToLogical( cPoint, pcFix->m_fLongitude, pcFix->m_fLatitude, (float)pcFix->m_iAltitude );
		nToDraw++;

		if( nToDraw==1 )
			pDC->MoveTo(SafePoint(cPoint));
		else
			pDC->LineTo(SafePoint(cPoint));

		LoadVertex(pDC, c3DTransformation, cPoint, 0);
		cHitList.AddHitPoint(new CHitPoint(SafePoint(cPoint),pcFix));

		}

	if( pcLastFix )
		{
		CPoint cBasePoint, cLastPoint;
		c3DTransformation.WorldToLogical(cLastPoint, pcLastFix->m_fLongitude, pcLastFix->m_fLatitude,  (float)pcLastFix->m_iAltitude );
		c3DTransformation.WorldToLogical(cBasePoint, pcLastFix->m_fLongitude, pcLastFix->m_fLatitude, 0 );
		pDC->MoveTo(SafePoint(cLastPoint));
		pDC->LineTo(SafePoint(cBasePoint));
		m_sAltitude=pcLastFix->m_iAltitude;
		LoadLabel( pDC, cLastPoint );
		}

	pDC->SelectObject( oldPen ); 
	pDC->SelectObject( oldBrush ); 
	
	cGreyPen.DeleteObject();
	cRedPen.DeleteObject();
	cGreyBrush.DeleteObject();
	cRedBrush.DeleteObject();
	delete[] pcPoints;

	}


void Winscore::CTrace::Draw(CDC* pDC, C3DTransformation &c3DTransformation, CTime &cBegin, CTime &end, long lDisplayLength, BOOL bPointDisplay, short sAltMax, short sAltMin, CHitList &cHitList)
	{

	if( !m_bVisible ) return;
	if( m_cPtrList.IsEmpty() !=0  )  return;
	float	fLastX=0., fLastY=0;
	short	sLastAlt=0;
	CFix*	pcLastFix=NULL;
	CPen	cGreyPen;

	COLORREF crClimbColor		=((CWSViewCtrl*)m_pWSViewCtl)->GetColor(WSC_CLIMB_COLOR);
	BOOL bDifferentClimbColor	=((CWSViewCtrl*)m_pWSViewCtl)->GetDisplayOption(WSO_DIFFERENT_CLIMB_COLOR);
	BOOL bFullTraceLength		=((CWSViewCtrl*)m_pWSViewCtl)->GetDisplayOption(WSO_FULL_TRACE_DISPLAY);
	BOOL bScenery				=((CWSViewCtrl*)m_pWSViewCtl)->m_bTexturePresent;

	CPen cClimbPen;
	cClimbPen.CreatePen( PS_SOLID, (bScenery)?(3):(1), crClimbColor );
	CPen cCruisePen;
	cCruisePen.CreatePen( PS_SOLID, (bScenery)?(3):(1), RGB(m_R, m_G, m_B)  );

	int nPoints=m_cPtrList.GetCount();
	CPoint *pcPoints=new CPoint[nPoints];

	//=====  Draw the grey trace ===================================

	cGreyPen.CreatePen( PS_SOLID, 1, RGB(155, 155, 155)  );
	CPen* pOldPen=pDC->SelectObject( &cGreyPen );

	int nToDraw=0;
	POSITION pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);
		if( !pcFix ) continue;	

		if( pcFix->m_cTime<cBegin ) continue;
		if( pcFix->m_cTime>end ) break;

		pcLastFix=pcFix;

		if( !bFullTraceLength )
			{
			if( lDisplayLength>0 && pcFix->m_cTime < end.GetTime()-lDisplayLength ) continue;
			}

		c3DTransformation.WorldToLogical(pcPoints[nToDraw], pcFix->m_fLongitude, pcFix->m_fLatitude, 0 );
		pcPoints[nToDraw]=SafePoint(pcPoints[nToDraw]);
		nToDraw++;
		}

 	if( nToDraw>0 ) 
		{
		pDC->Polyline( pcPoints, nToDraw );
		}

	//=====  Draw the colored trace ===================================
	
	pDC->SelectObject( cCruisePen );

	CFix* pcPrevFix=NULL;
	bool bFirst=true;
	pos = m_cPtrList.GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)m_cPtrList.GetNext(pos);
		if( !pcFix ) continue;	

		if( pcFix->m_cTime<cBegin ) continue;
		if( pcFix->m_cTime>end ) break;

		if( m_cStartTime.GetTime()>0  && pcFix->m_cTime<m_cStartTime  ) continue;

		pcLastFix=pcFix;

		if( !bFullTraceLength )
			{
			if( lDisplayLength>0 && pcFix->m_cTime < end.GetTime()-lDisplayLength ) continue;
			}

		if( bDifferentClimbColor && pcPrevFix && pcPrevFix->m_iAltitude<pcFix->m_iAltitude )
				pDC->SelectObject( &cClimbPen );
		else
				pDC->SelectObject( &cCruisePen );


		CPoint cPoint=c3DTransformation.WorldToLogical( pcFix->m_fLongitude, pcFix->m_fLatitude, (float)pcFix->m_iAltitude );
		

		if( bFirst )
			{
			pDC->MoveTo(SafePoint(cPoint));
			bFirst=false;
			}
		else
			pDC->LineTo(SafePoint(cPoint));

		if( bPointDisplay )
			{
			LoadVertex(pDC, c3DTransformation, cPoint, 0);
			}

		pcPrevFix=pcFix;

		cHitList.AddHitPoint(new CHitPoint(SafePoint(cPoint),pcFix));

		nToDraw++;

		if( m_cFinishTime.GetTime()>0 && pcFix->m_cTime>m_cFinishTime ) break;
		}

	if( pcLastFix )
		{
		// Load the label and the altitude stick
		CPoint cBasePoint, cLastPoint;
		c3DTransformation.WorldToLogical(cLastPoint, pcLastFix->m_fLongitude, pcLastFix->m_fLatitude,  (float)pcLastFix->m_iAltitude );
		c3DTransformation.WorldToLogical(cBasePoint, pcLastFix->m_fLongitude, pcLastFix->m_fLatitude, 0 );
		pDC->MoveTo(SafePoint(cLastPoint));
		pDC->LineTo(SafePoint(cBasePoint));
		m_sAltitude=pcLastFix->m_iAltitude;
		LoadLabel( pDC, cLastPoint );
		}

	// cleanup
	pDC->SelectObject( pOldPen );
	cGreyPen.DeleteObject();
	cCruisePen.DeleteObject();
	cClimbPen.DeleteObject();

	delete[] pcPoints;

	}

void Winscore::CTrace::SetTime( CTime &cStartTime, CTime &cFinishTime)
	{
	m_cStartTime=cStartTime;
	m_cFinishTime=cFinishTime;
	}


void Winscore::CTrace::LoadVertex(CDC* pDC, C3DTransformation &c3DTransformation, CPoint &cPoint, int iVertexSize)
	{
	if( IsPointClipped(cPoint) ) return;

	if( iVertexSize==0 )
			{
			iVertexSize=0;
			CPoint cPoint1, cPoint2;
			c3DTransformation.WorldToLogical(cPoint1, 0, 0, 0);
			c3DTransformation.WorldToLogical(cPoint2, 1, 0, 0);
			int iSize=abs(cPoint1.x-cPoint2.x);

			if( iSize>  2000 ) 
				iVertexSize=1;
			if( iSize> 25000 ) 
				iVertexSize=2;
			if( iSize> 50000 ) 
				iVertexSize=3;
			if( iSize>100000 ) 
				iVertexSize=4;
			if( iSize>150000 ) 
				iVertexSize=5;
			if( iSize>200000 ) 
				iVertexSize=6;
			if( iSize>500000 ) 
				iVertexSize=7;

			}

	CPoint upperLeft, lowerRight;
	upperLeft=cPoint;
	upperLeft.Offset(-iVertexSize, iVertexSize);
	lowerRight=cPoint;
	lowerRight.Offset(iVertexSize, -iVertexSize);
	CRect cRect(upperLeft, lowerRight);
	//pDC->Ellipse( cRect );
	pDC->Arc( cRect, cPoint, cPoint );


	}

void Winscore::CTrace::LoadLabel(CDC* pDC, CPoint &cPoint )
	{

	m_dLabelPositionX=cPoint.x;
	m_dLabelPositionY=cPoint.y;

	pDC->SetTextColor( RGB(m_R, m_G, m_B ) );
	if( !IsPointClipped(cPoint) ) 
		pDC->TextOut( cPoint.x, cPoint.y, m_strContestNo, m_strContestNo.GetLength() );
	}

void Winscore::CTrace::SetVisible(bool bVisible)
	{
	m_bVisible=bVisible;
	}

bool Winscore::CTrace::IsPilotVisible()
	{
	return m_bVisible;
	}

int Winscore::CTrace::GetNumFixes()
	{
	return m_cPtrList.GetCount();
	}


void Winscore::CTrace::SetColor(COLORREF lColor)
	{
	m_R=GetRValue(lColor);
	m_G=GetGValue(lColor);
	m_B=GetBValue(lColor);
	m_bGLUpToDate=false;
	}

void Winscore::CTrace::Highlight(CDC* pDC)
	{
	CRect cRect;
	CBrush cBrush;
	cBrush.CreateSolidBrush( RGB(255,255,255) );

	int nLines=2;
	if( m_strAdditionalText.GetLength()>0 ) nLines++;

	pDC->SetTextAlign( TA_BOTTOM );

#define m_to_ft  3.281  /* m to feet. */

	CString strLine1=m_strContestNo + " - " + m_strLastName + ", " + m_strFirstName;
	CString strLine2;
	if( m_bAltitudeMeters )
		strLine2.Format("Alt = %4.1lf m", (double)m_sAltitude/m_to_ft);
	else
		strLine2.Format("Alt = %d ft", m_sAltitude);


	CSize 	cLine1Size=pDC->GetTextExtent(strLine1);	
	CSize	cLine2Size=pDC->GetTextExtent(strLine2);
	CSize	cLine3Size=pDC->GetTextExtent(m_strAdditionalText);

	cRect.SetRect(	(int)m_dLabelPositionX, 
					(int)m_dLabelPositionY, 
					(int)m_dLabelPositionX+(max(max(cLine1Size.cx,cLine2Size.cx),cLine3Size.cx))+8,
					(int)m_dLabelPositionY-cLine1Size.cy-cLine2Size.cy-cLine3Size.cy );

	pDC->FillRect(  &cRect, &cBrush );
	pDC->DrawEdge( &cRect, EDGE_BUMP         , BF_RECT );


	pDC->SetBkColor( RGB(255,255,255) );
//	pDC->FrameRect(  &cRect, &cBrush );


	pDC->TextOut( (int)m_dLabelPositionX+4, (int)m_dLabelPositionY-((nLines-1)*cLine1Size.cy), strLine1, strLine1.GetLength() );
	pDC->TextOut( (int)m_dLabelPositionX+4, (int)m_dLabelPositionY-((nLines-2)*cLine1Size.cy), strLine2, strLine2.GetLength() );
	if( m_strAdditionalText.GetLength()>0 )
		pDC->TextOut( (int)m_dLabelPositionX+4, (int)m_dLabelPositionY, m_strAdditionalText, m_strAdditionalText.GetLength() );
	m_bHighlighted=true;

	}

double Winscore::CTrace::GetDistanceToLabel(CPoint &point)
{
return sqrt( (m_dLabelPositionY-point.y)*(m_dLabelPositionY-point.y) + (m_dLabelPositionX-point.x)*(m_dLabelPositionX-point.x) );
}

