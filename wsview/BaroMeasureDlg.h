#if !defined(AFX_BAROMEASUREDLG_H__162EEB04_3333_42A5_9D19_6E779B5DC03E__INCLUDED_)
#define AFX_BAROMEASUREDLG_H__162EEB04_3333_42A5_9D19_6E779B5DC03E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaroMeasureDlg.h : header file

#include "barotrace.h"
//

/////////////////////////////////////////////////////////////////////////////
// CBaroMeasureDlg dialog

class CBaroMeasureDlg : public CDialog
{
// Construction
public:
	CString m_strClip;
	long m_lEnd;
	long m_lStart;
	void SetRange( long lStart, long lEnd );
	CBaroMeasureDlg(CBaroTrace *pTrace, 
					EUnits		m_eUnits, 
					EAltUnits	m_eAltUnits, 
					short		m_uDisplayMask,
					CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBaroMeasureDlg)
	enum { IDD = IDD_BARO_MEASURE };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaroMeasureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CBaroTrace *m_pTrace;
	EUnits		m_eUnits;
	EAltUnits	m_eAltUnits;
	short		m_uDisplayMask;

	// Generated message map functions
	//{{AFX_MSG(CBaroMeasureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAROMEASUREDLG_H__162EEB04_3333_42A5_9D19_6E779B5DC03E__INCLUDED_)
