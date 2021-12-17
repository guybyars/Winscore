#pragma once


// CSSAPicker dialog

class CSSAPicker : public CDialogEx
{
	DECLARE_DYNAMIC(CSSAPicker)

public:
	CSSAPicker(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSSAPicker();

// Dialog Data
	enum { IDD = IDD_SAA_PICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_iSSA;
};
