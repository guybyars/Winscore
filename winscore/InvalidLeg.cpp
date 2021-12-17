//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// InvalidLeg.cpp: implementation of the CInvalidLeg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "InvalidLeg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CInvalidLeg, CInvalidLeg, DB_SCHEMA_ID)

CInvalidLeg::CInvalidLeg()
{
	m_iFirstTurnpoint=0;
	m_iSecondTurnpoint=0;
}

CInvalidLeg::~CInvalidLeg()
{

}

CInvalidLeg::CInvalidLeg(CInvalidLeg* pcInvalidLeg)
{
	m_iFirstTurnpoint=pcInvalidLeg->m_iFirstTurnpoint;
	m_iSecondTurnpoint=pcInvalidLeg->m_iSecondTurnpoint;
}




void CInvalidLeg::Serialize(CArchive & ar)
	{
	if (ar.IsStoring())
		{
		ar << m_iFirstTurnpoint;
		ar << m_iSecondTurnpoint;
		}
	else
		{
		ar >> m_iFirstTurnpoint;
		ar >> m_iSecondTurnpoint;
		}
	}




int CALLBACK CompareInvalidLegs(LPARAM lParam1, LPARAM lParam2, LPARAM iColumn)
{
	switch( iColumn )
		{
	case 0:
		return  ( ((CInvalidLeg*)lParam1)->m_iFirstTurnpoint > ((CInvalidLeg*)lParam2)->m_iFirstTurnpoint)?(1):(-1);
	case 1:		
		return  ( ((CInvalidLeg*)lParam1)->m_iSecondTurnpoint > ((CInvalidLeg*)lParam2)->m_iSecondTurnpoint)?(1):(-1);
		}
	return 0;
}

