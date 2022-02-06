//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CONTESTINFODLG_H__631CD9C1_40D9_11D1_ACCF_000000000000__INCLUDED_)
#define AFX_CONTESTINFODLG_H__631CD9C1_40D9_11D1_ACCF_000000000000__INCLUDED_

#include "Winscore.h"	// Added by ClassView
#include "classlistctrl.h"
#include "winscoredoc.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ContestInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContestInfoDlg dialog

class CContestInfoDlg : public CDialog
{
// Construction
public:
	void EditClass();
	CString m_cDescription;
	ECoordFormat m_eCoordinateFormat;
	EUnits m_eUnits;
	EDatum m_eDatum;
	EContest m_eContest;
	void SetContest(EContest eContest);
	CContestInfoDlg( CWinscoreDoc *pDoc, CWnd* pParent = NULL);   //  constructor
	int m_iSSA;
	
	CWSClass	m_acClass[NUMCLASSES];

// Dialog Data
	//{{AFX_DATA(CContestInfoDlg)
	enum { IDD = IDD_CONTEST_INFO };
	CButton	m_cClassPropButton;
	CClassListCtrl	m_cClassListCtrl;
	CButton	m_CRegionalButton;
	CButton	m_CNationalButton;
	CString	m_strLocation;
	CString	m_strContestName;
	int		m_iNumDays;
	CTime	m_cContestStartDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContestInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CWinscoreDoc *m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CContestInfoDlg)
	afx_msg void OnNationalRadio();
	afx_msg void OnRegionalRadio();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDatetimechangeStartdatepicker(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusClasslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickClasslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClasspropertiesbutton();
	afx_msg void OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContestSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSsaButton();
	int m_iNumPracticeDays;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTESTINFODLG_H__631CD9C1_40D9_11D1_ACCF_000000000000__INCLUDED_)

