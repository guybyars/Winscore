//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_INVALIDLEGDLG_H__52978671_6839_11D1_AD0C_000000000000__INCLUDED_)
#define AFX_INVALIDLEGDLG_H__52978671_6839_11D1_AD0C_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InvalidLegDlg.h : header file
//

#include "TurnpointCombo.h"
#include "WinscoreListCtrl.h"
#include "WinscoreDoc.h"	// Added by ClassView
#include "InvalidLegList.h"


/////////////////////////////////////////////////////////////////////////////
// CInvalidLegDlg dialog

class CInvalidLegDlg : public CDialog
{
// Construction
public:
	CInvalidLegList* m_pcInvalidLegList;
	CWinscoreDoc* m_pDoc;
	CInvalidLegDlg(CWnd* pParent = NULL);   //  constructor
	void UpdateListBox();

// Dialog Data
	//{{AFX_DATA(CInvalidLegDlg)
	enum { IDD = IDD_INVALID_LEGS };
	CWinscoreListCtrl	m_cListBox;
	CTurnpointCombo	m_cTurnpointToCombo;
	CTurnpointCombo	m_cTurnpointFromCombo;
	CButton	m_cDelete;
	CButton	m_cApply;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvalidLegDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInvalidLegDlg)
	afx_msg void OnApply();
	afx_msg void OnDelete();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFromCombo();
	afx_msg void OnSelchangeToCombo();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVALIDLEGDLG_H__52978671_6839_11D1_AD0C_000000000000__INCLUDED_)

