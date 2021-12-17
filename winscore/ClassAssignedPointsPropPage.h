//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#include "afxwin.h"
#if !defined(AFX_CLASSASSIGNEDPOINTSPROPPAGE_H__864B55D8_AB0C_11D3_B129_0080C84B0161__INCLUDED_)
#define AFX_CLASSASSIGNEDPOINTSPROPPAGE_H__864B55D8_AB0C_11D3_B129_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassAssignedPointsPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClassAssignedPointsPropPage dialog

class CClassAssignedPointsPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CClassAssignedPointsPropPage)

// Construction
public:
	CClassAssignedPointsPropPage();
	~CClassAssignedPointsPropPage();

// Dialog Data
	//{{AFX_DATA(CClassAssignedPointsPropPage)
	enum { IDD = IDD_ASSIGNEDPOINTSPROPPAGE };
	double m_dD1;
	double m_dDm;
	CString m_strUnits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassAssignedPointsPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassAssignedPointsPropPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	EUnits	m_eUnits;
	CStatic m_cUnitsTextDm;
	CStatic m_cUnitsTextD1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSASSIGNEDPOINTSPROPPAGE_H__864B55D8_AB0C_11D3_B129_0080C84B0161__INCLUDED_)

