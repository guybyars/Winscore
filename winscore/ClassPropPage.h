//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CLASSPROPPAGE_H__864B55D9_AB0C_11D3_B129_0080C84B0161__INCLUDED_)
#define AFX_CLASSPROPPAGE_H__864B55D9_AB0C_11D3_B129_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClassPropPage dialog

class CClassPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CClassPropPage)

// Construction
public:
	CClassPropPage();
	~CClassPropPage();

// Dialog Data
	//{{AFX_DATA(CClassPropPage)
	enum { IDD = IDD_CLASSPROPPAGE };
	BOOL	m_bHandicappedCheck;
	CString	m_strName;
	double	m_dSMTD;
	double	m_dSMTT;
	CString	m_strUnits;
	BOOL	m_bWorstDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassPropPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_iAirfieldBonusPoints;
	afx_msg void OnEnChangeAirfieldBonusEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSPROPPAGE_H__864B55D9_AB0C_11D3_B129_0080C84B0161__INCLUDED_)

