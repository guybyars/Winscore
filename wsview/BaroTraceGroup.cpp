//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// BaroTraceGroup.cpp: implementation of the BaroTraceGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSView.h"
#include "BaroTraceGroup.h"
#include "BaroTrace.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaroTraceGroup::CBaroTraceGroup()
{

}

CBaroTraceGroup::~CBaroTraceGroup()
{
Purge();
}

void CBaroTraceGroup::Purge()
	{
	if( m_cPtrArray.GetSize()==0 ) return;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		CBaroTrace* pcTrace=(CBaroTrace*) m_cPtrArray[i];
		if( pcTrace ) delete pcTrace;
		}
	m_cPtrArray.RemoveAll();
	}

int CBaroTraceGroup::AddTrace(CBaroTrace *pTrace)
{
  return m_cPtrArray.Add(pTrace);
}

void CBaroTraceGroup::GetDataMinMax(long lStart, long lEnd, short uDisplayMask, short &sMaxAltitude, short &sMinAltitude)
{
	sMaxAltitude=0;
	sMinAltitude=32000;

	if( m_cPtrArray.GetSize()==0 ) return;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		CBaroTrace* pcTrace=(CBaroTrace*) m_cPtrArray[i];
		if( !pcTrace || !pcTrace->m_bVisible ) continue;

		CBaroPoint *pPointMax=NULL;
		CBaroPoint *pPointMin=NULL;

		pPointMax=pcTrace->GetMaxAltitudeInRange(lStart, lEnd, PRESSURETRACE);
		pPointMin=pcTrace->GetMinAltitudeInRange(lStart, lEnd, PRESSURETRACE);
		if( pPointMin && pPointMax && uDisplayMask&PRESSURETRACE)
			{
			sMaxAltitude=max(sMaxAltitude, pPointMax->GetAltitude(true) );
			sMinAltitude=min(sMinAltitude, pPointMin->GetAltitude(true) );
			}

		pPointMax=pcTrace->GetMaxAltitudeInRange(lStart, lEnd, GPSTRACE);
		pPointMin=pcTrace->GetMinAltitudeInRange(lStart, lEnd, GPSTRACE);
		if( pPointMin && pPointMax && uDisplayMask&GPSTRACE)
			{
			sMaxAltitude=max(sMaxAltitude, pPointMax->GetAltitude(false) );
			sMinAltitude=min(sMinAltitude, pPointMin->GetAltitude(false) );
			}

		}

}


void CBaroTraceGroup::DrawTraces( CDC* pdc, C2DTransform &c2DTransform, bool bRefresh, EUnits eUnits, EAltUnits eAltUnits, short sTrace)
	{

	if( IsSinglePilotVisible()==NULL ) 
		{
		sTrace=PRESSURETRACE;
		}

	if( m_cPtrArray.GetSize()==0 ) return;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		CBaroTrace* pcTrace=(CBaroTrace*) m_cPtrArray[i];
		if( !pcTrace ) continue;

		pcTrace->DrawTrace( pdc,  c2DTransform, bRefresh, sTrace, eUnits, eAltUnits);
		}

	}

short CBaroTraceGroup::GetPilotIndex(CString cContestNo)
{
	if( m_cPtrArray.GetSize()==0 ) return -1;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		CBaroTrace* pcTrace=(CBaroTrace*) m_cPtrArray[i];
		if( pcTrace ) 
			if( pcTrace->m_strContestNo==cContestNo ) return i;
		}
	return -1;
}

CBaroTrace* CBaroTraceGroup::GetTrace(short sIndex)
{
	if( sIndex+1>m_cPtrArray.GetSize() ) return NULL;
	return (CBaroTrace*) m_cPtrArray[sIndex];
}

CBaroTrace* CBaroTraceGroup::IsSinglePilotVisible()
{
	CBaroTrace *pcTrace=NULL, *pcVisibleTrace=NULL;
	int nPilots=0;
	if( m_cPtrArray.GetSize()==0 ) return NULL;
	for( int i=0; i< m_cPtrArray.GetSize();  i++ ) 
		{
		pcTrace=(CBaroTrace*)m_cPtrArray[i];
		if( !pcTrace ) continue;

		if( pcTrace->m_bVisible ) 
			{
			nPilots++;
			pcVisibleTrace=pcTrace;
			}
		}
	if( nPilots>1 ) return NULL;
	return pcVisibleTrace;
}


