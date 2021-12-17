//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_FDRSETTINGSPROPPAGE_H__210D2F73_B798_11D3_B132_0080C84B0161__INCLUDED_)
#define AFX_FDRSETTINGSPROPPAGE_H__210D2F73_B798_11D3_B132_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FDRSettingsPropPage.h : header file
//
#include "WinscoreDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CFDRSettingsPropPage dialog

class CFDRSettingsPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFDRSettingsPropPage)

// Construction
public:
	void MasqueForm();
	int m_iPortID;
	CFDRSettingsPropPage();
	~CFDRSettingsPropPage();

// Dialog Data
	//{{AFX_DATA(CFDRSettingsPropPage)
	enum { IDD = IDD_FDR_SETTINGS_PROPPAGE };
	CButton	m_cSUAIgnore;
	CStatic	m_cSUAFileText;
	CStatic	m_cSUAFileText2;
	CEdit	m_cSUAFileEdit;
	CEdit	m_cSUAFileEdit2;
	CButton	m_cSUAButton;
	CButton	m_cSUAButton2;
	CComboBox	m_cPortCombo;
	BOOL	m_bSUACheck;
	CString	m_strSUAFile;
	CString	m_strSUAFile2;
	BOOL	m_bSUAIgnore;
	CWinscoreDoc *m_pDoc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFDRSettingsPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFDRSettingsPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseLogPath();
	afx_msg void OnBrowseSuafile();
	afx_msg void OnSuacheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeSuafilename();
	afx_msg void OnBnClickedBrowseSuafile2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDRSETTINGSPROPPAGE_H__210D2F73_B798_11D3_B132_0080C84B0161__INCLUDED_)

