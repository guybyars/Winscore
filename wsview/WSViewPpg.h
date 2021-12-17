//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSVIEWPPG_H__E0748855_5814_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_WSVIEWPPG_H__E0748855_5814_11D3_BA96_0010A4043281__INCLUDED_

#include "FontCombo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WSViewPpg.h : Declaration of the CWSViewPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CWSViewPropPage : See WSViewPpg.cpp.cpp for implementation.

class CWSViewPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWSViewPropPage)
	DECLARE_OLECREATE_EX(CWSViewPropPage)

// Constructor
public:
	bool m_bFontChanged;
	CWSViewPropPage();

// Dialog Data
	//{{AFX_DATA(CWSViewPropPage)
	enum { IDD = IDD_PROPPAGE_WSVIEW };
	CFontCombo	m_cFontCombo;
	CButton	m_cVertScaleGroup;
	CButton	m_cSceneryCheck;
	CComboBox	m_cColorCombo;
	CSliderCtrl	m_cSlider;
	CStatic	m_cBackgroundColor;
	BOOL	m_bSceneryCheck;
	//}}AFX_DATA

	CBitmap m_bitmap;
	COLORREF m_crBackgroundColor;
	COLORREF m_crClimbColor;
	COLORREF m_crWaypointColor;
	COLORREF m_crStartRadiusColor;
	COLORREF m_crFinishRadiusColor;
	COLORREF m_crTaskColor;
	COLORREF m_crAirspaceColor;
	BOOL	 m_bSceneryState;
	void FillPropControl(CBitmap &bitmap, CStatic &cStatic, COLORREF crColor);

	int	m_iLastPos;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CWSViewPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeBackgroundButton();
	afx_msg void OnResetbutton();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeColorCombo();
	afx_msg void OnSceneryCheck();
	afx_msg void OnSelchangeFontCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSVIEWPPG_H__E0748855_5814_11D3_BA96_0010A4043281__INCLUDED)

