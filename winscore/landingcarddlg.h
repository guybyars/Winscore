//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_LANDINGCARDDLG_H__A7D93BE3_536C_11D1_ACE9_000000000000__INCLUDED_)
#define AFX_LANDINGCARDDLG_H__A7D93BE3_536C_11D1_ACE9_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LandingCard.h : header file
//

#include "TurnpointListCtrl.h"
#include "TurnpointCombo.h"
#include "TurnpointEdit.h"
#include "LandingCardList.h"	// Added by ClassView
#include "WinscoreDoc.h"
#include "resizingdialog.h"
#include "Contestant.h"	// Added by ClassView
#include "Gate.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CLandingCard dialog

class CLandingCardDlg : public CResizingDialog
{
// Construction
public:
	CString GetLandoutLocationText();
	void SetStatusText();
	CTime& GetDate();
	CGate& GetStartGate();
	CGate	m_cStartGate;
	int		m_iTurnpointSought;
	CContestant* GetContestant();

	void	ApplyDialog();
	int		m_nAcheivedTurnpoints;
	void	RefreshTaskList();
	CLandingCard* m_pPreselect;
	bool	m_bPreselect;
	CTime	m_cPreselectDate;
	CString	m_strPreselectCID;

	void EnableLandoutRadios(BOOL fEnable);
	CTask* GetTask();
	CLandingCard* GetLandingCard();
	void EnableLanded(BOOL fEnable);
	void EnableAcheived(BOOL fEnable, CTask* pcTask);
	void MaskForm();
	void SetFormFromCard(CLandingCard* pcInputCard);
	CLandingCardList* m_pcLandingCardList;
	void SetLandingCardList(CLandingCardList *pcLandingCardList);
	CLandingCardDlg(CWnd* pParent = NULL);   // standard constructor


	CWinscoreDoc*	m_pDoc;
	CLocation		m_cLandoutLocation;
	int				m_iLandoutTurnpoint;
	CTime			m_cSelectedDate;
	CFlight*		m_pcFlight;
	CLocation		m_acTurnpointLocations[MAXTASKTPS];
	int				m_aiTurnpointFixes[MAXTASKTPS];

	bool SetLandingCardFromDlg(CLandingCard &cLandingCard);


// Dialog Data
	//{{AFX_DATA(CLandingCardDlg)
	enum { IDD = IDD_LANDING };
	CButton	m_cChangeTATLocationButton;
	CButton	m_cFlightLogGroup;
	CButton	m_cViewLogButton;
	CButton	m_cLandingLocationText;
	CButton	m_cStartGateGroup;
	CButton	m_cResetButton;
	CButton	m_cStartGateCheck;
	CButton	m_cStartGateButton;
	CButton	m_cLandingLocationButton;
	CTurnpointListCtrl	m_cCheckedTurnpointListCtrl;
	CTurnpointEdit	m_cTurnpointEdit;
	CButton	m_cFinishedCheck;
	CButton	m_cOK;
	CStatic	m_cStatusText;
	CComboBox	m_cContestantCombo;
	CTurnpointListCtrl	m_cTurnpointList;
	CStatic	m_cStartFinishText;
	CStatic	m_cDistanceText;
	CComboBox	m_cDateCombo;
	CButton	m_cAddButton;
	CString	m_cContestNo;
	CString	m_strDistanceText;
	BOOL	m_bAirfield;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLandingCardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLandingCardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddButton();
	virtual void OnOK();
	afx_msg void OnSelchangeDateCombo();
	afx_msg void OnSelchangeContestantCombo();
	afx_msg void OnSetfocusTurnpointList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTurnpointList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnFinishedcheck();
	afx_msg void OnUpdateTurnpointEdit();
	afx_msg void OnItemchangedCheckedlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCheckedlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditupdateContestantCombo();
	afx_msg void OnApply();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAirfield();
	afx_msg void OnLandingLocation();
	afx_msg void OnStartgate();
	afx_msg void OnStartGateCheck();
	afx_msg void OnReset();
	afx_msg void OnViewlog();
	afx_msg void OnChangeturnpointbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetListVisibility(ETaskType eType);
	void UpdateDistanceText();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANDINGCARDDLG_H__A7D93BE3_536C_11D1_ACE9_000000000000__INCLUDED_)

