//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_REALTIMEPROPPAGE_H__BF394273_CF28_11D3_B138_0080C84B0161__INCLUDED_)
#define AFX_REALTIMEPROPPAGE_H__BF394273_CF28_11D3_B138_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealTimePropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRealTimePropPage dialog

class CRealTimePropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRealTimePropPage)

// Construction
public:
	CRealTimePropPage();
	~CRealTimePropPage();

// Dialog Data
	//{{AFX_DATA(CRealTimePropPage)
	enum { IDD = IDD_REALTIME_PROP_PAGE };
	int		m_iRefreshRate;
	CString	m_strRealTimePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRealTimePropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRealTimePropPage)
	afx_msg void OnBrowseRealtimePath();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEPROPPAGE_H__BF394273_CF28_11D3_B138_0080C84B0161__INCLUDED_)

