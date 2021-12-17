//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_EXPORTSCORESHEETDLG_H__9D4534A1_63A5_11D1_AD02_000000000000__INCLUDED_)
#define AFX_EXPORTSCORESHEETDLG_H__9D4534A1_63A5_11D1_AD02_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExportScoresheetDlg.h : header file
//
#include "WinscoreDoc.h"
#include "classlistctrl.h"


/////////////////////////////////////////////////////////////////////////////
// CExportScoresheetDlg dialog

class CExportScoresheetDlg : public CDialog
{
// Construction
public:
	CString m_cComment;
	EExportType m_eExportType;
	void SetDefaultFileName();
	CWinscoreDoc* m_pDoc;
	CString m_cRootFileName;
	CExportScoresheetDlg(CWnd* pParent = NULL);   // standard constructor

void  CExportScoresheetDlg::ExportScoresheet(	CStdioFile	&cFile,
												EClass		eClass, 
												CTime		&cDate, 
												EExportType	eExportType, 
												EScoresheetType eType);

void  CExportScoresheetDlg::ExportHeader(	CStdioFile		 &cFile,
											CTime			&cDate, 
											EExportType		eExportType, 
											EScoresheetType eType);

void  CExportScoresheetDlg::ExportTrailer(	CStdioFile		 &cFile,
											EExportType		eExportType );

// Dialog Data
	//{{AFX_DATA(CExportScoresheetDlg)
	enum { IDD = IDD_EXPORT_SCORESHEET };
	CClassListCtrl	m_cClassListCtrl;
	CComboBox	m_cFormatCombo;
	CButton	m_cDayRadio;
	CButton	m_cCumRadio;
	CComboBox	m_cDateCombo;
	CString	m_cFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportScoresheetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportScoresheetDlg)
	afx_msg void OnBrowseButton();
	afx_msg void OnCumRadio();
	afx_msg void OnDayRadio();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeDateCombo2();
	afx_msg void OnSelchangeFormatCombo();
	afx_msg void OnComment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void GreyRadioButtons(BOOL fGrey);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTSCORESHEETDLG_H__9D4534A1_63A5_11D1_AD02_000000000000__INCLUDED_)

