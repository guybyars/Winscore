//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_FDRSETTINGS_H__5B5EB160_BC90_11D1_8F45_0040954D32B2__INCLUDED_)
#define AFX_FDRSETTINGS_H__5B5EB160_BC90_11D1_8F45_0040954D32B2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FDRSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFDRSettings dialog

class CFDRSettings : public CDialog
{
// Construction
public:
	CFDRSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFDRSettings)
	enum { IDD = IDD_FDR_SETTINGS };
	CComboBox	m_cStartTypeCombo;
	CStatic	m_cStartHeadingText;
	CEdit	m_cStartHeadingEdit;
	CEdit	m_cHeadingEdit;
	CComboBox	m_cTypeCombo;
	CStatic	m_strHeadingText;
	double	m_dFinishRadius;
	double	m_dStartRadius;
	int		m_iFinishBase;
	int		m_iStartCeiling;
	int		m_iStartHeading;
	int		m_iFinishHeading;
	CString	m_strFinishRadiusText;
	CString	m_strStartRadiusText;
	//}}AFX_DATA

	EGateType m_eFinishGateType;
	EGateType m_eStartGateType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFDRSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFDRSettings)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeStartcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDRSETTINGS_H__5B5EB160_BC90_11D1_8F45_0040954D32B2__INCLUDED_)

