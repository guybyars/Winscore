//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_FLIGHTANALYSISDLG_H__6DD82178_F636_11D2_B0F1_0080C84B0161__INCLUDED_)
#define AFX_FLIGHTANALYSISDLG_H__6DD82178_F636_11D2_B0F1_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlightAnalysisDlg.h : header file
//


#include "winscoredoc.h"
#include "resizingdialog.h"
#include "afxwin.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CFlightAnalysisDlg dialog

class CFlightAnalysisDlg : public CResizingDialog
{
// Construction
public:
	CFlight* m_pcFlight;
	CFlightAnalysisDlg(EUnits eUnits, EAltUnits eAltUnits,CWnd* pParent = NULL);   // standard constructor

	void CreateControlColumns(CListCtrl& ListCtrl);
	void EnsureVisibleCenter( int iItem );
	void EnsureVisibleTop( int iItem );

	int	m_iStartItem;
	int m_iFinishItem;
	int m_aiTurnpointItems[2*MAXTASKTPS];
	int m_nTPItems;
	int m_iTPItemPtr;
	int m_iLandingItem;
	int m_iPEVPtr;
	EUnits m_eUnits;
	EAltUnits m_eAltUnits;

	CTask *m_pcTask;

	std::vector<int>   m_vPEVPos;

// Dialog Data
	//{{AFX_DATA(CFlightAnalysisDlg)
	enum { IDD = IDD_FLIGHT_ANALYSIS };
	CButton	m_cPrevButton;
	CButton	m_cNextButton;
	CButton	m_cStartButton;
	CButton	m_cFinishButton;
	CButton m_cPEVButton;
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlightAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlightAnalysisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFindFinish();
	afx_msg void OnFindStart();
	afx_msg void OnNextTurnpoint();
	afx_msg void OnPrevTurnpoint();
	afx_msg void OnLanding();
	afx_msg void OnBnClickedPEV();
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnOK();
	afx_msg void OnEditCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIGHTANALYSISDLG_H__6DD82178_F636_11D2_B0F1_0080C84B0161__INCLUDED_)

