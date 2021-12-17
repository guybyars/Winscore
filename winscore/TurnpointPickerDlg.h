//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_TURNPOINTPICKERDLG_H__ACC21CA1_9A94_11D3_B124_7030F7000000__INCLUDED_)
#define AFX_TURNPOINTPICKERDLG_H__ACC21CA1_9A94_11D3_B124_7030F7000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurnpointPickerDlg.h : header file


#include "turnpointarray.h"


/////////////////////////////////////////////////////////////////////////////
// CTurnpointPickerDlg dialog

class CTurnpointPickerDlg : public CDialog
{
// Construction
public:
	CTurnpoint* m_pSelectedTurnpoint;
	CTurnpointPickerDlg(CTurnpointArray &turnPointArray,   CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTurnpointPickerDlg)
	enum { IDD = IDD_TURNPOINT_PICKER };
	CListCtrl	m_cListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurnpointPickerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CTurnpointArray &m_turnPointArray;

	// Generated message map functions
	//{{AFX_MSG(CTurnpointPickerDlg)
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNPOINTPICKERDLG_H__ACC21CA1_9A94_11D3_B124_7030F7000000__INCLUDED_)

