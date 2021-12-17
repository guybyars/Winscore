//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CONTESTSETTINGSDLG_H__E8011361_E46C_11D3_B181_CE04AD000000__INCLUDED_)
#define AFX_CONTESTSETTINGSDLG_H__E8011361_E46C_11D3_B181_CE04AD000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContestSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContestSettingsDlg dialog

class CContestSettingsDlg : public CDialog
{
// Construction
public:
	CContestSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContestSettingsDlg)
	enum { IDD = IDD_CONTEST_SETTINGS };
	BOOL	m_bGuestsAtBottom;
	int		m_iLatLongFormat;
	int		m_iDistanceUnits;
	BOOL	m_bTwoGateCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContestSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContestSettingsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_iDatum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTESTSETTINGSDLG_H__E8011361_E46C_11D3_B181_CE04AD000000__INCLUDED_)

