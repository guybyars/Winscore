//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_PENALITYDLG_H__61B64631_5F38_11D1_ACFC_000000000000__INCLUDED_)
#define AFX_PENALITYDLG_H__61B64631_5F38_11D1_ACFC_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PenalityDlg.h : header file
//
#include "WinscoreListCtrl.h"
#include "WinscoreDoc.h"
#include "resizingdialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPenalityDlg dialog

class CPenalityDlg : public CResizingDialog
{
// Construction
public:
	int m_iSortColumn;
	void InitializeValues();
	CPenalityList* m_pcPenalityList;
	CWinscoreDoc* m_pDoc;
	void UpdatePointsRadio();
	void MaskForm();
	void UpdateListBox();


	CPenalityDlg(CWnd* pParent = NULL);   // standard constructor

	EPenality m_ePenality;

// Dialog Data
	//{{AFX_DATA(CPenalityDlg)
	enum { IDD = IDD_PENALITY };
	CEdit	m_ctlExplaination;
	CComboBox	m_cPenalityCode;
	CStatic	m_cPercentOrPointsText;
	CEdit	m_cDailyPenaligy;
	CStatic	m_cDailyPenalityText;
	CButton	m_cDailyPenalityGroupText;
	CComboBox	m_cTypeCombo;
	CComboBox	m_cContestNoCombo;
	CListCtrl	m_cPenalityList;
	CButton	m_cDelete;
	CComboBox	m_cDateCombo;
	CButton	m_cApply;
	double	m_dDailyPenaligy;
	CString	m_cExplaination;
	CString	m_cPointsOrPercent;
	CString	m_strPenalityCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenalityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPenalityDlg)
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTypeCombo();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnChangeCode();
	afx_msg void OnItemchangedPenalityList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnKeydownPenalityList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnSelchangeCodeCombo();
	afx_msg void OnEditchangeCodeCombo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnColumnclickPenalityList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strPreselectCID;
	CTime   m_cPreselectDate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PENALITYDLG_H__61B64631_5F38_11D1_ACFC_000000000000__INCLUDED_)

