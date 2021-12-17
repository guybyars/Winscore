//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_IMPORTDATADLG_H__9BF07EE6_A7B3_11D2_B644_0080C84B0161__INCLUDED_)
#define AFX_IMPORTDATADLG_H__9BF07EE6_A7B3_11D2_B644_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportDataDlg dialog

class CImportDataDlg : public CDialog
{
// Construction
public:
	int m_iOverWrite;
	CTime m_cCurrentDate;
	CImportDataDlg(CWnd* pParent = NULL);   // standard constructor


	CEventList       m_EventList;
	CTaskList		 m_TaskList;
	CPenalityList	 m_PenalityList;
	CWinscoreDoc*    m_pDoc;


// Dialog Data
	//{{AFX_DATA(CImportDataDlg)
	enum { IDD = IDD_IMPORT_EVENTS };
	CListBox	m_cListBox;
	CButton	m_cImportButton;
	CButton	m_cAcceptButton;
	CString	m_cImportFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportDataDlg)
	afx_msg void OnAccept();
	afx_msg void OnBrowse();
	afx_msg void OnImportButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDATADLG_H__9BF07EE6_A7B3_11D2_B644_0080C84B0161__INCLUDED_)

