//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_EXCLUDECONTESTANTSDLG_H__AE52AEB8_83BA_11D3_B116_0080C84B0161__INCLUDED_)
#define AFX_EXCLUDECONTESTANTSDLG_H__AE52AEB8_83BA_11D3_B116_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExcludeContestantsDlg.h : header file
//

#include "winscoredoc.h"

/////////////////////////////////////////////////////////////////////////////
// CExcludeContestantsDlg dialog

class CExcludeContestantsDlg : public CDialog
{
// Construction
public:
	CWinscoreDoc* m_pDoc;
	CExcludeContestantsDlg(CWnd* pParent = NULL);   // standard constructor
	void  LoadPilotListBox();

// Dialog Data
	//{{AFX_DATA(CExcludeContestantsDlg)
	enum { IDD = IDD_EXCLUDE_CONTESTANTS };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcludeContestantsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExcludeContestantsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCLUDECONTESTANTSDLG_H__AE52AEB8_83BA_11D3_B116_0080C84B0161__INCLUDED_)

