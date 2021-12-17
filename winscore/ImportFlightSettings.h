//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_IMPORTFLIGHTSETTINGS_H__9C2B3D37_D934_11D2_B0BC_0080C84B0161__INCLUDED_)
#define AFX_IMPORTFLIGHTSETTINGS_H__9C2B3D37_D934_11D2_B0BC_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportFlightSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportFlightSettings dialog

class CImportFlightSettings : public CDialog
{
// Construction
public:
	int m_iPortID;
	CImportFlightSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportFlightSettings)
	enum { IDD = IDD_FDR_UPLOAD_SETTINGS };
	CComboBox	m_cPortCombo;
	CString	m_strExePath;
	CString	m_strLogPath;
	CString	m_strUploadPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportFlightSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportFlightSettings)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseExe();
	afx_msg void OnBrowseLogPath();
	afx_msg void OnBrowseUpload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTFLIGHTSETTINGS_H__9C2B3D37_D934_11D2_B0BC_0080C84B0161__INCLUDED_)

