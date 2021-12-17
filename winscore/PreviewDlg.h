//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_PREVIEWDLG_H__AE7757A1_A5DC_11D1_8F45_444553540000__INCLUDED_)
#define AFX_PREVIEWDLG_H__AE7757A1_A5DC_11D1_8F45_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PreviewDlg.h : header file
//
#include "WinscoreDoc.h"
#include "Winscore.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog

class CPreviewDlg : public CDialog
{
// Construction
public:
	EScoresheetStatus m_eScoresheetStatus;
	CWinscoreDoc*	m_pDoc;
	CPreviewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreviewDlg)
	enum { IDD = IDD_PREVIEW };
	int		m_iRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreviewDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CPreviewDlg::GreyClassButtons( BOOL bActive );


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWDLG_H__AE7757A1_A5DC_11D1_8F45_444553540000__INCLUDED_)

