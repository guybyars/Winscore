#if !defined(AFX_ACCOMBOBOX_H__4F7DDD23_9DFC_4475_B957_21EF5631E964__INCLUDED_)
#define AFX_ACCOMBOBOX_H__4F7DDD23_9DFC_4475_B957_21EF5631E964__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CACComboBox window

class CACComboBox : public CComboBox
{
// Construction
public:
	CACComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CACComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CACComboBox)
	afx_msg void OnEditupdate();
	//}}AFX_MSG

	bool m_bAutoComplete;

	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOMBOBOX_H__4F7DDD23_9DFC_4475_B957_21EF5631E964__INCLUDED_)
