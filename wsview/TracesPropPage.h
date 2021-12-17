#if !defined(AFX_TRACESPROPPAGE_H__C2B71433_41F1_4E07_9910_FFF3BECCFA74__INCLUDED_)
#define AFX_TRACESPROPPAGE_H__C2B71433_41F1_4E07_9910_FFF3BECCFA74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracesPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTracesPropPage : Property page dialog

class CTracesPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CTracesPropPage)
	DECLARE_OLECREATE_EX(CTracesPropPage)

// Constructors
public:
	CTracesPropPage();

// Dialog Data
	//{{AFX_DATA(CTracesPropPage)
	enum { IDD = IDD_OLE_PROPPAGE_TRACES };
	CButton	m_cFullTraceCheck;
	CEdit	m_cDisplayLength;
	CStatic	m_cMinutesText;
	CStatic	m_cDisplayLengthText;
	BOOL	m_bDifferentColorCheck;
	int		m_iDisplayLenfth;
	BOOL	m_bPointDisplay;
	BOOL	m_bFullTraceCheck;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CTracesPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnFullTraceCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACESPROPPAGE_H__C2B71433_41F1_4E07_9910_FFF3BECCFA74__INCLUDED_)
