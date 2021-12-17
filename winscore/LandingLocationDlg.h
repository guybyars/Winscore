#if !defined(AFX_LANDINGLOCATIONDLG_H__DB3386FA_A53B_4E87_A4A5_87E7772E23AA__INCLUDED_)
#define AFX_LANDINGLOCATIONDLG_H__DB3386FA_A53B_4E87_A4A5_87E7772E23AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LandingLocationDlg.h : header file
//

#include "winscoredoc.h"
#include "turnpointcombo.h"
#include "Location.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CLandingLocationDlg dialog

class CLandingLocationDlg : public CDialog
{
// Construction
public:
	CString m_strLandoutTurnpointName;
	CLocation		m_cFurthestProgressLocation;
	CLocation		m_cLandoutLocation;
	int				m_iLandoutTurnpoint;
	BOOL			m_bUseFurthestProgress;
	CWinscoreDoc*	m_pDoc;

	CLandingLocationDlg(	CWinscoreDoc*	pDoc, 
							CLocation		&LandingcLocation,
							CLocation		&cFurthestProgress,
							int				iLandoutTurnpoint,
							BOOL			bUseFurthestProgress,
							CWnd*			pParent = NULL);   // standard constructor

	void UpdateLandoutRadio();
	void EnableLandoutRadios(int iOpt);

// Dialog Data
	//{{AFX_DATA(CLandingLocationDlg)
	enum { IDD = IDD_LANDING_LOCATION };
	CButton	m_cProgressRadio;
	CStatic	m_cLatLongText;
	CButton	m_cLatLongButton;
	CButton	m_cAtLatLongRadio;
	CButton	m_cAtTurnpointRadio;
	CTurnpointCombo	m_cLandoutTurnpoint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLandingLocationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLandingLocationDlg)
	virtual void OnOK();
	afx_msg void OnAtturnpointRadio();
	afx_msg void OnAtlatlongRadio();
	afx_msg void OnLatlongButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLandoutTurnpointCombo2();
	afx_msg void OnProgressradio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANDINGLOCATIONDLG_H__DB3386FA_A53B_4E87_A4A5_87E7772E23AA__INCLUDED_)
