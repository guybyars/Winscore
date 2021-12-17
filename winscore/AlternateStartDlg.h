#if !defined(AFX_ALTERNATESTARTDLG_H__F12D3522_C2F4_4B22_A476_A910B0361E39__INCLUDED_)
#define AFX_ALTERNATESTARTDLG_H__F12D3522_C2F4_4B22_A476_A910B0361E39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlternateStartDlg.h : header file
//

#include "flight.h"
#include "WinscoreDoc.h"
#include "summary.h"
#include "scorerecord.h"
#include "winscoredefs.h"


/////////////////////////////////////////////////////////////////////////////
// CAlternateStartDlg dialog

class CAlternateStartDlg : public CDialog
{
// Construction
public:
	CTime m_cStartTime;
	CAlternateStartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlternateStartDlg)
	enum { IDD = IDD_ALTERNATE_START };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlternateStartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlternateStartDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CWinscoreDoc* m_pDoc;

	int			m_nMaxRet;
	CTime		m_caStartTimes[15];
	CTimeSpan	m_caTOCs[15];
	int			m_iaPenalties[15];
	double		m_daSpeeds[15];
	double		m_daDistances[15];
	int			m_iaPointDiff[15];
	int			m_iaScore[15];
	int			m_iBestScorePtr;
	int			m_nStarts;
	bool        m_bHandicapped;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALTERNATESTARTDLG_H__F12D3522_C2F4_4B22_A476_A910B0361E39__INCLUDED_)
