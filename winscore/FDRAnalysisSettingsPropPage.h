//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_FDRANALYSISSETTINGSPROPPAGE_H__210D2F75_B798_11D3_B132_0080C84B0161__INCLUDED_)
#define AFX_FDRANALYSISSETTINGSPROPPAGE_H__210D2F75_B798_11D3_B132_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FDRAnalysisSettingsPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFDRAnalysisSettingsPropPage dialog

class CFDRAnalysisSettingsPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFDRAnalysisSettingsPropPage)

// Construction
public:
	CFDRAnalysisSettingsPropPage();
	~CFDRAnalysisSettingsPropPage();

// Dialog Data
	//{{AFX_DATA(CFDRAnalysisSettingsPropPage)
	enum { IDD = IDD_FDR_ANALYSIS_PROPPAGE };
	CEdit	m_cUserTimezone;
	double	m_dInnerRadius;
	double	m_dOuterRadius;
	CString	m_cUnitsText1;
	CString	m_cUnitsText2;
	int		m_iTimeZoneOption;
	int		m_iUserTimezone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFDRAnalysisSettingsPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFDRAnalysisSettingsPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnUserradio();
	afx_msg void OnSystemradio();
	afx_msg void OnLogradio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDRANALYSISSETTINGSPROPPAGE_H__210D2F75_B798_11D3_B132_0080C84B0161__INCLUDED_)

