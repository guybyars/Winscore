//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Averager.cpp: implementation of the CAverager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Averager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAverager::~CAverager()
{
	free(pdVals);
	free(pbVals);
}

void CAverager::SetValue(double d)
{
	pdVals[m_iLastPoint]=d;
	pbVals[m_iLastPoint]=true;
	
	m_iLastPoint++;
	if( m_iLastPoint==m_iMaxPoints ) m_iLastPoint=0;
}

CAverager::CAverager(int inPoints)
{
	m_iLastPoint=0;
	m_iMaxPoints=inPoints;

	pdVals=(double*)malloc( inPoints*sizeof(double) );
	pbVals=(bool*)malloc( inPoints*sizeof(bool) )  ;
	for( int i=0; i< inPoints; i++ )
		{
		pdVals[i]=0.0;
		pbVals[i]=false;
		}

}

double CAverager::Average()
{
	int NumPts=0;
	double dAve=0.0;
	for( int i=0; i<m_iMaxPoints; i++ )
		{
		if( pbVals[i] )
			{
			dAve+=pdVals[i];
			NumPts++;
			}
		}
	return (NumPts>0)?(dAve/(double)NumPts):(0.0);
}
