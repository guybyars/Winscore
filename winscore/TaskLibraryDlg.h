#pragma once
#include "afxcmn.h"
#include "afxcoll.h"
#include "tasklist.h"
#include "resizingdialog.h"

// CTaskLibraryDlg dialog

class CTaskLibraryDlg : public CResizingDialog
{
	DECLARE_DYNAMIC(CTaskLibraryDlg)

public:
	CTaskLibraryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskLibraryDlg();

// Dialog Data
	enum { IDD = IDD_TASKLIBRARY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLibraryPath;
	CTaskList	m_cTaskList;
	CTask*		m_pTask;

	CListCtrl m_cListCtrl;
	afx_msg void OnBnClickedSelect();
	CWinscoreDoc* m_pDoc;
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedChangeFolder();
	int LoadTaskLibrary(CString strLibraryPath);
	afx_msg void OnNMDblclkListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedViewtask();
	afx_msg void OnHdnItemclickListctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
