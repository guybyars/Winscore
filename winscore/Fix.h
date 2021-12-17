//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Fix.h: interface for the CFix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIX_H__3720CEC1_5819_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_FIX_H__3720CEC1_5819_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "position.h"


class CFix  : public CObject
{

	DECLARE_SERIAL(CFix)

public:
	CString m_strContestNo;
	CFix( CFix &cFix);
	CFix( CString& cContestNo, CTime& cInterpolationTime, CPosition *cPos1, CPosition *cPos2);
	CFix( CString& cContestNo, CPosition *cPos );
	CFix( int iYear, int iMonth, int iDay, CString strRecord );

	CFix();
	CTime	m_cTime;
	float	m_fLatitude;
	float	m_fLongitude;
	int		m_iAltitude;
	short	m_sMotor;

	CFix(	CTime	&cTime,
			float	fLatitude,
			float	fLongitude,
			int		iAltitude,
			short	sMotor );

	virtual void Serialize(CArchive & ar);

	virtual ~CFix();

};

#endif // !defined(AFX_FIX_H__3720CEC1_5819_11D3_BA96_0010A4043281__INCLUDED_)


