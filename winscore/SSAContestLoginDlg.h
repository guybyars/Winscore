#pragma once


// CSSAContestLoginDlg dialog

class CSSAContestLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSSAContestLoginDlg)

public:
	CSSAContestLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSSAContestLoginDlg();

// Dialog Data
	enum { IDD = IDD_SSA_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUsername;
	CString m_strPassword;
	BOOL m_bRemember;
};
