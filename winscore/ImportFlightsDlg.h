//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_IMPORTFLIGHTSDLG_H__9C2B3D36_D934_11D2_B0BC_0080C84B0161__INCLUDED_)
#define AFX_IMPORTFLIGHTSDLG_H__9C2B3D36_D934_11D2_B0BC_0080C84B0161__INCLUDED_

#include "ResizingDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportFlightsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportFlightsDlg dialog

class CImportFlightsDlg : public CResizingDialog
{
// Construction
public:
//	void CreateBatchFile(CString cBatch, CString &cCommand);
	CWinscoreDoc* m_pDoc;

	CStringArray m_cImportArray, m_cOutputArray;

	CString m_strIGCFileName;
	CString m_strLogPath;

	void ImportIGCFile();
	void ConvertImportedFiles(CStringArray &cBatchArray);
	
	CImportFlightsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportFlightsDlg)
	enum { IDD = IDD_IMPORT_FLIGHTS };
	CButton	m_cUploadRadio;
	CStatic	m_cManufacturerText;
	CButton	m_cImportFilesButton;
	CComboBox	m_cSourceCombo;
	CButton	m_cUploadButton;
	CString	m_strStatus;
	int		m_iUploadRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportFlightsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportFlightsDlg)
	afx_msg void OnUpload();
	afx_msg void OnView();
	afx_msg void OnImport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAnalyze();
	afx_msg void OnSettings();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioUpload();
	afx_msg void OnRadioImport();
	afx_msg void OnImportFiles();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cStaticInfo;
	CProgressCtrl m_cProgress;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTFLIGHTSDLG_H__9C2B3D36_D934_11D2_B0BC_0080C84B0161__INCLUDED_)




