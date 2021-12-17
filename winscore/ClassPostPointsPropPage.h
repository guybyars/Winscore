//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_CLASSPOSTPOINTSPROPPAGE_H__864B55D7_AB0C_11D3_B129_0080C84B0161__INCLUDED_)
#define AFX_CLASSPOSTPOINTSPROPPAGE_H__864B55D7_AB0C_11D3_B129_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassPostPointsPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClassPostPointsPropPage dialog

class CClassPostPointsPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CClassPostPointsPropPage)

// Construction
public:
	CClassPostPointsPropPage();
	~CClassPostPointsPropPage();

// Dialog Data
	//{{AFX_DATA(CClassPostPointsPropPage)
	enum { IDD = IDD_POSTPOINTSPROPPAGE };
	int		m_iAirfieldBonus;
	double	m_dMDP_B;
	double	m_dMDP_M;
	double	m_dMSP_B;
	double	m_dMSP_M;
	double	m_dNonFinishPoints;
	BOOL	m_bScoredCompletionRatioCheck;
	BOOL	m_bShortTaskFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassPostPointsPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassPostPointsPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeNofinishPointsEdit();
	afx_msg void OnStfCheck();
	afx_msg void OnScrCheck();
	afx_msg void OnChangeMspMEdit();
	afx_msg void OnChangeMspBEdit();
	afx_msg void OnChangeMdpMEdit();
	afx_msg void OnChangeMdpBEdit();
	afx_msg void OnChangeAirfieldBonusEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSPOSTPOINTSPROPPAGE_H__864B55D7_AB0C_11D3_B129_0080C84B0161__INCLUDED_)

