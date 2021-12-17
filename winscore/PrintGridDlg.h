//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_PRINTGRIDDLG_H__CE5B7F6A_B53F_11D3_B12E_0080C84B0161__INCLUDED_)
#define AFX_PRINTGRIDDLG_H__CE5B7F6A_B53F_11D3_B12E_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintGridDlg.h : header file
//

#include "winscoredoc.h"
#include "classlistctrl.h"


class  CGridData
	{
	public:
		CGridData();

		int		iNumDaysToPrint;
		int		iPrintStartNumber;
		int		iPrintNumRows;
		int		iPrintNumClasses;
		EClass	aiPrintClasses[NUMCLASSES];
		short	aiPrintDays[NUMCLASSES];
		bool	bLetterScheme;
		int		iSpaces;
	};

/////////////////////////////////////////////////////////////////////////////
// CPrintGridDlg dialog

class CPrintGridDlg : public CDialog
{
// Construction
public:
	CWinscoreDoc* m_pDoc;
	CPrintGridDlg(CWinscoreDoc* pDoc, CGridData* pGridData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintGridDlg)
	enum { IDD = IDD_PRINT_GRID_POSITIONS };
	CSpinButtonCtrl	m_cStartSpin;
	CSpinButtonCtrl	m_cSpacesSpin;
	CSpinButtonCtrl	m_cDaysToPrintSpin;
	CStatic	m_cSpacesText;
	CEdit	m_cSpacesEdit;
	CEdit	m_cNumRowsEdit;
	CSpinButtonCtrl	m_cRowsSpin;
	CClassListCtrl	m_cClassListCtrl;
	int		m_iStartNumber;
	int		m_iNumRows;
	int		m_iSpaces;
	int		m_iNumDaysToPrint;
	BOOL	m_bLetterCheck;
	//}}AFX_DATA

	CGridData*	m_pGridData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintGridDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDayPlus();
	afx_msg void OnDayMinus();
	afx_msg void OnDeltaposNumRowsSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTGRIDDLG_H__CE5B7F6A_B53F_11D3_B12E_0080C84B0161__INCLUDED_)

