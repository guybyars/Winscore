//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CONTESTANTADDRESS_H__6AF118E1_421B_11D1_ACD1_000000000000__INCLUDED_)
#define AFX_CONTESTANTADDRESS_H__6AF118E1_421B_11D1_ACD1_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ContestantAddress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContestantAddress dialog

class CContestantAddress : public CDialog
{
// Construction
public:
	CContestantAddress(CWnd* pParent = NULL);   // constructor

// Dialog Data
	//{{AFX_DATA(CContestantAddress)
	enum { IDD = IDD_CONTESTANT_ADDRESS };
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strCity;
	CString	m_strState;
	CString	m_strZip1;
	CString	m_strZip2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContestantAddress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContestantAddress)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTESTANTADDRESS_H__6AF118E1_421B_11D1_ACD1_000000000000__INCLUDED_)

