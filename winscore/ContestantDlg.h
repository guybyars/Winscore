//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CONTESTANTDLG_H__23C6BD64_40EF_11D1_ACD0_000000000000__INCLUDED_)
#define AFX_CONTESTANTDLG_H__23C6BD64_40EF_11D1_ACD0_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ContestantDlg.h : header file
//

#include "ContestantList.h"
#include "WinscoreDoc.h"
#include "WinscoreListCtrl.h"
#include "resizingdialog.h"
#include "ACComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CContestantDlg dialog

class CContestantDlg : public CResizingDialog
{
// Construction
public:
	CContestant* m_pPreselect;
	void UpdateFromContestant( CContestant* cContestant);
	void CContestantDlg::UpdateFromListboxSel();
	void CContestantDlg::UpdateClassCombo();
	void ResetContestData();

	CString m_strAddress2;
	CString m_strAddress1;
	CString m_strCity;
	CString	m_strState;
	CString	m_strZipcode1, m_strZipcode2;
	EClass	m_eClass;
	CFDRecorder	m_pcFDRecorder;

	float	m_fSpan;
	bool	m_bWinglets;
	bool	m_bTurbulation;
	bool	m_bRootFairings;
	float	m_fWeight;
	CGliderInfo m_cGliderInfo;

	void UpdateHandicapFields();
	CWinscoreDoc* m_pcWinscoreDoc;
	void SetDocument(CWinscoreDoc* pcWinscoreDoc);
	CContestantList* m_pcontestantList;


	void SetContestantList( CContestantList* pcContestantList);
	void UpdateListBox();
	CContestantDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContestantDlg)
	enum { IDD = IDD_CONTESTANT };
	CButton	m_cHandicapLookupButton;
	CACComboBox	m_cGliderMakeCombo;
	CButton	m_cImportButton;
	CListCtrl	m_cCListCtrlBox;
	CButton	m_CDelete;
	CEdit	m_CLastName;
	CEdit	m_CContestNo;
	CEdit	m_cHandicapEditBox;
	CStatic	m_cHandicapText;
	CComboBox	m_cClassCombo;
	CString	m_strContestNo;
	CString	m_strFirstName;
	double	m_dHandicap;
	CString	m_strGlider;
	CString	m_strLastName;
	CString	m_strMiddleName;
	BOOL	m_fFDRecorderUsed;
	int		m_iSSANumber;
	BOOL	m_bMotorized;
	BOOL	m_bWithdrew;
	BOOL	m_bGuest;
	BOOL	m_bForeign;
	CString m_strFDR_ID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContestantDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContestantDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnSelchangeClassCombo();
	afx_msg void OnContestantPersonal();
	afx_msg void OnDelete();
	afx_msg void OnFdrButton();
	afx_msg void OnRecorderCheck();
	virtual void OnCancel();
	afx_msg void OnKillfocusContestNo();
	afx_msg void OnChangeContestNo();
	afx_msg void OnSetfocusContestNo();
	afx_msg void OnItemchangedControlBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusControlBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickControlBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownControlBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImportContestant();
	afx_msg void OnItemclickControlBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHclookupbutton();
	afx_msg void OnForeignCheck();
	afx_msg void OnImportSSA();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTESTANTDLG_H__23C6BD64_40EF_11D1_ACD0_000000000000__INCLUDED_)

