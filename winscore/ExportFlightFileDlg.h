//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_EXPORTFLIGHTFILEDLG_H__509D9A33_7007_11D3_B10D_0080C84B0161__INCLUDED_)
#define AFX_EXPORTFLIGHTFILEDLG_H__509D9A33_7007_11D3_B10D_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportFlightFileDlg.h : header file
//
#include "classlistctrl.h"
#include "winscoredoc.h"

/////////////////////////////////////////////////////////////////////////////
// CExportFlightFileDlg dialog

class CExportFlightFileDlg : public CDialog
{
// Construction
public:
	CWinscoreDoc* m_pDoc;
	CExportFlightFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportFlightFileDlg)
	enum { IDD = IDD_EXPORT_FLIGHT_FILE };
//	CButton	m_cViewButton;
	CButton	m_cExportButton;
	CButton	m_cCancelButton;
	CProgressCtrl	m_cProgress;
	CComboBox	m_cDateCombo;
	CClassListCtrl	m_cClassList;
	CButton	m_cBrowseButton;
	CString	m_strFileName;
	CString	m_strStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportFlightFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportFlightFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnViewButton();
	afx_msg void OnBrowseButton();
	afx_msg void OnSelchangeDateCombo();
	afx_msg void OnExcludeButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTFLIGHTFILEDLG_H__509D9A33_7007_11D3_B10D_0080C84B0161__INCLUDED_)

