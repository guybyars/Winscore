//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// HitList.h: interface for the CHitList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HITLIST_H__44FF43FE_8548_43B0_9255_1CFB89BD8B6D__INCLUDED_)
#define AFX_HITLIST_H__44FF43FE_8548_43B0_9255_1CFB89BD8B6D__INCLUDED_

#include <afxcoll.h>
#include "Location.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHitPoint
{
public:
	CString GetPointName();
	long GetAltitude();
	long GetTime();
	void GetLocation(CLocation&);
	CPoint GetPoint();

enum EPointType	{	eLogPoint	=0,
					eNavPoint	=1,
					eEither		=3};

	void Highlight(CDC* pDC);
	CHitPoint(CPoint cPoint, void* ptr, EPointType ePointType=eLogPoint );
	
	CHitPoint(CHitPoint* ptr );

	virtual ~CHitPoint();
	short	m_sX;
	short   m_sY;
	void*	m_ptr;
	short   m_ePointType;
};


class CHitList  
{
public:
	CHitPoint* Get( CPoint &cPoint, double dToler=25, CHitPoint::EPointType ePointType=CHitPoint::eLogPoint);
	void Initialize(int,int);
	CHitList();
	void	AddHitPoint( CHitPoint *pHitPoint );
	virtual ~CHitList();

private:
	CPtrList m_cPtrList;
	int m_iXMax;
	int m_iYMax;

	

};



#endif // !defined(AFX_HITLIST_H__44FF43FE_8548_43B0_9255_1CFB89BD8B6D__INCLUDED_)

