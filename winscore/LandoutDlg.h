//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_LANDOUTDLG_H__A7D93BE2_536C_11D1_ACE9_000000000000__INCLUDED_)
#define AFX_LANDOUTDLG_H__A7D93BE2_536C_11D1_ACE9_000000000000__INCLUDED_

#include "Location.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LandoutDlg.h : header file
//

#include "TurnpointCombo.h"
#include "WinscoreDoc.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CLandoutDlg dialog

class CLandoutDlg : public CDialog
{
// Construction

private: 
	void CLandoutDlg::UpdateDirectionButtons();
	void CLandoutDlg::UpdateHMSButtons();
	void CLandoutDlg::MaskLatLong(BOOL	fEnable);

public:
	CWinscoreDoc* m_pDoc;
	CLocation m_cLocation;
	CLandoutDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CLandoutDlg)
	enum { IDD = IDD_LANDOUT_COORDINATES };
	CButton	m_cWestRadio;
	CButton	m_cSouthRadio;
	CButton	m_cNorthRadio;
	CStatic	m_cLongSecText;
	CStatic	m_cLongMinText;
	CStatic	m_cLongDegText;
	CButton	m_cLongText;
	CEdit	m_cLongSec;
	CEdit	m_cLongMin;
	CEdit	m_cLongDeg;
	CStatic	m_cLatSecText;
	CStatic	m_cLatMinText;
	CButton	m_cFormatText;
	CStatic	m_cLatDegText;
	CButton	m_cLatText;
	CEdit	m_cLatSec;
	CEdit	m_cLatMin;
	CEdit	m_cLatDegrees;
	CButton	m_cHHMMSS;
	CButton	m_cHHMMMM;
	CButton	m_cEastRadio;
	int		m_iLatDegDlg;
	double	m_dLatMinDlg;
	int		m_iLatSecDlg;
	int		m_iLongDegDlg;
	double	m_dLongMinDlg;
	int		m_iLongSecDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLandoutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLandoutDlg)
	afx_msg void OnEast();
	afx_msg void OnHhmmmm();
	afx_msg void OnHhmmss();
	afx_msg void OnTurnpointCheck();
	afx_msg void OnNorth();
	afx_msg void OnSouth();
	afx_msg void OnWest();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANDOUTDLG_H__A7D93BE2_536C_11D1_ACE9_000000000000__INCLUDED_)

