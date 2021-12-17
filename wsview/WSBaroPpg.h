//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSBAROPPG_H__49134346_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)
#define AFX_WSBAROPPG_H__49134346_1CE2_11D4_B1D2_54F85E000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WSBaroPpg.h : Declaration of the CWSBaroPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CWSBaroPropPage : See WSBaroPpg.cpp.cpp for implementation.

class CWSBaroPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWSBaroPropPage)
	DECLARE_OLECREATE_EX(CWSBaroPropPage)

// Constructor
public:
	CWSBaroPropPage();

// Dialog Data
	//{{AFX_DATA(CWSBaroPropPage)
	enum { IDD = IDD_PROPPAGE_WSBARO };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CWSBaroPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSBAROPPG_H__49134346_1CE2_11D4_B1D2_54F85E000000__INCLUDED)

