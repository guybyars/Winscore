//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// InvalidLeg.h: interface for the CInvalidLeg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVALIDLEG_H__52978672_6839_11D1_AD0C_000000000000__INCLUDED_)
#define AFX_INVALIDLEG_H__52978672_6839_11D1_AD0C_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CInvalidLeg : public CObject  
{
DECLARE_SERIAL(CInvalidLeg)

public:	
	
	virtual void Serialize(CArchive & ar);

	int m_iSecondTurnpoint;
	int m_iFirstTurnpoint;
	CInvalidLeg();
	CInvalidLeg(CInvalidLeg* pcInvalidLeg);
	virtual ~CInvalidLeg();

};

int CALLBACK CompareInvalidLegs(LPARAM lParam1, LPARAM lParam2, LPARAM iColumn);

#endif // !defined(AFX_INVALIDLEG_H__52978672_6839_11D1_AD0C_000000000000__INCLUDED_)



