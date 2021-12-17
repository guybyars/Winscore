//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_GATEPROPPAGE_H__6C94C2B7_E714_4B5E_9585_B037DB93337D__INCLUDED_)
#define AFX_GATEPROPPAGE_H__6C94C2B7_E714_4B5E_9585_B037DB93337D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GatePropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGatePropPage : Property page dialog

class CGatePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGatePropPage)
	DECLARE_OLECREATE_EX(CGatePropPage)

// Constructors
public:
	CGatePropPage();

// Dialog Data
	//{{AFX_DATA(CGatePropPage)
	enum { IDD = IDD_OLE_PROPPAGE_GATE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CGatePropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GATEPROPPAGE_H__6C94C2B7_E714_4B5E_9585_B037DB93337D__INCLUDED_)

