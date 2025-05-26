#pragma once

#include "ACComboBox.h"
#include "Contestant.h"
#include "winscoredoc.h"
#include "afxwin.h"

// CContestant1Dlg dialog

class CContestant1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CContestant1Dlg)

public:
	CContestant1Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContestant1Dlg();

	CContestant *m_pContestant;
	CContestant* m_pNewContestant;
	CWinscoreDoc *m_pcWinscoreDoc;
	CString m_strAddress2;
	CString m_strAddress1;
	CString m_strCity;
	CString	m_strState;
	CString	m_strZipcode1, m_strZipcode2;
	CString m_strFDR_ID;
	EClass	m_eClass;

	float	m_fSpan;
	bool	m_bWinglets;
	bool	m_bTurbulation;
	bool	m_bRootFairings;
	float	m_fWeight;
	CGliderInfo m_cGliderInfo;

	void UpdateFromContestant( CContestant* pcContestant );
	void UpdateHandicapFields();
	void UpdateClassCombo();

// Dialog Data
	enum { IDD = IDD_CONTESTANT1 };

	CButton	m_cHandicapLookupButton;
	CACComboBox	m_cGliderMakeCombo;
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
	int		m_iSSANumber;
	BOOL	m_bMotorized;
	BOOL	m_bWithdrew;
	BOOL	m_bGuest;
	BOOL	m_bForeign;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeClassCombo();
	afx_msg void OnClickedHclookupbutton();
	afx_msg void OnBnClickedContestantPersonal();
	afx_msg void OnCbnSelchangeGliderMake();
};
