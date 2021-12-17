#pragma once

#include "WinscoreDoc.h"
#include "afxwin.h"


// CSelectClassDlg dialog

class CSelectClassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectClassDlg)

public:
	CSelectClassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectClassDlg();

	CWinscoreDoc	*m_pDoc;
	EClass			m_eClass;

// Dialog Data
	enum { IDD = IDD_SELECT_CLASS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cClassCombo;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
