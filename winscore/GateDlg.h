//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_GATEDLG_H__F156C616_988E_11D3_B123_0834EA000000__INCLUDED_)
#define AFX_GATEDLG_H__F156C616_988E_11D3_B123_0834EA000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GateDlg.h : header file
//

#include "gate.h"
#include "TurnpointCombo.h"
#include "WinscoreDoc.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CGateDlg dialog

class CGateDlg : public CDialog
{



// Construction
public:

enum EGateDlgType {	eStart=0, 
					eFinish=1 };


	CWinscoreDoc* m_pDoc;
	void MaskForm();
	CGateDlg( CWinscoreDoc *pDoc, EGateDlgType eGateDlgType , CGate &cGate, CTask*	pTask,  CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGateDlg)
	enum { IDD = IDD_GATE };
	CButton	m_cFirstLegCheck;
	CButton	m_cPerpCheck;
	CStatic	m_cHeadingText;
	CEdit	m_cHeightEdit;
	CComboBox	m_cTypeCombo;
	CStatic	m_cTypeText;
	CStatic	m_cRadiusText;
	CEdit	m_cRadiusEdit;
	CTurnpointCombo	m_cPointCombo;
	CStatic	m_cHeightText;
	CEdit	m_cBaseEdit;
	CEdit	m_cHeadingEdit;
	CStatic	m_cBaseText;
	int		m_iBase;
	int		m_iHeading;
	int		m_iHeight;
	double	m_dRadius;
	bool    m_bPerpToCourse;
	bool    m_bScoreOutTheTop;
	CString	m_strUnitsText;
	CString	m_strUnitsSpeedText;
	//}}AFX_DATA

	CGate		 &m_cGate;
	EGateDlgType  m_eGateDlgType;
	EUnits		  m_eUnits;

	CTask*	m_pTask;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGateDlg)
	afx_msg void OnGpscheck();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTypecombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeRadiusEdit();
	afx_msg void OnBnClickedPerpCheck();
	CButton m_cOutTheTopCheck;
	afx_msg void OnBnClickedMaxGroundSpeed();
	CButton m_cPreStartAltitudeCheckBox;
	CStatic m_cMaxGroundSpeedText;
	CStatic m_cMaxGroundSpeedTextUnits;
	CStatic m_cMaxStartAltitudeText;
	CStatic m_cMSAUnits;
	CStatic m_cPreStartAltitudeUnits;
	CEdit m_cSpeedEdit;
	double m_dMaxSpeed;
	double m_dMaxGroundSpeed;
	CEdit m_cPreStartAltitude;
	int m_iPreStartAltitude;
	int m_iMaxAltitude;

	CEdit m_cMaxAltitude;
	CStatic m_cPEVStartWindowStatic;
	int m_iPEVWaitTime;
	int m_iPEVStartWindow;
	CEdit m_cPEVWaitTime;
	CEdit m_cPEVStartWindow;
	CButton m_cPEVEnable;
	CStatic m_cPEVWaitTimeStatic;
	afx_msg void OnBnClickedEnablePev();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GATEDLG_H__F156C616_988E_11D3_B123_0834EA000000__INCLUDED_)

