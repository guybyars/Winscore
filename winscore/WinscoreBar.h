// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#include "WSTreeItem.h"

#define CPANETYPE   CDockablePane

class CWinscoreBar : public CPANETYPE
{
public:
	CWinscoreBar();

// Attributes
protected:
	CTreeCtrl	m_wndTree;
	HTREEITEM   m_hRoot;
	HTREEITEM   m_hTurnpointItem;
	HTREEITEM   m_hContestantItem, m_hTaskItem;

	CImageList  m_cImageList;

// Operations
public:

// Overrides

// Implementation
public:
	virtual ~CWinscoreBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
protected:
public:
	BOOL OnShowControlBarMenu(CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	CTreeCtrl& GetTreeCtrl(void);
	int LoadTreeCtrl(CDocument* pDoc);

	void DeleteTreeChildren(HTREEITEM hItem, bool bDeleteParent);
	void DeleteWSTreeItem( HTREEITEM hItem);
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CWSTreeItem* GetSelectedItem(void);
	void LoadClassesInTree(HTREEITEM hItem, bool bKids, CWSTreeItem::ETreeType eType, CTime cTime=0);
	void LoadDateClassTreeChildren(HTREEITEM hItem, CTime cDate, EClass eClass);
};
