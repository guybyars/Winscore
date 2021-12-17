//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// LandingCard.cpp: implementation of the CLandingCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "LandingCard.h"
#include "WinscoreDoc.h"
#include "location.h"
#include "Global_Prototypes.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CURRENTSCHEMA 4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CLandingCard, CLandingCard, VERSIONABLE_SCHEMA| CURRENTSCHEMA)


CLandingCard::CLandingCard()
{
	m_fCompleted=0;
	m_nTurnpoints=0;
	for( int i=0; i<MAXTASKTPS+2; i++) m_aiTurnpoints[i]=0;
	m_iLandoutTurnpoint=0;
	m_iTurnpointSought=0;
	m_iOptions=0;
	m_cContestNo="";
	m_bDesignatedAirfield=FALSE;
	m_bLocalStartGate=FALSE;
	m_bUseTPLocations=FALSE;

}


CLandingCard::~CLandingCard()
{

}

void CLandingCard::Serialize(CArchive & ar)
	{

	if (ar.IsStoring())
		{
		int iDum=0;
		ar << (WORD)m_fCompleted;
		ar << m_nTurnpoints;
		for( int i=0; i<m_nTurnpoints; i++) ar << m_aiTurnpoints[i];
		ar << m_iLandoutTurnpoint;
		ar << m_iTurnpointSought;
		ar << m_cContestNo;
		m_cLandoutLocation.Serialize(ar);
		ar << m_cDate;
		ar << m_bDesignatedAirfield;
		ar << (WORD)m_iOptions;
		ar << (WORD)iDum;
		ar << (WORD)m_bLocalStartGate;
		if( m_bLocalStartGate)
			m_cStartGate.Serialize(ar);
		ar << (WORD)m_bUseTPLocations;
		if( m_bUseTPLocations )
			{
			for( int i=0; i<m_nTurnpoints; i++)
				{
				ar<<m_aiTurnpointFixes[i];
				m_acTurnpointLocations[i].Serialize(ar);
				}
			}
		}
	else
		{
		UINT iSchema=ar.GetObjectSchema( );
		if( iSchema==1  )
			{
			WORD w;
			ar >> w;
			m_fCompleted=(int)w;
			ar >> m_nTurnpoints;
			int aiTemp[MAXTASKTPS+2];
			for( int i=0; i<MAXTASKTPS+2; i++) ar >> aiTemp[i];
			m_nTurnpoints--;
			if(m_fCompleted)m_nTurnpoints--;
			m_nTurnpoints=max(m_nTurnpoints,0);
			for( int j=0; j<m_nTurnpoints; j++ )
				m_aiTurnpoints[j]=aiTemp[j+1];
			ar >> m_iLandoutTurnpoint;
			ar >> m_iTurnpointSought;
			ar >> m_cContestNo;
			m_cLandoutLocation.Serialize(ar);
			ar >> m_cDate;
			ar >> m_bDesignatedAirfield;
			ar >> w; //dum
			ar >> w; //dum
			m_iOptions=0;
			}
		else if( iSchema==2  )
			{
			WORD w;
			ar >> w;
			m_fCompleted=(int)w;
			ar >> m_nTurnpoints;
			for( int i=0; i<m_nTurnpoints; i++) ar >> m_aiTurnpoints[i];
			ar >> m_iLandoutTurnpoint;
			ar >> m_iTurnpointSought;
			ar >> m_cContestNo;
			m_cLandoutLocation.Serialize(ar);
			ar >> m_cDate;
			ar >> m_bDesignatedAirfield;
			ar >> w; 
			m_iOptions=w;
			ar >> w; //dum
			}
		else if( iSchema==3  )
			{
			WORD w;
			ar >> w;
			m_fCompleted=(int)w;
			ar >> m_nTurnpoints;
			for( int i=0; i<m_nTurnpoints; i++) ar >> m_aiTurnpoints[i];
			ar >> m_iLandoutTurnpoint;
			ar >> m_iTurnpointSought;
			ar >> m_cContestNo;
			m_cLandoutLocation.Serialize(ar);
			ar >> m_cDate;
			ar >> m_bDesignatedAirfield;
			ar >> w; 
			m_iOptions=w;
			ar >> w; //dum
			ar >> w; 
			m_bLocalStartGate=w==0?false:true;
			m_cStartGate.Serialize(ar);
			}
		else if( iSchema==4  )
			{
			WORD w;
			ar >> w;
			m_fCompleted=(int)w;
			ar >> m_nTurnpoints;
			for( int i=0; i<m_nTurnpoints; i++) ar >> m_aiTurnpoints[i];
			ar >> m_iLandoutTurnpoint;
			ar >> m_iTurnpointSought;
			ar >> m_cContestNo;
			m_cLandoutLocation.Serialize(ar);
			ar >> m_cDate;
			ar >> m_bDesignatedAirfield;
			ar >> w; 
			m_iOptions=w;
			ar >> w; //dum
			ar >> w; 
			m_bLocalStartGate=w==0?false:true;
			if( m_bLocalStartGate ) 
				m_cStartGate.Serialize(ar);
			ar >> w; 
			m_bUseTPLocations=w==0?false:true;
			if( m_bUseTPLocations )
				{
				for( int i=0; i<m_nTurnpoints; i++) 
					{
					ar<<m_aiTurnpointFixes[i];
					m_acTurnpointLocations[i].Serialize(ar);
					}
				}
			}
	}
}
