//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TraceGroup.cpp: implementation of the CTraceGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSView.h"
#include "Trace.h"
#include "hitlist.h"
#include "TraceGroup.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceGroup::CTraceGroup()
	{
	m_lTraceDisplayLength=30;
	m_bPointDisplay=true;
	}

CTraceGroup::~CTraceGroup()
	{
	Purge();
	}

int CTraceGroup::AddTrace(Winscore::CTrace &cTrace)
{
  return m_cPtrArray.Add(new Winscore::CTrace(cTrace) );
}

bool CTraceGroup::AddFix(short sPilotIndex, CFix &cFix)
	{
	if( sPilotIndex<0 || sPilotIndex>m_cPtrArray.GetSize() ) return false;

	((Winscore::CTrace*)m_cPtrArray[sPilotIndex])->AddFix(cFix);

	return true;
	}

void CTraceGroup::Draw(CDC* pDC, C3DTransformation &c3DTransformation, CTime &cStart, CTime &cEnd, EClass eActiveClass, short sAltMax, short sAltMin, CHitList &cHitList)
	{
	if( m_cPtrArray.GetSize()==0 ) return;

	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace ) 
			{
			if( pcTrace->m_eClass==eActiveClass )
				pcTrace->Draw( pDC, c3DTransformation,cStart,cEnd, m_lTraceDisplayLength*60, m_bPointDisplay, sAltMax, sAltMin,  cHitList);
			}
		}


	}

void CTraceGroup::Draw(CDC* pDC, C3DTransformation &c3DTransformation,short sTimeIndexStart,  short sTimeIndexStop, short sAltMax, short sAltMin, CHitList &cHitList )
	{
	if( m_cPtrArray.GetSize()==0 ) return;

	for( int i=0; i< m_cPtrArray.GetSize(); i++ )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace ) pcTrace->Draw( pDC, c3DTransformation,sTimeIndexStart, sTimeIndexStop, sAltMax, sAltMin, cHitList );
		}

	}


void CTraceGroup::Invalidate()
{
	if( m_cPtrArray.GetSize()==0 ) return;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace ) pcTrace->m_bGLUpToDate=false;
		}
}

void CTraceGroup::AddTimes(short sPilotIndex, CTime &cStartTime, CTime &cFinishTime)
{
	if( sPilotIndex<0 || sPilotIndex>m_cPtrArray.GetSize() ) return;

	((Winscore::CTrace*)m_cPtrArray[sPilotIndex])->SetTime(cStartTime, cFinishTime);

}

short CTraceGroup::GetPilotIndex(CString cContestNo)
{
	if( m_cPtrArray.GetSize()==0 ) return -1;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace ) 
			if( pcTrace->m_strContestNo==cContestNo ) return i;
		}
	return -1;
}

void CTraceGroup::SetPilotVisible(CString strCID, bool bVisible)
{
	short sIndex=GetPilotIndex(strCID);
	if( sIndex >=0 )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[sIndex];
		if( pcTrace ) 
			pcTrace->SetVisible(bVisible);
		}

}

bool CTraceGroup::IsPilotVisible(CString strCID)
	{
	short sIndex=GetPilotIndex(strCID);
	if( sIndex >=0 )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[sIndex];
		if( pcTrace ) 
			return pcTrace->IsPilotVisible();
		}
	return FALSE;
	}


void CTraceGroup::Purge()
	{
	if( m_cPtrArray.GetSize()==0 ) return;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace ) delete pcTrace;
		}
	m_cPtrArray.RemoveAll();
	}

int CTraceGroup::GetNumFixes(CString strCID)
	{
	short sIndex=GetPilotIndex(strCID);
	if( sIndex >=0 )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[sIndex];
		return pcTrace->GetNumFixes();
		}
	return 0;
	}

void CTraceGroup::SetDisplayLength(long lLength)
	{
	m_lTraceDisplayLength=lLength;
	}

void CTraceGroup::SetPilotColor(CString strContestNo, long lColor)
	{
	short sIndex=GetPilotIndex(strContestNo);
	if( sIndex >=0 )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[sIndex];
		if( pcTrace )
			pcTrace->SetColor(lColor);
		}
	}


void CTraceGroup::Highlight(CString strContestID)
	{
	short sIndex=GetPilotIndex(strContestID);
	if( sIndex >=0 )
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[sIndex];
//		if( pcTrace )
//			pcTrace->Highlight();
		}

	}

void CTraceGroup::Highlight(int iIndex, CDC* pDC)
	{
	Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[iIndex];
	if( pcTrace ) 
		{
		pcTrace->Highlight(pDC);
		}
	}

int CTraceGroup::GetClosestPilot(CPoint &point, short sRadius)
	{
	if( m_cPtrArray.GetSize()==0 ) return -1;
	int iSelected= -1;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace && pcTrace->IsPilotVisible() ) 
			{
			double dRad=pcTrace->GetDistanceToLabel(point);
			if( dRad<(double)sRadius ) 
				iSelected=i;
			}
		}
	return iSelected;
	}

BOOL CTraceGroup::AnyoneHighlighted(bool bClear)
{
	BOOL bHighlighted=false;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace && pcTrace->m_bHighlighted ) 
			{
			bHighlighted=true;
			if( bClear ) pcTrace->m_bHighlighted=false;
			}
		}
	return bHighlighted;
}

int CTraceGroup::GetHighlightedContestant()
{
	BOOL bHighlighted=false;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace && pcTrace->m_bHighlighted ) 
			{
			return i;
			}
		}
	return -1;
}

Winscore::CTrace* CTraceGroup::GetTrace(CString strContestNo)
	{
	if( m_cPtrArray.GetSize()==0 ) return NULL;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		Winscore::CTrace* pcTrace=(Winscore::CTrace*) m_cPtrArray[i];
		if( pcTrace && pcTrace->m_strContestNo==strContestNo ) return pcTrace;
		}
	return NULL;
	}

Winscore::CTrace* CTraceGroup::GetTrace(int i)
	{
	return  (Winscore::CTrace*) m_cPtrArray[i];
	}
