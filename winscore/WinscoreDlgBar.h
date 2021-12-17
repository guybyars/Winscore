//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WinscoreDlgBar.h: interface for the CWinscoreDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSCOREDLGBAR_H__84A0DE81_49F4_11D1_ACDD_000000000000__INCLUDED_)
#define AFX_WINSCOREDLGBAR_H__84A0DE81_49F4_11D1_ACDD_000000000000__INCLUDED_

#include "Winscore.h"	// Added by ClassView
#include "WinscoreDoc.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWinscoreDlgBar : public CPaneDialog  
{
public:
	void GreyAll();
	void SetDate(CTime cDate);
	void SetClass(EClass eClass);
	CTime	GetDayCombo();
	EClass	GetClassCombo();
	void	LoadDayCombo(CWinscoreDoc *pDoc);
	void	LoadClassCombo(CWinscoreDoc *pDoc);
	void	SetView(EViews eView);
	EViews	GetView();

	BOOL	Create(CWnd* pParentWnd);
	CWinscoreDlgBar();
	virtual ~CWinscoreDlgBar();

};

#endif // !defined(AFX_WINSCOREDLGBAR_H__84A0DE81_49F4_11D1_ACDD_000000000000__INCLUDED_)

