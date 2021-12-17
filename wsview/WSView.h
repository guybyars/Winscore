//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSVIEW_H__E074884C_5814_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_WSVIEW_H__E074884C_5814_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WSView.h : main header file for WSVIEW.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#define _WINDLL  1

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWSViewApp : See WSView.cpp for implementation.

class CWSViewApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSVIEW_H__E074884C_5814_11D3_BA96_0010A4043281__INCLUDED)

