//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//{{AFX_INCLUDES()
#include "wsview.h"
#include "wsbaro.h"
//}}AFX_INCLUDES
#if !defined(AFX_FLIGHTDISPLAYDLG_H__897FDB85_592B_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_FLIGHTDISPLAYDLG_H__897FDB85_592B_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlightDisplayDlg.h : header file
//

#include "sxbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CFlightDisplayDlg dialog

class CFlightDisplayDlg : public CResizingDialog
{
// Construction
public:
	bool m_bPlaying;
	CTime m_cFlightEndTime;
	CTime m_cFlightBeginTime;
	CTask* m_pcTask;
	CFlight* m_pcFlight;
	CFlightDisplayDlg(CWnd* pParent = NULL);   // standard constructor
	CWinscoreDoc* m_pDoc;
	void  UpdateTime( int iPos );
	
	bool OnMoveCursor(long lTime);

// Dialog Data
	//{{AFX_DATA(CFlightDisplayDlg)
	enum { IDD = IDD_DISPLAY_FLIGHT };
	CButton	m_cPlayButton;
	CEdit	m_cLatLong;
	CEdit	m_cTimeDisplay;
//	CSliderCtrl	m_cSlider;
	CWSView	m_cWSView;
	CWSBaro	m_cWSBaro;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlightDisplayDlg)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlightDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFlightDisplayHelp();
	afx_msg void OnZoominbutton();
	afx_msg void OnZoomoutbutton();
	afx_msg void OnZoomtaskbutton();
	afx_msg void OnDisplayoptionsbutton();
	afx_msg void OnAutoscale();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIGHTDISPLAYDLG_H__897FDB85_592B_11D3_BA96_0010A4043281__INCLUDED_)

