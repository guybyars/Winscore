//{{AFX_INCLUDES()
#include "wsview.h"
//}}AFX_INCLUDES
#if !defined(AFX_CHANGETURNPOINTLOCDLG_H__F723A3F7_4044_4F40_9E5E_58975517448F__INCLUDED_)
#define AFX_CHANGETURNPOINTLOCDLG_H__F723A3F7_4044_4F40_9E5E_58975517448F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeTurnpointLocDlg.h : header file
//

#include "WinscoreDoc.h"
#include "location.h"
#include "turnpointcombo.h"
#include "flight.h"
#include "ResizingDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CChangeTurnpointLocDlg dialog

class CChangeTurnpointLocDlg : public CResizingDialog
{
// Construction
public:
	int m_iFix;
	void UpdateFix();
	CChangeTurnpointLocDlg(	CWinscoreDoc*	pDoc,
							CContestant*	pcContestant,
							CTask*			pcTask,
							CFlight*		pcFlight,
							CWnd*			pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeTurnpointLocDlg)
	enum { IDD = IDD_CHANGE_TURNPOINT_LOCATION };
	CSpinButtonCtrl	m_cSpin;
	CComboBox m_cTurnpointCombo;
	CString	m_strControlPointDistanceText;
	CString	m_strDistanceFlown;
	CString	m_strLogPointEdit;
	CWSView	m_cWSViewCtrl;
	double	m_dDist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeTurnpointLocDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeTurnpointLocDlg)
	afx_msg void OnSelchangeTurnpointCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnChangeplusbutton();
	afx_msg void OnChangeminusbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWinscoreDoc*	m_pDoc;
	CFlight*		m_pcFlight;
	CContestant*	m_pcContestant;
	CTask*			m_pcTask;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGETURNPOINTLOCDLG_H__F723A3F7_4044_4F40_9E5E_58975517448F__INCLUDED_)
