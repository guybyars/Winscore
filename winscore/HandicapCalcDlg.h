#if !defined(AFX_HANDICAPCALCDLG_H__64B833DA_7D5A_4A19_85CC_D058D678D777__INCLUDED_)
#define AFX_HANDICAPCALCDLG_H__64B833DA_7D5A_4A19_85CC_D058D678D777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HandicapCalcDlg.h : header file
//
#include "gliderinfolist.h"
#include "ACComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CHandicapCalcDlg dialog

class CHandicapCalcDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	bool m_bInitDialog;
	void RecalculateHandicap();
	CHandicapCalcDlg(CGliderInfoList &cGIList, CGliderInfo *pcGliderInfo, CWnd* pParent = NULL);
	  // standard constructor

// Dialog Data
	//{{AFX_DATA(CHandicapCalcDlg)
	enum { IDD = IDD_HANDICAP };
	CButton	m_cRootFaringCheckBox;
	CButton	m_cTurbulationCheckBox;
	CButton	m_cWingletCheckBox;
	CComboBox	m_cManufacturerCombo;
	CEdit	m_cHandicapEdit;
	CEdit	m_cSpanEdit;
	CEdit	m_cWeightEdit;
	CACComboBox	m_cModel;
	float	m_fSpan;
	float	m_fWeight;
	CString	m_strNotes;
	CString	m_strSpan;
	CString	m_strWeight;
	CString	m_strManufacturer;
	double	m_dHandicap;
	BOOL	m_bWingletCheck;
	BOOL	m_bTurbulationCheck;
	BOOL	m_bRootFaringCheck;
	//}}AFX_DATA

	CGliderInfoList &m_cGIList;
	CGliderInfo*	m_pcGliderInfo;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandicapCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHandicapCalcDlg)
	virtual void OnOK();
	afx_msg void OnSelchangeModelCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateSpanEdit();
	afx_msg void OnUpdateWeightEdit();
	afx_msg void OnSelchangeManufacturerCombo();
	afx_msg void OnTurbulationCheck();
	afx_msg void OnWingletCheck();
	afx_msg void OnRootFairingCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDICAPCALCDLG_H__64B833DA_7D5A_4A19_85CC_D058D678D777__INCLUDED_)
