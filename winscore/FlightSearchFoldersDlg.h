#if !defined(AFX_FLIGHTSEARCHFOLDERSDLG_H__3A1577C1_693E_4449_A9BA_E35E02CD6BCD__INCLUDED_)
#define AFX_FLIGHTSEARCHFOLDERSDLG_H__3A1577C1_693E_4449_A9BA_E35E02CD6BCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlightSearchFoldersDlg.h : header file
//

#include "HScrollListBox.h"
#include "afxwin.h"
#include "IGCDirectories.h"

/////////////////////////////////////////////////////////////////////////////
// CFlightSearchFoldersDlg dialog

class CFlightSearchFoldersDlg : public CDialog
{
// Construction
public:
	CString m_strDefault;
	CFlightSearchFoldersDlg(CStringArray &strArray, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlightSearchFoldersDlg)
	enum { IDD = IDD_FLIGHT_SEARCH };
	CButton	m_cDeleteButton;
	CHScrollListBox	m_cList;
	//}}AFX_DATA

	CStringArray &m_strArray;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlightSearchFoldersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlightSearchFoldersDlg)
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveup();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd2();
	CButton m_cRelativeButton;
	afx_msg void OnBnAutoDateManage();
	CButton m_cAddAbsolute;
	CButton m_cAutoDateCheckBox;
	CButton m_cMoveUpButton;
	void MasqueForm();

	CIGCDirectories m_cIGCDir;
	bool m_bAutoDateCheck;
	CWinscoreDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIGHTSEARCHFOLDERSDLG_H__3A1577C1_693E_4449_A9BA_E35E02CD6BCD__INCLUDED_)
