//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_ADDTURNPOINTDLG_H__6E8CA248_397A_11D1_ACC1_000000000000__INCLUDED_)
#define AFX_ADDTURNPOINTDLG_H__6E8CA248_397A_11D1_ACC1_000000000000__INCLUDED_

#include "Turnpoint.h"
#include "WinscoreListCtrl.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AddTurnpointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddTurnpointDlg dialog

class CAddTurnpointDlg : public CResizingDialog
{
// Construction
public:
	int GetID();
	CTurnpoint* m_pPreselect;
	BOOL m_bTasks;
	CAddTurnpointDlg(CWnd* pParent = NULL);   //  constructor
	void SetTurnpointArray(CTurnpointArray*);

// Dialog Data
	//{{AFX_DATA(CAddTurnpointDlg)
	enum { IDD = IDD_TURNPOINT };
	CButton	m_cTurnpointCheck;
	CButton	m_cStartCheck;
	CButton	m_cAirfieldCheck;
	CButton	m_cHomeCheck;
	CButton	m_cFinishCheck;
	CButton	m_cImport;
	CWinscoreListCtrl	m_cTurnpointListControl;
	CButton	m_CDelete;
	CStatic	m_cLongSecText;
	CStatic	m_cLatSecText;
	CEdit	m_CName;
	CEdit	m_CLongSec;
	CEdit	m_CLatSec;
	CButton	m_CApply;
	CEdit	m_CID;
	CButton	m_CSouth;
	CButton	m_CWest;
	CButton	m_CNorth;
	CButton	m_CEast;
	int		m_iLatDeg;
	int		m_iLongDeg;
	int		m_iLongSec;
	CString	m_Name;
	double	m_dLatMin;
	double	m_dLongMin;
	int		m_iLatSec;
	int		m_iElevation;
	CString	m_strID;
	//}}AFX_DATA

	CTurnpointArray* m_pturnpointArray;  
	ECoordFormat  m_eCoordinateFormat;

private:

	int CAddTurnpointDlg::NextAvailable();
	int CAddTurnpointDlg::FindName(CString strName);
	CTurnpoint* NewTurnpoint(int iID, CString strName, CLocation* cLocation, int iAttribute, int iElevation);
	void UpdateListBox();
	void UpdateFormFromTurnpoint(CTurnpoint* pcTurnpointItem);
	void CAddTurnpointDlg::UpdateDirectionButtons();
	void CAddTurnpointDlg::UpdateHMSButtons();	
	void UpdateFormFromListbox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddTurnpointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddTurnpointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWest();
	afx_msg void OnSouth();
	afx_msg void OnNorth();
	afx_msg void OnEast();
	afx_msg void OnApply();
	afx_msg void OnKillfocusLatMin();
	afx_msg void OnKillfocusLongMin();
	afx_msg void OnKillfocusTurnpointId();
	afx_msg void OnDelete();
	virtual void OnCancel();
	afx_msg void OnItemchangedListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemclickListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImport();
	afx_msg void OnKeydownListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusName();
	afx_msg void OnChangeName();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnChangeTurnpointId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

 

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTURNPOINTDLG_H__6E8CA248_397A_11D1_ACC1_000000000000__INCLUDED_)

