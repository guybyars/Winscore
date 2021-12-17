//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_MEASURINGDLG_H__862CE34F_01D3_4943_96CC_415E5FED6152__INCLUDED_)
#define AFX_MEASURINGDLG_H__862CE34F_01D3_4943_96CC_415E5FED6152__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasuringDlg.h : header file
//

#include "3DTransformation.h"
#include "hitlist.h"

/////////////////////////////////////////////////////////////////////////////
// CMeasuringDlg dialog

class CMeasuringDlg : public CDialog
{
// Construction
public:
	void DisplayControlPointInfo(CControlPoint *pPoint, EAltUnits eAltUnits);
	void UpdateTurnpointText(CHitPoint* pcHit);
	void SetInitialAltitude(int iAlt);
	void UpdateTotals(CHitPoint *pStart, CPoint &cEndPoint, C3DTransformation &c3DTransformation, CHitPoint *pEnd, EUnits eUnits, BOOL bAltitudeMeters, CString strUnitsText);
	void UpdateRunningTotals(CHitPoint *pHit1, CHitPoint *pHit2, EUnits eUnits);
	bool m_bMeasurmentMade;
	void ResetText();
	CMeasuringDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasuringDlg)
	enum { IDD = IDD_MEASURING };
	CString	m_strText;
	CString	m_strSpeed;
	CString	m_strTime;
	CString	m_strAveROC;
	CString	m_strAltitude;
	CString	m_strTurnpoint;
	//}}AFX_DATA

	double			m_dMeasureDistance;
	CTimeSpan		m_cMeasureTimeSpan;
	int				m_iMeasureAltitude;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasuringDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasuringDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASURINGDLG_H__862CE34F_01D3_4943_96CC_415E5FED6152__INCLUDED_)

