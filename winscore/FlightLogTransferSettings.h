#if !defined(AFX_FLIGHTLOGTRANSFERSETTINGS_H__FF012A16_E6C0_4563_9694_A34D5B282A93__INCLUDED_)
#define AFX_FLIGHTLOGTRANSFERSETTINGS_H__FF012A16_E6C0_4563_9694_A34D5B282A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlightLogTransferSettings.h : header file
//

#include "WinscoreDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CFlightLogTransferSettings dialog

class CFlightLogTransferSettings : public CDialog
{
// Construction
public:

	CFlightLogTransferSettings(CWnd* pParent = NULL);   // standard constructor

	CWinscoreDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CFlightLogTransferSettings)
	enum { IDD = IDD_LOG_TRANSFER_SETINGS };
	CStatic	m_cTransferText2;
	CEdit	m_cTop;
	CStatic	m_cTransferText;
	CButton	m_cCondenseCheck;
	CStatic	m_cMinTimeText;
	CEdit	m_cTime;
	int		m_iTime;
	int		m_iTop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlightLogTransferSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlightLogTransferSettings)
	afx_msg void OnExcludeSpecific();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCondenseCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIGHTLOGTRANSFERSETTINGS_H__FF012A16_E6C0_4563_9694_A34D5B282A93__INCLUDED_)
