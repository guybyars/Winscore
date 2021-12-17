//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ClassTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "ClassTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassTabCtrl

CClassTabCtrl::CClassTabCtrl()
{
}

CClassTabCtrl::~CClassTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CClassTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CClassTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassTabCtrl message handlers


EClass CClassTabCtrl::GetCurClass()
{

	int iSel=GetCurSel(); 
	ASSERT(iSel>-1);
	return m_aeClasses[iSel];
}


