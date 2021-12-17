//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_EDITFLIGHTDLG_H__01480083_F4E5_11D2_B0F1_0080C84B0161__INCLUDED_)
#define AFX_EDITFLIGHTDLG_H__01480083_F4E5_11D2_B0F1_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditFlightDlg.h : header file
//
#include "Afxdtctl.h"
#include "Location.h"	// Added by ClassView
#include "turnpointedit.h"
#include "winscoredoc.h"
/////////////////////////////////////////////////////////////////////////////
// CEditFlightDlg dialog

class CEditFlightDlg : public CDialog
{
// Construction
public:
	bool m_bSafetyFinish;
	void Recalculate();
	bool m_bDirty;
	int m_nAcheivedPts;
	CTurnpointArray* m_pcTurnpointArray;
	CFlight* m_pcFlight;
	CEditFlightDlg(CWnd* pParent = NULL);   // standard constructor
	CTask *m_pcTask;
	CWinscoreDoc *m_pDoc;

// Dialog Data
	//{{AFX_DATA(CEditFlightDlg)
	enum { IDD = IDD_EDIT_FLIGHT };
	CButton	m_cSafetyCheck;
	CButton	m_cAltStartButton;
	CButton	m_cChangeScoredLandingPointButton;
	CButton	m_cScoredLandingPointCheck;
	CButton	m_cTurnpointCheck;
	CButton	m_cAirfieldBonus;
	CStatic	m_cAttemptingText;
	CButton	m_cLandoutGroup;
	CEdit	m_cEditDistance;
	CButton	m_cDistanceCheck;
	CButton	m_cRollTimeCheck;
	CButton	m_cStartTimeCheck;
	CButton	m_cLandingTimeCheck;
	CButton	m_cFinishTimeCheck;
	CTurnpointCombo	m_cAttemptingCombo;
	CTurnpointCombo	m_cStartCombo;
	CDateTimeCtrl	m_cTimePickerFinishCtrl;
	CDateTimeCtrl	m_cTimePickerStartCtrl;
	CDateTimeCtrl	m_cTimePickerRollCtrl;
	CDateTimeCtrl	m_cTimePickerLandingCtrl;
	CTurnpointEdit	m_cEditTurnpoints;
	CTurnpointCombo	m_cTurnpointCombo;
	CButton	m_cCompletedCheck;
	CString	m_strEditTurnpoints;
	CTime	m_cTimePickerLanding;
	CTime	m_cTimePickerFinish;
	CTime	m_cTimePickerRoll;
	CTime	m_cTimePickerStart;
	BOOL	m_bAirfieldBonus;
	double	m_dDistance;
	CString	m_strScoredLandingPoint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFlightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void UpdateFlightFromDlg();
	void UpdateDlgFromFlight();

	// Generated message map functions
	//{{AFX_MSG(CEditFlightDlg)
	virtual void OnOK();
	afx_msg void OnLocationButton();
	afx_msg void OnCompletedCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposTpSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRollingFiniahCheck();
	afx_msg void OnDatetimechangeDatetimepickerFinish(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLandingLocation();
	afx_msg void OnLandedcheck();
	afx_msg void OnFinishcheck();
	afx_msg void OnStartcheck();
	afx_msg void OnRollcheck();
	afx_msg void OnDistancecheck();
	afx_msg void OnRecalculate();
	afx_msg void OnTurnpointcheck();
	afx_msg void OnChanglandoutbutton();
	afx_msg void OnScoredlpcheck();
	afx_msg void OnChangefixes();
	afx_msg void OnChangeEditTurnpoints();
	afx_msg void OnEditchangeAttemptingCombo();
	afx_msg void OnSelchangeAttemptingCombo();
	afx_msg void OnAirfield();
	afx_msg void OnAltStart();
	afx_msg void OnSafetyCheck();
	afx_msg void OnDatetimechangeDatetimepickerStart(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void MaskForm();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFLIGHTDLG_H__01480083_F4E5_11D2_B0F1_0080C84B0161__INCLUDED_)

